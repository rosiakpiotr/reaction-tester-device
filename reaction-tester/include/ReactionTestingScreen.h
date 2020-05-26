#ifndef REACTIONTESTINGSCREEN_H
#define REACTIONTESTINGSCREEN_H

#include "Screen.h"
#include "TestingNode.h"
#include "AnimationPool.h"
#include "FallTowardsScreenAnimation.h"
#include "ReactionTest.h"
#include "TextList.h"

struct ReactionTestConfig
{
    int nodeCount;
    int testCasesCount;
    int difficulty;
    std::vector<std::string> captions;
};

class ReactionTestingScreen : public Screen
{
public:
    ReactionTestingScreen(Resources* res): Screen(res) {}

    void prepare(const ScreenParam& arg);

    void handleEvent(const sf::Event& windowEvent);

    void tick();

    const std::vector<sf::Drawable*> getDrawablesToDraw();

    void enableDrawing();
    void disableDrawing();

private:
    void createBackground();
    void createNodes();
    void createCasesLeftText();
    void createTimingLists();
    void attachAnimations();

    void prepareReactionTest();
    void handleReactionTest();

    void startNewTestCase();

    void selectAndMarkNodeAt(int index);
    void updateCasesLeft(int number);
    void resetAllNodes();

    void holdUpWithNextTextFor(sf::Time howLong);
    bool celebrationsFinished();
    void addTimingToList(int listIndex);

    TestingNode* selectNodeAt(int index);

    void setNodeStatusAtIndex(NodeStatus status, int index);

    void parseArg(const ScreenParam& arg);
    void parseIntParams(const std::vector<int>& argIntParams);
    void parseStringParams(const std::vector<std::string>& argStringParams);

    sf::Sprite background;
    sf::Text casesLeft;
    std::vector<TestingNode> nodes;
    std::vector<TextList> timingLists;

    std::vector<sf::Drawable*> drawables;

    ReactionTestConfig testConfig;

    sf::Clock celebrationTimer;
    sf::Time celebrationDuration;

    AnimationPool animationManager;
    ReactionTest tester;
};

#endif // REACTIONTESTINGSCREEN_H
