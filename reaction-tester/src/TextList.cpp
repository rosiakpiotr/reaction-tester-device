#include "TextList.h"

TextList::TextList(const sf::Font& font, unsigned int characterSize): font(font), characterSize(characterSize)
{
    spacing = characterSize;
}

void TextList::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const sf::Text& row : textRows)
    {
        target.draw(row, states);
    }
}

void TextList::addRows(std::vector<std::string> rowsContents)
{
    for (const std::string& rowContent: rowsContents)
    {
        addRow(rowContent);
    }
}

void TextList::addRow(const std::string& content)
{
    sf::Text newText;

    newText.setFont(font);
    newText.setString(content);
    newText.setCharacterSize(characterSize);
    newText.setFillColor(sf::Color::Black);

    textRows.push_back(newText);

    repositionRows();
}

void TextList::setColor(sf::Color newColor)
{
    for (auto& row: textRows)
    {
        row.setFillColor(newColor);
    }
}

void TextList::setPosition(sf::Vector2f newPosition)
{
    firstRowPosition = newPosition;
    repositionRows();
}

void TextList::repositionRows()
{
    int i = 0;
    for (sf::Text& row : textRows)
    {
        sf::Vector2f newRowPosition;

        newRowPosition.x = firstRowPosition.x;
        newRowPosition.y = firstRowPosition.y + ((characterSize + spacing) * i++);

        row.setPosition(newRowPosition);
    }
}
