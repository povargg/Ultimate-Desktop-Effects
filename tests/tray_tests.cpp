#include "../src/tray_input.h"
#include <cstdio>

int main() {
    const auto packed = [](UINT event) { return MAKELPARAM(event, kTrayIconId); };
    const WPARAM anchor = MAKEWPARAM(-34, 710);
    const struct Case {
        const char* name;
        WPARAM wp;
        LPARAM lp;
        TrayAction expected;
    } cases[] = {
        {"legacy tray left click opens settings", kTrayIconId, WM_LBUTTONUP, TrayAction::Settings},
        {"legacy tray right click opens the menu", kTrayIconId, WM_RBUTTONUP, TrayAction::Menu},
        {"legacy context menu stays available", kTrayIconId, WM_CONTEXTMENU, TrayAction::Menu},
        {"v4 tray left click opens settings", anchor, packed(WM_LBUTTONUP), TrayAction::Settings},
        {"v4 icon selection opens settings", anchor, packed(NIN_SELECT), TrayAction::Settings},
        {"v4 keyboard activation opens settings", anchor, packed(NIN_KEYSELECT), TrayAction::Settings},
        {"v4 context menu opens the menu", anchor, packed(WM_CONTEXTMENU), TrayAction::Menu},
        {"v4 right button release does not toggle the menu", anchor, packed(WM_RBUTTONUP), TrayAction::None},
        {"hover leaves panels alone", anchor, packed(WM_MOUSEMOVE), TrayAction::None},
        {"unrelated legacy icon is ignored", kTrayIconId + 1, WM_LBUTTONUP, TrayAction::None},
        {"unrelated v4 icon is ignored", kTrayIconId, MAKELPARAM(NIN_SELECT, kTrayIconId + 1), TrayAction::None},
    };
    int failed = 0;
    for (const auto& test : cases) {
        const bool pass = TrayActionForCallback(test.wp, test.lp) == test.expected;
        std::printf("%s %s\n", pass ? "PASS" : "FAIL", test.name);
        if (!pass) ++failed;
    }
    std::printf("\n%zu passed, %d failed\n", std::size(cases) - failed, failed);
    return failed ? 1 : 0;
}
