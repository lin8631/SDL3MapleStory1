#include "Graphics/IRenderer.h"
#include <SDL3/SDL.h>

namespace Engine {

std::vector<IEventListener*> EventManager::s_listeners;
bool EventManager::s_quit = false;

void EventManager::addListener(IEventListener* listener) {
    s_listeners.push_back(listener);
}

void EventManager::removeListener(IEventListener* listener) {
    auto it = std::find(s_listeners.begin(), s_listeners.end(), listener);
    if (it != s_listeners.end()) {
        s_listeners.erase(it);
    }
}

void EventManager::pollEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        for (auto* listener : s_listeners) {
            listener->onEvent(event);
        }
        if (event.type == SDL_EVENT_QUIT) {
            s_quit = true;
        }
    }
}

void EventManager::quit() {
    s_quit = true;
}

bool EventManager::isQuit() {
    return s_quit;
}

}
