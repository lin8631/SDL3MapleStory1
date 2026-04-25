#include "Resource/ConcreteResourceLoader.h"
#include "wz/Property.hpp"

Engine::ITexture* ConcreteResourceLoader::loadTexture(void* node) {
    auto wzNode = static_cast<wz::Property<wz::WzCanvas>*>(node);
    if (!wzNode) return nullptr;
    
    SDL_Texture* tex = Texture::load(wzNode);
    if (!tex) return nullptr;
    
    float fw, fh;
    SDL_GetTextureSize(tex, &fw, &fh);
    int w = static_cast<int>(fw);
    int h = static_cast<int>(fh);
    
    struct ConcreteTexture : public Engine::ITexture {
        SDL_Texture* tex;
        int width;
        int height;
        
        ConcreteTexture(SDL_Texture* t, int w, int h) : tex(t), width(w), height(h) {}
        ~ConcreteTexture() override {
            if (tex) SDL_DestroyTexture(tex);
        }
        SDL_Texture* getSDLTexture() override { return tex; }
        int getWidth() const override { return width; }
        int getHeight() const override { return height; }
    };
    
    return new ConcreteTexture(tex, w, h);
}

Engine::ISound* ConcreteResourceLoader::loadSound(void* node) {
    auto wzNode = static_cast<wz::Node*>(node);
    if (!wzNode) return nullptr;
    
    auto* wrap = Sound::Wrap::load(wzNode);
    if (!wrap || wrap->pcm_data.empty()) return nullptr;
    
    struct ConcreteSound : public Engine::ISound {
        std::vector<uint8_t> data;
        ConcreteSound(std::vector<uint8_t>&& d) : data(std::move(d)) {}
        const std::vector<uint8_t>& getPCMData() const override { return data; }
    };
    
    return new ConcreteSound(std::move(wrap->pcm_data));
}

SDL_Texture* ConcreteResourceLoader::createBlankTexture(SDL_PixelFormat format, int width, int height) {
    return Texture::createBlankTexture(format, width, height);
}
