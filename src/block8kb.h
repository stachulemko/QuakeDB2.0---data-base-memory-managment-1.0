#ifndef BLOCK8KB_H
#define BLOCK8KB_H

#include <iostream>
#include <vector>
#include "blockHeader.h"
#include "allTuple.h"
#include "typesDataConverter.h"
#include "typeManagerAllVarsTypes.h"



class block8kb{
    private:
        //std::vector<uint8_t> dataBlock; // 8192 bytes
        blockHeader header;
        std::vector<tuple> tuples;

        int32_t freeSpace = 0 ;
        int32_t unitSize = 0 ;
        int32_t blockSize = 8192 ;


    public:
        block8kb(int32_t freeSpace, int32_t unitSize);

        ~block8kb();

        std::vector<uint8_t> marshallBlock8kb();

        void unmarshallBlock8kb(const std::vector<uint8_t>& data);

        //std::vector<uint8_t> getDataBlock() const { return dataBlock; }

        //void setDataBlock(const std::vector<uint8_t>& data) { dataBlock = data; }

        void setData(blockHeader& header , std::vector<tuple>& tuples);

};
#endif


