#include "fileManager.h"


// Funkcja do zapisywania bloku do pliku .bin
bool saveBlock(const std::string& path, const std::string& fileName, const std::vector<uint8_t>& blockData) {
    std::string fullPath = path + "/" + fileName + ".bin";
    
    std::ofstream file(fullPath, std::ios::binary | std::ios::out);
    
    if (!file.is_open()) {
        //std::cerr << "Nie można utworzyć pliku: " << fullPath << std::endl;
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(blockData.data()), blockData.size());
    file.close();
    
    //std::cout << "Blok zapisany (" << blockData.size() << " bajtów): " << fullPath << std::endl;
    return true;
}

// Funkcja do odczytywania bloku z pliku .bin
std::vector<uint8_t> loadBlock(const std::string& path, const std::string& fileName) {
    std::string fullPath = path + "/" + fileName + ".bin";
    std::vector<uint8_t> blockData;
    
    std::ifstream file(fullPath, std::ios::binary | std::ios::in | std::ios::ate);
    
    if (!file.is_open()) {
        //std::cerr << "Nie można otworzyć pliku: " << fullPath << std::endl;
        return blockData; // Zwróć pusty vector
    }
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    blockData.resize(size);
    file.read(reinterpret_cast<char*>(blockData.data()), size);
    file.close();
    
    //std::cout << "Blok wczytany (" << size << " bajtów): " << fullPath << std::endl;
    return blockData;
}



bool createBinFile(const std::string& path, const std::string& fileName) {
    std::string fullPath = path + "/" + fileName + ".bin";

    // Do not overwrite if it already exists
    if (std::filesystem::exists(fullPath)) {
        LOG_ERROR("File already exists, not overwriting: " + fullPath);
        return false;
    }

    std::ofstream file(fullPath, std::ios::binary | std::ios::out | std::ios::trunc);

    if (!file.is_open()) {
        LOG_ERROR("Cannot create file: " + fullPath);
        return false;
    }
    LOG_DEBUG("Clear file was created :" + fullPath);
    file.close();
    return true;
}


int32_t getFileSize(const std::string& path, const std::string& fileName){
    std::string fullPath = path + "/" + fileName;
    std::ifstream file(fullPath, std::ios::binary | std::ios::ate);
    
    if (!file.is_open()) {
        //std::cerr << "Nie można otworzyć pliku: " << fullPath << std::endl;
        return -1; // Zwróć -1 w przypadku błędu
    }
    
    std::streamsize size = file.tellg();
    file.close();
    
    return static_cast<int32_t>(size);
}


void saveBlockWithNum(const std::string& path, const std::string& fileName, const std::vector<uint8_t>& blockData,int32_t blockNum){
    std::string fullPath = path + "/" + fileName;
    std::ofstream file(fullPath, std::ios::binary | std::ios::in | std::ios::out);
    LOG_DEBUG("data size to save: "<<blockData.size()<<" to file: "<<fullPath<<" at block number: "<<blockNum<<"\n");
    if (!file.is_open()) {
        LOG_ERROR("Cannot open file: " + fullPath);
        //std::cerr << "Nie można otworzyć pliku: " << fullPath << std::endl;
        return;
    }
    
    file.seekp(blockNum * blockSize, std::ios::beg);
    file.write(reinterpret_cast<const char*>(blockData.data()), blockData.size());
    file.close();
}

bool checkIfFileExists(const std::string& path, const std::string& fileName){
    std::string fullPath = path + "/" + fileName;
    return std::filesystem::exists(fullPath);
}

void clearFolder(const std::string& path){
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        std::filesystem::remove_all(entry.path());
    }
}