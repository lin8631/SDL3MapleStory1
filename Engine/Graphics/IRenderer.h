#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <memory>
#include <vector>
#include <functional>

namespace Engine {

struct Color {
    uint8_t r, g, b, a;
    
    Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255) 
        : r(r), g(g), b(b), a(a) {}
};

struct Rect {
    int x, y, w, h;
    
    Rect(int x = 0, int y = 0, int w = 0, int h = 0) 
        : x(x), y(y), w(w), h(h) {}
};

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

class IRenderer {
public:
    virtual ~IRenderer() = default;
    
    virtual SDL_Renderer* getSDLRenderer() = 0;
    
    virtual void setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;
    virtual void clear() = 0;
    virtual void present() = 0;
    
    virtual void copy(SDL_Texture* texture, const Rect* src, const Rect* dst) = 0;
    virtual void copyEx(SDL_Texture* texture, const Rect* src, const Rect* dst, double angle, const Point* center, SDL_FlipMode flip) = 0;
    
    virtual void setRenderTarget(SDL_Texture* texture) = 0;
    virtual SDL_Texture* createTexture(SDL_PixelFormat format, int access, int w, int h) = 0;
    virtual void destroyTexture(SDL_Texture* texture) = 0;
    
    virtual void drawPoint(int x, int y) = 0;
    virtual void drawLine(int x1, int y1, int x2, int y2) = 0;
    virtual void drawRect(const Rect* rect) = 0;
    virtual void drawFillRect(const Rect* rect) = 0;
    
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
};

class IWindow {
public:
    virtual ~IWindow() = default;
    
    virtual SDL_Window* getSDLWindow() = 0;
    virtual IRenderer* getRenderer() = 0;
    
    virtual void setSize(int width, int height) = 0;
    virtual void getSize(int* width, int* height) = 0;
    virtual void setTitle(const char* title) = 0;
    
    virtual void clear() = 0;
    virtual void present() = 0;
    virtual void pollEvents() = 0;
    
    virtual bool isQuit() const = 0;
};

class IEventListener {
public:
    virtual ~IEventListener() = default;
    virtual void onEvent(const SDL_Event& event) = 0;
};

class EventManager {
public:
    static void addListener(IEventListener* listener);
    static void removeListener(IEventListener* listener);
    static void pollEvents();
    static void quit();
    
    static bool isQuit();
    
private:
    static std::vector<IEventListener*> s_listeners;
    static bool s_quit;
};

}
