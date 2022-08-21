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

#ifndef GCOMMON_H
#define GCOMMON_H

#include <QDebug>
#include <QVariantMap>
#include <osg/AnimationPath>
#include <osg/Matrix>
#include <osg/Quat>
#include <osg/Vec3d>

namespace GCommon {

extern void printVec3d(const std::string& name, const osg::Vec3d& vec3d);
extern void printVec4d(const std::string& name, const osg::Vec4d& vec4d);
extern osg::Matrix getMatrix(const QVariantMap& value);
extern std::tuple<osg::Vec3d, osg::Vec3d, osg::Vec3d> getHomePos(const QVariantMap& value);
extern std::vector<osg::ref_ptr<osg::AnimationPath>> getFlyList(const QVariantList& value);

}

#endif // GCOMMON_H
