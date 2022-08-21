/*********************************************************************************
 *Copyright(C): Juntuan.Lu, 2020-2030, All rights reserved.
 *Author:  Juntuan.Lu
 *Version: 1.0
 *Date:  2021/10/23
 *Email: 931852884@qq.com
 *Description:
 *Others:
 *Function List:
 *History:
 **********************************************************************************/

#include "gosgcontrol.h"
#include "gcommon.h"
#include <OpenThreads/ScopedLock>
#include <QCoreApplication>
#include <QDir>
#include <QThread>
#include <osg/ComputeBoundsVisitor>
#include <osg/CullFace>
#include <osg/Fog>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>
#include <osgUtil/Simplifier>
#include <osgViewer/ViewerEventHandlers>

#define USE_CULLFACE 1
#define USE_GFOG 1
#define USE_GSKY_BOX 0
#define USE_GLIGHT 1
#define USE_GPLATFORM 1
#define USE_GPARTICLE 1
#define USE_GANIMATION 1

GOsgControl::GOsgControl(QObject* parent)
    : QObject(parent)
    , m_rootGroup(new osg::Group)
    , m_rootNodeGroup(new osg::MatrixTransform)
{
    auto loadErrorFunction = [this]() {
        m_hasError = true;
        m_loading = false;
        m_errorMessage = "Failed to load model !";
        emit rootNodeChanged();
        emit loadingChanged();
        emit hasErrorChanged();
        emit errorMessageChanged();
    };
    auto loadFinishedFunction = [this]() {
        m_loading = false;
        m_viewer->home();
        emit rootNodeChanged();
        emit loadingChanged();
    };
    m_loadThread = QThread::create([=]() {
        m_loading = true;
        emit loadingChanged();
        osg::ref_ptr<osg::Node> loadNode = osgDB::readNodeFile(m_rootNodeUrl.toLocalFile().toStdString());
        //        osgUtil::Simplifier simplifier(0.1, 4.0);
        //        loadNode->accept(simplifier);
        //        osgUtil::Optimizer optimzer1;
        //        optimzer1.optimize(loadNode);
        //        osgDB::writeNodeFile(*loadNode, "./car.osgb");
        if (m_loadThread->isInterruptionRequested()) {
            return;
        }
        QMutexLocker locker(&m_mutex);
        (void)locker;
        if (!loadNode.valid()) {
            loadErrorFunction();
            return;
        }
        m_rootNode = loadNode;
        m_rootNodeGroup->addChild(m_rootNode);
        double vectorSize = -1;
        {
            osg::ComputeBoundsVisitor boundVisitor;
            m_rootNodeGroup->accept(boundVisitor);
            const osg::BoundingBox& box = boundVisitor.getBoundingBox();
            double length = box.xMax() - box.xMin();
            double width = box.yMax() - box.yMin();
            double height = box.zMax() - box.zMin();
            m_platformTranslate = osg::Vec3d(-box.center().x(), -box.center().y(), -box.center().z() + height / 2);
            vectorSize = std::max(std::max(length, width), height);
        }
        if (vectorSize < 0) {
            loadErrorFunction();
            return;
        }
        m_rootNodeGroup->setMatrix(GCommon::getMatrix(m_rootNodeMatrix) * osg::Matrix::translate(m_platformTranslate));
#if USE_CULLFACE
        osg::ref_ptr<osg::CullFace> cullface = new osg::CullFace(osg::CullFace::BACK);
        m_rootNode->getOrCreateStateSet()->setAttribute(cullface);
        m_rootNode->getOrCreateStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::ON);
        m_rootNode->setCullingActive(true);
#endif
#if USE_GFOG
        {
            osg::ref_ptr<osg::Fog> fog = static_cast<osg::Fog*>(m_rootNode->getOrCreateStateSet()->getAttribute(osg::StateAttribute::FOG));
            if (!fog.valid()) {
                fog = new osg::Fog;
                m_rootNode->getOrCreateStateSet()->setAttributeAndModes(fog);
            }
            fog->setMode(osg::Fog::LINEAR);
            fog->setStart(0.0);
            fog->setEnd(vectorSize * 10);
            fog->setColor(osg::Vec4d(0.1f, 0.1f, 0.08f, 1.0f));
            fog->setDensity(1.0);
            fog->setUseRadialFog(true);
        }
#endif
#if USE_GSKY_BOX
        {
            if (m_skyNode.valid()) {
                m_rootGroup->removeChild(m_skyNode);
                m_skyNode = nullptr;
            }
            m_skyNode = GSkyBox::create("./sources/sky", vectorSize * 15);
            m_rootGroup->addChild(m_skyNode);
        }
#endif
#if USE_GLIGHT
        {
            if (m_lightNode.valid()) {
                m_rootGroup->removeChild(m_lightNode);
                m_lightNode = nullptr;
            }
            m_lightNode = GLight::create(m_viewer->getCamera(), vectorSize * 5);
            m_rootGroup->addChild(m_lightNode);
        }
#endif
#if USE_GPLATFORM
        {
            if (m_platformNode.valid()) {
                m_rootGroup->removeChild(m_platformNode);
                m_platformNode = nullptr;
            }
            m_platformNode = GPlatform::create(vectorSize * 4, 20);
            m_rootGroup->addChild(m_platformNode);
        }
#endif
#if USE_GPARTICLE
        {
            if (m_particle.valid()) {
                m_rootGroup->removeChild(m_particle);
                m_particle = nullptr;
            }
            m_particle = new GParticle(vectorSize / 5);
            m_particle->setMatrix(GCommon::getMatrix(m_particleMatrix));
            m_rootGroup->addChild(m_particle);
        }
#endif
#if USE_GANIMATION
        {
            GAnimationNodeVisitor<GAnimationManager, osgAnimation::AnimationManagerBase> animationNodeVisitor;
            m_animationManager = animationNodeVisitor.getNode(m_rootNode);
            if (m_animationManager.valid()) {
                m_animationList.clear();
                m_animationsStatus.clear();
                for (unsigned int i = 0; i < m_animationManager->getAnimationList().size(); i++) {
                    const auto& ani = m_animationManager->getAnimationList().at(i);
                    m_animationList.append(QString::fromStdString(ani->getName()));
                    m_animationsStatus.insert(QString::number(i), QVariantMap { { "running", false } });
                }
                if (!m_animationList.empty()) {
                    emit animationListChanged();
                    emit animationsStatusChanged();
                }
                m_animationManager->setAnimationFinishedCallback([this](int index) {
                    m_animationsStatus.insert(QString::number(index), QVariantMap { { "running", false } });
                    emit animationsStatusChanged();
                });
            } else if (!m_animationList.empty()) {
                m_animationList.clear();
                m_animationsStatus.clear();
                emit animationListChanged();
                emit animationsStatusChanged();
            }
        }
#endif
        if (m_manipulator.valid()) {
            m_manipulator->setLimit(vectorSize * 2, vectorSize * 10, vectorSize / 20);
            if (m_homePos.empty()) {
                m_manipulator->setHomePosition(osg::Vec3d(0, -vectorSize * 2, vectorSize / 2), osg::Vec3d(0, 0, 0), osg::Vec3d(0, 0, 1));
            }
        }
        osgUtil::Optimizer optimzer;
        optimzer.optimize(m_rootGroup);
        m_viewer->updateTraversal();
        m_viewer->home();
        loadFinishedFunction();
    });
}

