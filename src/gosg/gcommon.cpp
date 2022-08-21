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

#include "gcommon.h"
#include <iostream>

namespace GCommon {

void printVec3d(const std::string& name, const osg::Vec3d& vec3d)
{
    std::cout << "\"" << name << "\""
              << ": "
              << "\""
              << vec3d.x() << "," << vec3d.y() << "," << vec3d.z()
              << "\"" << std::endl;
}

void printVec4d(const std::string& name, const osg::Vec4d& vec4d)
{
    std::cout << "\"" << name << "\""
              << ": "
              << "\""
              << vec4d.x() << "," << vec4d.y() << "," << vec4d.z() << "," << vec4d.w()
              << "\"" << std::endl;
}
template <typename T>
T getVecForString(const QString& value)
{
    T vec;
    auto list = value.split(",");
    if (list.length() != T::num_components) {
        return T();
    }
    bool ok = false;
    for (int i = 0; i < T::num_components; i++) {
        double t = list[i].trimmed().toDouble(&ok);
        if (!ok) {
            return T();
        }
        vec._v[i] = t;
    }
    return vec;
}

osg::Matrix getMatrix(const QVariantMap& value)
{
    osg::Vec3d translate = getVecForString<osg::Vec3d>(value.value("translate", "0,0,0").toString());
    osg::Vec4d rotate = getVecForString<osg::Vec4d>(value.value("rotate", "0,0,0,1").toString());
    osg::Vec3d scale = getVecForString<osg::Vec3d>(value.value("scale", "1,1,1").toString());
    return osg::Matrix::scale(scale) * osg::Matrix::rotate(rotate) * osg::Matrix::translate(translate);
}

std::tuple<osg::Vec3d, osg::Vec3d, osg::Vec3d> getHomePos(const QVariantMap& value)
{
    std::tuple<osg::Vec3d, osg::Vec3d, osg::Vec3d> pos;
    std::get<0>(pos) = getVecForString<osg::Vec3d>(value.value("eye", "0,0,0").toString());
    std::get<1>(pos) = getVecForString<osg::Vec3d>(value.value("center", "0,0,0").toString());
    std::get<2>(pos) = getVecForString<osg::Vec3d>(value.value("up", "0,0,0").toString());
    return pos;
}

std::vector<osg::ref_ptr<osg::AnimationPath>> getFlyList(const QVariantList& value)
{
    std::vector<osg::ref_ptr<osg::AnimationPath>> pathList;
    for (const auto& pvalue : value) {
        osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;
        path->setLoopMode(osg::AnimationPath::NO_LOOPING);
        path->setName(pvalue.toMap().value("name").toString().toStdString());
        const auto& list = pvalue.toMap().value("path").toList();
        for (const auto& p : list) {
            osg::AnimationPath::ControlPoint pos;
            pos.setPosition(getVecForString<osg::Vec3d>(p.toMap().value("position", "0,0,0").toString()));
            pos.setRotation(getVecForString<osg::Vec4d>(p.toMap().value("rotation", "0,0,0,1").toString()));
            pos.setScale(getVecForString<osg::Vec3d>(p.toMap().value("scale", "1,1,1").toString()));
            path->insert(p.toMap().value("time", 0).toDouble(), pos);
        }
        pathList.push_back(path);
    }
    return pathList;
}

}
