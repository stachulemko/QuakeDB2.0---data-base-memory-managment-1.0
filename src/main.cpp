#include <iostream>
#include <gtest/gtest.h>
#include "typesDataConverter.h"
#include "headerTuple.h"

int main() {
    HeaderTuple headerTuple1;
    std::bitset<280> bits1 = headerTuple1.marshallHeaderTuple(1,2,3,4,556,true,123);
    HeaderTuple headerTuple2;
    headerTuple2.unmarshallHeaderTuple(bits1);
    //std::vector<uint8_t>vec = 
}