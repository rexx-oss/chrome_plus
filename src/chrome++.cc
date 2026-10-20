#include <windows.h>
#include <psapi.h>
#include "detours.h"
#include "config.h"
#include "green.h"
#include "hijack.h"
#include "policies.h"
#include "portable.h"
#include "upgradenotification.h"
#include "utils.h"

using Startup = int (*)();
static bool should_run_exit_cmd = false;
Startup ExeMain = nullptr;

void ChromePlus() {
  MakeGreen();                         // Strips DPAPI and Machine ID
  IgnorePolicies();                    // Ignores enterprise policies
  SuppressFalseUpgradeNotification();  // Silences "out of date" notice
}

void ChromePlusCommand(LPWSTR param) {
  if (!wcsstr(param, L"--portable")) {
    Portable(param);
  } else {
    ChromePlus();
    LaunchCommands(config.GetLaunchOnStartup());
    should_run_exit_cmd = true;
  }
}

int Loader() {
  LPWSTR param = GetCommandLineW();
  if (!wcsstr(param, L"-type=")) {
    ChromePlusCommand(param);
  }
  return ExeMain();
}

void InstallLoader() {
  MODULEINFO mi;
  GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &mi, sizeof(MODULEINFO));
  ExeMain = reinterpret_cast<Startup>(mi.EntryPoint);

  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());
  DetourAttach(reinterpret_cast<LPVOID*>(&ExeMain), reinterpret_cast<void*>(Loader));
  DetourTransactionCommit();
}

__declspec(dllexport) void portable() {}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID pv) {
  if (dwReason == DLL_PROCESS_ATTACH) {
    DisableThreadLibraryCalls(hModule);
    hInstance = hModule;
    LoadSysDll(hModule);
    InstallLoader();
  } else if (dwReason == DLL_PROCESS_DETACH && ::should_run_exit_cmd) {
    LaunchCommands(config.GetLaunchOnExit());
    should_run_exit_cmd = false;
  }
  return TRUE;
}
