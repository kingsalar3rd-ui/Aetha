# AETHA — Geode mod

AETHA is a cross-platform Geometry Dash utility mod targeting Windows 64-bit and Android 64-bit.

## Current scaffold (v0.1.0)

- Adds an AETHA button to the main menu.
- Opens a basic AETHA Core popup.
- Adds a gameplay telemetry overlay.
- Displays an estimated update rate, average update delta in milliseconds, and PlayLayer update-tick count.
- Exposes settings for enabling the counter, showing ticks, refresh interval, and future interface accent color.
- Includes GitHub Actions builds for Windows x64 and Android arm64.

## Important telemetry note

The displayed `FPS*` is an estimate based on `PlayLayer::update` calls, not a guaranteed measurement of rendered display frames. The tick counter counts calls to that gameplay update hook. It is intentionally labelled this way until a render-loop measurement is implemented and validated.

## Compatibility target

This project pins Geode SDK `v5.11.2` and Geometry Dash `2.2081` for Windows and Android, matching the intended 2.2081-era target. Do not change the SDK version without checking the installed Geode loader and target game version.

## Build locally

Install the Geode CLI, CMake, Ninja, a supported C++ toolchain, and (for Android) the Android NDK. Then:

```sh
geode sdk install v5.11.2
geode sdk install-binaries -p android64
```

Build Windows:

```sh
geode build -p Win64
```

Build Android arm64:

```sh
geode build -p android64
```

The exact CLI target spelling can vary by CLI version; `geode build --help` lists supported targets. The official build action in `.github/workflows/multi-platform.yml` is configured for `Win64` and `Android64`.

## Current limitations

- This is a source scaffold, not a compiled `.geode` package.
- The interface is a first functional placeholder, not the final customizable menu.
- Frame stepping, physics inspection, and advanced gameplay utilities are not implemented yet.
- Build and runtime validation must be completed against the exact installed Geode loader and game builds.
