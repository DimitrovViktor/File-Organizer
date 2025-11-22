# File & Directory Organizer

This CLI app sorts files in directories into folders based on their extensions. It can also combine sorted folders from deeper within directories into a main directory and delete empty folders. 

---

![App](https://github.com/user-attachments/assets/ccd4cd8d-545d-489e-a4c8-a3b2de4224f8)

---

## Features:

Log - every time an important action is performed it is recorded in a log file which is automatically created in the directory where the app is located.

### Organize Files
 - Takes file extensions
 - Divides files into categories
 - Creates category folders
 - Sorts files
 - Relocates files to category folders

### Check files
 - Lists all files in directory

### Merge Folders
(Note: This function is created to reduce the number of sorted file folders with the same names. It's better if you use Organize Files first)
 - Renames files by using a randomized string to ensure duplicate names don't clash
 - Merges folders with the same names
 - Deletes empty folders
 - Takes files from within sorted folders and moves them to sorted folders with the same names in their parent directories (if present)
 
## To-do:
 - Fix for file names in different alphabets - either recognize them or rename them using latin characters so they are recognized
 - Add multithreading and optmize for speed and efficiency
