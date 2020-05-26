#ifndef ANIMATIONPOOL_H
#define ANIMATIONPOOL_H

#include <deque>

#include "Animation.h"
#include <functional>

class AnimationPool: public Animation
{
public:

    void tick();

    void add(Animation::Ptr newAnimation);

    bool isCompleted();

    void startNextAnimation(Animation::Ptr toStart);

    void setOnAnimationEnd(const std::function<void()>& foreignFunction);

private:

    void onAnimationStart();
    void onAnimationEnd();

    bool areAnyAnimationsLeft();

    Animation::Ptr currentAnimation;
    std::deque<Animation::Ptr> animations;

    std::function<void()> onAnimationEndUserProvidedF;
};

#endif // ANIMATIONPOOL_H
