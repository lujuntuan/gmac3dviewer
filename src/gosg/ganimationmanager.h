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

#ifndef GANIMATIONMANAGER_H
#define GANIMATIONMANAGER_H

#include <osgAnimation/BasicAnimationManager>
#include <osgViewer/Viewer>

class GAnimationManager : public osgAnimation::BasicAnimationManager {
public:
    using AnimationCompletedCallback = std::function<void(int)>;
    explicit GAnimationManager(const AnimationManagerBase& b);
    ~GAnimationManager();

public:
    inline osgAnimation::AnimationList& getAnimationList() { return _animations; }
    void setAnimationFinishedCallback(const AnimationCompletedCallback& callback);

public:
    bool playAnimation(int index, bool reset = true, double duration = -1, double end = -1, double start = -1);
    bool stopAnimation(int index, bool reset = true);
    bool stopAnimationAll(bool reset = true);
    bool animationIsPlay(int index);
    bool hasAnyPlaying() const;

protected:
    void setEndTime(osgAnimation::Animation* animation, double end);
    double getEndTime(osgAnimation::Animation* animation) const;
    virtual void update(double time) override;

private:
    AnimationCompletedCallback m_animationFinishedCallback = nullptr;
    std::map<osgAnimation::Animation*, int> m_ptrOfIndex;
    std::map<osgAnimation::Animation*, double> m_ptrOfEnd;
};
#endif // GANIMATIONMANAGER_H
