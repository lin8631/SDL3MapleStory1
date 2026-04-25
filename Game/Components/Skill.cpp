#include "Skill.h"
#include "Sound.h"
#include "wz/Property.hpp"
#include <string>
#include <SDL3/SDL.h>
#include "Resources/Wz.h"

Skill::Wrap *Skill::Wrap::load(int id)
{
    static std::unordered_map<int, Wrap *> cache;
    if (cache.contains(id))
    {
        return cache[id];
    }
    else
    {
        Wrap *wrap = new Wrap(id);
        if (wrap->node == nullptr)
        {
            SDL_Log("ERROR: Skill::Wrap::load: failed to load skill %d", id);
            delete wrap;
            cache[id] = nullptr;
            return nullptr;
        }
        cache[id] = wrap;
        return wrap;
    }
}

Skill::Wrap::Wrap(int id) : id(id)
{
    std::string str = std::to_string(id);
    std::u16string id_str(str.begin(), str.end());

    auto skillPath = u"Skill.img/" + id_str;
    node = Wz::Sound->get_root()->find_from_path(skillPath);
    if (node != nullptr)
    {
        for (auto &[key, val] : node->get_children())
        {
            sounds[key] = Sound::Wrap::load(val[0]);
        }
    }
    
    auto wzSkillPath = id_str.substr(0, id_str.length() - 4) + u".img/skill/" + id_str;
    node = Wz::Skill->get_root()->find_from_path(wzSkillPath);
    if (!node)
    {
        SDL_LogWarn(0, "Skill::Wrap::init: skill node not found! id=%d, path=%s, Wz::Skill=%p", 
            id, 
            std::string(wzSkillPath.begin(), wzSkillPath.end()).c_str(),
            (void*)Wz::Skill);
        return;
    }
    if (auto hit = node->get_child(u"hit"))
    {
        hits.push_back(AnimatedSprite::Wrap::load(hit->get_child(u"0")));
    }
    auto level_node = node->get_child(u"level");
    if (level_node)
    {
        for (int i = 1; i < level_node->children_count() + 1; i++)
        {
            auto it = level_node->get_child(std::to_string(i));
            if (it == nullptr)
            {
                break;
            }
            level.push_back(it);
        }
    }
    // 加载动作
    if (node->get_child(u"action"))
    {
        if (node->get_child(u"action")->type == wz::Type::String)
        {
            action_str = dynamic_cast<wz::Property<wz::wzstring> *>(node->get_child(u"action"))->get();
        }
        else
        {
            // 0,1分别代表单手武器,双手武器的动作,这里就取默认的0
            action_str = dynamic_cast<wz::Property<wz::wzstring> *>(node->find_from_path(u"action/0"))->get();
        }
    }
    // 加载特效
    if (node->get_child(u"effect"))
    {
        if (node->get_child(u"effect")->get_child(u"0")->type == wz::Type::Canvas)
        {
            effects.push_back(AnimatedSprite::Wrap::load(node->get_child(u"effect")));
        }
        else
        {
            // 默认选择单手武器的特效
            effects.push_back(AnimatedSprite::Wrap::load(node->find_from_path(u"effect/0")));
        }
    }
    for (int i = 0;; i++)
    {
        auto e = "effect" + std::to_string(i);
        if (node->get_child(e))
        {
            effects.push_back(AnimatedSprite::Wrap::load(node->get_child(e)));
        }
        else
        {
            break;
        }
    }
}

Skill::Skill(int id)
{
    skiw = Wrap::load(id);
}