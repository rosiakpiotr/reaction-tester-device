#ifndef TESTING_NODE_H
#define TESTING_NODE_H

#include <SFML/Graphics.hpp>
#include <functional>

#include "../utility.h"

enum NodeStatus { NEUTRAL, POSSIBLE, CORRECT, WRONG };

/*
    One node represents one element that shows on the screen to test reaction.
    It comes with caption of single character. That character is later used to identify node.
*/
class TestingNode : public sf::Drawable
{
public:
    TestingNode(const sf::Texture* nodeTexture, const sf::Font* captionFont, const std::string& caption);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void setPosition(sf::Vector2f);

    sf::Vector2f getPosition();

    void setScale(float scaleX, float scaleY);

    void setColor(sf::Color color);

    sf::Color getColor();

    sf::FloatRect getLocalBounds();

    void setVisualStatus(NodeStatus newStatus);

private:
    sf::Sprite nodeItself;
    sf::Text captionDrawable;
};

#endif // TESTING_NODE_H
