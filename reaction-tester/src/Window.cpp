#include "Window.h"

Window::Window(Resources& res): resources(res)
{

}

void Window::create()
{
    windowInstance = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(constants::window::width, constants::window::height), constants::window::title);
}

void Window::selectScreen(std::shared_ptr<Screen> newScreen)
{
    currentScreen = newScreen;
}

void Window::setFramerateLimit(unsigned int limit)
{
    windowInstance->setFramerateLimit(limit);
}

void Window::run()
{
    while(windowInstance->isOpen())
    {
        sf::Event e;
        while(windowInstance->pollEvent(e))
        {
            currentScreen->handleEvent(e);

            if (e.type == sf::Event::Closed)
            {
                windowInstance->close();
            }
        }

        currentScreen->tick();
        changeScreenIfReady();

        windowInstance->clear();
        const std::vector<sf::Drawable*> screenDrawables = currentScreen->getDrawablesToDraw();
        for(auto& drawable: screenDrawables)
            windowInstance->draw(*drawable);
        windowInstance->display();
    }
}

void Window::changeScreenIfReady()
{
    bool isReady = currentScreen->getNextScreen() != nullptr;
    if(isReady)
    {
        selectScreen(currentScreen->getNextScreen());
    }
}
