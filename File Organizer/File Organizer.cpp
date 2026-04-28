#include <iostream>
#include <filesystem>
#include <vector>
#include <map>
#include <fstream>
#include <ctime>
#include <thread>
#include <mutex>
#include <algorithm>
#include <set>

namespace fs = std::filesystem;

std::mutex fsMutex;
std::mutex logMutex;
std::mutex coutMutex;

const std::string SKIP_SUFFIX = "_KEEP";

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

std::set<std::string> ALL_KNOWN_EXTENSIONS;

void buildExtensionSet() {
    for (const auto& category : CATEGORIES) {
        for (const auto& ext : category.second) {
            ALL_KNOWN_EXTENSIONS.insert(ext);
        }
    }
}


struct ProjectSignature {
    std::string subfolder;
    std::vector<std::string> markerFiles;
    std::vector<std::string> markerDirs;
};

const std::vector<ProjectSignature> PROJECT_SIGNATURES = {

    // coding projects
    { "coding_projects", { ".sln", ".vcxproj", ".csproj" },           { ".vs" } },                    // visual studio
    { "coding_projects", { "Cargo.toml" },                            {} },                            // rust
    { "coding_projects", { "go.mod" },                                {} },                            // go
    { "coding_projects", { "CMakeLists.txt", "Makefile" },            {} },                            // cmake / make
    { "coding_projects", { "package.json" },                          { "node_modules" } },            // node.js
    { "coding_projects", { "pom.xml", "build.gradle" },               { ".idea" } },                   // java / intellij
    { "coding_projects", { "requirements.txt", "setup.py", "pyproject.toml" }, { ".venv", "venv" } },  // python
    { "coding_projects", { "Gemfile" },                               {} },                            // ruby
    { "coding_projects", { "composer.json" },                         {} },                            // php
    { "coding_projects", { ".gitignore" },                            { ".git" } },                    // generic git repo

    // audio projects
    { "audio_projects",  { ".flp" },             {} },         // fl studio
    { "audio_projects",  { ".als" },             {} },         // ableton
    { "audio_projects",  { ".logicx" },          {} },         // logic pro
    { "audio_projects",  { ".rpp" },             {} },         // reaper
    { "audio_projects",  { ".cpr" },             {} },         // cubase
    { "audio_projects",  { ".ptx", ".ptf" },     {} },         // pro tools

    // visual projects
    { "visual_projects", { ".blend", ".blend1" },                     {} },                            // blender
    { "visual_projects", { ".psd" },                                  {} },                            // photoshop
    { "visual_projects", { ".aep" },                                  {} },                            // after effects
    { "visual_projects", { ".prproj" },                               {} },                            // premiere pro
    { "visual_projects", { ".drp" },                                  {} },                            // davinci resolve
    { "visual_projects", { ".xcf" },                                  {} },                            // gimp
    { "visual_projects", { ".kra" },                                  {} },                            // krita

    // game engine projects
    { "game_projects",   {},                     { "Assets", "ProjectSettings" } },  // unity
    { "game_projects",   { ".uproject" },        { "Content" } },                    // unreal engine
    { "game_projects",   { "project.godot" },    {} },                               // godot

    // notes projects
    { "notes_projects",  {},                     { ".obsidian" } },   // obsidian vault
    { "notes_projects",  { ".enex" },            {} },                // evernote export
    { "notes_projects",  {},                     { ".notion" } },     // notion export
};


std::string genLog();

std::string askDir(std::string& target_dir, std::ofstream& logfile);

void fileCheck(const fs::path& target_dir, std::ofstream& logfile);

void organize(const fs::path& target_dir, const fs::path& root_dir, std::ofstream& logfile);

fs::path resolveDuplicate(const fs::path& destinationDir, const fs::path& fileName, const fs::path& sourcePath = "");

void merger(const fs::path& target_dir, const fs::path& target_dir_initial, std::ofstream& logfile);

std::string detectProject(const fs::path& dir);

void organizeProjects(const fs::path& target_dir, std::ofstream& logfile);

// protected -> projects -> organize -> merge
void fullOrganize(const fs::path& target_dir, std::ofstream& logfile);

void collectProtected(const fs::path& target_dir, std::ofstream& logfile);

