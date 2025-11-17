#ifndef DATANULLBITMAPTUPLE_H
#define DATANULLBITMAPTUPLE_H

#include "typeManagerAllVarsTypes.h"
#include <iostream>
#include <bitset>
#include <vector>
#include <cstdint>

/*
+-----------------------+
| Null bitmap           | <-- depend on the number of rows 
+-----------------------+
| (data)                | <-- data 
+-----------------------+
*/

class DataNullBitMapTuple{
    private:
        std::vector<bool>bitMap;
        std::vector<allVars>dataValues; 
        int32_t sizeAll=0;

        //int32_t freeSize = 0 ; 
        //int8_t unitSize = 0 ;
    public:


        int32_t getSize(){
            int32_t sizeTmp = bitMap.size();
            for (int i=0;i<dataValues.size();i++){
                sizeTmp+=getSizeByVal(dataValues[i]);
            }
            return sizeTmp;
        }

        DataNullBitMapTuple();
        
        void unmarshallDataNullBitMapTuple(std::vector<uint8_t> inputData);

        std::vector<uint8_t> marshallDataNullBitMapTupleWithData(){
            return marshallDataNullBitMapTuple(bitMap,dataValues);
        }

        std::vector<uint8_t> marshallDataNullBitMapTuple(std::vector<bool>bitMap,std::vector<allVars>data);
        
        void setData(std::vector<bool>bitMap,std::vector<allVars>dataValues ){
            this->bitMap = bitMap;
            this->dataValues = dataValues;
        }

        std::vector<bool> getBitMap() const { return bitMap; }
        std::vector<allVars> getData() const { return dataValues; }

        void showData(){
            std::cout<<"bitMap: ";
            for (int i=0;i<bitMap.size();i++){
                std::cout<<bitMap[i]<<" ";
            }
            std::cout<<std::endl;
            std::cout<<"dataValues: ";
            for (int i=0;i<dataValues.size();i++){
                showVariantVariable(dataValues[i]);
                std::cout<<" ";
            }
            std::cout<<std::endl;
        }
};

#endif