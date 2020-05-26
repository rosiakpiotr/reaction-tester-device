#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/System.hpp>
#include <memory>

class Animation
{
public:
    Animation() = default;
    Animation(const sf::Time duration): targetDuration(duration) {}

    virtual void tick() = 0;

    bool isCompleted();

    void start();

    typedef std::shared_ptr<Animation> Ptr;

    virtual void onAnimationStart() = 0;
    virtual void onAnimationEnd() = 0;

protected:
    float getProgressPercentage();

private:
    sf::Time targetDuration;
    sf::Time elapsedSinceStart;
    sf::Clock timer;
    bool isAnimationFinished;
};

#endif // ANIMATION_H
