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

#ifndef GMANIPULATOR_H
#define GMANIPULATOR_H

#include <osg/AnimationPath>
#include <osgGA/OrbitManipulator>
#include <osgViewer/Viewer>

class GManipulator : public osgGA::OrbitManipulator {
public:
    using FlyCompletedCallback = std::function<void(int)>;
    explicit GManipulator();
    ~GManipulator();

public:
    inline std::vector<osg::ref_ptr<osg::AnimationPath>> flyList() { return m_flyList; }
    inline void setFlyList(const std::vector<osg::ref_ptr<osg::AnimationPath>>& flyList) { m_flyList = flyList; }
    inline int flyIndex() const { return m_flyIndex; }
    inline void setFlyFinishedCallback(const FlyCompletedCallback& callback) { m_flyFinishedCallback = callback; }
    void setLimit(double maxPosition, double maxDistance, double minDistance);
    std::tuple<osg::Vec3d, osg::Vec3d, osg::Vec3d> getHomePoint() const;
    std::tuple<osg::Vec3d, osg::Quat, osg::Vec3d> getFlyPoint() const;
    bool playFly(int index);
    bool stopFly();

protected:
    inline void setByInverseMatrix(const osg::Matrixd& matrix) override { setByMatrix(osg::Matrixd::inverse(matrix)); }
    inline osg::Matrixd getMatrix() const override { return m_matrix; }
    inline osg::Matrixd getInverseMatrix() const override { return osg::Matrixd::inverse(m_matrix); }
    void setByMatrix(const osg::Matrixd& matrix) override;
    void home(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us) override;
    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us) override;
    bool handleFrame(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us) override;
    virtual bool handleKeyDown(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us) override;
    virtual bool performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy) override;
    virtual bool performMovementMiddleMouseButton(const double eventTimeDelta, const double dx, const double dy) override;
    virtual bool performMovementRightMouseButton(const double eventTimeDelta, const double dx, const double dy) override;

private:
    osg::ref_ptr<osg::AnimationPath> getAnimationPath(int index = -1) const;
    void resetAnimation();
    bool processFlyFrame(const osg::ref_ptr<osg::AnimationPath>& path, double time);

private:
    std::vector<osg::ref_ptr<osg::AnimationPath>> m_flyList;
    osg::Matrixd m_matrix;
    FlyCompletedCallback m_flyFinishedCallback = nullptr;
    int m_flyIndex = -1;
    int m_timeNum = -1;
    double m_eaTime = 0;
    double m_timeOffset = 0;
    double m_timeScale = 1.0;
    double m_realTime = 0;
    double m_animTime = 0;
    double m_maxPosition = 0;
    double m_maxDistance = 0;
    double m_minDistance = 0;
};

#endif // GMANIPULATOR_H
