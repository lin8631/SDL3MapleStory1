#include "Systems/Systems.h"
#include "Core/Core.h"
#include "Entities/Entities.h"
#include "UI/UI.h"
#include "Resources/Wz.h"
#include "Resource/ConcreteResourceLoader.h"
#include "Engine/Resource/IResourceLoader.h"
#include "Components/Sound.h"
#include "Components/Mob.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include "entt/entt.hpp"
#include <string>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int width = 800;
int height = 600;
bool show_imgui_demo = false;
bool show_character_name = true;
bool show_pet_name = true;
float master_volume = 1.0f;
float bgm_volume = 0.3f;
float sfx_volume = 0.3f;
float ui_volume = 0.3f;
float player_skill_volume = 0.3f;
float mob_volume = 0.3f;
bool master_muted = false;
bool bgm_muted = false;
bool sfx_muted = false;
bool ui_muted = false;
bool player_skill_muted = false;
bool mob_muted = false;

enum class ShowMode { Always = 0, Near = 1, Attacked = 2, Hide = 3 };
int npc_show_mode = (int)ShowMode::Always;
int portal_show_mode = (int)ShowMode::Always;
int mob_show_mode = (int)ShowMode::Always;
int selected_npc_index = -1;
int selected_portal_index = -1;
int selected_mob_index = -1;
float near_distance = 200.0f;
static SDL_Window *imgui_window = nullptr;
static SDL_Renderer *imgui_renderer = nullptr;

