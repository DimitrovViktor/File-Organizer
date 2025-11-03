#include <iostream>
#include <filesystem>
#include <vector>
#include <map>
#include <fstream>
#include <random>

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
        ".mp3", ".wav", ".aac", ".flac", ".ogg", ".m4a", ".wma", ".aiff", ".mid", ".midi", ".flp"
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

std::string askDir(std::string& target_dir);

void organize(const fs::path& target_dir);

std::string randNameGen();

void merger(const fs::path& target_dir);


int main()
{
    std::string target_dir;
    int userOption;
    int dirCounter;

    do {
        std::cout << "=====================\n File Organizer\n =====================\n "
            << "Options:\n"
            << "1. Organize files (will create folders for each category for sorted files)\n"
            << "2. Check file names and extensions\n"
            << "3. Merge(organize folders)\n"
            << "Your Option: ";
        std::cin >> userOption;

        switch (userOption) {
        case 1:
            askDir(target_dir);
            organize(target_dir);
            std::cout << "=====================\n File Organizer\n =====================\n ";
            std::cout << "Operation complete. \n"
                << "Files have been organized.\n"
                << "If you wish to organize folders you can merge.\n"
                << "Options:\n"
                << "3. Merge(organize folders)\n"
                << "4. Go back to main menu \n"
                << "0. Quit\n"
                << "Your Option: ";
            std::cin >> userOption;
            break;
        case 2:
            askDir(target_dir);
            // Iterate through directory and display file names
            std::cout << target_dir << "\nContains the following files:\n";
            for (const auto& entry : fs::directory_iterator(target_dir)) {
                std::cout << entry.path() << std::endl;
            }
            break;
        case 3:
            askDir(target_dir);
            merger(target_dir);
            std::cout << "=====================\n File Organizer\n =====================\n ";
            std::cout << "Operation complete. \n"
                << "Folders have been merged.\n"
                << "If you wish to organize folders you can merge.\n"
                << "Options:\n"
                << "3. Merge(organize folders)\n"
                << "4. Go back to main menu \n"
                << "0. Quit\n"
                << "Your Option: ";
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

std::string askDir(std::string& target_dir) {
    std::cout << "=====================\n File Organizer\n =====================\n ";
    std::cout << "Enter Directory Path: ";
    std::cin >> target_dir;
    return target_dir;
};

std::string randNameGen()
{

    srand((unsigned)time(NULL));
    int randNumber = 6 + (rand() % 11);

    // Define the list of possible characters
    const std::string CHARACTERS
        = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuv"
        "wxyz0123456789";

    // Create a random number generator
    std::random_device rd;
    std::mt19937 generator(rd());

    // Create a distribution to uniformly select from all
    // characters
    std::uniform_int_distribution<> distribution(
        0, CHARACTERS.size() - 1);

    // Generate the random string
    std::string random_string;
    for (int i = 0; i < randNumber; ++i) {
        random_string
            += CHARACTERS[distribution(generator)];
    }

    return random_string;
}

void merger(const fs::path& target_dir) {
    
    //bool recStop;
    std::string entryName;
    std::string innerEntryName;
    std::string innerEntryExt;

    for (const auto& entry : fs::directory_iterator(target_dir)) { // iterate through directory
        //recStop = 0;
        entryName = entry.path().filename().string();
        
        

        // check if name is in categories && file is dir
        if (is_directory(entry) && CATEGORIES.find(entryName) != CATEGORIES.end()) {
            merger(entry); // check name and files inside (recursively)
            for (const auto& innerEntry : fs::directory_iterator(entry)) {
                innerEntryExt = innerEntry.path().extension().string();
                entryName = entry.path().filename().string();
            for (const auto& category : CATEGORIES) {
                const std::string& categoryName = category.first;
                const std::vector<std::string>& extensions = category.second;

                for (const std::string& existingExt : extensions) {

                    if (innerEntryExt == existingExt) {
                        

                            innerEntryName = innerEntry.path().filename().string();
                            fs::path initialPath = innerEntry.path();
                            fs::path fileExtension = innerEntry.path().extension();
                            fs::path fileToMove = initialPath.filename();
                            fs::path destinationPath = (target_dir / categoryName / fileToMove);
                            if (innerEntry.path().string().find(target_dir.string()) == 0) {
                                fs::path newFileName = randNameGen() + fileExtension.string();
                                destinationPath = initialPath.parent_path() / newFileName;
                            }

                            fs::rename(initialPath, destinationPath); // move file to final folder
                            break;
                        }
                        if (fs::is_empty(entry) && fs::is_directory(entry)) {
                            fs::remove(entry);
                        }
                    }
                }
            }
        }
        else if (is_directory(entry)) {
            merger(entry);
            continue;
        }
     
    }
};
