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

#ifndef GOSGCONTROL_H
#define GOSGCONTROL_H

#include "ganimationmanager.h"
#include "gcoord.h"
#include "glight.h"
#include "gmanipulator.h"
#include "gnodevisitor.h"
#include "gparticle.h"
#include "gplatform.h"
#include "gskybox.h"
#include <QColor>
#include <QMutex>
#include <QObject>
#include <QQmlParserStatus>
#include <QUrl>
#include <QVariantMap>

class GOsgControl : public QObject, public QQmlParserStatus {
    Q_OBJECT
    Q_DISABLE_COPY(GOsgControl)
    Q_PROPERTY(QUrl rootNode READ rootNode WRITE setRootNode NOTIFY rootNodeChanged)
    Q_PROPERTY(QStringList animationList READ animationList NOTIFY animationListChanged)
    Q_PROPERTY(QVariantMap homePos READ homePos WRITE setHomePos NOTIFY homePosChanged)
    Q_PROPERTY(QVariantList flyPosList READ flyPosList WRITE setFlyPosList NOTIFY flyPosListChanged)
    Q_PROPERTY(QVariantMap animationsStatus READ animationsStatus NOTIFY animationsStatusChanged)
    Q_PROPERTY(QVariantMap rootNodeMatrix READ rootNodeMatrix WRITE setRootNodeMatrix NOTIFY rootNodeMatrixChanged)
    Q_PROPERTY(QVariantMap particleMatrix READ particleMatrix WRITE setParticleMatrix NOTIFY particleMatrixChanged)
    Q_PROPERTY(int flyIndex READ flyIndex NOTIFY flyIndexChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
    Q_PROPERTY(bool hasError READ hasError NOTIFY hasErrorChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
public:
    explicit GOsgControl(QObject* parent = 0);
    ~GOsgControl();

protected:
    virtual void classBegin() override;
    virtual void componentComplete() override;

public:
    inline QUrl rootNode() const { return m_rootNodeUrl; }
    inline QStringList animationList() const { return m_animationList; }
    inline QVariantMap homePos() const { return m_homePos; }
    inline QVariantList flyPosList() const { return m_flyPosList; }
    inline QVariantMap animationsStatus() const { return m_animationsStatus; }
    inline QVariantMap rootNodeMatrix() const { return m_rootNodeMatrix; }
    inline QVariantMap particleMatrix() const { return m_particleMatrix; }
    inline int flyIndex() const { return m_flyIndex; }
    inline bool loading() const { return m_loading; }
    inline bool hasError() const { return m_hasError; }
    inline QString errorMessage() const { return m_errorMessage; }
    void setRootNode(const QUrl& rootNodeUrl);
    void setHomePos(const QVariantMap& homePos);
    void setFlyPosList(const QVariantList& flyPosList);
    void setRootNodeMatrix(const QVariantMap& rootNodeMatrix);
    void setParticleMatrix(const QVariantMap& particleMatrix);

public:
    void init(osgViewer::Viewer* viewer);
    bool checkFrameAllowed();
    void requestDestroy();

public slots:
    QUrl getUrlForLocal(const QString& path);
    bool playAnimation(int index, bool reset = true, double duration = -1, double end = -1, double start = -1);
    bool playAnimationForName(const QString& name, bool reset = true, double duration = -1, double end = -1, double start = -1);
    bool stopAnimation(int index, bool reset = true);
    bool stopAnimationForName(const QString& name, bool reset = true);
    bool stopAnimationAll(bool reset = true);
    bool playFly(int index);
    bool playFlyForName(const QString& name);
    bool stopFly();
    void playParticle(const QColor& fromColor, const QColor& toColor, double speed);
    void stopParticle();
    void playGrow(const QString& name, const QColor& color);
    void stopGrow(const QString& name);

private:
    osgViewer::Viewer* m_viewer = nullptr;
    QThread* m_loadThread = nullptr;
    QUrl m_rootNodeUrl;
    QStringList m_animationList;
    QVariantMap m_homePos;
    QVariantList m_flyPosList;
    QVariantMap m_animationsStatus;
    QVariantMap m_rootNodeMatrix;
    QVariantMap m_particleMatrix;
    osg::ref_ptr<osg::Group> m_rootGroup;
    osg::ref_ptr<osg::MatrixTransform> m_rootNodeGroup;
    osg::ref_ptr<osg::Node> m_rootNode;
    osg::ref_ptr<osg::Node> m_skyNode;
    osg::ref_ptr<osg::Node> m_platformNode;
    osg::ref_ptr<osg::Node> m_lightNode;
    osg::ref_ptr<osg::Node> m_glowNode;
    osg::ref_ptr<GParticle> m_particle;
    osg::ref_ptr<GManipulator> m_manipulator;
    osg::ref_ptr<GAnimationManager> m_animationManager;
    osg::Vec3d m_platformTranslate;
    int m_flyIndex = -1;
    bool m_loading = false;
    bool m_hasError = false;
    bool m_requestDestroy = false;
    QString m_errorMessage;
    QMutex m_mutex;

signals:
    void rootNodeChanged();
    void animationListChanged();
    void homePosChanged();
    void flyPosListChanged();
    void animationsStatusChanged();
    void rootNodeMatrixChanged();
    void particleMatrixChanged();
    void flyIndexChanged();
    void loadingChanged();
    void hasErrorChanged();
    void errorMessageChanged();
};

#endif // GOSGCONTROL_H
