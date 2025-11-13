#include "QuakeSharedBuffers.h"

std::vector<ShareBuffer*>* buffers = nullptr;

ShareBuffer::ShareBuffer() : data(blockSize) {
}

void initSharedBuffers(int32_t bufferSize) {
    size_t bufferSizeMegaBytes = bufferSize;
    static bool initialized = false;
    if(initialized) return;

    initialized = true;
    int32_t numBuffers = (bufferSizeMegaBytes * 1024) / (sharedBufferSize / 1024);
    buffers = new std::vector<ShareBuffer*>(numBuffers, nullptr);
}

void addBuffer(int32_t id, int32_t blockNum, std::vector<uint8_t> data, int32_t count,int32_t state) {
    if(data.size() != blockSize) return;
    if(buffers == nullptr) return;
    
    for(size_t i = 0; i < buffers->size(); i++) {
        if((*buffers)[i] == nullptr) {
            ShareBuffer* newBuffer = new ShareBuffer();
            newBuffer->id = id;
            newBuffer->blockNum = blockNum;
            newBuffer->data = data;
            newBuffer->count = count;
            newBuffer->pinCount = 1;
            (*buffers)[i] = newBuffer;
            return;
        }
    }
}