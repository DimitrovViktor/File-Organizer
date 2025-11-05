# File Organizer

## Features:
1. Organize Files
 - Takes file extensions
 - Splits files into categories
 - Creates category folders
 - Sorts files
 - Relocates files to category folders

2. Check files
 - Lists all files in directory

3. Merge Folders
(Note: This function is created to reduce the number of sorted file folders with the same names. It's better if you use Organize Files first)
 - Renames files by using a randomized string to ensure duplicate names don't clash
 - Merges folders with the same names
 - Deletes empty folders
 - Takes files from within sorted folders and moves them to sorted folders with the same names in their parent directories (if present)
 
## To-do:
 - Fix for file names in different languages - either recognize them or rename them using latin characters
 - Generate operation log
 - Merger function to create a category folder for moved files before moving (if not present already) or just call organize function inside
