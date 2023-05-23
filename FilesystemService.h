#ifndef FILESYSTEM_SERVICE_H
#define FILESYSTEM_SERVICE_H
#include <filesystem>
#include <string>

class FilesystemService {
public:
    static std::filesystem::path generateFilePath(const std::string& fileName, const std::string& fileExtension, const std::string& directoryName = ".");
};

#endif