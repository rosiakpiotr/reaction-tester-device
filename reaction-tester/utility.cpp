#include "utility.h"

struct setup
{
    setup()
    {
        srand(time(NULL));
    }
}make;

std::string filenameWithoutExtension(const std::string& filename)
{
    size_t lastindex = filename.find_last_of(".");
    return filename.substr(0, lastindex);
}

std::string makeResourcePath(const std::string& rootPath, const std::string& resName)
{
    return rootPath + "/" + resName;
}

void disableSfmlConsoleOutput()
{
    sf::err().rdbuf(0);
}

sf::Color highContrastColorTo(sf::Color original)
{
    sf::Color newColor;

    newColor.r = 255 - original.r;
    newColor.g = 255 - original.g;
    newColor.b = 255 - original.b;

    return newColor;
}

int randomNumberInRange(int lowerBound, int upperBound)
{
    return rand() % upperBound + lowerBound;
}

int indexOfAnswerInCaptions(sf::Keyboard::Key answer, std::vector<std::string> nodeCaptions)
{
    char ascii = sfmlKeyToAsciiUpperCase(answer);

    int index = 0;
    for (const auto& caption : nodeCaptions)
    {
        if (caption[0] == ascii) { return index; }
        index++;
    }

    return -1;

}

int sfmlKeyToAsciiUpperCase(sf::Keyboard::Key key)
{
    return 65 + key;
}

sf::Vector2f computePositionBelow(sf::Vector2f object, int spacing)
{
    return sf::Vector2f(object.x, object.y - spacing);
}

std::string timingToStringMs(sf::Time timing)
{
    sf::Int32 milliseconds = timing.asMilliseconds();
    return std::to_string(milliseconds) + "ms";
}
