#ifndef SCREEN_H
#define SCREEN_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "Resources.h"

// Allows different screens pass differrent kind of informations.
struct ScreenParam
{
    std::vector<int> intParams;
    std::vector<std::string> stringParams;
};

class Screen
{
public:
    Screen(Resources* resources): resources(resources) {}

    virtual void prepare(const ScreenParam& arg) = 0;

    virtual void handleEvent(const sf::Event& windowEvent) = 0;

    virtual void tick() = 0;

    virtual const std::vector<sf::Drawable*> getDrawablesToDraw() = 0;

    virtual std::shared_ptr<Screen> getNextScreen() { return nextScreen; }

    typedef std::shared_ptr<Screen> Ptr;

protected:
    Resources* resources;
    Screen::Ptr nextScreen;
};

namespace ScreenFactory
{
    // Factory method for abstract class Screen.
    template <class T, typename... Args>
    Screen::Ptr createScreen(Args... args)
    {
        Screen::Ptr screen(new T(args...));
        return screen;
    }
}

#endif // SCREEN_H
