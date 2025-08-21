# Backupper

## Author
keRveL

## Description

!FOR v 2.0 see releases!

Backupper is a simple Windows utility that copies all files from a selected directory to a timestamped backup folder. The backup folder is named using a timestamp in the format `backups/YYYY-MM-DD_HH-MM-SS`, which means your backups are both lexicographically (alphabetically) and chronologically sorted for easy browsing. It is designed to be run from the Windows Explorer context menu and uses message boxes for all user interaction (no console window).

**Note:** The word "backup" in this program refers to creating a copy of the files in the selected directory. This is not a "real" backup in the traditional sense of the word. It simply creates a copy of the files.

## Features
- Confirms with the user before starting the backup process.
- Checks if the directory contains any files; exits gracefully if none are found.
- Creates a timestamped backup folder in the source directory under `backups\YYYY-MM-DD_HH-MM-SS`.
- Copies all files from the source directory to the backup folder.
- Displays a success message with a list of all copied files.
- Handles errors gracefully and informs the user if a file cannot be copied.

## Usage
1. **Build the program**  
   Compile with:
   ```
   g++ back.cpp -o backupper.exe -mwindows
   g++ context_menu_toggle.cpp -o context_menu_toggle.exe -mwindows -lshlwapi -municode
   ```
2. **Add to Context Menu**  
   Use the `addremove.exe` program to add or remove the context menu entry for ">> Run Backupper <<".

3. **Run the Backup**  
   - Right-click in any folder background in Explorer.
   - Select ">> Run Backupper <<".
   - The program will back up all files in that folder.

4. **Run by Double-Clicking**  
   - If no folder is selected via the context menu, the program will back up files in the current working directory.

## Notes
- The backup folder will be created as `backups\YYYY-MM-DD_HH-MM-SS` inside the selected directory.
- Only regular files are backed up; subdirectories are ignored.
- All user prompts and errors are shown via message boxes.
- This is not a "real" backup in the traditional sense of the word. It simply creates a copy of the files in the selected directory.

## License
[Specify your license here]

Backupper is a simple utility that creates a copy of the files in a selected directory and places them in a timestamped subfolder named `backups/YYYY-MM-DD_HH-MM-SS`. This repository contains versions for both Windows and Linux.

**Practical tip:**  
You can either:
- Place the files in a custom program directory (for example: `Program Files\keRveLsBackupper\`) and use the context menu option (recommended),  
- **OR** simply place `backupper.exe` inside the directory you want to copy and double-click it to make a backup of that directory.

A personal tool, one of the simplest yet most frequently used. So why not share it? It may be a peculiar program, but if the shoe fits, wear it, Cinderella.

## Platforms

### Windows
- The Windows version uses a graphical interface with message boxes for user interaction.
- It integrates with the Windows Explorer context menu for easy access.

[See the Windows version](windows/ReadMe.md)

### Linux
- The Linux version is a command-line utility.
- It uses standard Linux tools and libraries for file operations.

[See the Linux version](linux/ReadMe.md)
