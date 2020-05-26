#include "ReactionTestingScreen.h"
#include "ColorfulLogger.h"

void ReactionTestingScreen::prepare(const ScreenParam& arg)
{
    parseArg(arg);

    createBackground();
    createNodes();
    createCasesLeftText();
    createTimingLists();

    prepareReactionTest();

    enableDrawing();

    attachAnimations();

    animationManager.setOnAnimationEnd([&](){startNewTestCase();});
    animationManager.start();
}

void ReactionTestingScreen::parseArg(const ScreenParam& arg)
{
    parseIntParams(arg.intParams);
    parseStringParams(arg.stringParams);
}

void ReactionTestingScreen::parseIntParams(const std::vector<int>& intParams)
{
    testConfig.nodeCount = intParams.at(0);
    testConfig.testCasesCount = intParams.at(1);
    testConfig.difficulty = intParams.at(2);
}

void ReactionTestingScreen::parseStringParams(const std::vector<std::string>& stringParams)
{
    // First 'testConfig.nodeCount' elements in stringParams are captions for nodes.
    for (int i = 0; i < testConfig.nodeCount; i++)
    {
        testConfig.captions.push_back(stringParams.at(i));
    }
}

void ReactionTestingScreen::prepareReactionTest()
{
    tester = ReactionTest(
        testConfig.nodeCount,
        testConfig.testCasesCount,
        testConfig.difficulty);
}

void ReactionTestingScreen::createTimingLists()
{
    for (TestingNode& node : nodes)
    {
        TextList newTimingList(*resources->getFontRawPtr("basic"));

        sf::Vector2f nodePosition = leftTopCornerOfDrawableWithCenterOrigin(node);
        newTimingList.setPosition(sf::Vector2f(nodePosition.x, constants::window::height * 0.5));

        timingLists.push_back(newTimingList);
    }
}

void ReactionTestingScreen::createCasesLeftText()
{
    casesLeft.setFont(*resources->getFontRawPtr("basic"));
    casesLeft.setFillColor(sf::Color::Black);
    casesLeft.setPosition(5, 5);
}

void ReactionTestingScreen::updateCasesLeft(int number)
{
    casesLeft.setString("Cases left: " + std::to_string(number));
}

void ReactionTestingScreen::attachAnimations()
{
    for (auto it = nodes.rbegin(); it != nodes.rend(); ++it)
    {
        TestingNode& node = *it;

        FallTowardsScreenAnimationParam param;
        param.fromScale = 4;
        param.toScale = 1;

        Animation::Ptr nodeAnim(
            new FallTowardsScreenAnimation<TestingNode>(node, sf::milliseconds(130), param));

        animationManager.add(nodeAnim);
    }
}

void ReactionTestingScreen::createBackground()
{
    background.setTexture(*resources->getTextureRawPtr("background"));
}

void ReactionTestingScreen::createNodes()
{
    int differenceDistance = constants::window::width / testConfig.nodeCount;
    int firstNodeXPos = differenceDistance / 2.f;
    int nodesYPos = constants::window::height * 0.3;

    int index = 0;
    for(const std::string& letter: testConfig.captions)
    {
        const sf::Texture* nodeTexture = resources->getTextureRawPtr("testingNode");
        const sf::Font* captionFont = resources->getFontRawPtr("testingNodeCaptionFont");

        TestingNode newNode(nodeTexture, captionFont, letter);

        sf::Vector2f nodePos(firstNodeXPos + differenceDistance * index++, nodesYPos);
        newNode.setPosition(nodePos);

        nodes.push_back(newNode);
    }
}

void ReactionTestingScreen::enableDrawing()
{
    drawables.push_back(&background);

    for (TestingNode& node: nodes)
    {
        drawables.push_back(&node);
    }

    for (TextList& timingList: timingLists)
    {
        drawables.push_back(&timingList);
    }

    drawables.push_back(&casesLeft);
}

void ReactionTestingScreen::disableDrawing()
{
    drawables.clear();
}

void ReactionTestingScreen::holdUpWithNextTextFor(sf::Time howLong)
{
    celebrationDuration = howLong;
    celebrationTimer.restart();
}

void ReactionTestingScreen::handleEvent(const sf::Event& windowEvent)
{
    if (windowEvent.type == sf::Event::KeyPressed && tester.hasTestCaseStarted())
    {
        int index = indexOfAnswerInCaptions(windowEvent.key.code, testConfig.captions);
        bool isCorrect = tester.answer(index);

        if (index > testConfig.nodeCount || index < testConfig.nodeCount)
        {
            index = tester.getRandomNodeIndex();
        }

        setNodeStatusAtIndex(isCorrect ? NodeStatus::CORRECT : NodeStatus::WRONG, index);
        updateCasesLeft(tester.casesLeftCount());

        // In this context tester.getRandomNodeIndex returns index of just selected node.
        int whichList = tester.getRandomNodeIndex();
        addTimingToList(whichList);

        // aka celebrations
        holdUpWithNextTextFor(sf::milliseconds(1000));
    }
}

void ReactionTestingScreen::addTimingToList(int index)
{
    TextList& timingList = timingLists.at(index);
    std::string timingText = tester.timingToFormattedText();

    timingList.addRow(timingText);
}

void ReactionTestingScreen::tick()
{
    if(!animationManager.isCompleted())
    {
        animationManager.tick();
    }
    else
    {
        handleReactionTest();
    }
}

void ReactionTestingScreen::handleReactionTest()
{
    if (celebrationsFinished())
    {
        if (tester.hasTestCasesLeft())
        {
            if (tester.isTestCaseReady())
            {
                setNodeStatusAtIndex(NodeStatus::POSSIBLE, tester.getRandomNodeIndex());
            }
            else
            {
                if (!tester.hasTestCaseStarted())
                    tester.startNewCase();

                resetAllNodes();
            }
        }
        else
        {
            /* Test ended here */
            resetAllNodes();
        }


        tester.tick();
    }
}

void ReactionTestingScreen::resetAllNodes()
{
    for (auto& node : nodes)
    {
        node.setVisualStatus(NodeStatus::NEUTRAL);
    }
}

bool ReactionTestingScreen::celebrationsFinished()
{
    return celebrationTimer.getElapsedTime() >= celebrationDuration;
}

void ReactionTestingScreen::startNewTestCase()
{
    tester.startNewCase();
}

TestingNode* ReactionTestingScreen::selectNodeAt(int index)
{
    return &nodes.at(index);
}

void ReactionTestingScreen::setNodeStatusAtIndex(NodeStatus status, int index)
{
    TestingNode* node = selectNodeAt(index);
    node->setVisualStatus(status);
}

const std::vector<sf::Drawable*> ReactionTestingScreen::getDrawablesToDraw()
{
    return drawables;
}
