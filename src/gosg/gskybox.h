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

#ifndef GSKYBOX_H
#define GSKYBOX_H

#include <osg/Node>

namespace GSkyBox {

extern osg::ref_ptr<osg::Node> create(const std::string& dir, double size);

};

#endif // GSKYBOX_H
