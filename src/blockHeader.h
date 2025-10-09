#ifndef BLOCKHEADER_H
#define BLOCKHEADER_H

#include <iostream>
#include <vector>
#include "typeManagerAllVarsTypes.h"




/*
-----------------------------------
|         data block 8kb          |
|pointer to next block (by blockNUms)|   - int32_t ;  
|blockIndetification              |   -  int32_t               |
|pd_lsn - for wall indetification |   -  int32_t
|pd_checksum - control sum        |   -  int16_t
|pd_flags - state flag            |   -  int16_t
|contain toast                    |   -  int8_t 
----------------------------------
*/


class blockHeader{
    private:
        int32_t nextblock =0 ;
        int32_t blockIndetification =0 ;
        int32_t pd_lsn;          // 4 bytes
        int16_t pd_checksum;     // 2 bytes
        int16_t pd_flags;        // 2 bytes
        int8_t contain_toast;    // 1 byte
    public:
        
        int32_t getSize(){
            return 17;
        }

        blockHeader();

        ~blockHeader();

        std::vector<uint8_t> marshallBlockHeaderWithData(){
            return marshallBlockHeader(nextblock,blockIndetification,pd_lsn,pd_checksum,pd_flags,contain_toast);
        }

        std::vector<uint8_t> marshallBlockHeader(int32_t nextblock, int32_t blockIndetification, int32_t pd_lsn, int16_t pd_checksum, int16_t pd_flags, int8_t contain_toast);

        void unmarshallBlockHeader(const std::vector<uint8_t>& data);

        void setData(int32_t nextblock, int32_t blockIndetification, int32_t pd_lsn, int16_t pd_checksum, int16_t pd_flags, int8_t contain_toast){
            this->nextblock = nextblock;
            this->blockIndetification = blockIndetification;
            this->pd_lsn = pd_lsn;
            this->pd_checksum = pd_checksum;
            this->pd_flags = pd_flags;
            this->contain_toast = contain_toast;
        }
        // getters

        int32_t getNextblock() const { return nextblock; }
        int32_t getBlockIndetification() const { return blockIndetification; }
        int32_t getPdLsn() const { return pd_lsn; }
        int16_t getPdChecksum() const { return pd_checksum; }
        int16_t getPdFlags() const { return pd_flags; }
        int8_t getContainToast() const { return contain_toast; }

        void showData(){
            std::cout<<"nextblock: "<<nextblock<<std::endl;
            std::cout<<"blockIndetification: "<<blockIndetification<<std::endl;
            std::cout<<"pd_lsn: "<<pd_lsn<<std::endl;
            std::cout<<"pd_checksum: "<<pd_checksum<<std::endl;
            std::cout<<"pd_flags: "<<pd_flags<<std::endl;
            std::cout<<"contain_toast: "<<(int)contain_toast<<std::endl;
        }


};












#endif