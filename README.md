<div align="center">

<img src="docs/images/ultimate-desktop-effects-icon.png" alt="Ultimate Desktop Effects icon" width="112" height="112">

# Ultimate Desktop Effects

**Snow, rain, falling leaves, and Halloween effects for the Windows desktop.**

[**Download for Windows**](https://github.com/povargg/Ultimate-Desktop-Effects/releases/latest/download/Ultimate-Desktop-Effects-x64.exe) · [**Portable version**](https://github.com/povargg/Ultimate-Desktop-Effects/releases/latest/download/Ultimate-Desktop-Effects-Portable.exe) · [**All releases**](https://github.com/povargg/Ultimate-Desktop-Effects/releases)

**English** · [Deutsch](README.de.md) · [Русский](README.ru.md)

Windows 10 / 11 · x64 · C++17 · MIT license

Created by **[povargg](https://github.com/povargg)**, building on **[flipkips’ LayerSnow](https://github.com/flipkips/desktop-snow-overlay)**.

</div>

## What you can do

Snow settles on your windows, rain collects along the taskbar, and pixel leaves drift across the screen. In Halloween mode, sweets fall and spiders come down on threads. You can switch between all four effects from the settings window or tray menu.

Each effect keeps its own settings. If you switch away, the scene pauses where it is, right down to the particles and piles. Switch back and it picks up where it stopped. Only the selected effect runs and draws.

| Snow | Rain |
| :---: | :---: |
| <img src="docs/figures/desktop-snow.webp" alt="Snowflakes and a snowbank over a winter desktop" width="380"> | <img src="docs/figures/desktop-rain.webp" alt="Rain falling over a sunset desktop" width="380"> |

| Falling leaves | Halloween |
| :---: | :---: |
| <img src="docs/figures/desktop-leaves.webp" alt="Leaves drifting across a sunny desktop" width="380"> | <img src="docs/figures/desktop-halloween.webp" alt="Pixel sweets and hanging spiders over a sunset desktop" width="380"> |

## Four effects

| Effect | What it does |
| --- | --- |
| **Snow** | Soft flakes and four crystal shapes make up the snowfall. Wind and gusts move it around, and drifts on windows and the taskbar can melt or slide off in clumps. There are four snowfall presets. |
| **Rain** | Drops splash and collect into pools with waves, then evaporate. You can turn on lightning, adjust its branches and brightness, and toggle the screen flash on its own. |
| **Leaves** | Pick pink sakura petals, green spring and summer leaves, or yellow, orange, and red autumn leaves. Each type has four pixel variants that sway, tumble, and pile up on top of each other. |
| **Halloween** | There are six pixel sweets and four types of spiders. You can show just sweets, just spiders, or both. Spiders come down on threads, sometimes pause partway, and have an equal chance to climb back up or drop. **Click a visible thread to cut it.** |

### Shared controls

- Show effects **over windows or behind them**.
- Collect particles on **window tops and the taskbar**.
- Adjust density, fall speed, wind, size, pile height, opacity and frame rate where supported by each effect.
- Brush piles away with the cursor, shake them off, or clear them.
- Pause and hide the overlay in fullscreen apps and games.
- Support multiple monitors and per-monitor display scaling.
- Optional Windows autostart and light/dark settings windows.
- **Five interface languages:** English, Deutsch, Русский, Українська and Română.

The leaves and Halloween sprites are native **16×16 pixel art**. Settled sprites retain their full visible face and overlap using compact collision footprints, producing a dense, slightly uneven layer.

## Download

**Current version: 1.0.5 · Windows x64**

From version 1.0.3, use **Check for updates** in the notification area menu. The app checks GitHub in the background, verifies the download with SHA-256 and restarts after you confirm installation. If you have an installation older than 1.0.3, install this release once to enable in-app updates.

| Download | Use |
| --- | --- |
| [**Ultimate-Desktop-Effects-x64.exe**](https://github.com/povargg/Ultimate-Desktop-Effects/releases/latest/download/Ultimate-Desktop-Effects-x64.exe) | Install once, then launch from the Start menu. |
| [**Ultimate-Desktop-Effects-Portable.exe**](https://github.com/povargg/Ultimate-Desktop-Effects/releases/latest/download/Ultimate-Desktop-Effects-Portable.exe) | Run immediately without installation. Keep `Portable` in the filename. |
| [**Source code ZIP**](https://github.com/povargg/Ultimate-Desktop-Effects/releases/latest/download/Ultimate-Desktop-Effects-Source.zip) | C++ source, assets, build scripts, tests and documentation. |

Requires **Windows 10 version 1703 or newer, or Windows 11, 64-bit**, with Direct3D 11 support. The executable includes its C++ runtime; no .NET, Python or separate Visual C++ runtime package is needed to run it.

The release is **not digitally signed**.

## Quick start

1. Download the installer or portable executable above.
2. Open it. The installer uses your user account and does not require administrator rights.
3. Find the **UE icon** in the notification area.
4. **Left-click** it for settings, or **right-click** for the quick menu.
5. Pick **Snow / Rain / Leaves / Halloween** at the top of the panel.

Additional rain options are in its weather settings. The leaves and Halloween modes have their own appearance panels. In Halloween, thread cutting can be disabled and its click tolerance adjusted.

## Settings and installation

Installation folder:

```text
%LOCALAPPDATA%\Programs\UltimateDesktopEffects
```

Settings, including portable mode:

```text
%APPDATA%\UltimateDesktopEffects\settings.ini
```

On first launch, existing effect settings can be imported from the previous `LayerSnow` / `SnowLayer`, `LayerRain`, `LayerLeaves` and `LayerHalloween` configuration folders. New profiles then live together in Ultimate Desktop Effects. Interface language and autostart are shared; effect parameters remain independent.

Scenes and piles are kept in memory while switching effects. Settings and the last selected effect survive a restart; the visual scene starts fresh after reopening the app.

To uninstall, use Windows **Installed apps**, the Start menu shortcut, or `uninstall.exe` in the installation folder. The normal dialog lets you choose whether to remove settings.

| Command-line option | Action |
| --- | --- |
| `/portable` | Run without opening the installer. |
| `/uninstall` | Open the uninstaller. |
| `/uninstall /quiet` | Remove the installation and settings without a dialog. |
| `/autostart` | Start the application in its Windows autostart mode. |

## Native and local

Ultimate Desktop Effects is a **native C++17 application**, using **Direct3D 11, Direct2D, DirectWrite and DirectComposition**. It runs one process, one overlay and one active simulation. Inactive effects retain bounded CPU state; their renderers and closed settings panels release their graphics resources when you switch.

Particle budgets, pile caches, window event updates and reduced frame rates for static scenes come from the individual effects. A CPU-only comparison of the combined dispatcher against direct simulation calls stayed within a few percent on the build host; see the [validation report](docs/VALIDIERUNG.md) for the measurements and their limits.

The app works locally, with **no accounts, telemetry or effect downloads at runtime**. GitHub links open in your browser only when clicked.

## Build from source

All required sprites and the Windows icon are included. Image generation is not required to build the app.

### MinGW-w64 — Windows or Linux

```sh
python3 build_mingw.py --cxx x86_64-w64-mingw32-g++-posix
```

The published executable was built with MinGW-w64 and a statically linked C++ runtime.

### Visual Studio — Windows

Install Visual Studio 2022 Build Tools with the **Desktop development with C++** workload and Windows SDK, then run:

```bat
build.bat
```

For a native Windows MinGW-w64 installation, run `python build_mingw.py --cxx g++`.

Build outputs are placed in `dist/`: the installer, portable executable and uninstaller share the same program bytes and select their mode from their filename or command-line options.

## Tests and validation

```sh
sh run-tests.sh
python3 tests/release_checks.py
```

Run these commands on Windows with MinGW-w64 after creating `build/`.

```powershell
g++ -std=c++17 tests/tray_tests.cpp -o build/tray_tests.exe
.\build\tray_tests.exe
```

Native simulation tests require a C++17 compiler with AddressSanitizer and UndefinedBehaviorSanitizer. Release checks require Python 3, Pillow and the built Windows executables.

- **1.0.0 baseline:** 185 native C++ simulation tests and 15 asset/integration/package checks passed.
- **1.0.1 update:** 15 release checks plus four targeted text/icon checks passed. The simulation logic was unchanged.
- **1.0.2 update** passed 188 simulation and settings tests, 11 tray callback tests and 15 package checks. An isolated Windows desktop also verified that both callback formats open the app windows and that the renderer clips snow on maximized displays.
- Coverage includes effect switching, independent profiles, piles, window movement, mixed display scaling, lightning, spider state transitions and thread cutting.

The 1.0.2 runtime checks use an isolated Windows test desktop. The callback tests send the same messages as the shell and WinDock directly to the app. Static release checks cover installer dispatch and mouse-hook registration.

Detailed records: [validation](docs/VALIDIERUNG.md), [baseline test log](docs/testprotokoll.txt), [1.0.1 patch notes](docs/PATCH-1.0.1.md) and [patch test log](docs/patch-1.0.1-testprotokoll.txt).

## Project layout

```text
src/          Shared application, settings, renderer dispatcher and effect modules
res/          Windows resources, original pixel art and application icon
tests/       Native simulation and release verification tests
tools/       Asset preparation, offline preview and inspection tools
docs/        Architecture, provenance, validation and preview images
licenses/    Third-party runtime notices
```

## Credits and license

**[flipkips](https://github.com/flipkips)** created **[LayerSnow / desktop-snow-overlay](https://github.com/flipkips/desktop-snow-overlay)**, which provides the original snow effect and the Windows overlay, window detection, tray and installation foundation.

**[povargg](https://github.com/povargg)** developed the rain, pixel leaves and Halloween adaptations and combined them into Ultimate Desktop Effects.

The original attribution and copyright notices are retained. The project is distributed under the **[MIT License](LICENSE)**; additional MinGW-w64 and GCC runtime notices are in [`licenses/`](licenses/). Pixel asset generation prompts and preparation sources are included with the assets.

If you enjoy the original snow effect, consider visiting and starring [flipkips’ repository](https://github.com/flipkips/desktop-snow-overlay).
