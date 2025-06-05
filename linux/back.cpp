#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>      // For std::string
#include <cstdint>     // For uintmax_t
#include <stdio.h>
#include <memory> // Add this at the top

namespace fs = std::filesystem;


// ANSI escape codes for coloring terminal output
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string RESET = "\033[0m";
const std::string YELLOW_BRIGHT = "\033[93m";  // Bright yellow


std::string formatSize(uintmax_t size) {
    const char* baseUnits[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    int unitIndex = 0;
    double reducedSize = static_cast<double>(size);
    while (reducedSize >= 1024 && unitIndex < 8) {
        reducedSize /= 1024;
        unitIndex++;
    }
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << reducedSize;
    return oss.str() + " " + baseUnits[unitIndex];
}

std::string formatTime(double seconds) {
    if (seconds < 0.001) {
        // Less than 1 millisecond, show in microseconds
        double microseconds = seconds * 1000000;  // Convert seconds to microseconds
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(3) << microseconds;
        return oss.str() + " Microseconds";
    }

    if (seconds < 1) {
        // Less than 1 second, show in milliseconds
        double milliseconds = seconds * 1000;  // Convert seconds to milliseconds
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(3) << milliseconds;
        return oss.str() + " Milliseconds";
    }
    
    if (seconds < 60) {
        // Less than 1 minute, show in seconds
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(3) << seconds;
        return oss.str() + " Seconds";
    }

    if (seconds < 3600) {
        // Less than 1 hour, show in minutes
        double minutes = seconds / 60;
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(3) << minutes;
        return oss.str() + " Minutes";
    }

    // For 1 hour or more, show in hours
    double hours = seconds / 3600;
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3) << hours;
    return oss.str() + " Hours";
}

// Function to generate current timestamp in the format "YYYY-MM-DD_HH-MM-SS"
std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    
    // Use the desired format "YYYY-MM-DD_HH-MM-SS"
    oss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d_%H-%M-%S");
    
    return oss.str();
}



void logError(const std::string& message); // <-- Add this line before copyProgrammingFiles

void copyProgrammingFiles(const fs::path& sourceDir, const fs::path& backupDir) {

    try {
        auto startTime = std::chrono::high_resolution_clock::now();
        uint64_t totalBytesCopied = 0;

        // Create backup directory named after the timestamp
        std::string timestamp = getCurrentTimestamp();
        fs::path timeBasedBackupDir = backupDir / timestamp;

        if (!fs::exists(timeBasedBackupDir)) {
            fs::create_directories(timeBasedBackupDir);
            std::cout << GREEN << "Created backup directory: " << timeBasedBackupDir << RESET << std::endl;
        }

        // Extensions considered programming files
        // To copy all files, leave the vector empty: {}
        //aka uncomment the line below to copy all files with explicit extensions
        
        //std::vector<std::string> programmingExtensions = {".cpp", ".h", ".py", ".java", ".cs"};
         std::vector<std::string> programmingExtensions = {}; // Uncomment to copy all files

        // Traverse only the current directory (no recursion into subfolders)
        for (const auto& entry : fs::directory_iterator(sourceDir)) {
            if (entry.is_regular_file()) {
                // Skip files that are already in the backup directory
                if (entry.path().parent_path() == timeBasedBackupDir) {
                    continue; // Skip copying files already in the backup directory
                }

                std::string extension = entry.path().extension().string();
                std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);  // Case-insensitive comparison

                // If vector is empty, copy all files. Otherwise, filter by extension.
                if (programmingExtensions.empty() ||
                    std::find(programmingExtensions.begin(), programmingExtensions.end(), extension) != programmingExtensions.end()) {

                    std::string fileName = entry.path().filename().string();
                    std::cout << "Copying: " << fileName << " -> /backup/" << timestamp << "/" << fileName;

                    try {
                        // Perform the copy operation
                        fs::copy(entry.path(), timeBasedBackupDir / entry.path().filename());

                        // Update total bytes copied
                        totalBytesCopied += fs::file_size(entry.path());

                        // Print [OK] after file copy (same line, [OK] in green)
                        std::cout << " " << RESET << "[" << GREEN << "OK" << RESET << "]" << std::endl;
                    } catch (const fs::filesystem_error& ex) {
                        std::string errMsg = std::string(RED) + "Error copying file: " + entry.path().string() + " - " + ex.what() + RESET;
                        std::cout << " " << RESET << "[" << RED << "Err" << RESET << "]" << std::endl;
                        std::cerr << errMsg << std::endl;
                        logError(errMsg);
                    }
                }
            }
        }

        // Calculate and display summary
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = endTime - startTime;
        

        std::string TotalDisplay =  "Time used: " + formatTime(duration.count()) + ", Total bytes copied: " + formatSize(totalBytesCopied);
        


    // Print the line of '─' characters directly
    std::cout << YELLOW_BRIGHT;
    for (size_t i = 0; i < TotalDisplay.length(); ++i) {
        std::cout << "─";
    }
    std::cout << RESET << std::endl;

       // std::wcout << TheFancyLine;
        std::cout << TotalDisplay << std::endl;





    } 
        
    catch (const fs::filesystem_error& ex) {
        std::string errMsg = std::string(RED) + "Error accessing source directory: " + ex.what() + RESET;
        std::cerr << errMsg << std::endl;
        logError(errMsg);
    }

}

void logError(const std::string& message) {
    static std::unique_ptr<std::ofstream> errorFile;
    if (!errorFile) {
        errorFile = std::make_unique<std::ofstream>("back.log", std::ios::app);
    }
    if (errorFile && *errorFile) {
        *errorFile << message << std::endl;
    }
}


int main() {
    fs::path sourceDir = fs::current_path();  // Current working directory
    fs::path backupDir = sourceDir / "backup"; // Create backup inside current directory

    copyProgrammingFiles(sourceDir, backupDir);

    return 0;
}
