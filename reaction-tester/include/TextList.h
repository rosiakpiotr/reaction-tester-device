#ifndef TEXTLIST_H
#define TEXTLIST_H

#include <SFML/Graphics.hpp>
#include <iostream>

#include "../utility.h"

class TextList : public sf::Drawable
{
public:
    TextList(const sf::Font& font, unsigned int characterSize = 30);

    void addRow(const std::string& text);

    void addRows(std::vector<std::string> texts);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void setColor(sf::Color newColor);

    void setPosition(sf::Vector2f newPosition);

private:

    void repositionRows();

    std::vector<sf::Text> textRows;
    const sf::Font& font;

    sf::Vector2f firstRowPosition;

    int spacing;
    int characterSize;
};

#endif // TEXTLIST_H
