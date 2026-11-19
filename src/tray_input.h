#pragma once
#include "common.h"
#include <shellapi.h>

constexpr UINT kTrayIconId = 1;

enum class TrayAction { None, Settings, Menu };

inline TrayAction TrayActionForCallback(WPARAM wp, LPARAM lp) {
    // Explorer tags v4 callbacks with the icon ID. A tray that missed NIM_SETVERSION
    // can still send the original format, with the icon ID in wParam.
    const bool version4 = HIWORD(lp) == kTrayIconId;
    const bool legacy = lp == LOWORD(lp) && wp == kTrayIconId;
    if (!version4 && !legacy) return TrayAction::None;

    switch (LOWORD(lp)) {
    case WM_LBUTTONUP:
    case NIN_SELECT:
    case NIN_KEYSELECT:
        return TrayAction::Settings;
    case WM_CONTEXTMENU:
        return TrayAction::Menu;
    case WM_RBUTTONUP:
        // A v4 tray may send this before WM_CONTEXTMENU. Opening twice would toggle the flyout closed.
        return legacy ? TrayAction::Menu : TrayAction::None;
    default:
        return TrayAction::None;
    }
}
