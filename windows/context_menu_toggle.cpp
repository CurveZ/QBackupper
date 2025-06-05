#include <windows.h>
#include <string>
#include <shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

const wchar_t* keyPath = L"SOFTWARE\\Classes\\Directory\\Background\\shell\\Backupper_by_keRveL";
const wchar_t* commandPath = L"SOFTWARE\\Classes\\Directory\\Background\\shell\\Backupper_by_keRveL\\command";
const wchar_t* menuText = L">> Run Backupper <<";

std::wstring getBackupperPath() {
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    std::wstring dirPath = exePath;
    size_t pos = dirPath.find_last_of(L"\\/");
    if (pos != std::wstring::npos) dirPath = dirPath.substr(0, pos + 1);
    std::wstring backupperPath = dirPath + L"backupper.exe";
    return backupperPath;
}

bool fileExists(const std::wstring& path) {
    return PathFileExistsW(path.c_str()) == TRUE;
}

bool contextMenuExists() {
    HKEY hKey;
    LONG result = RegOpenKeyExW(HKEY_CURRENT_USER, keyPath, 0, KEY_READ, &hKey);
    if (result == ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return true;
    }
    return false;
}

void addContextMenu(const std::wstring& backupperPath) {
    HKEY hKey;
    if (RegCreateKeyExW(HKEY_CURRENT_USER, keyPath, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        RegSetValueExW(hKey, NULL, 0, REG_SZ, (const BYTE*)menuText, (DWORD)((wcslen(menuText) + 1) * sizeof(wchar_t)));
        RegCloseKey(hKey);

        if (RegCreateKeyExW(HKEY_CURRENT_USER, commandPath, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
            std::wstring command = L"\"" + backupperPath + L"\" \"%V\"";
            RegSetValueExW(hKey, NULL, 0, REG_SZ, (const BYTE*)command.c_str(), (DWORD)((command.size() + 1) * sizeof(wchar_t)));
            RegCloseKey(hKey);
        }
    }
}

void removeContextMenu() {
    RegDeleteKeyW(HKEY_CURRENT_USER, commandPath);
    RegDeleteKeyW(HKEY_CURRENT_USER, keyPath);
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    std::wstring backupperPath = getBackupperPath();
    if (!fileExists(backupperPath)) {
        MessageBoxW(NULL, (L"Could not find backupper.exe at:\n" + backupperPath).c_str(), L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    if (contextMenuExists()) {
        // Ask for confirmation before removing
        int response = MessageBoxW(
            NULL,
            L"The context menu entry already exists.\n\nDo you want to remove it?",
            L"Remove Context Menu",
            MB_OKCANCEL | MB_ICONQUESTION
        );

        if (response == IDCANCEL) {
            //MessageBoxW(NULL, L"Operation canceled by the user.", L"Canceled", MB_OK | MB_ICONINFORMATION);
            return 0;
        }

        removeContextMenu();
        MessageBoxW(NULL, L"Context Menu Removed...", L"Backupper", MB_OK | MB_ICONINFORMATION);
    } else {
        // Ask for confirmation before adding
        int response = MessageBoxW(
            NULL,
            (L"The context menu entry does not exist.\n\nDo you want to add it?\n\nPath:\n" + backupperPath).c_str(),
            L"Add Context Menu",
            MB_OKCANCEL | MB_ICONQUESTION
        );

        if (response == IDCANCEL) {
            MessageBoxW(NULL, L"Operation canceled by the user.", L"Canceled", MB_OK | MB_ICONINFORMATION);
            return 0;
        }

        addContextMenu(backupperPath);
        MessageBoxW(
            NULL,
            (L"Context menu item added.\nPath:\n" + backupperPath + L"\n\nRun again to remove it.").c_str(),
            L"Backupper",
            MB_OK | MB_ICONINFORMATION
        );
    }

    return 0;
}