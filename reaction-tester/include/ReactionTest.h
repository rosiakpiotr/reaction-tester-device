#ifndef REACTIONTEST_H
#define REACTIONTEST_H

#include "../utility.h"

class ReactionTest
{
public:
    ReactionTest() = default;

    ReactionTest(int nodeCount, int testingCasesCount, int difficulty);

    int getRandomNodeIndex();

    void tick();

    bool hasTestCasesLeft();

    void startNewCase();

    bool isTestCaseReady();

    bool answer(int index);

    bool hasTestCaseStarted();

    sf::Time answerTime();

    int casesLeftCount();

    std::string timingToFormattedText();

private:

    int nodeCount;
    int testingCasesCount;
    int difficulty;

    bool testCaseReady;
    bool testStarted;
    bool lastAnswerCorrect;

    int currentNodeIndex;
    sf::Time markNodeAfter;

    int casesLeft;
    sf::Time lastTestTiming;

    sf::Clock markingNodeDelayTimer;
};

#endif // REACTIONTEST_H
