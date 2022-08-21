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

#ifndef GPARTICLE_H
#define GPARTICLE_H

#include <osg/MatrixTransform>

class GParticle : public osg::MatrixTransform {
public:
    explicit GParticle(double size);
    ~GParticle();
    void play(const osg::Vec4d& fromColor, const osg::Vec4d& toColor, double speed);
    void stop();

private:
    osg::Object* m_emitter = nullptr;
};

#endif // GPARTICLE_H
