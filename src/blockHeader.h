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
        blockHeader();

        ~blockHeader();

        std::vector<uint8_t> marshallBlockHeader(int32_t nextblock, int32_t blockIndetification, int32_t pd_lsn, int16_t pd_checksum, int16_t pd_flags, int8_t contain_toast);

        void unmarshallBlockHeader(const std::vector<uint8_t>& data);

        // getters

        int32_t getNextblock() const { return nextblock; }
        int32_t getBlockIndetification() const { return blockIndetification; }
        int32_t getPdLsn() const { return pd_lsn; }
        int16_t getPdChecksum() const { return pd_checksum; }
        int16_t getPdFlags() const { return pd_flags; }
        int8_t getContainToast() const { return contain_toast; }


};












#endif