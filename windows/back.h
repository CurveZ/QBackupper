#ifndef BACK_H
#define BACK_H

#include <string>
#include <windows.h> // For MessageBox and Windows API functions
#include <variant> // For std::variant
#include <iostream> // For std::cout
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <vector>


// Function to copy a single file with message box notifications
inline std::variant<std::wstring, bool> copyFile(const std::string& sourceFile, const std::string& destinationFile) {
    // Convert std::string to LPCWSTR (wide string for Windows API)
    std::wstring source = std::wstring(sourceFile.begin(), sourceFile.end());
    std::wstring destination = std::wstring(destinationFile.begin(), destinationFile.end());

    // Use Windows API CopyFileW function (wide-character version)
    if (!CopyFileW(source.c_str(), destination.c_str(), FALSE)) {
        // Retrieve the error code
        DWORD errorCode = GetLastError();

        // Format the error message
        wchar_t errorMessage[256];
        FormatMessageW(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            errorCode,
            0,
            errorMessage,
            sizeof(errorMessage) / sizeof(wchar_t),
            NULL
        );

        // Return the error message as a std::wstring
        return std::wstring(errorMessage);
    }

    // If the copy succeeds, return true
    return true;
}

#endif // BACK_H