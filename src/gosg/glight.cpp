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

#include "glight.h"
#include <osg/Camera>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/MatrixTransform>

namespace GLight {

static osg::ref_ptr<osg::Node> createLightSource(unsigned int num, const osg::Vec3d& trans, const osg::Vec4d& color, float attenuation)
{
    osg::ref_ptr<osg::Light> light = new osg::Light;
    light->setLightNum(num);
    light->setDiffuse(color);
    light->setPosition(osg::Vec4d(0.0, 0.0, 0.0, 1.0));
    light->setSpecular(osg::Vec4d(0.9, 0.9, 0.6, 1.0));
    light->setConstantAttenuation(attenuation);
    osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource;
    lightSource->setLocalStateSetModes(osg::StateAttribute::ON);
    lightSource->setLight(light);
    osg::ref_ptr<osg::MatrixTransform> sourceTrans = new osg::MatrixTransform;
    sourceTrans->setMatrix(osg::Matrix::translate(trans));
    sourceTrans->addChild(lightSource);
    return sourceTrans;
}

osg::ref_ptr<osg::Node> create(osg::Camera* camera, double distance)
{
    osg::ref_ptr<osg::Group> lightGroup = new osg::Group;
    osg::ref_ptr<osg::Node> light0 = createLightSource(0, osg::Vec3d(0.0, -distance * 2, distance), osg::Vec4d(1.0, 1.0, 0.90, 1.0), 1.5f);
    osg::ref_ptr<osg::Node> light1 = createLightSource(1, osg::Vec3d(0.0, distance * 2, distance), osg::Vec4d(1.0, 1.0, 0.90, 1.0), 1.5f);
    osg::ref_ptr<osg::Node> light2 = createLightSource(2, osg::Vec3d(-distance * 2, 0, distance), osg::Vec4d(1.0, 1.0, 0.90, 1.0), 1.5f);
    osg::ref_ptr<osg::Node> light3 = createLightSource(3, osg::Vec3d(distance * 2, 0, distance), osg::Vec4d(1.0, 1.0, 0.90, 1.0), 1.5f);

    camera->getStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    camera->getStateSet()->setMode(GL_LIGHT0, osg::StateAttribute::ON);
    camera->getStateSet()->setMode(GL_LIGHT1, osg::StateAttribute::ON);
    camera->getStateSet()->setMode(GL_LIGHT2, osg::StateAttribute::ON);
    camera->getStateSet()->setMode(GL_LIGHT3, osg::StateAttribute::ON);
    lightGroup->addChild(light0);
    lightGroup->addChild(light1);
    lightGroup->addChild(light2);
    lightGroup->addChild(light3);
    return lightGroup;
}

}
