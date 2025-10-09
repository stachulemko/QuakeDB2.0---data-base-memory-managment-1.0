#ifndef TYPEMANAGERALLVARSTYPES_H   
#define TYPEMANAGERALLVARSTYPES_H
#include "typesDataConverter.h"
#include <iostream>
#include <variant>
#include <vector>

extern const int16_t int32_tIndetification;
extern const int16_t int64_tIndetification;
extern const int16_t stringIndetification;

extern const int16_t rowIndetification;
extern const int16_t tableHeaderIndetification;
extern const int16_t blockHeaderIndetification;
extern const int16_t bitMapIndetification;
extern const int16_t dataIndetification;
extern const int16_t tupleIndetification;
extern const int16_t allBlockIndetification;
extern const int16_t typeTableIndetification;


//table header
extern const int16_t dataNullTableHeader;
extern const int16_t columnNamesTableHeader;

extern const int16_t byteUnit;

using allVars = std::variant<int32_t, int64_t, std::string>;

int16_t getTypeId(const allVars& value);
void showVariantVariable(allVars vec);
void showVariantVectorOfVector(std::vector<std::vector<allVars>> vec);
std::vector<uint8_t> getBytes(const allVars& value);
int32_t getSizeByVal(const allVars& value);
allVars unmarshallBytes(std::vector<uint8_t> bytes, int16_t typeIndetification);  

#endif