GOsgControl::~GOsgControl()
{
    if (m_loadThread) {
        m_loadThread->requestInterruption();
        m_loadThread->terminate();
        m_loadThread->wait();
        m_loadThread->deleteLater();
    }
}

void GOsgControl::classBegin()
{
}

void GOsgControl::componentComplete()
{
}

void GOsgControl::setRootNode(const QUrl& rootNodeUrl)
{
    QMutexLocker locker(&m_mutex);
    (void)locker;
    if (m_rootNodeUrl != rootNodeUrl) {
        m_rootNodeUrl = rootNodeUrl;
        if (m_rootNode.valid()) {
            m_rootNodeGroup->removeChild(m_rootNode);
            m_rootNode = nullptr;
        }
        if (m_loadThread) {
            m_loadThread->terminate();
            m_loadThread->wait();
            m_loadThread->start();
        }
    }
}

void GOsgControl::setHomePos(const QVariantMap& homePos)
{
    QMutexLocker locker(&m_mutex);
    (void)locker;
    if (m_homePos != homePos) {
        m_homePos = homePos;
        if (m_manipulator.valid()) {
            const auto& pos = GCommon::getHomePos(homePos);
            m_manipulator->setHomePosition(std::get<0>(pos), std::get<1>(pos), std::get<2>(pos));
            m_viewer->home();
        }
        emit homePosChanged();
    }
}

