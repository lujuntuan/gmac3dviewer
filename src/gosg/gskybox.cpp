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
#include <osg/Depth>
#include <osg/ShapeDrawable>
#include <osg/Switch>
#include <osg/TexEnv>
#include <osg/TexGen>
#include <osg/TexMat>
#include <osg/TextureCubeMap>
#include <osgDB/ReadFile>
#include <osgUtil/CullVisitor>

namespace GSkyBox {

osg::ref_ptr<osg::Node> create(const std::string& dir, double size)
{
    class TexMatCallback : public osg::NodeCallback {
    public:
        explicit TexMatCallback(const osg::ref_ptr<osg::TexMat>& texMat)
            : m_texMat(texMat)
        {
        }
        virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
        {
            osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
            if (cv) {
                const osg::Matrix& mv = *(cv->getModelViewMatrix());
                const osg::Matrix& r = osg::Matrix::rotate(osg::DegreesToRadians(112.0f), 0.0f, 0.0f, 1.0f) * osg::Matrix::rotate(osg::DegreesToRadians(90.0f), 1.0f, 0.0f, 0.0f);
                const osg::Quat& q = mv.getRotate();
                const osg::Matrix& c = osg::Matrix::rotate(q.inverse());
                if (m_texMat) {
                    m_texMat->setMatrix(c * r);
                }
            }
            traverse(node, nv);
        }

    private:
        osg::ref_ptr<osg::TexMat> m_texMat;
    };
    class MoveEarthySkyTransform : public osg::Transform {
        virtual bool computeLocalToWorldMatrix(osg::Matrix& matrix, osg::NodeVisitor* nv) const
        {
            osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
            if (cv) {
                osg::Vec3 eyePointLocal = cv->getEyeLocal();
                matrix.preMult(osg::Matrix::translate(eyePointLocal));
            }
            return true;
        }
        virtual bool computeWorldToLocalMatrix(osg::Matrix& matrix, osg::NodeVisitor* nv) const
        {
            osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
            if (cv) {
                osg::Vec3 eyePointLocal = cv->getEyeLocal();
                matrix.postMult(osg::Matrix::translate(-eyePointLocal));
            }
            return true;
        }
    };
    auto readCubeMap = [](const std::string& dir) -> osg::ref_ptr<osg::TextureCubeMap> {
        osg::ref_ptr<osg::TextureCubeMap> cubemap = new osg::TextureCubeMap;
        osg::ref_ptr<osg::Image> imagePosX = osgDB::readImageFile(dir + "/right.jpg");
        osg::ref_ptr<osg::Image> imageNegX = osgDB::readImageFile(dir + "/left.jpg");
        osg::ref_ptr<osg::Image> imagePosY = osgDB::readImageFile(dir + "/bottom.jpg");
        osg::ref_ptr<osg::Image> imageNegY = osgDB::readImageFile(dir + "/top.jpg");
        osg::ref_ptr<osg::Image> imagePosZ = osgDB::readImageFile(dir + "/front.jpg");
        osg::ref_ptr<osg::Image> imageNegZ = osgDB::readImageFile(dir + "/back.jpg");
        if (imagePosX && imageNegX && imagePosY && imageNegY && imagePosZ && imageNegZ) {
            cubemap->setImage(osg::TextureCubeMap::POSITIVE_X, imagePosX);
            cubemap->setImage(osg::TextureCubeMap::NEGATIVE_X, imageNegX);
            cubemap->setImage(osg::TextureCubeMap::POSITIVE_Y, imagePosY);
            cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Y, imageNegY);
            cubemap->setImage(osg::TextureCubeMap::POSITIVE_Z, imagePosZ);
            cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Z, imageNegZ);
            cubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
            cubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
            cubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);
            cubemap->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
            cubemap->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
        }
        return cubemap;
    };
    osg::ref_ptr<osg::StateSet> stateSet = new osg::StateSet();
    osg::ref_ptr<osg::TexEnv> te = new osg::TexEnv;
    te->setMode(osg::TexEnv::REPLACE);
    stateSet->setTextureAttributeAndModes(0, te, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    osg::ref_ptr<osg::TexGen> tg = new osg::TexGen;
    tg->setMode(osg::TexGen::NORMAL_MAP);
    stateSet->setTextureAttributeAndModes(0, tg, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    osg::ref_ptr<osg::TexMat> tm = new osg::TexMat;
    stateSet->setTextureAttribute(0, tm);
    osg::ref_ptr<osg::TextureCubeMap> skymap = readCubeMap(dir);
    stateSet->setTextureAttributeAndModes(0, skymap, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    stateSet->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);
    osg::ref_ptr<osg::Depth> depth = new osg::Depth;
    depth->setFunction(osg::Depth::ALWAYS);
    depth->setRange(1.0, 10.0);
    stateSet->setAttributeAndModes(depth, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    stateSet->setRenderBinDetails(-1, "RenderBin");
    osg::ref_ptr<osg::Drawable> drawable = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0, 0, 0), size));
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->setCullingActive(false);
    geode->setStateSet(stateSet);
    drawable->setUseVertexBufferObjects(true);
    geode->addDrawable(drawable);
    osg::ref_ptr<osg::Transform> transform = new MoveEarthySkyTransform();
    transform->setCullingActive(false);
    transform->addChild(geode);
    osg::ref_ptr<osg::ClearNode> clearNode = new osg::ClearNode;
    clearNode->setCullCallback(new TexMatCallback(tm));
    clearNode->addChild(transform);
    osg::ref_ptr<osg::Switch> switchNode = new osg::Switch();
    switchNode->addChild(clearNode, true);
    return switchNode;
}

};
