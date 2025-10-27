#include "mvccGenerator.h"

namespace{
    int64_t newestTransactionId = 0;
}

void incrementTransaction(){
    newestTransactionId++;
}

int64_t getIdTransaction(){
    return newestTransactionId;
}