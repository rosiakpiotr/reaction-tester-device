#ifndef CONSTATNS_H
#define CONSTANTS_H

#include <string>

#define LOG_RESOURCE_LOADS

namespace constants
{
    namespace res
    {
        const std::string resourcesRoot = "res";
        const std::vector<std::string> textureFilenames = { "background.jpg", "testingNode.png" };
        const std::vector<std::string> fontFilenames = { "testingNodeCaptionFont.ttf", "basic.ttf" };
    }

    namespace message
    {
        const std::string errorLoadingResources = "Could not load all the resources. Reinstall software or provide missing files.";
    }

    namespace window
    {
        const int width = 1280;
        const int height = 720;
        const int maxFPS = 144;
        const std::string title = "ReactionTester Desktop";
    }
}

#endif // CONSTANTS_H
