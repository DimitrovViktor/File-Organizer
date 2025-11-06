#include <iostream>
#include <filesystem>
#include <vector>
#include <map>
#include <fstream>
#include <random>
#include <ctime>

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

std::string genLog();

std::string askDir(std::string& target_dir, std::ofstream& logfile);

void fileCheck(const fs::path& target_dir, std::ofstream& logfile);

void organize(const fs::path& target_dir, std::ofstream& logfile);

std::string randNameGen();

void merger(const fs::path& target_dir, const fs::path& target_dir_initial, std::ofstream& logfile);


int main()
{
    int operationCounter;
    std::string logName = genLog();
    std::ofstream logfile(logName);

    std::string target_dir;
    std::string target_dir_initial;
    int userOption;

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
            logfile << "CHOSEN OPTION: 1. Organize files\n";
            askDir(target_dir, logfile);
            

            organize(target_dir, logfile);
            operationCounter++;
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
            logfile << "CHOSEN OPTION: 2. Check file names and extensions\n";
            askDir(target_dir, logfile);
            fileCheck(target_dir, logfile);
            operationCounter++;
            break;

        case 3:
            logfile << "CHOSEN OPTION: 3. Merge(organize folders)\n";
            askDir(target_dir, logfile);

            target_dir_initial = target_dir;
            merger(target_dir, target_dir_initial, logfile);
            operationCounter++;
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

    logfile << "operations complete: " << operationCounter << std::endl;
    logfile.close();

    return 0;
}

std::string genLog() {
    time_t timestamp;
    time(&timestamp);

    struct tm timeinfo;
    localtime_s(&timeinfo, &timestamp);

    char currentTime[32];
    std::strftime(currentTime, sizeof(currentTime), "%Y%m%d_%H%M%S", &timeinfo);

    std::string logName = "log_";
    logName += currentTime;
    logName += ".txt";
    return logName;
}

std::string askDir(std::string& target_dir, std::ofstream& logfile) {
    std::cout << "=====================\n File Organizer\n =====================\n ";
    std::cout << "Enter Directory Path: ";
    std::cin >> target_dir;
    logfile << "CHOSEN Directory: " << target_dir << "\n";
    return target_dir;
};

void fileCheck(const fs::path& target_dir, std::ofstream& logfile) {

    logfile << "Printing out files inside: " << target_dir << "\n";

    std::cout << target_dir << "\nContains the following files:\n";

    for (const auto& entry : fs::directory_iterator(target_dir)) {

        std::cout << entry.path() << std::endl;

        if (is_directory(entry)) {

            logfile << "Checking subdirectory: [PARENT DIRECTORY]" << target_dir << " [SUBDIRECTORY] " << target_dir << "\n";
            fileCheck(entry, logfile);
        }
    }
}

void organize(const fs::path& target_dir, std::ofstream& logfile) {

    std::string fileExt;
    bool recStop;
    std::string entryName;

    logfile << "Iterating through: " << target_dir << "\n";

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
                logfile << "Organizing SUBDIRECTORY: " << "[PARENTDIRECTORY] " << target_dir << " [SUBDIRECTORY] " << initialPath << "\n";
                organize(initialPath, logfile);
                continue;
            }

            
            fs::path destinationPath = (target_dir / categoryName / fileToMove);
            

            for (const std::string& existingExt : extensions) {

                if (fileExt == existingExt) { // check if file extension is in categories

                    if (!fs::exists(target_dir / categoryName)) { // if there is no category folder create one
                        if (fs::create_directory(target_dir / categoryName)) {
                            logfile << "Directory created: " << categoryName << "\n";
                            std::cout << "Directory created: " << categoryName << "\n";
                            fs::rename(initialPath, destinationPath); // move file to category folder
                        }
                        else {
                            logfile << "Failed to create: " << categoryName << "\n";
                            std::cout << "Failed to create: " << categoryName << "\n";
                        }
                    }
                    else {
                        logfile << "Directory already exists: " << categoryName << "\n";
                        std::cout << "Directory already exists \n";
                        fs::rename(initialPath, destinationPath);
                    }

                    std::cout << "File: "
                        << entry.path()
                        << "\n Extension: "
                        << fileExt
                        << "\n Category: "
                        << categoryName << "\n";
                    logfile << "File: "
                        << entry.path()
                        << "| Extension: "
                        << fileExt
                        << "| Category: "
                        << categoryName << "\n";
                }
                
            }
        }
        

    }

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

void merger(const fs::path& target_dir, const fs::path& target_dir_initial, std::ofstream& logfile) {
    
    //bool recStop;
    std::string entryName;
    std::string innerEntryName;
    std::string innerEntryExt;

    for (const auto& entry : fs::directory_iterator(target_dir)) { // iterate through directory
        //recStop = 0;
        entryName = entry.path().filename().string();
        
        // check if name is in categories && file is dir
        if (is_directory(entry) && CATEGORIES.find(entryName) != CATEGORIES.end()) {

            merger(entry, target_dir_initial, logfile); // check name and files inside (recursively)

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
                            fs::path destinationPath = (target_dir_initial / categoryName / fileToMove);

                            if (innerEntry.path().string().find(target_dir.string()) == 0) {
                                fs::path newFileName = randNameGen() + fileExtension.string();
                                logfile << "File name changed: " << "[OLD NAME] " << innerEntry << " [NEW NAME] " << newFileName << "\n";

                                if (!fs::exists(target_dir_initial / categoryName) || !fs::is_directory(target_dir_initial / categoryName)) {
                                    logfile << "Creating category directorty " << (target_dir_initial / categoryName) << "\n";
                                    fs::create_directory(target_dir_initial / categoryName);
                                }

                                logfile << "Moving file " << "[OLD NAME] " << innerEntry;
                                destinationPath = (target_dir_initial / categoryName / newFileName);
                                logfile << " [NEW NAME] " << destinationPath << "\n";
                            }

                            fs::rename(initialPath, destinationPath); // move file to final folder
                            break;
                        }
                    }
                }
            }
        }
        else if (is_directory(entry)) {

            merger(entry, target_dir_initial, logfile);

            if (fs::is_empty(entry)) {
                fs::remove(entry);
            }
            continue;
        }
     
    }
};
