#include "Animation.h"

bool Animation::isCompleted()
{
    return elapsedSinceStart >= targetDuration;;
}

void Animation::start()
{
    timer.restart();
}

float Animation::getProgressPercentage()
{
    elapsedSinceStart = timer.getElapsedTime();
    return elapsedSinceStart / targetDuration;
}
