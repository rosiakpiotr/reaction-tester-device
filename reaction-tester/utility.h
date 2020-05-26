#ifndef UTILITY_H
#define UTILITY_H

#include <time.h>
#include <sys/stat.h>
#include <algorithm>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

std::string filenameWithoutExtension(const std::string& filename);

std::string makeResourcePath(const std::string& rootPath, const std::string& resName);

// Disables various SFML messages like "failed to load texture etc...".
void disableSfmlConsoleOutput();

template <typename DrawableType> void centerOrigin(DrawableType* node)
{
    sf::FloatRect localBounds = node->getLocalBounds();

    sf::Vector2f newOrigin;
    newOrigin.x = localBounds.left + localBounds.width / 2.f;
    newOrigin.y = localBounds.top + localBounds.height / 2.f;

    node->setOrigin(sf::Vector2f(newOrigin));
}

sf::Color highContrastColorTo(sf::Color original);

int randomNumberInRange(int lowerBound, int upperBound);

int indexOfAnswerInCaptions(sf::Keyboard::Key answer, std::vector<std::string> nodeCaptions);

int sfmlKeyToAsciiUpperCase(sf::Keyboard::Key key);

sf::Vector2f computePositionBelow(sf::Vector2f object, int spacing);

std::string timingToStringMs(sf::Time timing);

template <class T>
sf::Vector2f leftTopCornerOfDrawableWithCenterOrigin(T& drawable)
{
    sf::FloatRect drawableBounds = drawable.getLocalBounds();
    sf::Vector2f position = drawable.getPosition();

    return sf::Vector2f(
                position.x - drawableBounds.width / 2.f,
                position.y - drawableBounds.height / 2.f);
}

#endif // UTILITY_H
