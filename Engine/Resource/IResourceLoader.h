#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <memory>
#include <vector>

namespace Engine {

class ITexture {
public:
    virtual ~ITexture() = default;
    virtual SDL_Texture* getSDLTexture() = 0;
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
};

class ISound {
public:
    virtual ~ISound() = default;
    virtual const std::vector<uint8_t>& getPCMData() const = 0;
};

class IResourceLoader {
public:
    virtual ~IResourceLoader() = default;
    
    virtual ITexture* loadTexture(void* node) = 0;
    virtual ISound* loadSound(void* node) = 0;
    
    virtual SDL_Texture* createBlankTexture(SDL_PixelFormat format, int width, int height) = 0;
};

class ResourceManager {
public:
    static void setLoader(IResourceLoader* loader);
    static IResourceLoader* getLoader();
    
    static ITexture* loadTexture(void* node);
    static ISound* loadSound(void* node);
    static SDL_Texture* createBlankTexture(SDL_PixelFormat format, int width, int height);

private:
    static IResourceLoader* s_loader;
};

}
