# Backupper

A simple C++ program to copy files from the current directory into a timestamped subfolder.

## Features

- Copies all files (or only certain files, if you want) from the current directory to `backup/YYYY-MM-DD_HH-MM-SS/`
- Directories are named so they are sorted both alphabetically and by date.
- Colored terminal output for status messages
- Human-readable file size and time formatting
- Error logging to `back.log` (only if errors occur)
- Easy to switch between copying all files or just code files

## Usage

1. **Build:**
    ```sh
    g++ -o backupper back.cpp --static
    ```

2. **Run:**
    ```sh
    ./backupper
    ```

3. **Switch between all files and programming files:**
    - To copy only programming files, use:
      ```cpp
      std::vector<std::string> programmingExtensions = {".cpp", ".h", ".py", ".java", ".cs"};
      ```
    - To copy all files, use:
      ```cpp
      std::vector<std::string> programmingExtensions = {};
      ```

## Notes

- Only files in the current directory are copied (no subdirectories).
- Backups are stored in a `backup` folder in the current directory.
- Errors are logged to `back.log` if they occur.


## Example Output

```
Created backup directory: /home/user/project/backup/2025-06-05_14-23-01
Copying: main.cpp -> /backup/2025-06-05_14-23-01/main.cpp [OK]
Copying: notes.txt -> /backup/2025-06-05_14-23-01/notes.txt [OK]
────────────────────────────────────────────────────────────
Time used: 0.01 Seconds, Total bytes copied: 7.00 KB
```

---

*Knock knock. Lettuce in, it's time to back up your files!*