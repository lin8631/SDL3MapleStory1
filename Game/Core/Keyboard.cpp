#include "Keyboard.h"
#include <fstream>
#include <sstream>

static SDL_Scancode string_to_scancode(const std::string &str)
{
    static const std::unordered_map<std::string, SDL_Scancode> map = {
        {"UP", SDL_SCANCODE_UP}, {"DOWN", SDL_SCANCODE_DOWN}, {"LEFT", SDL_SCANCODE_LEFT}, {"RIGHT", SDL_SCANCODE_RIGHT},
        {"LALT", SDL_SCANCODE_LALT}, {"X", SDL_SCANCODE_X}, {"Z", SDL_SCANCODE_Z}, {"LCTRL", SDL_SCANCODE_LCTRL},
        {"A", SDL_SCANCODE_A}, {"SPACE", SDL_SCANCODE_SPACE}, {"F", SDL_SCANCODE_F}, {"Y", SDL_SCANCODE_Y},
        {"D", SDL_SCANCODE_D}, {"S", SDL_SCANCODE_S}, {"G", SDL_SCANCODE_G}, {"H", SDL_SCANCODE_H},
        {"C", SDL_SCANCODE_C}, {"V", SDL_SCANCODE_V}, {"B", SDL_SCANCODE_B}, {"J", SDL_SCANCODE_J},
        {"K", SDL_SCANCODE_K}, {"R", SDL_SCANCODE_R}, {"T", SDL_SCANCODE_T}, {"U", SDL_SCANCODE_U},
        {"N", SDL_SCANCODE_N}, {"Q", SDL_SCANCODE_Q}, {"E", SDL_SCANCODE_E}, {"M", SDL_SCANCODE_M},
        {"1", SDL_SCANCODE_1}, {"2", SDL_SCANCODE_2}, {"3", SDL_SCANCODE_3}, {"4", SDL_SCANCODE_4},
        {"5", SDL_SCANCODE_5}, {"6", SDL_SCANCODE_6}, {"7", SDL_SCANCODE_7}, {"O", SDL_SCANCODE_O},
        {"P", SDL_SCANCODE_P}, {"L", SDL_SCANCODE_L}, {"W", SDL_SCANCODE_W}, {"I", SDL_SCANCODE_I},
    };
    auto it = map.find(str);
    return it != map.end() ? it->second : SDL_SCANCODE_UNKNOWN;
}

static uint8_t string_to_action(const std::string &str)
{
    static const std::unordered_map<std::string, uint8_t> map = {
        {"UP", Keyboard::UP}, {"DOWN", Keyboard::DOWN}, {"LEFT", Keyboard::LEFT}, {"RIGHT", Keyboard::RIGHT},
        {"JUMP", Keyboard::JUMP}, {"SIT", Keyboard::SIT}, {"PICK", Keyboard::PICK}, {"ATTACK", Keyboard::ATTACK},
    };
    auto it = map.find(str);
    return it != map.end() ? it->second : 0;
}

static uint8_t string_to_ui(const std::string &str)
{
    if (str == "WorldMap") return UIIndex::UI_WorldMap;
    if (str == "Item") return UIIndex::UI_UIItem;
    return 0;
}

void Keyboard::init()
{
    std::ifstream file("./Config/keyboard.txt");
    if (!file.is_open())
    {
        init_default();
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') continue;
        auto pos = line.find('=');
        if (pos == std::string::npos) continue;

        std::string key = line.substr(0, pos);
        std::string val = line.substr(pos + 1);
        
        if (key.rfind("action.", 0) == 0)
        {
            action[string_to_action(key.substr(7))] = string_to_scancode(val);
        }
        else if (key.rfind("ui.", 0) == 0)
        {
            ui[string_to_scancode(key.substr(3))] = string_to_ui(val);
        }
        else if (key.rfind("skill.", 0) == 0)
        {
            skill[string_to_scancode(key.substr(6))] = std::stoi(val);
        }
    }
    file.close();
}

void Keyboard::init_default()
{
    action[UP] = SDL_SCANCODE_UP;
    action[DOWN] = SDL_SCANCODE_DOWN;
    action[LEFT] = SDL_SCANCODE_LEFT;
    action[RIGHT] = SDL_SCANCODE_RIGHT;
    action[JUMP] = SDL_SCANCODE_LALT;
    action[SIT] = SDL_SCANCODE_X;
    action[PICK] = SDL_SCANCODE_Z;
    action[ATTACK] = SDL_SCANCODE_LCTRL;

    ui[SDL_SCANCODE_W] = UIIndex::UI_WorldMap;
    ui[SDL_SCANCODE_I] = UIIndex::UI_UIItem;

    skill[SDL_SCANCODE_A] = 14101006;
    skill[SDL_SCANCODE_SPACE] = 2201002;
    skill[SDL_SCANCODE_F] = 2221006;
    skill[SDL_SCANCODE_Y] = 1111008;
    skill[SDL_SCANCODE_D] = 1101004;
    skill[SDL_SCANCODE_S] = 1311006;
    skill[SDL_SCANCODE_G] = 4111005;
    skill[SDL_SCANCODE_H] = 4111002;
    skill[SDL_SCANCODE_C] = 4001003;
    skill[SDL_SCANCODE_V] = 3001005;
    skill[SDL_SCANCODE_B] = 4211006;
    skill[SDL_SCANCODE_J] = 1311001;
    skill[SDL_SCANCODE_K] = 1121006;
    skill[SDL_SCANCODE_R] = 4211002;
    skill[SDL_SCANCODE_T] = 4201004;
    skill[SDL_SCANCODE_U] = 1121008;
    skill[SDL_SCANCODE_N] = 4121007;
    skill[SDL_SCANCODE_Q] = 2201004;
    skill[SDL_SCANCODE_E] = 4101004;
    skill[SDL_SCANCODE_M] = 1101007;
    skill[SDL_SCANCODE_1] = 2221005;
    skill[SDL_SCANCODE_2] = 2121005;
    skill[SDL_SCANCODE_3] = 3121006;
    skill[SDL_SCANCODE_4] = 3101005;
    skill[SDL_SCANCODE_5] = 2001002;
    skill[SDL_SCANCODE_6] = 21100005;
    skill[SDL_SCANCODE_7] = 3221005;
    skill[SDL_SCANCODE_O] = 4201005;
    skill[SDL_SCANCODE_P] = 3111003;
    skill[SDL_SCANCODE_L] = 2101004;
}