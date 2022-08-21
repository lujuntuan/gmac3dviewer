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

#ifndef GLIGHT_H
#define GLIGHT_H

#include <osg/Node>

namespace GLight {

extern osg::ref_ptr<osg::Node> create(osg::Camera* camera, double distance);

};

#endif // GLIGHT_H
