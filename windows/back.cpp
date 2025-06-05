/*
 * Program: Backupper
 * Author: keRveL
 * Date: May 2025
 * Description:
 * This program is a simple backup utility designed to copy all files from a specified directory
 * to a backup folder. The backup folder is created dynamically with a timestamp to ensure unique
 * backups. The program is invoked via the context menu in Windows Explorer and uses message boxes
 * for user interaction (no console window).
 *
 * Features:
 * - Confirms with the user before starting the backup process.
 * - Checks if the directory contains any files; exits gracefully if none are found.
 * - Creates a timestamped backup folder in the source directory.
 * - Copies all files from the source directory to the backup folder.
 * - Displays a success message with a list of all copied files.
 * - Handles errors gracefully and informs the user if a file cannot be copied.
 *
 * Usage:
 * - Add the program to the Windows Explorer context menu using a registry entry.
 * - Right-click in a directory and select the program to back up files in that directory.
 */

#include "back.h"
#include <windows.h>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <vector>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Ask the user for confirmation
    int response = MessageBoxW(
        NULL,
        L"Are you sure you want to run the backup?",
        L"Confirm Backup",
        MB_OKCANCEL | MB_ICONQUESTION
    );

    if (response == IDCANCEL) {
        // User chose "Cancel"
        MessageBoxW(NULL, L"Backup operation canceled.", L"Canceled", MB_OK | MB_ICONINFORMATION);
        return 0; // Exit the program
    }

    // Get the command-line arguments
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    // Determine the source directory
    std::wstring currentPath;
    if (argc < 2) {
        // No command-line argument provided, use the current working directory
        wchar_t buffer[MAX_PATH];
        if (GetCurrentDirectoryW(MAX_PATH, buffer)) {
            currentPath = buffer;
        } else {
            MessageBoxW(NULL, L"Failed to get the current working directory!", L"Error", MB_OK | MB_ICONERROR);
            return 1;
        }
    } else {
        // Use the directory provided as a command-line argument
        currentPath = argv[1];
    }

    // Check if there are any files in the directory
    bool hasFiles = false;
    for (const auto& entry : std::filesystem::directory_iterator(currentPath)) {
        if (entry.is_regular_file()) {
            hasFiles = true;
            break;
        }
    }

    if (!hasFiles) {
        // No files in the directory
        MessageBoxW(NULL, L"No files found in the directory to back up.", L"No Files Found", MB_OK | MB_ICONINFORMATION);
        return 0; // Exit the program
    }

    // Generate a timestamp for the backup folder
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::wstringstream timestamp;
    timestamp << std::put_time(std::localtime(&time_t_now), L"%Y-%m-%d_%H-%M-%S");

    // Create the backup directory
    std::wstring backupDir = std::filesystem::path(currentPath) / L"backups" / timestamp.str();
    std::filesystem::create_directories(backupDir);

    // List to store the names of successfully copied files
    std::vector<std::wstring> copiedFiles;

    // Iterate through all files in the source directory
    for (const auto& entry : std::filesystem::directory_iterator(currentPath)) {
        if (entry.is_regular_file()) {
            // Get the source file path
            std::wstring sourceFile = entry.path().wstring();

            // Construct the destination file path
            std::wstring destinationFile = (std::filesystem::path(backupDir) / entry.path().filename()).wstring();

            // Call the copyFile function
            auto result = copyFile(std::string(sourceFile.begin(), sourceFile.end()),
                                   std::string(destinationFile.begin(), destinationFile.end()));

            // Check the result
            if (std::holds_alternative<std::wstring>(result)) {
                // If the result is an error message
                std::wstring errorMessage = L"Error copying file: " + std::get<std::wstring>(result) +
                    L"\nSource: " + sourceFile +
                    L"\nDestination: " + destinationFile +
                    L"\nOperation aborted!";
                
                int response = MessageBoxW(
                    NULL,
                    errorMessage.c_str(),
                    L"File Copy Error",
                    MB_OK | MB_ICONERROR);
                return 1; // Exit the program
            } else {
                // Add the file name to the list of copied files
                copiedFiles.push_back(entry.path().filename().wstring());
            }
        }
    }

    // Create a success message with the list of copied files
    std::wstringstream successMessage;
    successMessage << L"All files copied successfully!\n\nCopied Files: [" << copiedFiles.size() << L"]\n";
    for (const auto& file : copiedFiles) {
        successMessage << L"- " << file << L"\n";
    }

    // Display the success message
    MessageBoxW(NULL, successMessage.str().c_str(), L"Success", MB_ICONINFORMATION);

    return 0;
}