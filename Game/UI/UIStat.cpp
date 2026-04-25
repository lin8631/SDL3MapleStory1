#include "UIStat.h"
#include "Resources/Wz.h"
#include "Button.h"
#include "Systems/UI.h"
#include <algorithm>

void UIStat::run()
{
}

void UIStat::show()
{
    UIStat::x = Camera::w / 2;
    UIStat::y = Camera::h / 2;

    auto ui_node = Wz::UI->get_root();
    backgrnd = Texture::load(dynamic_cast<wz::Property<wz::WzCanvas> *>(ui_node->find_from_path(u"UIWindow.img/Stat/backgrnd")));

    Button::load(u"Basic.img/BtClose", BtClose);

    if (backgrnd)
    {
        UIStat::x = std::max(0.0f, std::min(UIStat::x, (float)Camera::w - backgrnd->w));
        UIStat::y = std::max(0.0f, std::min(UIStat::y, (float)Camera::h - backgrnd->h));
    }

    ui_index.push_back(UIIndex::UI_UIStat);
    UIStat::open = true;
}

void UIStat::hide()
{
    ui_index.remove(UIIndex::UI_UIStat);
    UIStat::open = false;
}

void UIStat::over()
{
    for (auto &[key, val] : UIStat::position_map)
    {
        auto rect = val;
        rect.x += UIStat::x;
        rect.y += UIStat::y;
        Button::over(rect, *key);
    }
}

bool UIStat::mousein()
{
    SDL_FPoint point = {Cursor::x, Cursor::y};
    SDL_FRect rect;
    rect.x = UIStat::x;
    rect.y = UIStat::y;
    rect.w = UIStat::backgrnd->w;
    rect.h = UIStat::backgrnd->h;
    if (SDL_PointInRectFloat(&point, &rect))
    {
        return true;
    }
    return false;
}

void UIStat::click()
{
    for (auto &[key, val] : UIStat::position_map)
    {
        auto rect = val;
        rect.x += UIStat::x;
        rect.y += UIStat::y;
        Button::click(rect, *key, UIStat::click_map);
    }
}

void UIStat::BtClose_func()
{
    UIStat::hide();
}
