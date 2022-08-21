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

#include "gmanipulator.h"
#include "gcommon.h"
#include <iostream>

#define FIRST_CONTROLPOINT_TIME 1

GManipulator::GManipulator()
    : osgGA::OrbitManipulator(DEFAULT_SETTINGS)
{
    setAllowThrow(false);
    setVerticalAxisFixed(false);
}

GManipulator::~GManipulator()
{
}

void GManipulator::setLimit(double maxPosition, double maxDistance, double minDistance)
{
    m_maxPosition = maxPosition;
    m_maxDistance = maxDistance;
    m_minDistance = minDistance;
}

std::tuple<osg::Vec3d, osg::Vec3d, osg::Vec3d> GManipulator::getHomePoint() const
{
    std::tuple<osg::Vec3d, osg::Vec3d, osg::Vec3d> homePoint;
    getTransformation(std::get<0>(homePoint), std::get<1>(homePoint), std::get<2>(homePoint));
    return homePoint;
}

std::tuple<osg::Vec3d, osg::Quat, osg::Vec3d> GManipulator::getFlyPoint() const
{
    std::tuple<osg::Vec3d, osg::Quat, osg::Vec3d> flyPoint;
    getTransformation(std::get<0>(flyPoint), std::get<1>(flyPoint));
    std::get<2>(flyPoint) = osg::Vec3d(1, 1, 1);
    return flyPoint;
}

bool GManipulator::playFly(int index)
{
    if (!getAnimationPath(index).valid()) {
        return false;
    }
    m_flyIndex = index;
    const auto& p = getFlyPoint();
    osg::AnimationPath::ControlPoint point;
    point.setPosition(std::get<0>(p));
    point.setRotation(std::get<1>(p));
    point.setScale(std::get<2>(p));
    auto it = getAnimationPath()->getTimeControlPointMap().find(FIRST_CONTROLPOINT_TIME);
    if (it != getAnimationPath()->getTimeControlPointMap().end()) {
        getAnimationPath()->getTimeControlPointMap().erase(it);
    }
    getAnimationPath()->insert(FIRST_CONTROLPOINT_TIME, point);
    resetAnimation();
    return true;
}

bool GManipulator::stopFly()
{
    if (!getAnimationPath().valid()) {
        m_flyIndex = -1;
        return false;
    }
    m_flyIndex = -1;
    return true;
}

void GManipulator::setByMatrix(const osg::Matrixd& matrix)
{
    _center = osg::Vec3d(0, 0, -_distance) * matrix;
    _rotation = matrix.getRotate();
    _distance = matrix.getTrans().length();
}

void GManipulator::home(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
    resetAnimation();
    osgGA::OrbitManipulator::home(ea, us);
}

bool GManipulator::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
    osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&us);
    if (ea.getEventType() == osgGA::GUIEventAdapter::DOUBLECLICK) {
        if (viewer) {
            osgUtil::LineSegmentIntersector::Intersections intersections;
            viewer->computeIntersections(ea.getX(), ea.getY(), intersections);
            if (intersections.size() > 0) {
                const auto& p = *(intersections.begin());
                GCommon::printVec3d("pick-point", p.getWorldIntersectPoint());
            }
        }
    }
    osg::Vec3d oldCenter = _center;
    osg::Quat oldRotation;
    oldRotation = _rotation;
    double oldDistance = _distance;
    bool reval = osgGA::OrbitManipulator::handle(ea, us);
    if (_center.length() > m_maxPosition || _distance > m_maxDistance || _distance < m_minDistance) {
        _center = oldCenter;
        _rotation = oldRotation;
        _distance = oldDistance;
    }
    return reval;
}

bool GManipulator::handleFrame(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
    if (!std::isnan(_distance)) {
        m_matrix = osg::Matrixd::translate(0, 0, _distance) * osg::Matrixd::rotate(_rotation) * osg::Matrixd::translate(_center);
    }
    m_eaTime = ea.getTime();
    if (getAnimationPath().valid()) {
        return processFlyFrame(getAnimationPath(), ea.getTime());
    }
    return osgGA::OrbitManipulator::handleFrame(ea, us);
}

bool GManipulator::handleKeyDown(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
    if (ea.getKey() == osgGA::GUIEventAdapter::KEY_F5) {
        const auto& homePoint = getHomePoint();
        const auto& flyPoint = getFlyPoint();
        std::cout << "***********HomePoint***********" << std::endl;
        GCommon::printVec3d("eye", std::get<0>(homePoint));
        GCommon::printVec3d("center", std::get<1>(homePoint));
        GCommon::printVec3d("up", std::get<2>(homePoint));
        std::cout << "***********FlyPoint***********" << std::endl;
        GCommon::printVec3d("position", std::get<0>(flyPoint));
        GCommon::printVec4d("rotation", std::get<1>(flyPoint).asVec4());
        GCommon::printVec3d("scale", std::get<2>(flyPoint));
    }
    return osgGA::OrbitManipulator::handleKeyDown(ea, us);
}

bool GManipulator::performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
    float scale = -0.3f * _distance * getThrowScale(eventTimeDelta);
    panModel(dx * scale, dy * scale, 0);
    return true;
}

bool GManipulator::performMovementMiddleMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
    (void)dx;
    zoomModel(dy * getThrowScale(eventTimeDelta), true);
    return true;
}

bool GManipulator::performMovementRightMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
    osg::Matrix rotation_matrix;
    rotation_matrix.makeRotate(_rotation);
    osg::Vec3d sideVector = getSideVector(rotation_matrix);
    osg::Vec3d localUp(0.0, 0.0, 1.0);
    osg::Vec3d forwardVector = localUp ^ sideVector;
    sideVector = forwardVector ^ localUp;
    forwardVector.normalize();
    sideVector.normalize();
    osg::Quat newRotatex;
    newRotatex.makeRotate(dy, sideVector);
    osg::Quat newRotatey;
    newRotatey.makeRotate(-dx, localUp);
    _rotation = _rotation * newRotatex * newRotatey;
    _center = newRotatex * newRotatey * _center;
    return true;
}

osg::ref_ptr<osg::AnimationPath> GManipulator::getAnimationPath(int index) const
{
    if (index < 0) {
        index = m_flyIndex;
    }
    if (index >= 0 && index < m_flyList.size()) {
        return m_flyList.at(index);
    }
    return osg::ref_ptr<osg::AnimationPath>();
}

void GManipulator::resetAnimation()
{
    if (getAnimationPath().valid()) {
        m_timeOffset = getAnimationPath()->getFirstTime() - m_eaTime;
    } else {
        m_timeOffset = 0;
    }
    m_timeNum = -1;
}

bool GManipulator::processFlyFrame(const osg::ref_ptr<osg::AnimationPath>& path, double time)
{
    osg::AnimationPath::ControlPoint cp;
    double animTime = (time + m_timeOffset) * m_timeScale;
    path->getInterpolatedControlPoint(animTime, cp);
    if (m_timeNum == -1) {
        m_realTime = time;
        m_animTime = animTime;
    }
    m_timeNum++;
    double animDelta = (animTime - m_animTime);
    if (animDelta >= path->getPeriod()) {
        m_flyIndex = -1;
        if (m_flyFinishedCallback) {
            m_flyFinishedCallback(m_flyIndex);
        }
        m_realTime = time;
        m_animTime = animTime;
        m_timeNum = 0;
    }
    cp.getMatrix(m_matrix);
    setByMatrix(m_matrix);
    return false;
}
