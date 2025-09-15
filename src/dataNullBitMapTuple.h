#ifndef DATANULLBITMAPTUPLE_H
#define DATANULLBITMAPTUPLE_H

#include "typeManagerAllVarsTypes.h"
#include <iostream>
#include <bitset>
#include <vector>
#include <cstdint>

/*
+-----------------------+
| Null bitmap           | <-- depend on the number of rows 
+-----------------------+
| (data)                | <-- data 
+-----------------------+
*/

class DataNullBitMapTuple{
    private:
        std::vector<bool>bitMap;
        std::vector<allVars>dataValues; 
        int32_t sizeAll=0;

        //int32_t freeSize = 0 ; 
        //int8_t unitSize = 0 ;
    public:
        DataNullBitMapTuple();
        
        void unmarshallDataNullBitMapTuple(std::vector<uint8_t> inputData);

        std::vector<uint8_t> marshallDataNullBitMapTuple(std::vector<bool>bitMap,std::vector<allVars>data);
        
        std::vector<bool> getBitMap() const { return bitMap; }
        std::vector<allVars> getData() const { return dataValues; }
};

#endif