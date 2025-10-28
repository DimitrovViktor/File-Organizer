#include <iostream>
#include <filesystem>
#include <vector>
#include <map>
#include <fstream>

namespace fs = std::filesystem;

const std::map<std::string, std::vector<std::string>> CATEGORIES = {

    {"Documents", {
        ".pdf", ".doc", ".docx", ".txt", ".rtf", ".odt", ".tex", ".md", ".wps",
        ".xls", ".xlsx", ".ods", ".csv", ".tsv", ".ppt", ".pptx", ".odp", ".key"
    }},


    {"Images", {
        ".jpg", ".jpeg", ".png", ".gif", ".bmp", ".tiff", ".tif", ".svg", ".webp",
        ".heic", ".ico", ".raw", ".psd", ".xcf", ".ai", ".eps"
    }},


    {"Audio", {
        ".mp3", ".wav", ".aac", ".flac", ".ogg", ".m4a", ".wma", ".aiff", ".mid", ".midi"
    }},


    {"Videos", {
        ".mp4", ".mkv", ".avi", ".mov", ".wmv", ".flv", ".webm", ".mpeg", ".mpg", ".3gp"
    }},


    {"Archives", {
        ".zip", ".rar", ".7z", ".tar", ".gz", ".bz2", ".xz", ".iso", ".cab"
    }},


    {"Code", {
        ".c", ".cpp", ".h", ".hpp", ".cs", ".java", ".py", ".js", ".ts", ".html", ".htm",
        ".css", ".php", ".rb", ".swift", ".go", ".kt", ".rs", ".sh", ".bat", ".json", ".xml",
        ".yml", ".yaml", ".sql", ".lua", ".pl", ".m", ".r"
    }},


    {"Executables", {
        ".exe", ".msi", ".bin", ".sh", ".run", ".app", ".jar", ".bat", ".cmd", ".apk"
    }},


    {"System", {
        ".ini", ".cfg", ".conf", ".log", ".sys", ".tmp", ".bak", ".db", ".db3", ".sqlite"
    }},


    {"Shortcuts", {
        ".lnk", ".url", ".desktop"
    }},


    {"Design", {
        ".dwg", ".dxf", ".blend", ".skp", ".3ds", ".obj", ".stl", ".step", ".stp"
    }},


    {"Data", {
        ".json", ".xml", ".yaml", ".yml", ".dat", ".bin", ".db", ".sql", ".sav", ".pkl", ".parquet"
    }},


    {"Misc", {
        ".torrent", ".ics", ".apk", ".crdownload", ".part", ".bak"
    }}
};


void organize(const fs::path& target_dir);

int main()
{
    std::string target_dir;
    int userOption;

    do {
        std::cout << "=====================\n File Organizer\n =====================\n "
            << "Options:\n"
            << "1. Check file names and extensions \n"
            << "2. Organize files (will create folders for each category)\n"
            << "Your Option: ";
        std::cin >> userOption;

        switch (userOption) {
            case 1:
                std::cout << "=====================\n File Organizer\n =====================\n ";
                std::cout << "Enter Directory Path: ";
                std::cin >> target_dir;
                organize(target_dir);
                std::cout << "=====================\n File Organizer\n =====================\n ";
                std::cout << "Operation complete."
                          << "Options:\n"
                          << "4. Go back to main menu \n"
                          << "0. Quit\n"
                          << "Your Option: ";
                std::cin >> userOption;
                break;
            case 2:

                break;

            default:
                userOption = 0;
        }


    } while (userOption != 0);



    return 0;
}

void organize(const fs::path& target_dir) {

    // 0
    std::string fileExt;

    // 1. Iterate through directory and display file names
    std::cout << "\nFiles:\n";
    for (const auto& entry : fs::directory_iterator(target_dir)) {
        std::cout << entry.path() << std::endl;
    }

    // 2. Extract file extensions
    // display each file name, extension and category
    std::cout << "\nExtensions:\n";

    for (const auto& entry : fs::directory_iterator(target_dir)) {
        fileExt = entry.path().extension().string();
        for (const auto& category : CATEGORIES) {

            const std::string& categoryName = category.first;
            const std::vector<std::string>& extensions = category.second;

            for (const std::string& existingExt : extensions) {
                if (fileExt == existingExt) {
                    std::cout << "File: "
                        << entry.path()
                        << "\n Extension: "
                        << fileExt
                        << "\n Category: "
                        << categoryName << "\n";
                }
            }
        }
        

    }

};
