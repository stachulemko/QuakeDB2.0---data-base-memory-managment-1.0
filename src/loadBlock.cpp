#include "loadBlock.h"


std::vector<uint8_t> loadBlockFromFileNoCache(int32_t blockNum, std::string filePath){
    std::vector<uint8_t> buffer(blockSize);
    
    int fd = open(filePath.c_str(), O_RDONLY | O_DIRECT);
    if(fd < 0) {
        return buffer;
    }
    
    off_t offset = blockNum * blockSize;
    lseek(fd, offset, SEEK_SET);
    
    void* alignedBuffer = nullptr;
    if(posix_memalign(&alignedBuffer, 4096, blockSize) != 0) {
        close(fd);
        return buffer;
    }
    
    ssize_t bytesRead = read(fd, alignedBuffer, blockSize);
    
    if(bytesRead > 0) {
        memcpy(buffer.data(), alignedBuffer, bytesRead);
    }
    
    free(alignedBuffer);
    close(fd);
    return buffer;
}