void GOsgControl::setFlyPosList(const QVariantList& flyPosList)
{
    QMutexLocker locker(&m_mutex);
    (void)locker;
    if (m_flyPosList != flyPosList) {
        m_flyPosList = flyPosList;
        if (m_manipulator.valid()) {
            m_manipulator->setFlyList(GCommon::getFlyList(flyPosList));
        }
        emit flyPosListChanged();
    }
}

void GOsgControl::setParticleMatrix(const QVariantMap& particleMatrix)
{
    QMutexLocker locker(&m_mutex);
    (void)locker;
    if (m_particleMatrix != particleMatrix) {
        m_particleMatrix = particleMatrix;
        if (m_particle.valid()) {
            m_particle->setMatrix(GCommon::getMatrix(m_particleMatrix));
        }
        emit particleMatrixChanged();
    }
}

void GOsgControl::setRootNodeMatrix(const QVariantMap& rootNodeMatrix)
{
    QMutexLocker locker(&m_mutex);
    (void)locker;
    if (m_rootNodeMatrix != rootNodeMatrix) {
        m_rootNodeMatrix = rootNodeMatrix;
        m_rootNodeGroup->setMatrix(GCommon::getMatrix(rootNodeMatrix) * osg::Matrix::translate(m_platformTranslate));
        emit rootNodeMatrixChanged();
    }
}

void GOsgControl::init(osgViewer::Viewer* viewer)
{
    m_viewer = viewer;
    m_viewer->addEventHandler(new osgViewer::StatsHandler);
    osg::ref_ptr<GCoord> coord = new GCoord(m_viewer->getCamera());
    m_rootGroup->addChild(coord);
    m_manipulator = new GManipulator;
    m_manipulator->setFlyFinishedCallback([this](int index) {
        (void)index;
        m_flyIndex = -1;
        emit flyIndexChanged();
    });
    m_viewer->setCameraManipulator(m_manipulator);
    m_rootGroup->addChild(m_rootNodeGroup);
    m_viewer->setSceneData(m_rootGroup);
}

bool GOsgControl::checkFrameAllowed()
{
    if (m_requestDestroy) {
        return false;
    }
    if (m_loading) {
        return false;
    }
    // return m_viewer->checkNeedToDoFrame();
    return true;
}

void GOsgControl::requestDestroy()
{
    m_requestDestroy = true;
}

QUrl GOsgControl::getUrlForLocal(const QString& path)
{
    return QUrl::fromLocalFile(path);
}

bool GOsgControl::playAnimation(int index, bool reset, double duration, double end, double start)
{
    QMutexLocker locker(&m_mutex);
    (void)locker;
    if (!m_animationManager.valid()) {
        return false;
    }
    bool ok = m_animationManager->playAnimation(index, reset, duration, end, start);
    if (ok) {
        m_animationsStatus.insert(QString::number(index), QVariantMap { { "running", true } });
        emit animationsStatusChanged();
    }
    return ok;
}

bool GOsgControl::playAnimationForName(const QString& name, bool reset, double duration, double end, double start)
{
    int index = m_animationList.indexOf(name);
    return playAnimation(index, reset, duration, end, start);
}

bool GOsgControl::stopAnimation(int index, bool reset)
{
    QMutexLocker locker(&m_mutex);
    (void)locker;
    if (!m_animationManager.valid()) {
        return false;
    }
    bool ok = m_animationManager->stopAnimation(index, reset);
    if (ok) {
        m_animationsStatus.insert(QString::number(index), QVariantMap { { "running", false } });
        emit animationsStatusChanged();
    }
    return ok;
}

