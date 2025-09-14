//1.0.0
#ifndef TYPESDATACONVERTER_H
#define TYPESDATACONVERTER_H
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <bitset>


//extern bool returnCheckPoint = false;



std::vector<uint8_t>* marshalInt8_t(int8_t val);

std::vector<uint8_t>* marshalInt16_t(int16_t val);

std::vector<uint8_t>* marshalInt32_t(int32_t val);

std::vector<uint8_t>* marshalInt64_t(int64_t val);

std::vector<uint8_t>* marshalString(std::string val);

std::vector<uint8_t>* marshalBool(bool val);

void UnmarshalInt16_t(int16_t* val, std::vector<uint8_t>* int16InBinary);

void UnmarshalInt32_t(int32_t* val, std::vector<uint8_t>* int32InBinary);

void UnmarshalInt64_t(int64_t* val, std::vector<uint8_t>* int64InBinary);

void UnmarshalString(std::string* val, std::vector<uint8_t>* stringInBinary);

void UnmarshalBool(bool* val, std::vector<uint8_t>* boolInBinary);

void UnmarshalInt8_t(int8_t* val, std::vector<uint8_t>* int8InBinary);


void showBytes(const std::vector<uint8_t>& bytes);



#endif
