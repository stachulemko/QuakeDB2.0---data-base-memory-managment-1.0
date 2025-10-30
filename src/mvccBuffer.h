#ifndef SRC_MVCCBUFFER_H
#define SRC_MVCCBUFFER_H
#include <cstdint>
#include <iostream>
#include "allTuple.h"

#define testMVCCbufferSize 200  // test for now 

std::vector<MVCCBuffer*>* mvccBuffer = nullptr;
enum class RowStatus { COMMITTED, DELETED, IN_PROGRESS, NOT_EXIST ,ABORTED}; // not exist means no version

struct MVCCBuffer {
    int64_t xmin = -1;
    int64_t xmax = -1; 
    int64_t userId = -1;  // user id
    RowStatus status = RowStatus::NOT_EXIST;
    int32_t blockNum = -1;   // optional
    int8_t operationType = -1; // 0 - insert, 1 - delete, 2 - update
    tuple *tupleData = nullptr;
};


void initMvccSharedBuffers(int32_t bufferSize) {
    size_t bufferSizeMegaBytes = bufferSize;
    static bool initialized = false;
    if(initialized) return;

    initialized = true;
    int32_t numBuffers = (bufferSizeMegaBytes * 1024) / (testMVCCbufferSize / 1024);
    mvccBuffer = new std::vector<MVCCBuffer*>(numBuffers, nullptr);
}

#endif  