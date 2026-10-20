# Chrome++ Portable
[![LICENSE](https://img.shields.io/badge/License-GPL--3.0--only-blue.svg?style=for-the-badge&logo=github "LICENSE")](LICENSE) [![LAST COMMIT](https://img.shields.io/github/last-commit/rexx-oss/chrome_plus?color=blue&logo=github&style=for-the-badge "LAST COMMIT")](https://github.com/rexx-oss/chrome_plus/commits/main)

A lightweight `version.dll` proxy injection project for Chromium-based browsers (Google Chrome, Brave, etc.). It strips machine-bound encryption (Windows DPAPI) and redirects user data to provide true, self-contained cross-machine portability.

## Overview
- **True Portability Across PCs:** Bypasses Windows DPAPI encryption so saved website logins, cookies, and sessions remain valid when moving your portable drive to any other computer.
- **Pure Portability Focus:** Stripped of intrusive UI Automation, mouse gestures, and tab hooks for maximum stability across all Chromium derivatives (including Brave).
- **Zero Host Traces:** Keeps profile data and cache isolated to designated local folders.

## Capability Overview

### 1. Cross-Machine Persistence & Portability
- **DPAPI Encryption Bypass:** Intercepts `CryptProtectData` to store the master cookie key in a machine-independent format, preventing logouts when changing computers.
- **Hardware ID Masking:** Spoofs volume serial and machine ID queries to prevent hardware-bound profile invalidation.
- **Directory Redirection:** Customizes `data_dir` and `cache_dir` relative to the executable (`%app%`).
- **Command Injection:** Appends custom Chromium startup switches via `command_line`.
- **Process Automation:** Executes scripts or binaries on browser start (`launch_on_startup`) and exit (`launch_on_exit`).

### 2. Environment & Policy Controls
- **Ignore Enterprise Policies:** Suppresses host machine domain/enterprise policies with `ignore_policies`.
- **Silence Update Prompts:** Suppresses false "browser is out of date / relaunch" notices on portable builds with `suppress_false_upgrade_notification`.
- **Saved Password Access:** Views saved passwords in settings without triggering Windows PIN/password prompts via `show_password`.
- **Win32k Mitigation Control:** Provides `win32k` fallback support if startup mitigations conflict on specific Windows builds.

## Installation
1. Place compiled `version.dll` and `chrome++.ini` directly in the browser's directory (alongside `chrome.exe` or `brave.exe`).
2. Configure your desired data paths in `chrome++.ini` (default points to `..\data`).
3. Launch the browser normally.

> **Note on Existing Profiles:** To ensure logins are portable, log into your accounts *after* launching with `version.dll` so credentials are saved using the machine-independent format.

## Configuration Reference
All settings are managed via [`src/chrome++.ini`](src/chrome++.ini):

```ini
[general]
data_dir=%app%\..\data
cache_dir=%app%\..\data\Cache
command_line=
launch_on_startup=
launch_on_exit=
show_password=1
ignore_policies=1
suppress_false_upgrade_notification=1
win32k=0

## License
- Distributed under the GPL-3.0 License.
- Based on the original GreenChrome architecture by Shuax and the Chrome++ Next project by Bush2021.
