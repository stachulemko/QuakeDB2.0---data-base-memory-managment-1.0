//1.0.0
#include "typesDataConverter.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <bitset>
//---------------------------------
//Little-endian convertionStartand
//---------------------------------

//bool returnCheckPoint = false;


std::vector<uint8_t>* marshalBool(bool val) {
    std::vector<uint8_t>* result = new std::vector<uint8_t>(1);
    (*result)[0] = val ? 1 : 0;  
    return result;
}

std::vector<uint8_t>* marshalInt16_t(int16_t val) {
    std::vector<uint8_t>* bytes = new std::vector<uint8_t>(2);
    (*bytes)[0] = static_cast<uint8_t>(val & 0xFF);
    (*bytes)[1] = static_cast<uint8_t>((val >> 8) & 0xFF);
    return bytes;
}


std::vector<uint8_t>* marshalInt32_t(int32_t val) {
    std::vector<uint8_t>* bytes = new std::vector<uint8_t>(4);
    (*bytes)[0] = static_cast<uint8_t>(val & 0xFF);
    (*bytes)[1] = static_cast<uint8_t>((val >> 8) & 0xFF);
    (*bytes)[2] = static_cast<uint8_t>((val >> 16) & 0xFF);
    (*bytes)[3] = static_cast<uint8_t>((val >> 24) & 0xFF);
    return bytes;
}

std::vector<uint8_t>* marshalInt64_t(int64_t val) {
    std::vector<uint8_t>* bytes = new std::vector<uint8_t>(8);
    (*bytes)[0] = static_cast<uint8_t>(val & 0xFF);
    (*bytes)[1] = static_cast<uint8_t>((val >> 8) & 0xFF);
    (*bytes)[2] = static_cast<uint8_t>((val >> 16) & 0xFF);
    (*bytes)[3] = static_cast<uint8_t>((val >> 24) & 0xFF);
    (*bytes)[4] = static_cast<uint8_t>((val >> 32) & 0xFF);
    (*bytes)[5] = static_cast<uint8_t>((val >> 40) & 0xFF);
    (*bytes)[6] = static_cast<uint8_t>((val >> 48) & 0xFF);
    (*bytes)[7] = static_cast<uint8_t>((val >> 56) & 0xFF);
    return bytes;
}

std::vector<uint8_t>* marshalString(std::string val) {
    if(val.empty()) {
        return new std::vector<uint8_t>();
    }
    std::vector<uint8_t>* bytes = new std::vector<uint8_t>(val.size());
    for (size_t i = 0; i < val.size(); ++i) {
        (*bytes)[i] = static_cast<uint8_t>((val)[i]);
    }
    return bytes;
}

void UnmarshalInt16_t(int16_t* val, std::vector<uint8_t>* int16InBinary) {
    if(val == nullptr || int16InBinary == nullptr || int16InBinary->size() < 2) {
        //returnCheckPoint=true;
        return; 
    }
    int16_t value = 0;
    for (size_t i = 0; i < 4 && i < int16InBinary->size(); ++i) {
        value |= (static_cast<int16_t>((*int16InBinary)[i]) << (i * 8));
    }
    *val = value;
}

void UnmarshalInt32_t(int32_t* val, std::vector<uint8_t>* int32InBinary) {
    if(val == nullptr || int32InBinary == nullptr || int32InBinary->size() < 4) {
        //returnCheckPoint=true;
        return; 
    }
    int32_t value = 0;
    for (size_t i = 0; i < 4 && i < int32InBinary->size(); ++i) {
        value |= (static_cast<int32_t>((*int32InBinary)[i]) << (i * 8));
    }
    *val = value;
}

void UnmarshalInt64_t(int64_t* val, std::vector<uint8_t>* int64InBinary) {
    if(val == nullptr || int64InBinary == nullptr || int64InBinary->size() < 8) {
        //returnCheckPoint=true ;
        return; 
    }
    int64_t value = 0;
    for (size_t i = 0; i < 8 && i < int64InBinary->size(); ++i) {
        value |= (static_cast<int64_t>((*int64InBinary)[i]) << (i * 8));
    }
    *val = value;
}

void UnmarshalString(std::string* val, std::vector<uint8_t>* stringInBinary) {
    if(val == nullptr or stringInBinary == nullptr or stringInBinary->empty()) {
        //returnCheckPoint=true ;
        return;
    }
    *val = std::string(stringInBinary->begin(), stringInBinary->end());
}

void UnmarshalBool(bool* val, std::vector<uint8_t>* boolInBinary) {
    if(val == nullptr) {
        //returnCheckPoint=true;
        return; 
    }
    if (boolInBinary && !boolInBinary->empty()) {
        *val = ((*boolInBinary)[0] != 0);  
    } else {
        *val = false;  
    }
}


void showBytes(const std::vector<uint8_t>& bytes) {
    for (int i = 0; i < bytes.size(); i++) {
        std::cout << std::bitset<8>(bytes[i]) << " ";

    }
}