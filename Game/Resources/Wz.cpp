#include "Wz.h"

#include <SDL3/SDL.h>

void Wz::init(const std::string &filename_prefix)
{
    SDL_PathInfo info;
    if (!SDL_GetPathInfo(filename_prefix.c_str(), &info) || info.type != SDL_PATHTYPE_DIRECTORY)
    {
        const SDL_MessageBoxButtonData buttons[] = {
            { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "OK" }
        };
        std::string msg = "未找到游戏目录: " + filename_prefix;
        SDL_MessageBoxData data = {
            SDL_MESSAGEBOX_ERROR,
            nullptr,
            "Error",
            msg.c_str(),
            1,
            buttons,
            nullptr
        };
        SDL_ShowMessageBox(&data, nullptr);
        SDL_Quit();
        exit(1);
    }
    
    const auto iv = IV4(0xb9, 0x7d, 0x63, 0xe9);
    
    auto initFile = [&](wz::File* &file, const std::string &name) {
        std::string filepath = filename_prefix + name;
        SDL_PathInfo info;
        if (!SDL_GetPathInfo(filepath.c_str(), &info) || info.type != SDL_PATHTYPE_FILE)
        {
            SDL_LogWarn(0, "WZ file not found: %s", name.c_str());
            return;
        }
        file = new wz::File(iv, filepath.c_str());
        if (!file->parse(u""))
        {
            SDL_LogWarn(0, "Failed to parse WZ file: %s", name.c_str());
        }
    };
    
    // Base = (new wz::File(iv, (std::string(filename_prefix) + std::string("Base.wz")).c_str()));
    initFile(Character, "Character.wz");
    initFile(Effect, "Effect.wz");
    initFile(Etc, "Etc.wz");
    initFile(Item, "Item.wz");
    // List = (new wz::File(iv, (std::string(filename_prefix) + std::string("List.wz")).c_str()));
    initFile(Map, "Map.wz");
    initFile(Mob, "Mob.wz");
    // Morph = (new wz::File(iv, (std::string(filename_prefix) + std::string("Morph.wz")).c_str()));
    initFile(Npc, "Npc.wz");
    // Quest = (new wz::File(iv, (std::string(filename_prefix) + std::string("Quest.wz")).c_str()));
    initFile(Reactor, "Reactor.wz");
    initFile(Skill, "Skill.wz");
    initFile(Sound, "Sound.wz");
    initFile(String, "String.wz");
    // TamingMob = (new wz::File(iv, (std::string(filename_prefix) + std::string("TamingMob.wz")).c_str()));
    initFile(UI, "UI.wz");

    // Base->parse(u"Base");
    // Character->parse(u"Character");
    // Effect->parse(u"Effect");
    // Etc->parse(u"Etc");
    // Item->parse(u"Item");
    // List->parse();
    // Map->parse(u"Map");
    // Mob->parse(u"Mob");
    // Morph->parse();
    // Npc->parse(u"Npc");
    // Quest->parse();
    // Reactor->parse(u"Reactor");
    // Skill->parse(u"Skill");
    // Sound->parse(u"Sound");
    // String->parse(u"String");
    // TamingMob->parse();
    // UI->parse(u"UI");
}