# File & Directory Organizer

A multithreaded C++ CLI tool that organizes files into categorized folders, detects project directories, and merges scattered subdirectories with automatic logging.

![App](https://github.com/user-attachments/assets/ccd4cd8d-545d-489e-a4c8-a3b2de4224f8)

## Features

- **Full Organize** — runs the complete pipeline: preserves protected folders, extracts projects, sorts files by extension, and merges subdirectories.
- **File Sorting** — categorizes files into folders (Documents, Images, Audio, Videos, Code, Archives, etc.) recursively through subdirectories.
- **Project Detection** — recognizes coding (VS, Cargo, Node, Python…), audio (FL Studio, Ableton…), visual (Blender, Premiere…), and game engine projects and groups them under `Projects_sorted/`.
- **Folder Merging** — consolidates category folders from subdirectories into the root, resolving duplicate filenames automatically.
- **File Inspection** — lists all files and extensions across nested directories.
- **Protected Folders** — append `_KEEP` to any folder name to exclude it from sorting.
- **Unknown Extensions** — files with unrecognized or missing extensions are moved to `Preserved/Unknown_extensions/`.
- **Logging** — every operation is recorded in a timestamped log file.

## Installation

```bash
g++ -std=c++17 -pthread -o file_organizer file_organizer.cpp
```

## Usage

```bash
./file_organizer
```

Select an option from the menu:

```
1. Full organize (detects projects, sorts files, merges folders)
2. Organize files only
3. Check file names and extensions
4. Merge (organize folders)
5. Detect and organize projects only
```

Then enter the target directory path when prompted.
