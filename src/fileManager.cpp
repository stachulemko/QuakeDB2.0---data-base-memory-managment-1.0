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
    
    std::ofstream file(fullPath, std::ios::binary | std::ios::out);
    
    if (!file.is_open()) {
        //std::cerr << "Nie można utworzyć pliku: " << fullPath << std::endl;
        return false;
    }
    
    file.close();
    
    //std::cout << "Pusty plik .bin utworzony: " << fullPath << std::endl;
    return true;
}