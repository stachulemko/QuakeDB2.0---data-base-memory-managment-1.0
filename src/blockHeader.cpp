#include "blockHeader.h"



blockHeader::blockHeader(){
            pd_lsn = 0;
            pd_checksum = 0;
            pd_flags = 0;
            contain_toast = 0;
}

blockHeader::~blockHeader(){}

std::vector<uint8_t> blockHeader::marshallBlockHeader(int32_t nextblock, int32_t blockIndetification, int32_t pd_lsn, int16_t pd_checksum, int16_t pd_flags, int8_t contain_toast){
    std::vector<uint8_t> result;

    std::vector<uint8_t>* nextblockBin = marshalInt32_t(nextblock);
    result.insert(result.end(),nextblockBin->begin(),nextblockBin->end());
    delete nextblockBin;

    std::vector<uint8_t>* blockIndetificationBin = marshalInt32_t(blockIndetification);
    result.insert(result.end(),blockIndetificationBin->begin(),blockIndetificationBin->end());
    delete blockIndetificationBin;

    std::vector<uint8_t>* lsnBin = marshalInt32_t(pd_lsn);
    result.insert(result.end(),lsnBin->begin(),lsnBin->end());
    delete lsnBin;

    std::vector<uint8_t>* checksumBin = marshalInt16_t(pd_checksum);
    result.insert(result.end(),checksumBin->begin(),checksumBin->end());
    delete checksumBin;

    std::vector<uint8_t>* flagsBin = marshalInt16_t(pd_flags);
    result.insert(result.end(),flagsBin->begin(),flagsBin->end());
    delete flagsBin;

    std::vector<uint8_t>* toastBin = marshalInt8_t(contain_toast);
    result.insert(result.end(),toastBin->begin(),toastBin->end());
    delete toastBin;

    return result;
}

void blockHeader::unmarshallBlockHeader(const std::vector<uint8_t>& data){
    if(data.size() < 17) return;
    std::vector<uint8_t> nextBlockVec(data.begin(), data.begin() + 4);        // int32_t = 4 bytes
    UnmarshalInt32_t(&nextblock, &nextBlockVec);

    std::vector<uint8_t> blockIndetificationVec(data.begin() + 4, data.begin() + 8);  // int32_t = 4 bytes
    UnmarshalInt32_t(&blockIndetification, &blockIndetificationVec);

    std::vector<uint8_t> pd_lsnVec(data.begin() + 8, data.begin() + 12); // int32_t = 4 bytes
    UnmarshalInt32_t(&pd_lsn, &pd_lsnVec);

    std::vector<uint8_t> pd_checksumVec(data.begin() + 12, data.begin() + 14);     // int16_t = 2 bytes
    UnmarshalInt16_t(&pd_checksum, &pd_checksumVec);

    std::vector<uint8_t> pd_flagsVec(data.begin() + 14, data.begin() + 16);     // int16_t = 2 bytes
    UnmarshalInt16_t(&pd_flags, &pd_flagsVec);

    std::vector<uint8_t> contain_toastVec(data.begin() + 16, data.begin() + 17);     // int8_t = 1 byte
    UnmarshalInt8_t(&contain_toast, &contain_toastVec);
}