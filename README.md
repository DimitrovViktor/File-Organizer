# File & Directory Organizer

A C++ CLI application that automatically organizes files and directories based on file extensions and categories. This tool helps maintain clean directory structures by sorting files into appropriate folders.

---

![App](https://github.com/user-attachments/assets/ccd4cd8d-545d-489e-a4c8-a3b2de4224f8)

---

## Features:

Log - every time an important action is performed it is recorded in a log file which is automatically created in the directory where the app is located.

### Organize Files
Organization function that creates category folders and sorts files by extension (this process is repeated inside each subdirectory)
 - Takes file extensions
 - Divides files into categories
 - Creates category folders
 - Sorts files
 - Relocates files to category folders

### Check files
 - Lists all files in directory and prints them out

### Merge Folders
```
Note: I suggest using this after organizing files.
```
Moves files from categorized folders from subdirectories to category folders inside the parent(chosen) directory and assigns random names to files

 - Renames files by using a randomized string to ensure duplicate names don't clash
 - Merges folders with the same names
 - Deletes empty folders
 - Takes files from within sorted folders and moves them to sorted folders with the same names in their parent directories (if present)

## Supported File Types:
- Documents: PDF, DOC, TXT, XLS, PPT, and 15+ other formats
- Media: JPG, PNG, MP3, MP4, and 30+ image/audio/video formats
- Code: C++, Java, Python, HTML, CSS, and 20+ programming languages
- Archives: ZIP, RAR, 7Z, TAR, and other compressed formats
- System Files: EXE, MSI, INI, LOG, DB, and system-related formats
 
## Potential future work and improvements:
 - Fix for file names in different alphabets - either recognize them or rename them using latin characters so they are recognized
 - Add multithreading and optmize for speed and efficiency (in case of large folder sorting)
