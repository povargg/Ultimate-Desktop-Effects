#pragma once
#include "common.h"

std::wstring InstallDir();   // %LOCALAPPDATA%\Programs\UltimateDesktopEffects
std::wstring InstalledExe();
void RefreshInstallBranding(); // refresh shortcuts and Apps & Features after an update
bool IsInstalledCopy();      // running from the install folder
int  RunInstaller();
int  RunUninstaller(bool quiet);
void CloseRunningInstance(); // asks the running copy to exit and waits
