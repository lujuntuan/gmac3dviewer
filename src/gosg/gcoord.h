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

#ifndef GCOORD_H
#define GCOORD_H

#include <osg/Camera>

class GCoord : public osg::Camera {
public:
    explicit GCoord(osg::Camera* camera);
    ~GCoord();

protected:
    virtual void traverse(osg::NodeVisitor& nv) override;

private:
    osg::ref_ptr<osg::Camera> m_camera;
    osg::ref_ptr<osg::MatrixTransform> m_axis;
};

#endif // GCOORD_H
