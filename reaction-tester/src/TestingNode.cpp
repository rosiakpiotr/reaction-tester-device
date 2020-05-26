#include "TestingNode.h"

TestingNode::TestingNode(const sf::Texture* nodeTexture, const sf::Font* captionFont, const std::string& caption)
{
    nodeItself.setTexture(*nodeTexture);
    centerOrigin(&nodeItself);

    captionDrawable.setString(caption);
    captionDrawable.setFont(*captionFont);
    captionDrawable.setFillColor(sf::Color::Black);
    centerOrigin(&captionDrawable);
}

void TestingNode::setPosition(sf::Vector2f newPosition)
{
    nodeItself.setPosition(newPosition);
    captionDrawable.setPosition(newPosition);
}

sf::Vector2f TestingNode::getPosition()
{
    return nodeItself.getPosition();
}

void TestingNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(nodeItself);
    target.draw(captionDrawable);
}

void TestingNode::setScale(float scaleX, float scaleY)
{
    nodeItself.setScale(scaleX, scaleY);
    captionDrawable.setScale(scaleX, scaleY);
}

void TestingNode::setColor(sf::Color color)
{
    nodeItself.setColor(color);
    captionDrawable.setFillColor(highContrastColorTo(color));
}

sf::Color TestingNode::getColor()
{
    return nodeItself.getColor();
}

void TestingNode::setVisualStatus(NodeStatus newStatus)
{
    switch (newStatus)
    {
        case NodeStatus::NEUTRAL:
            setColor(sf::Color::White);
            break;

        case NodeStatus::POSSIBLE:
            setColor(sf::Color::Yellow);
            break;

        case NodeStatus::CORRECT:
            setColor(sf::Color::Green);
            break;

        case NodeStatus::WRONG:
            setColor(sf::Color::Red);
            break;
    }
}

sf::FloatRect TestingNode::getLocalBounds()
{
    return nodeItself.getLocalBounds();
}
