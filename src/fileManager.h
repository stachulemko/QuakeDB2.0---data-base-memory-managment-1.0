#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include "../../bufforing-stm/src/log.h"
#include "block8kb.h"
#include <filesystem>



const std::string PathTableRows = "";
const std::string PathTableHeaders = ""; 

bool saveBlock(const std::string& path, const std::string& fileName, const std::vector<uint8_t>& blockData);
void saveBlockWithNum(const std::string& path, const std::string& fileName, const std::vector<uint8_t>& blockData,int32_t blockNum);
std::vector<uint8_t> loadBlock(const std::string& path, const std::string& fileName);
bool createBinFile(const std::string& path, const std::string& fileName);
std::vector<uint8_t> loadFileRange(const std::string& filePath, size_t startOffset, size_t bytesToRead);
std::vector<uint8_t> loadBlockRange(const std::string& path, const std::string& fileName, const std::string& extension, size_t startOffset, size_t bytesToRead);
int32_t getFileSize(const std::string& path, const std::string& fileName);
bool checkIfFileExists(const std::string& path, const std::string& fileName);
void clearFolder(const std::string& path);

#endif




