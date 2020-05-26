#ifndef RESOURCES_H
#define RESOURCES_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "ColorfulLogger.h"
#include "../constants.h"
#include "../utility.h"

class Resources
{
public:
    bool loadAll();
    bool loadTextures();
    bool loadTexture(std::string textureFilename);
    bool loadFonts();
    bool loadFont(std::string fontFilename);

    std::shared_ptr<sf::Texture> getTexture(std::string name);
    std::shared_ptr<sf::Font> getFont(std::string name);

    const sf::Texture* getTextureRawPtr(std::string name);
    const sf::Font* getFontRawPtr(std::string name);

private:

    template <typename T>
    bool loadResources(std::vector<std::string> resourceFilenames);

    template <typename T>
    bool loadResource(std::string resName);

    template <typename T>
    void addResourceToMap(std::string key, std::shared_ptr<T> resourcePtr);


    std::map<std::string, std::shared_ptr<sf::Texture>> textures;
    std::map<std::string, std::shared_ptr<sf::Font>> fonts;
};


#endif // RESOURCES_H
