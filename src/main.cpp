#include <iostream>
#include <gtest/gtest.h>
#include "typesDataConverter.h"
#include "headerTuple.h"
#include "dataNullBitMapTuple.h"

int main() {
    /*
    DataNullBitMapTuple dataNullBitMapTuple;
    std::cout<<"1"<<std::endl;
    std::vector<uint8_t> vec = dataNullBitMapTuple.marshallDataNullBitMapTuple({true,false,true}, {int32_t(42), std::string("Hello"), int32_t(100)});
    std::cout<<"2"<<std::endl;
    DataNullBitMapTuple dataNullBitMapTuple2;
    std::cout<<"3"<<std::endl;
    dataNullBitMapTuple2.unmarshallDataNullBitMapTuple(vec);
    std::cout<<"4"<<std::endl;
    */
    DataNullBitMapTuple tuple;
    std::vector<uint8_t> empty=tuple.marshallDataNullBitMapTuple({true , true, false}, {"negro",12,"negral"});
    DataNullBitMapTuple tuple2;
    tuple2.unmarshallDataNullBitMapTuple(empty);
    //EXPECT_TRUE(tuple.getBitMap().empty());
    //EXPECT_TRUE(tuple.getData().empty());
    //HeaderTuple headerTuple1;
    //std::vector<uint8_t> bits1 = headerTuple1.marshallHeaderTuple(1,2,3,4,556,true,123);
    //HeaderTuple headerTuple2;
    //headerTuple2.unmarshallHeaderTuple(bits1);
    //std::cout<<headerTuple2.getTxMin()<<std::endl;
    //std::vector<uint8_t>vec = 
}