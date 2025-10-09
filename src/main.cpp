
#include <iostream>
#include <gtest/gtest.h>
#include "typesDataConverter.h"
#include "headerTuple.h"
#include "dataNullBitMapTuple.h"
#include "allTuple.h"
#include "tableHeader.h"
#include "allTableWithTableHeader.h"
int main() {
    
    DataNullBitMapTuple originalTuple;
    
    std::vector<bool> originalBitmap = {true, false, true, false, true, true};
    std::vector<allVars> originalData = {
        int32_t(12345),
        int64_t(987654321LL),
        std::string("Integration Test String"),
        int32_t(-42),
        int64_t(std::numeric_limits<int64_t>::max()),
        std::string("")
    };
    
    std::vector<uint8_t> marshalled = originalTuple.marshallDataNullBitMapTuple(originalBitmap, originalData);
    
    //EXPECT_FALSE(marshalled.empty());
    
    DataNullBitMapTuple reconstructedTuple;
    reconstructedTuple.unmarshallDataNullBitMapTuple(marshalled);
    
    std::vector<bool> resultBitmap = reconstructedTuple.getBitMap();
    std::vector<allVars> resultData = reconstructedTuple.getData();
    
}