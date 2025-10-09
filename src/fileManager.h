#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>


const std::string PathTableRows = "";
const std::string PathTableHeaders = ""; 

bool saveBlock(const std::string& path, const std::string& fileName, const std::vector<uint8_t>& blockData);
std::vector<uint8_t> loadBlock(const std::string& path, const std::string& fileName);
bool createBinFile(const std::string& path, const std::string& fileName);
std::vector<uint8_t> loadFileRange(const std::string& filePath, size_t startOffset, size_t bytesToRead);
std::vector<uint8_t> loadBlockRange(const std::string& path, const std::string& fileName, const std::string& extension, size_t startOffset, size_t bytesToRead);

#endif




