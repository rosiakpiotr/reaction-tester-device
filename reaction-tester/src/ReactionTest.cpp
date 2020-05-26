#include "ReactionTest.h"

ReactionTest::ReactionTest(int nodeCount, int testingCasesCount, int difficulty):
    nodeCount(nodeCount), testingCasesCount(testingCasesCount), difficulty(difficulty)
{
    casesLeft = testingCasesCount;
}

int ReactionTest::getRandomNodeIndex()
{
    return currentNodeIndex;
}

bool ReactionTest::hasTestCasesLeft()
{
    return casesLeft > 0;
}

bool ReactionTest::isTestCaseReady()
{
    return testCaseReady;
}

void ReactionTest::startNewCase()
{
    currentNodeIndex = randomNumberInRange(0, nodeCount);
    markNodeAfter = sf::milliseconds(randomNumberInRange(200, 1000));
    markingNodeDelayTimer.restart();
    testStarted = true;
}

bool ReactionTest::hasTestCaseStarted()
{
    return testStarted;
}

void ReactionTest::tick()
{
    if (markingNodeDelayTimer.getElapsedTime() >= markNodeAfter)
    {
        testCaseReady = true;
    }
}

sf::Time ReactionTest::answerTime()
{
    return markingNodeDelayTimer.getElapsedTime() - markNodeAfter;
}

bool ReactionTest::answer(int index)
{
    bool isCorrect = (index == currentNodeIndex) && (answerTime() > sf::milliseconds(0));
    lastAnswerCorrect = isCorrect;
    casesLeft -= 1;
    testStarted = testCaseReady = false;
    return isCorrect;
}

int ReactionTest::casesLeftCount()
{
    return casesLeft;
}

std::string ReactionTest::timingToFormattedText()
{
    int timingMs = answerTime().asMilliseconds();
    std::string text;

    if (timingMs < 0)
    {
        text = "--";
    }
    else
    {
        text = (lastAnswerCorrect ? "OK - " : "BAD - ") + std::to_string(timingMs) + " ms";
    }

    return text;
}