static void ToggleImGuiDemo()
{
    show_imgui_demo = !show_imgui_demo;
    if (show_imgui_demo && !imgui_window)
    {
        imgui_window = SDL_CreateWindow("Debug", Window::current_width, Window::current_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS);
        SDL_SetWindowPosition(imgui_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        imgui_renderer = SDL_CreateRenderer(imgui_window, nullptr);
        ImGui::CreateContext();
        ImGui_ImplSDL3_InitForSDLRenderer(imgui_window, imgui_renderer);
        ImGui_ImplSDLRenderer3_Init(imgui_renderer);
        
        ImGuiIO& io = ImGui::GetIO();
        ImFont *font = io.Fonts->AddFontFromFileTTF("../Game/Font/simsun.ttc", 20.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
        if (font)
        {
            io.FontDefault = font;
        }
    }
    else if (!show_imgui_demo && imgui_window)
    {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        SDL_DestroyRenderer(imgui_renderer);
        SDL_DestroyWindow(imgui_window);
        imgui_window = nullptr;
        imgui_renderer = nullptr;
    }
}

bool HasImGuiWindowFocus()
{
    if (!show_imgui_demo || !imgui_window) return false;
    return SDL_GetMouseFocus() == imgui_window;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    Window::tick_delta_time();
    Window::clear();
    animate_run();
    player_statemachine_run();
    mob_statemachine_run();
    npc_statemachine_run();
    buff_run();
    drop_run();
    ball_run();
    ui_run();
    world_run();
    summon_statemachine_run();
    pet_statemachine_run();
    camera_run();
    render_run();

    if (show_imgui_demo && imgui_window && imgui_renderer)
    {
        SDL_SetRenderTarget(imgui_renderer, nullptr);
        SDL_SetRenderDrawColor(imgui_renderer, 32, 32, 32, 255);
        SDL_RenderClear(imgui_renderer);
        
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        int w, h;
        SDL_GetCurrentRenderOutputSize(imgui_renderer, &w, &h);
        
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)w, (float)h);
        
        // Custom debug window - full screen, no title bar, not movable
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2((float)w, (float)h), ImGuiCond_Always);
        
        ImGui::Begin("Debug", nullptr, 
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
        
        if (ImGui::CollapsingHeader("画面设置"))
        {
            ImGui::Checkbox("显示角色名称", &show_character_name);
            ImGui::Checkbox("显示宠物名称", &show_pet_name);

            ImGui::Separator();

            ImGui::SliderFloat("距离远近", &near_distance, 50.0f, 500.0f, "%.0f");

            ImGui::SeparatorText("NPC");
            const char* npc_modes[] = { "始终显示", "仅在附近显示", "隐藏" };
            ImGui::Combo("##NPC Mode", &npc_show_mode, npc_modes, 3);

            ImGui::SeparatorText("Portal");
            const char* portal_modes[] = { "始终显示", "仅在附近显示", "隐藏" };
            ImGui::Combo("##Portal Mode", &portal_show_mode, portal_modes, 3);

            ImGui::SeparatorText("Mob");
            const char* mob_modes[] = { "始终显示", "隐藏", "仅在受到攻击时显示" };
            ImGui::Combo("##Mob Mode", &mob_show_mode, mob_modes, 3);

            ImGui::Separator();

            const char* resolutions[] = { "800x600", "1068x600", "1366x768" };
            int current_resolution = -1;
            for (int i = 0; i < 3; i++)
            {
                int w, h;
                sscanf(resolutions[i], "%dx%d", &w, &h);
                if ((int)Window::current_width == w && (int)Window::current_height == h)
                {
                    current_resolution = i;
                    break;
                }
            }
            ImGui::Combo("分辨率", &current_resolution, resolutions, 3);
            if (current_resolution >= 0)
            {
                int w, h;
                sscanf(resolutions[current_resolution], "%dx%d", &w, &h);
                Window::set_resolution(w, h);
            }
        }
        
        if (ImGui::CollapsingHeader("声音设置"))
        {
            ImGui::Checkbox("##Master", &master_muted);
            ImGui::SameLine();
            ImGui::SliderFloat("主音量", &master_volume, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
            if (!master_muted)
            {
                Sound::set_master_volume(master_volume);
            }
            else
            {
                Sound::set_master_volume(0.0f);
            }
            
            ImGui::Separator();
            
            ImGui::Checkbox("##BGM", &bgm_muted);
            ImGui::SameLine();
            ImGui::SliderFloat("BGM", &bgm_volume, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
            if (!bgm_muted && !master_muted)
            {
                Sound::set_volume(SoundType::BGM, bgm_volume);
            }
            else
            {
                Sound::set_volume(SoundType::BGM, 0.0f);
            }
            
            ImGui::Checkbox("##SFX", &sfx_muted);
            ImGui::SameLine();
            ImGui::SliderFloat("SFX", &sfx_volume, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
            if (!sfx_muted && !master_muted)
            {
                Sound::set_volume(SoundType::SFX, sfx_volume);
            }
            else
            {
                Sound::set_volume(SoundType::SFX, 0.0f);
            }
            
            ImGui::Checkbox("##UI", &ui_muted);
            ImGui::SameLine();
            ImGui::SliderFloat("UI", &ui_volume, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
            if (!ui_muted && !master_muted)
            {
                Sound::set_volume(SoundType::UI, ui_volume);
            }
            else
            {
                Sound::set_volume(SoundType::UI, 0.0f);
            }
            
            ImGui::Checkbox("##PlayerSkill", &player_skill_muted);
            ImGui::SameLine();
            ImGui::SliderFloat("玩家技能", &player_skill_volume, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
            if (!player_skill_muted && !master_muted)
            {
                Sound::set_volume(SoundType::PLAYER_SKILL, player_skill_volume);
            }
            else
            {
                Sound::set_volume(SoundType::PLAYER_SKILL, 0.0f);
            }
            
            ImGui::Checkbox("##Mob", &mob_muted);
            ImGui::SameLine();
            ImGui::SliderFloat("Mob", &mob_volume, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
            if (!mob_muted && !master_muted)
            {
                Sound::set_volume(SoundType::MOB, mob_volume);
            }
            else
            {
                Sound::set_volume(SoundType::MOB, 0.0f);
            }
        }
        
        ImGui::End();
        
        ImGui::Render();

        if (SDL_GetMouseFocus() == imgui_window)
        {
            SDL_ShowCursor();
        }

        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), imgui_renderer);
        SDL_RenderPresent(imgui_renderer);
    }

    if (!HasImGuiWindowFocus())
    {
        SDL_HideCursor();
    }

    Window::update();
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (argc == 3)
    {
        width = SDL_atoi(argv[1]);
        height = SDL_atoi(argv[2]);
    }

    static ConcreteResourceLoader resourceLoader;
    Engine::ResourceManager::setLoader(&resourceLoader);

    Camera::w = width;
    Camera::h = height;

    FreeType::init("./Font/");
    Wz::init("./Data/");
    Keyboard::init();
    Window::create_window("sdlMS", width, height);
    Character::init();
    Tomb::init();
    Damage::init();
    Cursor::init();
    StatusBar::init();
    QuickSlot::init();
    MiniMap::init();
    GainTip::init();
    UIBuff::init();
#ifndef __EMSCRIPTEN__
    Sound::init();
#endif
    Drop::init();

    // ImGui 会在 ToggleImGuiDemo 中按 F1 时初始化

    // World::load_map(222020111);

    World::load_map(1000000);
    Player::ent = World::registry->create();
    load_character(0, 0, true, Player::name, Player::ent);
    load_pet(u"5000016", Player::ent);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    if (show_imgui_demo && imgui_window)
    {
        if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event->window.windowID == SDL_GetWindowID(imgui_window))
        {
            ToggleImGuiDemo();
            return SDL_APP_CONTINUE;
        }
        if (SDL_GetMouseFocus() == imgui_window)
        {
            ImGui_ImplSDL3_ProcessEvent(event);
        }
    }

    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        if (event->key.scancode == SDL_SCANCODE_RETURN && (event->key.mod & SDL_KMOD_ALT) != 0)
        {
            Window::toggle_resolution();
        }
        if (event->key.scancode == SDL_SCANCODE_F1)
        {
            ToggleImGuiDemo();
        }
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    if (imgui_window)
    {
        ToggleImGuiDemo();
    }
    SDL_Quit();
}