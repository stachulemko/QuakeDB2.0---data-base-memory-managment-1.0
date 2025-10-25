#ifndef BLOCK8KB_H
#define BLOCK8KB_H

#include <iostream>
#include <vector>
#include "blockHeader.h"
#include "allTuple.h"
#include "typesDataConverter.h"
#include "typeManagerAllVarsTypes.h"

const int32_t blockSize = 8192 ;

class block8kb{
    private:
        //std::vector<uint8_t> dataBlock; // 8192 bytes
        blockHeader *header=nullptr;
        std::vector<tuple> tuples;

        int32_t freeSpace = 0 ;
        int32_t blockSizeAfterFreeSpace = 0;
    public:
        block8kb(int32_t freeSpace,int32_t nextblock, int32_t blockIndetification, int32_t pd_lsn, int16_t pd_checksum, int16_t pd_flags, int8_t contain_toast){
            //std::cout << "Constructor called with freeSpace: " << freeSpace << std::endl;
            //std::cout << "blockSize: " << blockSize << std::endl;
            
            this->freeSpace = freeSpace;
            //std::cout << "this->freeSpace set to: " << this->freeSpace << std::endl;
            
            this->blockSizeAfterFreeSpace = blockSize - this->freeSpace;
            //std::cout << "blockSizeAfterFreeSpace calculated: " << this->blockSizeAfterFreeSpace << std::endl;
            
            header = new blockHeader();
            header->setData(nextblock,blockIndetification,pd_lsn,pd_checksum,pd_flags,contain_toast);
            
            //std::cout << "Final blockSizeAfterFreeSpace: " << this->blockSizeAfterFreeSpace << std::endl;
        }

        ~block8kb();
        
        int32_t getblockSizeAfterFreeSpace()  { return this->blockSizeAfterFreeSpace; }
        int32_t getSize(){
            int32_t tmpSize = header->getSize();
            for (int j=0;j<tuples.size();j++){
                tmpSize+=tuples[j].getSize();
            }
            return tmpSize;
        }



        std::vector<uint8_t> marshallBlock8kb();

        void unmarshallBlock8kb(const std::vector<uint8_t>& data);

        //std::vector<uint8_t> getDataBlock() const { return dataBlock; }

        //void setDataBlock(const std::vector<uint8_t>& data) { dataBlock = data; }

        //void setData(blockHeader& header , std::vector<tuple>& tuples);

        
        void addData(tuple& tupleObj){
            tuples.push_back(tupleObj);
            
        }

        void showData(){
            header->showData();
            for (int i=0;i<tuples.size();i++){
                tuples[i].showData();
            }
        }

};
#endif


