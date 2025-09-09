#ifndef TYPEMANAGERALLVARSTYPES_H   
#define TYPEMANAGERALLVARSTYPES_H
#include <iostream>
#include <variant>
#include <vector>

int16_t rowIndetification = 1;
//class which provide all type for table 

using allVars = std::variant<int32_t, int64_t, std::string>;

int32_t getTypeId(const allVars& value);

void showVariantVariable(allVars vec);

void showVariantVectorOfVector(std::vector<std::vector<allVars>> vec);

#endif