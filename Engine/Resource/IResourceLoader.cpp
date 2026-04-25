#include "Resource/IResourceLoader.h"

namespace Engine {

IResourceLoader* ResourceManager::s_loader = nullptr;

void ResourceManager::setLoader(IResourceLoader* loader) {
    s_loader = loader;
}

IResourceLoader* ResourceManager::getLoader() {
    return s_loader;
}

ITexture* ResourceManager::loadTexture(void* node) {
    if (s_loader) {
        return s_loader->loadTexture(node);
    }
    return nullptr;
}

ISound* ResourceManager::loadSound(void* node) {
    if (s_loader) {
        return s_loader->loadSound(node);
    }
    return nullptr;
}

SDL_Texture* ResourceManager::createBlankTexture(SDL_PixelFormat format, int width, int height) {
    if (s_loader) {
        return s_loader->createBlankTexture(format, width, height);
    }
    return nullptr;
}

}
