#ifndef WINDOW_H
#define WINDOW_H

#include "Screen.h"
#include "Resources.h"

class Window
{
public:
    Window(Resources& res);

    void create();

    void selectScreen(std::shared_ptr<Screen> newScreen);

    void setFramerateLimit(unsigned int limit);

    void run();

private:
    // This is polling method. Constantly checks if currentScreen needs to change for another screen.
    void changeScreenIfReady();

    Resources& resources;
    std::shared_ptr<sf::RenderWindow> windowInstance;
    std::shared_ptr<Screen> currentScreen;
};

#endif // WINDOW_H