bool GOsgControl::stopAnimationForName(const QString& name, bool reset)
{
    int index = m_animationList.indexOf(name);
    return stopAnimation(index, reset);
}

bool GOsgControl::stopAnimationAll(bool reset)
{
    QMutexLocker locker(&m_mutex);
    (void)locker;
    if (!m_animationManager.valid()) {
        return false;
    }
    bool ok = m_animationManager->stopAnimationAll(reset);
    if (ok) {
        m_animationsStatus.clear();
        for (int i = 0; i < m_animationList.length(); i++) {
            m_animationsStatus.insert(QString::number(i), QVariantMap { { "running", false } });
        }
    }
    return ok;
}

bool GOsgControl::playFly(int index)
{
    QMutexLocker locker(&m_mutex);
    (void)locker;
    if (!m_manipulator.valid()) {
        return false;
    }
    bool ok = m_manipulator->playFly(index);
    if (ok) {
        m_flyIndex = index;
        emit flyIndexChanged();
    }
    return ok;
}

bool GOsgControl::playFlyForName(const QString& name)
{
    int index = -1;
    for (int i = 0; i < m_flyPosList.length(); i++) {
        const QVariantMap& fly = m_flyPosList.at(i).toMap();
        if (fly.value("name").toString() == name) {
            index = i;
            break;
        }
    }
    return playFly(index);
}

bool GOsgControl::stopFly()
{
    QMutexLocker locker(&m_mutex);
    (void)locker;
    if (!m_manipulator.valid()) {
        return false;
    }
    bool ok = m_manipulator->stopFly();
    if (ok) {
        m_flyIndex = -1;
        emit flyIndexChanged();
    }
    return ok;
}

void GOsgControl::playParticle(const QColor& fromColor, const QColor& toColor, double speed)
{
    QMutexLocker locker(&m_mutex);
    (void)locker;
    if (!m_rootNode.valid()) {
        return;
    }
    if (m_particle.valid()) {
        m_particle->play(osg::Vec4(fromColor.redF(), fromColor.greenF(), fromColor.blueF(), fromColor.alphaF()), osg::Vec4(toColor.redF(), toColor.greenF(), toColor.blueF(), toColor.alphaF()), speed);
    }
}

void GOsgControl::stopParticle()
{
    QMutexLocker locker(&m_mutex);
    (void)locker;
    if (!m_rootNode.valid()) {
        return;
    }
    if (m_particle.valid()) {
        m_particle->stop();
    }
}

void GOsgControl::playGrow(const QString& name, const QColor& color)
{
    QMutexLocker locker(&m_mutex);
    (void)locker;
    if (!m_rootNode.valid()) {
        return;
    }
    if (!m_glowNode.valid() || m_glowNode->getName() != name.toStdString()) {
        GNameNodeVisitor nameNodeVisitor;
        m_glowNode = nameNodeVisitor.getNode(m_rootNode, name.toStdString());
    }
    if (m_glowNode.valid()) {
        osg::Vec4d osgColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
        osg::ref_ptr<osg::Material> material;
        material = static_cast<osg::Material*>(m_glowNode->getOrCreateStateSet()->getAttribute(osg::StateAttribute::MATERIAL));
        if (!material.valid()) {
            material = new osg::Material;
            m_glowNode->getOrCreateStateSet()->setAttributeAndModes(material, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
        }
        material->setEmission(osg::Material::FRONT, osgColor);
        material->setShininess(osg::Material::FRONT, 128.0);
        material->setColorMode(osg::Material::AMBIENT);
    }
}

void GOsgControl::stopGrow(const QString& name)
{
    QMutexLocker locker(&m_mutex);
    (void)locker;
    if (!m_rootNode.valid()) {
        return;
    }
    if (!m_glowNode.valid() || m_glowNode->getName() != name.toStdString()) {
        GNameNodeVisitor nameNodeVisitor;
        m_glowNode = nameNodeVisitor.getNode(m_rootNode, name.toStdString());
    }
    if (m_glowNode.valid()) {
        m_glowNode->getOrCreateStateSet()->removeAttribute(osg::StateAttribute::MATERIAL);
    }
}
