#ifndef ALLTABLE_H
#define ALLTABLE_H

#include <iostream>
#include "tableHeader.h"
#include "allTuple.h"
#include <vector>
#include "block8kb.h"

class allTable{
    private:
        tableHeader header;
        std::vector<block8kb> blocks;

        int32_t freeSpace = 0 ;
        int32_t unitSize = 0 ;
    public:
        allTable(int32_t freeSpace, int32_t unitSize);

        ~allTable();

        std::vector<uint8_t> marshallAllTable();

        void unmarshallAllTable(const std::vector<uint8_t>& data);

        
};






#endif