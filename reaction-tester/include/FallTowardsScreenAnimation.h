#ifndef FALLTOWARDSSCREENANIMATION_H
#define FALLTOWARDSSCREENANIMATION_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Animation.h"

struct FallTowardsScreenAnimationParam
{
    float fromScale;
    float toScale;
};

template <class DrawableType>
class FallTowardsScreenAnimation : public Animation
{
public:
    FallTowardsScreenAnimation(DrawableType& target, const sf::Time duration, FallTowardsScreenAnimationParam param):
        Animation(duration), target(target), scaleBeforeStart(param.fromScale), targetScale(param.toScale)
    {
        targetsOriginalColor = target.getColor();
        target.setColor(sf::Color::Transparent);
        target.setScale(scaleBeforeStart, scaleBeforeStart);
    }

    void tick()
    {
        float progress = getProgressPercentage();
        float factor = scaleBeforeStart + ((targetScale - scaleBeforeStart) * progress);
        target.setScale(factor, factor);
    }

protected:

    void onAnimationStart()
    {
        target.setColor(targetsOriginalColor);
    }

    void onAnimationEnd()
    {
    }

private:

    DrawableType& target;
    sf::Color targetsOriginalColor;
    float scaleBeforeStart, targetScale;
};

#endif // FALLTOWARDSSCREENANIMATION_H
