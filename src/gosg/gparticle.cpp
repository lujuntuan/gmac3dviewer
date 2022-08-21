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

#include "gparticle.h"
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Switch>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ModularProgram>
#include <osgParticle/ParticleSystemUpdater>

GParticle::GParticle(double size)
{
    osgParticle::Particle particleTempalte;
    particleTempalte.setShape(osgParticle::Particle::QUAD);
    particleTempalte.setLifeTime(5);
    particleTempalte.setSizeRange(osgParticle::rangef(size / 50, size / 5));
    particleTempalte.setColorRange(osgParticle::rangev4(osg::Vec4d(1, 1, 1, 1), osg::Vec4d(1, 1, 1, 1)));
    particleTempalte.setMass(0.01f);
    particleTempalte.setRadius(0.1f);
    osg::ref_ptr<osgParticle::ParticleSystem> particleSystem = new osgParticle::ParticleSystem;
    particleSystem->setDataVariance(osg::Node::STATIC);
    particleSystem->setDefaultAttributes("./sources/particle/sphere.png", false, true);
    particleSystem->setDefaultParticleTemplate(particleTempalte);
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    particleSystem->setUseVertexBufferObjects(true);
    geode->addDrawable(particleSystem);
    this->addChild(geode);
    osg::ref_ptr<osgParticle::RandomRateCounter> particleGenerateRate = new osgParticle::RandomRateCounter();
    particleGenerateRate->setRateRange(1, 10);
    osg::ref_ptr<osgParticle::RadialShooter> particleShooter = new osgParticle::RadialShooter;
    particleShooter->setThetaRange(0.3, 0.6);
    particleShooter->setInitialSpeedRange(1, size / 10);
    osg::ref_ptr<osgParticle::ModularEmitter> emitter = new osgParticle::ModularEmitter;
    emitter->setParticleSystem(particleSystem);
    emitter->setCounter(particleGenerateRate);
    emitter->setShooter(particleShooter);
    this->addChild(emitter);
    osg::ref_ptr<osgParticle::ParticleSystemUpdater> particleSystemUpdater = new osgParticle::ParticleSystemUpdater;
    particleSystemUpdater->addParticleSystem(particleSystem);
    this->addChild(particleSystemUpdater);
    osg::ref_ptr<osgParticle::ModularProgram> particleMoveProgram = new osgParticle::ModularProgram;
    particleMoveProgram->setParticleSystem(particleSystem);
    this->addChild(particleMoveProgram);
    //
    emitter->setEnabled(false);
    m_emitter = emitter;
}

GParticle::~GParticle()
{
}

void GParticle::play(const osg::Vec4d& fromColor, const osg::Vec4d& toColor, double speed)
{
    osgParticle::ModularEmitter* emitter = static_cast<osgParticle::ModularEmitter*>(m_emitter);
    if (emitter) {
        double size = emitter->getParticleSystem()->getDefaultParticleTemplate().getSizeRange().maximum;
        emitter->getParticleSystem()->getDefaultParticleTemplate().setColorRange(osgParticle::rangev4(fromColor, toColor));
        osgParticle::RandomRateCounter* particleGenerateRate = static_cast<osgParticle::RandomRateCounter*>(emitter->getCounter());
        if (particleGenerateRate) {
            particleGenerateRate->setRateRange(1, speed);
        }
        osgParticle::RadialShooter* particleShooter = static_cast<osgParticle::RadialShooter*>(emitter->getShooter());
        if (particleShooter) {
            particleShooter->setInitialSpeedRange(1, size / 2 * speed / 10);
        }
        emitter->setEnabled(true);
    }
}

void GParticle::stop()
{
    osgParticle::ModularEmitter* emitter = static_cast<osgParticle::ModularEmitter*>(m_emitter);
    if (emitter) {
        emitter->setEnabled(false);
    }
}
