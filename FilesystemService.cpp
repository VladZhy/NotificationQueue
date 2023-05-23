#include "FilesystemService.h"

std::filesystem::path FilesystemService::generateFilePath(const std::string& fileName, const std::string& fileExtension, const std::string& directoryName) {
    const std::filesystem::path directoryPath = directoryName;
    std::filesystem::path filePath = directoryPath / fileName += fileExtension;
    
    if (!std::filesystem::exists(directoryPath)) {
        std::filesystem::create_directory(directoryPath);
    }

    int fileIndex = 0;

    while (std::filesystem::exists(filePath)) {
        ++fileIndex;
        
        filePath.replace_filename(fileName + "-" + std::to_string(fileIndex) + fileExtension);
    }

    return filePath;
}