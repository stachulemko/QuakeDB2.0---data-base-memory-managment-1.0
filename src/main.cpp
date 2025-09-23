#include <iostream>
#include <gtest/gtest.h>
#include "typesDataConverter.h"
#include "headerTuple.h"
#include "dataNullBitMapTuple.h"
#include "allTuple.h"

int main() {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {false, false, false};
    std::vector<allVars> data = {allVars(0), allVars(0L), allVars(std::string(""))};
    
    std::vector<uint8_t> marshalled = tpl1.marshallTuple(0, 0, 0, 0, 0, false, 0, bitMap, data);

    tpl2.unmarshallTuple(marshalled);

    std::cout<<tpl2.getBitMap().size()<<std::endl;
    std::cout<<tpl2.getData().size()<<std::endl;
}