void safeMove(const fs::path& src, const fs::path& dst);
void safeCreateDir(const fs::path& dir);
void logWrite(std::ofstream& logfile, const std::string& msg);
void safePrint(const std::string& msg);

bool isProtected(const std::string& name);

std::string pathToUtf8(const fs::path& p);

bool isKnownExtension(const std::string& ext);

void moveToUnknown(const fs::path& filePath, const fs::path& root_dir, std::ofstream& logfile);


int main()
{
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    buildExtensionSet();

    int operationCounter = 0;
    std::string logName = genLog();
    std::ofstream logfile(logName);

    std::string target_dir;
    std::string target_dir_initial;
    int userOption;

    do {
        std::cout << "=====================\n File Organizer\n =====================\n "
            << "Options:\n"
            << "1. Full organize (detects projects, sorts files, merges folders)\n"
            << "2. Organize files only (sorts files into category folders)\n"
            << "3. Check file names and extensions\n"
            << "4. Merge (organize folders)\n"
            << "5. Detect and organize projects only\n"
            << "Your Option: ";
        std::cin >> userOption;



        switch (userOption) {
        case 1:
            logfile << "CHOSEN OPTION: 1. Full organize\n";
            askDir(target_dir, logfile);

            fullOrganize(target_dir, logfile);
            operationCounter++;
            std::cout << "=====================\n File Organizer\n =====================\n ";
            std::cout << "Operation complete. \n"
                << "Projects have been extracted, files organized, and folders merged.\n"
                << "Options:\n"
                << "6. Go back to main menu \n"
                << "0. Quit\n"
                << "Your Option: ";
            std::cin >> userOption;
            break;

        case 2:
            logfile << "CHOSEN OPTION: 2. Organize files\n";
            askDir(target_dir, logfile);

            organize(target_dir, target_dir, logfile);
            operationCounter++;
            std::cout << "=====================\n File Organizer\n =====================\n ";
            std::cout << "Operation complete. \n"
                << "Files have been organized.\n"
                << "If you wish to organize folders you can merge.\n"
                << "Options:\n"
                << "4. Merge(organize folders)\n"
                << "5. Detect and organize projects\n"
                << "6. Go back to main menu \n"
                << "0. Quit\n"
                << "Your Option: ";
            std::cin >> userOption;
            break;

        case 3:
            logfile << "CHOSEN OPTION: 3. Check file names and extensions\n";
            askDir(target_dir, logfile);
            fileCheck(target_dir, logfile);
            operationCounter++;
            break;

        case 4:
            logfile << "CHOSEN OPTION: 4. Merge(organize folders)\n";
            askDir(target_dir, logfile);

            target_dir_initial = target_dir;
            merger(target_dir, target_dir_initial, logfile);
            operationCounter++;
            std::cout << "=====================\n File Organizer\n =====================\n ";
            std::cout << "Operation complete. \n"
                << "Folders have been merged.\n"
                << "Options:\n"
                << "4. Merge(organize folders)\n"
                << "5. Detect and organize projects\n"
                << "6. Go back to main menu \n"
                << "0. Quit\n"
                << "Your Option: ";
            std::cin >> userOption;
            break;

        case 5:
            logfile << "CHOSEN OPTION: 5. Detect and organize projects\n";
            askDir(target_dir, logfile);
            organizeProjects(target_dir, logfile);
            operationCounter++;
            std::cout << "=====================\n File Organizer\n =====================\n ";
            std::cout << "Operation complete. \n"
                << "Projects have been organized.\n"
                << "Options:\n"
                << "6. Go back to main menu \n"
                << "0. Quit\n"
                << "Your Option: ";
            std::cin >> userOption;
            break;

        case 6:
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
#ifdef _WIN32
    localtime_s(&timeinfo, &timestamp);
#else
    localtime_r(&timestamp, &timeinfo);
#endif

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
    std::cin.ignore();
    std::getline(std::cin, target_dir);
    logfile << "CHOSEN Directory: " << target_dir << "\n";
    target_dir;
    return target_dir;
};


std::string pathToUtf8(const fs::path& p) {
    try {
        auto u8 = p.u8string();
        return std::string(u8.begin(), u8.end());
    }
    catch (...) {
        return p.string();
    }
}

void logWrite(std::ofstream& logfile, const std::string& msg) {
    std::lock_guard<std::mutex> lock(logMutex);
    logfile << msg;
    logfile.flush();
}

void safePrint(const std::string& msg) {
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << msg;
}

void safeCreateDir(const fs::path& dir) {
    std::lock_guard<std::mutex> lock(fsMutex);
    std::error_code ec;
    if (!fs::exists(dir, ec)) {
        if (fs::create_directories(dir, ec)) {
            std::cout << "Directory created: " << pathToUtf8(dir.filename()) << "\n";
        }
        else {
            std::cout << "Failed to create: " << pathToUtf8(dir.filename()) << " (" << ec.message() << ")\n";
        }
    }
}

void safeMove(const fs::path& src, const fs::path& dst) {
    std::lock_guard<std::mutex> lock(fsMutex);
    std::error_code ec;

    if (fs::equivalent(src, dst, ec) && !ec) return;
    if (src == dst) return;

    if (fs::exists(src, ec)) {
        fs::rename(src, dst, ec);
        if (ec) {
            // fall back to copy+delete if rename fails (cross-drive, special chars, etc.)
            fs::copy(src, dst, fs::copy_options::recursive | fs::copy_options::overwrite_existing, ec);
            if (!ec) {
                fs::remove_all(src, ec);
            }
        }
    }
}

bool isProtected(const std::string& name) {
    if (name.size() < SKIP_SUFFIX.size()) return false;
    return name.compare(name.size() - SKIP_SUFFIX.size(), SKIP_SUFFIX.size(), SKIP_SUFFIX) == 0;
}

bool isKnownExtension(const std::string& ext) {
    if (ext.empty()) return false;
    std::string lower = ext;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return ALL_KNOWN_EXTENSIONS.count(lower) > 0;
}

void moveToUnknown(const fs::path& filePath, const fs::path& root_dir, std::ofstream& logfile) {
    fs::path unknownDir = root_dir / "Preserved" / "Unknown_extensions";
    safeCreateDir(unknownDir);

    fs::path dest;
    {
        std::lock_guard<std::mutex> lock(fsMutex);
        dest = resolveDuplicate(unknownDir, filePath.filename(), filePath);
    }

    logWrite(logfile, "Unknown extension: [FILE] " + pathToUtf8(filePath) + " [DEST] " + pathToUtf8(dest) + "\n");
    safePrint("Unknown extension: " + pathToUtf8(filePath.filename()) + " -> Preserved/Unknown_extensions/\n");

    safeMove(filePath, dest);
}


void fileCheck(const fs::path& target_dir, std::ofstream& logfile) {

    logWrite(logfile, "Printing out files inside: " + pathToUtf8(target_dir) + "\n");

    std::cout << pathToUtf8(target_dir) << "\nContains the following files:\n";

    try {
        for (const auto& entry : fs::directory_iterator(target_dir, fs::directory_options::skip_permission_denied)) {

            try {
                std::cout << pathToUtf8(entry.path()) << std::endl;

                if (is_directory(entry)) {

                    logWrite(logfile, "Checking subdirectory: [PARENT DIRECTORY]" + pathToUtf8(target_dir) + " [SUBDIRECTORY] " + pathToUtf8(entry.path()) + "\n");
                    fileCheck(entry, logfile);
                }
            }
            catch (const std::exception& e) {
                logWrite(logfile, "Error checking entry: " + std::string(e.what()) + "\n");
                safePrint("Warning: could not check entry, skipping (" + std::string(e.what()) + ")\n");
            }
        }
    }
    catch (const std::exception& e) {
        logWrite(logfile, "Error iterating directory: " + pathToUtf8(target_dir) + " " + std::string(e.what()) + "\n");
        safePrint("Warning: could not iterate directory " + pathToUtf8(target_dir) + " (" + std::string(e.what()) + ")\n");
    }
}


std::string detectProject(const fs::path& dir) {

    try {
        for (const auto& sig : PROJECT_SIGNATURES) {

            for (const auto& markerFile : sig.markerFiles) {
                std::error_code ec;
                if (fs::exists(dir / markerFile, ec)) {
                    return sig.subfolder;
                }
            }

            for (const auto& markerDir : sig.markerDirs) {
                std::error_code ec;
                if (fs::exists(dir / markerDir, ec) && fs::is_directory(dir / markerDir, ec)) {
                    return sig.subfolder;
                }
            }
        }
    }
    catch (...) {}

    return "";
}


void organizeProjects(const fs::path& target_dir, std::ofstream& logfile) {

    std::vector<fs::path> dirs;
    try {
        for (const auto& entry : fs::directory_iterator(target_dir, fs::directory_options::skip_permission_denied)) {
            try {
                if (is_directory(entry)) {
                    std::string name = entry.path().filename().string();
                    if (CATEGORIES.find(name) != CATEGORIES.end() || name == "Projects_sorted" || name == "Preserved") {
                        continue;
                    }
                    if (isProtected(name)) {
                        logWrite(logfile, "Skipping protected directory: " + pathToUtf8(entry.path()) + "\n");
                        safePrint("Skipping (protected): " + name + "\n");
                        continue;
                    }
                    dirs.push_back(entry.path());
                }
            }
            catch (const std::exception& e) {
                logWrite(logfile, "Error reading entry in organizeProjects: " + std::string(e.what()) + "\n");
            }
        }
    }
    catch (const std::exception& e) {
        logWrite(logfile, "Error iterating directory in organizeProjects: " + std::string(e.what()) + "\n");
        safePrint("Warning: could not iterate directory (" + std::string(e.what()) + ")\n");
        return;
    }

    std::vector<std::thread> threads;

    for (const auto& dir : dirs) {

        threads.emplace_back([&target_dir, &logfile, dir]() {

            try {
                std::string projectType = detectProject(dir);

                if (projectType.empty()) {
                    try {
                        for (const auto& sub : fs::directory_iterator(dir, fs::directory_options::skip_permission_denied)) {
                            try {
                                if (is_directory(sub)) {
                                    std::string subType = detectProject(sub.path());
                                    if (!subType.empty()) {
                                        fs::path projectsRoot = target_dir / "Projects_sorted" / subType;
                                        safeCreateDir(projectsRoot);

                                        fs::path dest;
                                        {
                                            std::lock_guard<std::mutex> lock(fsMutex);
                                            dest = resolveDuplicate(projectsRoot, sub.path().filename());
                                        }

                                        logWrite(logfile, "Project detected: [TYPE] " + subType + " [PATH] " + pathToUtf8(sub.path()) + " [DEST] " + pathToUtf8(dest) + "\n");
                                        safePrint("Project found: " + pathToUtf8(sub.path().filename()) + " -> " + subType + "\n");

                                        safeMove(sub.path(), dest);
                                    }
                                }
                            }
                            catch (const std::exception& e) {
                                logWrite(logfile, "Error checking nested project: " + std::string(e.what()) + "\n");
                            }
                        }
                    }
                    catch (const std::exception& e) {
                        logWrite(logfile, "Error iterating for nested projects: " + std::string(e.what()) + "\n");
                    }
                    return;
                }

                fs::path projectsRoot = target_dir / "Projects_sorted" / projectType;
                safeCreateDir(projectsRoot);

                fs::path dest;
                {
                    std::lock_guard<std::mutex> lock(fsMutex);
                    dest = resolveDuplicate(projectsRoot, dir.filename());
                }

                logWrite(logfile, "Project detected: [TYPE] " + projectType + " [PATH] " + pathToUtf8(dir) + " [DEST] " + pathToUtf8(dest) + "\n");
                safePrint("Project found: " + pathToUtf8(dir.filename()) + " -> " + projectType + "\n");

                safeMove(dir, dest);
            }
            catch (const std::exception& e) {
                logWrite(logfile, "Error in organizeProjects thread: " + std::string(e.what()) + "\n");
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    logWrite(logfile, "Project organization complete\n");
}


void collectProtected(const fs::path& target_dir, std::ofstream& logfile) {

    fs::path preservedRoot = target_dir / "Preserved";

    std::vector<fs::path> found;
    try {
        for (auto it = fs::recursive_directory_iterator(target_dir, fs::directory_options::skip_permission_denied);
            it != fs::recursive_directory_iterator(); ++it) {

            try {
                if (!it->is_directory()) continue;

                std::string name = it->path().filename().string();

                if (it->path() == preservedRoot) {
                    it.disable_recursion_pending();
                    continue;
                }

                if (isProtected(name)) {
                    found.push_back(it->path());
                    it.disable_recursion_pending(); // don't go deeper inside a _KEEP folder
                }
            }
            catch (const std::exception& e) {
                logWrite(logfile, "Error scanning for protected folders: " + std::string(e.what()) + "\n");
            }
        }
    }
    catch (const std::exception& e) {
        logWrite(logfile, "Error in collectProtected iteration: " + std::string(e.what()) + "\n");
        safePrint("Warning: error scanning for protected folders (" + std::string(e.what()) + ")\n");
    }

    if (found.empty()) return;

    safeCreateDir(preservedRoot);

    for (const auto& dir : found) {
        fs::path dest;
        {
            std::lock_guard<std::mutex> lock(fsMutex);
            dest = resolveDuplicate(preservedRoot, dir.filename());
        }

        logWrite(logfile, "Protected folder moved: [FROM] " + pathToUtf8(dir) + " [TO] " + pathToUtf8(dest) + "\n");
        safePrint("Protected folder moved: " + pathToUtf8(dir.filename()) + " -> Preserved/\n");

        safeMove(dir, dest);
    }
}


void fullOrganize(const fs::path& target_dir, std::ofstream& logfile) {

    // step 1: collect _KEEP folders into Preserved before anything else
    std::cout << "\n--- Step 1/4: Collecting protected folders ---\n";
    logWrite(logfile, "FULL ORGANIZE Step 1: Collecting protected folders in " + pathToUtf8(target_dir) + "\n");
    collectProtected(target_dir, logfile);

    // step 2: extract projects first so organize doesn't dismantle them
    std::cout << "\n--- Step 2/4: Detecting and extracting projects ---\n";
    logWrite(logfile, "FULL ORGANIZE Step 2: Detecting projects in " + pathToUtf8(target_dir) + "\n");
    organizeProjects(target_dir, logfile);

    // step 3: organize remaining files into category folders
    std::cout << "\n--- Step 3/4: Organizing files into categories ---\n";
    logWrite(logfile, "FULL ORGANIZE Step 3: Organizing files in " + pathToUtf8(target_dir) + "\n");
    organize(target_dir, target_dir, logfile);

    // step 4: merge category folders from subdirectories into the top level
    std::cout << "\n--- Step 4/4: Merging folders ---\n";
    logWrite(logfile, "FULL ORGANIZE Step 4: Merging folders in " + pathToUtf8(target_dir) + "\n");
    merger(target_dir, target_dir, logfile);

    logWrite(logfile, "FULL ORGANIZE complete for " + pathToUtf8(target_dir) + "\n");
}


void organize(const fs::path& target_dir, const fs::path& root_dir, std::ofstream& logfile) {

    std::string fileExt;
    std::string entryName;

    logWrite(logfile, "Iterating through: " + pathToUtf8(target_dir) + "\n");

    std::vector<fs::directory_entry> entries;
    try {
        for (const auto& entry : fs::directory_iterator(target_dir, fs::directory_options::skip_permission_denied)) {
            try {
                entries.push_back(entry);
            }
            catch (const std::exception& e) {
                logWrite(logfile, "Error snapshotting entry: " + std::string(e.what()) + "\n");
            }
        }
    }
    catch (const std::exception& e) {
        logWrite(logfile, "Error iterating directory in organize: " + pathToUtf8(target_dir) + " " + std::string(e.what()) + "\n");
        safePrint("Warning: could not iterate " + pathToUtf8(target_dir) + " (" + std::string(e.what()) + ")\n");
        return;
    }

    std::vector<fs::directory_entry> files;
    std::vector<fs::directory_entry> subdirs;

    for (const auto& entry : entries) {
        try {
            entryName = pathToUtf8(entry.path().filename());

            if (is_directory(entry)) {
                if (CATEGORIES.find(entry.path().filename().string()) != CATEGORIES.end()
                    || entry.path().filename().string() == "Projects_sorted"
                    || entry.path().filename().string() == "Preserved") {
                    continue;
                }

                if (!detectProject(entry.path()).empty()) {
                    logWrite(logfile, "Skipping project directory: " + pathToUtf8(entry.path()) + "\n");
                    safePrint("Skipping project: " + entryName + "\n");
                    continue;
                }

                if (isProtected(entry.path().filename().string())) {
                    logWrite(logfile, "Skipping protected directory: " + pathToUtf8(entry.path()) + "\n");
                    safePrint("Skipping (protected): " + entryName + "\n");
                    continue;
                }

                subdirs.push_back(entry);
            }
            else {
                files.push_back(entry);
            }
        }
        catch (const std::exception& e) {
            logWrite(logfile, "Error classifying entry: " + std::string(e.what()) + "\n");
        }
    }

    std::vector<std::thread> threads;
    for (const auto& subdir : subdirs) {
        threads.emplace_back([&root_dir, &logfile, subdir]() {
            try {
                logWrite(logfile, "Organizing SUBDIRECTORY: " + pathToUtf8(subdir.path()) + "\n");
                organize(subdir.path(), root_dir, logfile);
            }
            catch (const std::exception& e) {
                logWrite(logfile, "Error in organize thread: " + std::string(e.what()) + "\n");
            }
        });
    }

    for (const auto& entry : files) {

        try {
            fileExt = entry.path().extension().string();

            std::string fileExtLower = fileExt;
            std::transform(fileExtLower.begin(), fileExtLower.end(), fileExtLower.begin(), ::tolower);

            bool matched = false;

            for (const auto& category : CATEGORIES) {

                const std::string& categoryName = category.first;
                const std::vector<std::string>& extensions = category.second;

                fs::path initialPath = entry.path();
                fs::path fileToMove = initialPath.filename();

                for (const std::string& existingExt : extensions) {

                    if (fileExtLower == existingExt) {

                        matched = true;

                        fs::path categoryDir = target_dir / categoryName;

                        safeCreateDir(categoryDir);

                        fs::path destinationPath;
                        {
                            std::lock_guard<std::mutex> lock(fsMutex);
                            destinationPath = resolveDuplicate(categoryDir, fileToMove, initialPath);
                        }

                        safeMove(initialPath, destinationPath);

                        safePrint("File: " + pathToUtf8(entry.path())
                            + "\n Extension: " + fileExt
                            + "\n Category: " + categoryName + "\n");
                        logWrite(logfile, "File: " + pathToUtf8(entry.path())
                            + "| Extension: " + fileExt
                            + "| Category: " + categoryName + "\n");

                        break; // matched this category, stop checking extensions
                    }
                }
                if (matched) break; // stop checking other categories
            }

            if (!matched) {
                moveToUnknown(entry.path(), root_dir, logfile);
            }
        }
        catch (const std::exception& e) {
            logWrite(logfile, "Error processing file: " + std::string(e.what()) + "\n");
            safePrint("Warning: could not process a file (" + std::string(e.what()) + ")\n");
        }
    }

    for (auto& t : threads) {
        t.join();
    }
};


fs::path resolveDuplicate(const fs::path& destinationDir, const fs::path& fileName, const fs::path& sourcePath) {

    fs::path candidate = destinationDir / fileName;

    std::error_code ec;
    if (!fs::exists(candidate, ec)) {
        return candidate;
    }

    // source file is not a conflict with itself
    if (!sourcePath.empty() && fs::exists(sourcePath, ec) && fs::equivalent(candidate, sourcePath, ec) && !ec) {
        return candidate;
    }

    std::string stem = fileName.stem().string();
    std::string ext = fileName.extension().string();
    int counter = 1;

    while (fs::exists(candidate, ec)) {
        if (!sourcePath.empty() && fs::equivalent(candidate, sourcePath, ec) && !ec) {
            return candidate;
        }

        std::string suffix = "_" + std::to_string(counter);

        std::string newStem = stem;
        int maxStemLen = 200 - (int)ext.size() - (int)suffix.size();
        if ((int)newStem.size() > maxStemLen && maxStemLen > 0) {
            newStem = newStem.substr(0, maxStemLen);
        }

        candidate = destinationDir / (newStem + suffix + ext);
        counter++;
    }

    return candidate;
}

void merger(const fs::path& target_dir, const fs::path& target_dir_initial, std::ofstream& logfile) {

    std::string entryName;
    std::string innerEntryExt;

    std::vector<fs::directory_entry> entries;
    try {
        for (const auto& entry : fs::directory_iterator(target_dir, fs::directory_options::skip_permission_denied)) {
            try {
                entries.push_back(entry);
            }
            catch (const std::exception& e) {
                logWrite(logfile, "Error snapshotting entry in merger: " + std::string(e.what()) + "\n");
            }
        }
    }
    catch (const std::exception& e) {
        logWrite(logfile, "Error iterating directory in merger: " + pathToUtf8(target_dir) + " " + std::string(e.what()) + "\n");
        safePrint("Warning: could not iterate in merger (" + std::string(e.what()) + ")\n");
        return;
    }

    std::vector<std::thread> threads;

    for (const auto& entry : entries) { // iterate through directory
        try {
            entryName = entry.path().filename().string();

            if (is_directory(entry) && CATEGORIES.find(entryName) != CATEGORIES.end()) {

                merger(entry, target_dir_initial, logfile);

                std::vector<fs::directory_entry> innerEntries;
                try {
                    for (const auto& innerEntry : fs::directory_iterator(entry, fs::directory_options::skip_permission_denied)) {
                        try {
                            innerEntries.push_back(innerEntry);
                        }
                        catch (...) {}
                    }
                }
                catch (const std::exception& e) {
                    logWrite(logfile, "Error iterating inner entries: " + std::string(e.what()) + "\n");
                    continue;
                }

                for (const auto& innerEntry : innerEntries) {
                    try {
                        innerEntryExt = innerEntry.path().extension().string();
                        std::string innerExtLower = innerEntryExt;
                        std::transform(innerExtLower.begin(), innerExtLower.end(), innerExtLower.begin(), ::tolower);

                        bool matched = false;

                        for (const auto& category : CATEGORIES) {

                            const std::string& categoryName = category.first;
                            const std::vector<std::string>& extensions = category.second;

                            for (const std::string& existingExt : extensions) {

                                if (innerExtLower == existingExt) {

                                    matched = true;

                                    fs::path initialPath = innerEntry.path();
                                    fs::path fileToMove = initialPath.filename();

                                    safeCreateDir(target_dir_initial / categoryName);

                                    fs::path destinationPath;
                                    {
                                        std::lock_guard<std::mutex> lock(fsMutex);
                                        destinationPath = resolveDuplicate(target_dir_initial / categoryName, fileToMove, initialPath);
                                    }

                                    if (destinationPath.filename() != fileToMove) {
                                        logWrite(logfile, "File name changed: [OLD NAME] " + pathToUtf8(innerEntry.path()) + " [NEW NAME] " + pathToUtf8(destinationPath.filename()) + "\n");
                                    }

                                    logWrite(logfile, "Moving file [OLD PATH] " + pathToUtf8(initialPath) + " [NEW PATH] " + pathToUtf8(destinationPath) + "\n");

                                    safeMove(initialPath, destinationPath);
                                    break;
                                }
                            }
                            if (matched) break;
                        }

                        if (!matched && !is_directory(innerEntry)) {
                            moveToUnknown(innerEntry.path(), target_dir_initial, logfile);
                        }
                    }
                    catch (const std::exception& e) {
                        logWrite(logfile, "Error processing inner entry in merger: " + std::string(e.what()) + "\n");
                    }
                }
            }
            else if (is_directory(entry)) {

                if (entryName == "Projects_sorted" || entryName == "Preserved") {
                    continue;
                }

                if (isProtected(entryName)) {
                    logWrite(logfile, "Skipping protected directory: " + pathToUtf8(entry.path()) + "\n");
                    safePrint("Skipping (protected): " + entryName + "\n");
                    continue;
                }

                threads.emplace_back([entry, &target_dir_initial, &logfile]() {
                    try {
                        merger(entry, target_dir_initial, logfile);
                    }
                    catch (const std::exception& e) {
                        logWrite(logfile, "Error in merger thread: " + std::string(e.what()) + "\n");
                    }
                });
            }
        }
        catch (const std::exception& e) {
            logWrite(logfile, "Error processing entry in merger: " + std::string(e.what()) + "\n");
        }

    }

    for (auto& t : threads) {
        t.join();
    }

    for (const auto& entry : entries) {
        try {
            std::error_code ec;
            if (is_directory(entry) && fs::exists(entry, ec) && fs::is_empty(entry, ec)) {
                std::lock_guard<std::mutex> lock(fsMutex);
                fs::remove(entry, ec);
            }
        }
        catch (...) {}
    }
};
