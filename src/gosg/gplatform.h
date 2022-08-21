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

#ifndef GPLATFORM_H
#define GPLATFORM_H

#include <osg/Switch>
#include <osg/TextureCubeMap>

namespace GPlatform {

extern osg::ref_ptr<osg::Node> create(double size, int count);

};

#endif // GPLATFORM_H
