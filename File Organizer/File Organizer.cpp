#include <iostream>
#include <filesystem>
#include <vector>
#include <map>
#include <fstream>

namespace fs = std::filesystem;

const std::map<std::string, std::vector<std::string>> CATEGORIES = {

    {"Documents_sorted", {
        ".pdf", ".doc", ".docx", ".txt", ".rtf", ".odt", ".tex", ".md", ".wps",
        ".xls", ".xlsx", ".ods", ".csv", ".tsv", ".ppt", ".pptx", ".odp", ".key"
    }},


    {"Images_sorted", {
        ".jpg", ".jpeg", ".png", ".gif", ".bmp", ".tiff", ".tif", ".svg", ".webp",
        ".heic", ".ico", ".raw", ".psd", ".xcf", ".ai", ".eps"
    }},


    {"Audio_sorted", {
        ".mp3", ".wav", ".aac", ".flac", ".ogg", ".m4a", ".wma", ".aiff", ".mid", ".midi"
    }},


    {"Videos_sorted", {
        ".mp4", ".mkv", ".avi", ".mov", ".wmv", ".flv", ".webm", ".mpeg", ".mpg", ".3gp"
    }},


    {"Archives_sorted", {
        ".zip", ".rar", ".7z", ".tar", ".gz", ".bz2", ".xz", ".iso", ".cab"
    }},


    { "Code_sorted", {
        ".c", ".cpp", ".h", ".hpp", ".cs", ".java", ".py", ".js", ".ts", ".html", ".htm",
        ".css", ".php", ".rb", ".swift", ".go", ".kt", ".rs", ".sh", ".bat", ".json", ".xml",
        ".yml", ".yaml", ".sql", ".lua", ".pl", ".m", ".r"
    } },


    { "Executables_sorted", {
        ".exe", ".msi", ".bin", ".sh", ".run", ".app", ".jar", ".bat", ".cmd", ".apk"
    } },


    { "System_sorted", {
        ".ini", ".cfg", ".conf", ".log", ".sys", ".tmp", ".bak", ".db", ".db3", ".sqlite"
    } },


    { "Shortcuts_sorted", {
        ".lnk", ".url", ".desktop"
    } },


    { "Design_sorted", {
        ".dwg", ".dxf", ".blend", ".skp", ".3ds", ".obj", ".stl", ".step", ".stp"
    } },


    { "Data_sorted", {
        ".json", ".xml", ".yaml", ".yml", ".dat", ".bin", ".db", ".sql", ".sav", ".pkl", ".parquet"
    } },


    { "Misc_sorted", {
        ".torrent", ".ics", ".apk", ".crdownload", ".part", ".bak"
    } }
};


void organize(const fs::path& target_dir);

int main()
{
    std::string target_dir;
    int userOption;

    do {
        std::cout << "=====================\n File Organizer\n =====================\n "
            << "Options:\n"
            << "1. Organize files (will create folders for each category)\n"
            << "2. Check file names and extensions\n"
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
            // Iterate through directory and display file names
            std::cout << target_dir << "\nContains the following files:\n";
            for (const auto& entry : fs::directory_iterator(target_dir)) {
                std::cout << entry.path() << std::endl;
            }

            break;

        default:
            userOption = 0;
        }


    } while (userOption != 0);



    return 0;
}

void organize(const fs::path& target_dir) {

    std::string fileExt;
    bool recStop;
    std::string entryName;

    for (const auto& entry : fs::directory_iterator(target_dir)) { // iterate through directory
        recStop = 0;
        fileExt = entry.path().extension().string();
        entryName = entry.path().filename().string();
        
        if (is_directory(entry) && CATEGORIES.find(entryName) != CATEGORIES.end()) {
            recStop = 1;
        }
        for (const auto& category : CATEGORIES) {

            const std::string& categoryName = category.first;
            const std::vector<std::string>& extensions = category.second;
            
            fs::path initialPath = entry.path();
            fs::path fileToMove = initialPath.filename();

            if (is_directory(initialPath) && recStop == 0) { // if file is a directory call organize function on it too
                organize(initialPath);
                continue;
            }

            
            fs::path destinationPath = (target_dir / categoryName / fileToMove);
            

            for (const std::string& existingExt : extensions) {

                if (fileExt == existingExt) { // check if file extension is in categories

                    if (!fs::exists(target_dir / categoryName)) { // if there is no category folder create one
                        if (fs::create_directory(target_dir / categoryName)) {
                            std::cout << "Directory created: " << categoryName << "\n";
                            fs::rename(initialPath, destinationPath); // move file to category folder
                        }
                        else {
                            std::cout << "Failed to create: " << categoryName << "\n";
                        }
                    }
                    else {
                        std::cout << "Directory already exists \n";
                        fs::rename(initialPath, destinationPath);
                    }

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
