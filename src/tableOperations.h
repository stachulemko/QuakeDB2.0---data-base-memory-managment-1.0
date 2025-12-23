#ifndef TABLEOPERATIONS_H
#define TABLEOPERATIONS_H
#include "allTuple.h"
#include "../../bufforing-stm/src/QuakeSharedBuffers.h"


void addTupleToTable(tuple &tupleData, int32_t tableId){
    latestBufferFromDiskForInsert(tableId);
    
}







#endif