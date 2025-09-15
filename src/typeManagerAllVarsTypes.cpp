#include "typeManagerAllVarsTypes.h"
#include "typesDataConverter.h"
#include <iostream>


const int16_t int32_tIndetification = 4;
const int16_t int64_tIndetification = 6;  
const int16_t stringIndetification = 5;

const int16_t rowIndetification = 1;
const int16_t blockHeaderIndetification = 4;
const int16_t tableHeaderIndetification = 7;
const int16_t bitMapIndetification = 2;
const int16_t dataIndetification = 3;
const int16_t tupleIndetification = 8;

int16_t getTypeId(const allVars& value) {
    if (std::holds_alternative<int32_t>(value)) {
        return int32_tIndetification;
    }
    else if (std::holds_alternative<int64_t>(value)) {
        return int64_tIndetification;  
    }
    else if (std::holds_alternative<std::string>(value)) {
        return stringIndetification;
    }
    return -1;
}

int32_t getSizeByVal(const allVars& value) {
    if (std::holds_alternative<int32_t>(value)) {
        return sizeof(int32_t);
    }
    else if (std::holds_alternative<int64_t>(value)) {
        return sizeof(int64_t);
    }
    else if (std::holds_alternative<std::string>(value)) {
        const std::string& str = std::get<std::string>(value);
        return str.size();
    }
    return -1;
}

std::vector<uint8_t> getBytes(const allVars& value){
    std::vector<uint8_t> bytes1;
    if (std::holds_alternative<int32_t>(value)) {
        int32_t val = std::get<int32_t>(value);
        std::vector<uint8_t>* result = marshalInt32_t(val);
        if (result) {
            bytes1 = *result;
            delete result;
        }
    }
    else if (std::holds_alternative<int64_t>(value)) {
        int64_t val = std::get<int64_t>(value);
        std::vector<uint8_t>* result = marshalInt64_t(val);
        if (result) {
            bytes1 = *result;
            delete result;
        }
    }
    else if (std::holds_alternative<std::string>(value)) {
        std::string val = std::get<std::string>(value);
        std::vector<uint8_t>* result = marshalString(val);
        if (result) {
            bytes1 = *result;
            delete result;
        }
    }
    return bytes1; 
}

allVars unmarshallBytes(std::vector<uint8_t> bytes, int16_t typeIndetification) {
    if (typeIndetification == int32_tIndetification) {
        int32_t value = 0;
        UnmarshalInt32_t(&value, &bytes); 
        return allVars(value);
    }
    else if (typeIndetification == int64_tIndetification) { 
        int64_t value = 0;
        UnmarshalInt64_t(&value, &bytes); 
        return allVars(value);
    }
    else if (typeIndetification == stringIndetification) {
        std::string value;
        UnmarshalString(&value, &bytes); 
        return allVars(value);
    }
    return allVars(); 
}

void showVariantVariable(allVars vec) {
    if (std::holds_alternative<int32_t>(vec)) {
        std::cout << std::get<int32_t>(vec);
    }
    else if (std::holds_alternative<int64_t>(vec)) {
        std::cout << std::get<int64_t>(vec);
    }
    else if (std::holds_alternative<std::string>(vec)) {
        std::cout << std::get<std::string>(vec);
    }
    else {
        std::cout << "NULL";
    }
}

void showVariantVectorOfVector(std::vector<std::vector<allVars>> vec) {
    for (size_t i = 0; i < vec.size(); i++) {
        std::cout << "Row " << i + 1 << ": ";
        for (size_t j = 0; j < vec[i].size(); j++) {
            showVariantVariable(vec[i][j]);
            if (j < vec[i].size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
}