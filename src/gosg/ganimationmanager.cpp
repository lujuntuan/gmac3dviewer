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

#include "ganimationmanager.h"

#define USE_SYMMETRY 1

GAnimationManager::GAnimationManager(const AnimationManagerBase& b)
    : osgAnimation::BasicAnimationManager(b, osg::CopyOp::SHALLOW_COPY)
{
    for (int i = 0; i < (int)_animations.size(); i++) {
        osg::ref_ptr<osgAnimation::Animation> ani = _animations.at(i);
        m_ptrOfIndex.emplace(ani, i);
        m_ptrOfEnd.emplace(ani, -1);
    }
}

GAnimationManager::~GAnimationManager()
{
}

void GAnimationManager::setAnimationFinishedCallback(const AnimationCompletedCallback& callback)
{
    m_animationFinishedCallback = callback;
}

bool GAnimationManager::playAnimation(int index, bool reset, double duration, double end, double start)
{
    if (index < 0 || index >= (int)_animations.size()) {
        return false;
    }
    osg::ref_ptr<osgAnimation::Animation> ani = _animations.at(index);
    if (!ani.valid()) {
        return false;
    }
    double passTime = isPlaying(ani) ? _lastUpdate - ani->getStartTime() : -1;
    double lastDuration = ani->getDuration();
    double lastEndTime = getEndTime(ani);
    double lastStartTime = ani->getStartTime();
    osgAnimation::BasicAnimationManager::playAnimation(ani, 0, 1.0);
    if (start >= 0) {
        ani->setStartTime(_lastUpdate - start);
    } else {
        if (reset) {
            ani->setStartTime(_lastUpdate);
        } else {
            if (passTime >= 0) {
                if (passTime >= end) {
                    double offset = lastDuration - passTime;
                    ani->setStartTime(_lastUpdate - offset);
                } else {
                    double offset = end - passTime;
                    ani->setStartTime(_lastUpdate - offset);
                }
            } else {
                if (lastEndTime >= 0) {
                    double offset = lastEndTime - lastStartTime;
                    if (offset > duration) {
                        offset = 0;
                    }
                    ani->setStartTime(_lastUpdate - offset);
                } else {
                    ani->setStartTime(_lastUpdate);
                }
            }
        }
    }
    if (duration >= 0) {
        ani->setPlayMode(osgAnimation::Animation::ONCE);
        ani->setDuration(duration);
        if (end >= 0) {
            setEndTime(ani, ani->getStartTime() + end);
        } else {
            setEndTime(ani, ani->getStartTime() + duration);
        }
    } else {
        ani->setPlayMode(osgAnimation::Animation::LOOP);
        setEndTime(ani, -1);
    }
    return true;
}

bool GAnimationManager::stopAnimation(int index, bool reset)
{
    if (index < 0 || index >= (int)_animations.size()) {
        return false;
    }
    osg::ref_ptr<osgAnimation::Animation> ani = _animations.at(index);
    if (!ani.valid()) {
        return false;
    }
    osgAnimation::BasicAnimationManager::stopAnimation(ani);
    setEndTime(ani, _lastUpdate);
    if (reset) {
        ani->update(ani->getStartTime());
    }
    return true;
}

bool GAnimationManager::stopAnimationAll(bool reset)
{
    osgAnimation::BasicAnimationManager::stopAll();
    for (const auto& ani : getAnimationList()) {
        setEndTime(ani, _lastUpdate);
        if (reset) {
            ani->update(ani->getStartTime());
        }
    }
    return true;
}

bool GAnimationManager::animationIsPlay(int index)
{
    if (index < 0 || index >= (int)_animations.size()) {
        return false;
    }
    osg::ref_ptr<osgAnimation::Animation> ani = _animations.at(index);
    if (ani.valid()) {
        return isPlaying(ani);
    } else {
        return false;
    }
}

bool GAnimationManager::hasAnyPlaying() const
{
    return !_animationsPlaying.empty();
}

void GAnimationManager::setEndTime(osgAnimation::Animation* animation, double end)
{
    auto it = m_ptrOfEnd.find(animation);
    if (it != m_ptrOfEnd.end()) {
        m_ptrOfEnd.erase(it);
    }
    m_ptrOfEnd.emplace(animation, end);
}

double GAnimationManager::getEndTime(osgAnimation::Animation* animation) const
{
    return m_ptrOfEnd.at(animation);
}

void GAnimationManager::update(double time)
{
    _lastUpdate = time;
    for (TargetSet::iterator it = _targets.begin(); it != _targets.end(); ++it) {
        (*it).get()->reset();
    }
    for (AnimationLayers::reverse_iterator iterAnim = _animationsPlaying.rbegin(); iterAnim != _animationsPlaying.rend(); ++iterAnim) {
        std::vector<int> toremove;
        int priority = iterAnim->first;
        osgAnimation::AnimationList& list = iterAnim->second;
        for (unsigned int i = 0; i < list.size(); i++) {
            int index = m_ptrOfIndex[list[i]];
            double end = getEndTime(list[i]);
            if (!list[i]->update(time, priority) || (end >= 0 && time > end)) {
                setEndTime(list[i], time);
                toremove.push_back(i);
                if (m_animationFinishedCallback) {
                    m_animationFinishedCallback(index);
                }
            }
        }
        while (!toremove.empty()) {
            list.erase(list.begin() + toremove.back());
            toremove.pop_back();
        }
    }
}
