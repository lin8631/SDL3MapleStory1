#pragma once

#include "Engine/Resource/IResourceLoader.h"
#include "Core/Texture.h"
#include "Components/Sound.h"

class ConcreteResourceLoader : public ::Engine::IResourceLoader {
public:
    Engine::ITexture* loadTexture(void* node) override;
    Engine::ISound* loadSound(void* node) override;
    SDL_Texture* createBlankTexture(SDL_PixelFormat format, int width, int height) override;
};
