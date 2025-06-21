#include <iostream>
#include <filesystem>
#include <vector>
#include <map>
#include <fstream>

namespace fs = std::filesystem;

const std::map<std::string, std::vector<std::string>> CATEGORIES = {
    {"Documents", {"pdf", "docx", "txt"}},
    {"Images", {"jpg", "png", "gif"}},
};

void organize(const fs::path& target_dir) {

    // 1. Iterate through directory
    // 
    // 2. Extract file extensions
    // 
    // 3. Move files to category folders
    // 
    // 4. Handle name collisions
    // 
    // 5. Generate operation log
}

int main()
{
    std::cout << "File Organizer";
    return 0;
}