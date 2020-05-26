#include "Resources.h"

bool Resources::loadAll()
{
    bool texturesLoadedCorrectly = loadTextures();
    bool fontsLoadedCorrectly = loadFonts();
    return texturesLoadedCorrectly && fontsLoadedCorrectly;
}

bool Resources::loadTextures()
{
    return loadResources<sf::Texture>(constants::res::textureFilenames);
}

bool Resources::loadTexture(std::string textureFilename)
{
    return loadResource<sf::Texture>(textureFilename);
}

bool Resources::loadFonts()
{
    return loadResources<sf::Font>(constants::res::fontFilenames);
}

bool Resources::loadFont(std::string fontFilename)
{
    return loadResource<sf::Font>(fontFilename);
}

std::shared_ptr<sf::Texture> Resources::getTexture(std::string name)
{
    return textures.at(name);
}

std::shared_ptr<sf::Font> Resources::getFont(std::string name)
{
    return fonts.at(name);
}

const sf::Texture* Resources::getTextureRawPtr(std::string name)
{
    return getTexture(name).get();
}


const sf::Font* Resources::getFontRawPtr(std::string name)
{
    return getFont(name).get();
}

template <typename T> bool Resources::loadResources(std::vector<std::string> resourceFilenames)
{
    bool isOk = true;
    for(std::string filename: resourceFilenames)
    {
        if(loadResource<T>(filename))
        {
            #ifdef LOG_RESOURCE_LOADS
                std::string phrase = "Resources[" + std::string(typeid(T).name()) + "] - '" + filenameWithoutExtension(filename) + "' loaded.";
                ColorfulLogger::print(ColorfulLogger::Green, phrase);
            #endif
        } else {
            #ifdef LOG_RESOURCE_LOADS
                std::string phrase = "Resources[" + std::string(typeid(T).name()) +"] - failed loading '" + filenameWithoutExtension(filename) + "'.";
                ColorfulLogger::print(ColorfulLogger::Red, phrase);
            #endif
            isOk = false;
        }
    }

    return isOk;
}

template <typename T> bool Resources::loadResource(std::string resName)
{
    std::shared_ptr<T> resource = std::make_shared<T>();
    std::string resPath = makeResourcePath(constants::res::resourcesRoot, resName);
    if (resource->loadFromFile(resPath))
    {
        /* Adding resource to map (key is resource name). */
        addResourceToMap<T>(filenameWithoutExtension(resName), resource);
        return true;
    }
    return false;
}

template <> void Resources::addResourceToMap<sf::Texture>(std::string key, std::shared_ptr<sf::Texture> texturePtr)
{
    this->textures.insert(std::make_pair(key, texturePtr));
}


template <> void Resources::addResourceToMap<sf::Font>(std::string key, std::shared_ptr<sf::Font> fontPtr)
{
    this->fonts.insert(std::make_pair(key, fontPtr));
}
