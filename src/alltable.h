#ifndef ALLTABLE_H
#define ALLTABLE_H

#include <iostream>
#include "tableHeader.h"
#include "allTuple.h"
#include <vector>
#include "block8kb.h"

class allTable{
    private:
        //tableHeader *header = nullptr;
        std::vector<block8kb*> blocks;
        block8kb* blockTmp= nullptr;
        int32_t freeSpace = 0 ;

        int32_t nextblock =0 ;
        int32_t blockIndetification =0 ;
        int32_t pd_lsn=0;          
        int16_t pd_checksum=0;   
        int16_t pd_flags=0;
        int8_t contain_toast=0;  
    private:
        
    public:
        allTable(int32_t freeSpace){
            //int32_t oid,int8_t contain_toast,int32_t numberOfColumns,int64_t owner,int8_t pg_namespace,int32_t pg_constraint,int8_t rights,int32_t freeSpace, int32_t unitSize
            //header = new tableHeader(oid,contain_toast,numberOfColumns,owner,pg_namespace,pg_constraint,rights,freeSpace,unitSize);
            //blockTmp = new block8kb()
            this->freeSpace = freeSpace;
        }

        ~allTable();

        std::vector<uint8_t> marshallAllTable();
        
        void addNewBlock(int32_t freeSpace,int32_t nextblock, int32_t blockIndetification, int32_t pd_lsn, int16_t pd_checksum, int16_t pd_flags, int8_t contain_toast){
            blockTmp = new block8kb(freeSpace,nextblock,blockIndetification,pd_lsn,pd_checksum,pd_flags,contain_toast);
        }

        void addDataToBlock(tuple& tupleObj){
            if(blockTmp==nullptr){
                addNewBlock(freeSpace,nextblock,blockIndetification,pd_lsn,pd_checksum,pd_flags,contain_toast);
            }

            if(blockTmp->getSize() + tupleObj.getSize() <= blockTmp->getblockSizeAfterFreeSpace()){
                blockTmp->addData(tupleObj);
            } 
            else {
                blocks.push_back(blockTmp);
                addNewBlock(freeSpace,nextblock,blockIndetification,pd_lsn,pd_checksum,pd_flags,contain_toast);
            }
        }

        void unmarshallAllTable(const std::vector<uint8_t>& data);

        void setHeaderData(int32_t oid, int8_t contain_toast, int32_t numberOfColumns, int64_t owner, int8_t pg_namespace, int32_t pg_constraint, int8_t rights){
            //header->setData(oid,contain_toast,numberOfColumns,owner,pg_namespace,pg_constraint,rights,freeSpace,unitSize);
        }

        void showData(){
            //header->showData();
            for (int i=0;i<blocks.size();i++){
                blocks[i]->showData();
            }
            if(blockTmp!=nullptr){
                blockTmp->showData();
            }
        }


        
};






#endif