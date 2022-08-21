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

#include "gskybox.h"
#include <functional>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osg/Multisample>

namespace GPlatform {

osg::ref_ptr<osg::Node> create(double size, int count)
{
    double pm = size / count;
    double min = size / 10000.0;
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    if (size <= 0 || count <= 0) {
        return geode;
    }
    {
        osg::ref_ptr<osg::Geometry> lineGeo = new osg::Geometry();
        osg::ref_ptr<osg::Vec3Array> lineVertex = new osg::Vec3Array();
        osg::ref_ptr<osg::LineWidth> lineLength = new osg::LineWidth(2);
        osg::ref_ptr<osg::DrawArrays> linePrimitive = new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, (count + 1) * 4);
        osg::ref_ptr<osg::Vec4dArray> lineColor = new osg::Vec4dArray();
        osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
        for (int x = 0; x <= count; x++) {
            lineVertex->push_back(osg::Vec3(-size / 2 + x * pm, -size / 2, 0));
            lineVertex->push_back(osg::Vec3(-size / 2 + x * pm, size / 2, 0));
        }
        for (int y = 0; y <= count; y++) {
            lineVertex->push_back(osg::Vec3(-size / 2, -size / 2 + y * pm, 0));
            lineVertex->push_back(osg::Vec3(size / 2, -size / 2 + y * pm, 0));
        }
        lineColor->push_back(osg::Vec4d(0.6, 0.6, 0.6, 0.2));
        normals->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
        lineGeo->setVertexArray(lineVertex);
        lineGeo->getOrCreateStateSet()->setAttribute(lineLength, osg::StateAttribute::ON);
        lineGeo->addPrimitiveSet(linePrimitive);
        lineGeo->setColorArray(lineColor);
        lineGeo->setColorBinding(osg::Geometry::BIND_OVERALL);
        lineGeo->setNormalArray(normals);
        lineGeo->setNormalBinding(osg::Geometry::BIND_OVERALL);
        lineGeo->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
        lineGeo->setUseVertexBufferObjects(true);
        geode->addDrawable(lineGeo);
    }
    {
        osg::ref_ptr<osg::Geometry> rectGeo = new osg::Geometry();
        osg::ref_ptr<osg::Vec3Array> rectVertex = new osg::Vec3Array();
        osg::ref_ptr<osg::DrawArrays> rectPrimitive = new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4);
        osg::ref_ptr<osg::Vec4dArray> rectColor = new osg::Vec4dArray();
        osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
        rectVertex->push_back(osg::Vec3(-size / 2, -size / 2, min));
        rectVertex->push_back(osg::Vec3(-size / 2, size / 2, min));
        rectVertex->push_back(osg::Vec3(size / 2, size / 2, min));
        rectVertex->push_back(osg::Vec3(size / 2, -size / 2, min));
        rectColor->push_back(osg::Vec4d(0.0, 0.0, 0.0, 0.5));
        normals->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
        rectGeo->setVertexArray(rectVertex);
        rectGeo->addPrimitiveSet(rectPrimitive);
        rectGeo->setColorArray(rectColor);
        rectGeo->setColorBinding(osg::Geometry::BIND_OVERALL);
        rectGeo->setNormalArray(normals);
        rectGeo->setNormalBinding(osg::Geometry::BIND_OVERALL);
        rectGeo->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
        rectGeo->setUseVertexBufferObjects(true);
        geode->addDrawable(rectGeo);
    }
    return geode;
}

};
