#include "AnimationPool.h"

void AnimationPool::tick()
{
    if(currentAnimation->isCompleted())
    {
        currentAnimation->onAnimationEnd();

        if (areAnyAnimationsLeft())
        {
            currentAnimation = animations.back();
            animations.pop_back();
            startNextAnimation(currentAnimation);
        }
    }
    else
    {
        currentAnimation->tick();
    }
}

void AnimationPool::startNextAnimation(Animation::Ptr animation)
{
    animation->start();
    animation->onAnimationStart();
}

bool AnimationPool::areAnyAnimationsLeft()
{
    return !animations.empty();
}

void AnimationPool::add(Animation::Ptr newAnimation)
{
    animations.push_back(newAnimation);
    currentAnimation = newAnimation;
}

bool AnimationPool::isCompleted()
{
    bool completed = animations.front()->isCompleted();
    if (completed)
    {
        if (static bool do_once; !std::exchange(do_once, true))
            onAnimationEnd();
    }
    return completed;
}

void AnimationPool::setOnAnimationEnd(const std::function<void()>& foreignFunction)
{
    onAnimationEndUserProvidedF = foreignFunction;
}


void AnimationPool::onAnimationStart()
{
    /* broken, doesnt call when needed */
}

void AnimationPool::onAnimationEnd()
{
    onAnimationEndUserProvidedF();
}
