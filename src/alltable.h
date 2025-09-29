#ifndef ALLTABLE_H
#define ALLTABLE_H

#include <iostream>
#include "tableHeader.h"
#include "allTuple.h"
#include <vector>
#include "block8kb.h"

class allTable{
    private:
        tableHeader *header = nullptr;
        std::vector<block8kb*> blocks;



        int32_t freeSpace = 0 ;
        int32_t unitSize = 0 ;
    public:
        allTable(int32_t oid,int8_t contain_toast,int32_t numberOfColumns,int64_t owner,int8_t pg_namespace,int32_t pg_constraint,int8_t rights,int32_t freeSpace, int32_t unitSize){
            //int32_t oid,int8_t contain_toast,int32_t numberOfColumns,int64_t owner,int8_t pg_namespace,int32_t pg_constraint,int8_t rights,int32_t freeSpace, int32_t unitSize
            header = new tableHeader(oid,contain_toast,numberOfColumns,owner,pg_namespace,pg_constraint,rights,freeSpace,unitSize);
            this->freeSpace = freeSpace;
            this->unitSize = unitSize;
        }

        ~allTable();

        std::vector<uint8_t> marshallAllTable();

        void unmarshallAllTable(const std::vector<uint8_t>& data);

        void setHeaderData(int32_t oid, int8_t contain_toast, int32_t numberOfColumns, int64_t owner, int8_t pg_namespace, int32_t pg_constraint, int8_t rights){
            header->setData(oid,contain_toast,numberOfColumns,owner,pg_namespace,pg_constraint,rights,freeSpace,unitSize);
        }


        
};






#endif