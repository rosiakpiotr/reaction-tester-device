#include "Resources.h"
#include "Window.h"
#include "ReactionTestingScreen.h"
#include "utility.h"

int main(int argc, char* argv[])
{
    disableSfmlConsoleOutput();

    int nodesCount = 1;
    int casesCount = 5;
    int difficulty = 1;

    std::vector <std::string> captions = {"T"};

    Resources res;
    bool loadedCorrectly = res.loadAll();

    if (loadedCorrectly)
    {
        Window window(res);
        window.create();
        window.setFramerateLimit(constants::window::maxFPS);

        ScreenParam param;
        param.intParams = { nodesCount, casesCount, difficulty };
        param.stringParams = captions;

        Screen::Ptr firstSreen = ScreenFactory::createScreen<ReactionTestingScreen>(&res);
        firstSreen->prepare(param);
        window.selectScreen(firstSreen);
        window.run();
    }
    else
    {
        ColorfulLogger::print(ColorfulLogger::Red, constants::message::errorLoadingResources);
    }

}
