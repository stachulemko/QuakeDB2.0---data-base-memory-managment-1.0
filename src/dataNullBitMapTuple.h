#ifndef DATANULLBITMAPTUPLE_H
#define DATANULLBITMAPTUPLE_H

#include "typeManagerAllVarsTypes.h"
#include <iostream>
#include <bitset>
#include <vector>
#include <cstdint>

class DataNullBitMapTuple{
    private:
        std::vector<bool>bitMap;
        std::vector<allVars>dataValues; 
        int32_t sizeAll=0;
    public:
        DataNullBitMapTuple(){
            //std::cout<<"DataNullBitMapTuple()"<<std::endl;
        }
        
        void unmarshallDataNullBitMapTuple(std::vector<uint8_t> inputData){ 
            if(inputData.size()==0){
                return;
            }
            int16_t type=0;
            std::vector<uint8_t>typeBytes;
            typeBytes.insert(typeBytes.end(),inputData.begin(),inputData.begin()+2);
            UnmarshalInt16_t(&type,&typeBytes);
            //std::cout<<"type: "<<type<<std::endl;
            if(type == rowIndetification){
                int16_t bitMaptype=0;
                std::vector<uint8_t>bitMaptypeBytes;
                bitMaptypeBytes.insert(bitMaptypeBytes.end(),inputData.begin()+2,inputData.begin()+4);
                UnmarshalInt16_t(&bitMaptype,&bitMaptypeBytes);
                //std::cout<<"bitMapType: "<<bitMaptype<<std::endl;
                if(bitMaptype==bitMapIndetification){
                    int32_t sizeBitMap=0;
                    std::vector<uint8_t>sizeBitMapBytes;
                    sizeBitMapBytes.insert(sizeBitMapBytes.end(),inputData.begin()+4,inputData.begin()+8);
                    UnmarshalInt32_t(&sizeBitMap,&sizeBitMapBytes);
                    //std::cout<<"sizeBitMap: "<<sizeBitMap<<std::endl;

                    std::vector<uint8_t>dataBitMap;
                    dataBitMap.insert(dataBitMap.end(),inputData.begin()+8,inputData.begin()+8+sizeBitMap); 
                    
                    for (int i=0;i<dataBitMap.size();i++){
                        bool bitNull = true;
                        std::vector<uint8_t>dataBitTmp;
                        dataBitTmp.insert(dataBitTmp.end(),dataBitMap.begin()+i,dataBitMap.begin()+i+1); 
                        UnmarshalBool(&bitNull,&dataBitTmp);
                        bitMap.push_back(bitNull);
                    }

                    //std::cout<<"bitMap size: "<<bitMap.size()<<std::endl;

                    //for (int i=0;i<bitMap.size();i++){
                    //    std::cout<<"bitMap["<<i<<"]: "<<bitMap[i]<<std::endl;
                    //}
                    //std::cout<<"test1"<<std::endl;
                    int32_t tmpSize = 8+sizeBitMap;
                    int16_t typeValues=0;
                    std::vector<uint8_t>typeValuesBytes;
                    typeValuesBytes.insert(typeValuesBytes.end(),inputData.begin()+tmpSize,inputData.begin()+tmpSize+2);
                    UnmarshalInt16_t(&typeValues,&typeValuesBytes);
                    //std::cout<<"typeValues: "<<typeValues<<std::endl;
                    if(typeValues==dataIndetification){
                        int32_t sizeDataValues=0;
                        std::vector<uint8_t>sizeDataValuesBytes;
                        sizeDataValuesBytes.insert(sizeDataValuesBytes.end(),inputData.begin()+tmpSize+2,inputData.begin()+tmpSize+2+4);
                        UnmarshalInt32_t(&sizeDataValues,&sizeDataValuesBytes);
                        tmpSize+=6; 
                        //std::cout<<"sizeDataValues: "<<sizeDataValues<<std::endl;
                        int tmpValueSizeVal = 0;
                        while(tmpValueSizeVal != sizeDataValues){
                            int16_t valType = 0;
                            std::vector<uint8_t>valTypeBytes;
                            valTypeBytes.insert(valTypeBytes.end(),inputData.begin()+tmpSize,inputData.begin()+tmpSize+2);
                            UnmarshalInt16_t(&valType,&valTypeBytes);
                            tmpSize+=2;
                            //tmpValueSizeVal+=2;
                            
                            int32_t valSize = 0;
                            std::vector<uint8_t>valSizeBytes;
                            valSizeBytes.insert(valSizeBytes.end(),inputData.begin()+tmpSize,inputData.begin()+tmpSize+4);
                            UnmarshalInt32_t(&valSize,&valSizeBytes);
                            //std::cout<<"valSize: "<<valSize<<std::endl;

                            std::vector<uint8_t>values;
                            values.insert(values.end(),inputData.begin()+tmpSize+4,inputData.begin()+tmpSize+4+valSize);
                            allVars val = unmarshallBytes(values,valType); 
                            dataValues.push_back(val); 
                            tmpSize+=4+valSize;
                            tmpValueSizeVal+=valSize;
                            //std::cout<<"tmpValueSizeVal: "<<tmpValueSizeVal<<std::endl;
                        }
                        sizeAll = tmpSize;
                        //std::cout<<"raaaaaa"<<std::endl;
                    }
                }
            }
            
        }

        std::vector<uint8_t> marshallDataNullBitMapTuple(std::vector<bool>bitMap,std::vector<allVars>data){
            std::vector<uint8_t> result;

            std::vector<uint8_t>* type = marshalInt16_t(rowIndetification);
            result.insert(result.end(),type->begin(),type->end());
            delete type;

            std::vector<uint8_t>* bitMapType = marshalInt16_t(bitMapIndetification);
            result.insert(result.end(),bitMapType->begin(),bitMapType->end());
            delete bitMapType;

            int32_t sizeBitMap = bitMap.size();
            std::vector<uint8_t>* sizeBitMapBytes = marshalInt32_t(sizeBitMap);
            result.insert(result.end(),sizeBitMapBytes->begin(),sizeBitMapBytes->end());
            delete sizeBitMapBytes;

            for (size_t i=0;i<bitMap.size();i++){ 
                std::vector<uint8_t>* bitNull = marshalBool(bitMap[i]);
                result.insert(result.end(),bitNull->begin(),bitNull->end());
                delete bitNull;
            }

            std::vector<uint8_t>* dataType = marshalInt16_t(dataIndetification);
            result.insert(result.end(),dataType->begin(),dataType->end());
            delete dataType;

            int32_t sizeData=0;
            for (int i=0;i<data.size();i++){
                sizeData+=getSizeByVal(data[i]);
            }

            std::vector<uint8_t>* sizeDataBytes = marshalInt32_t(sizeData);
            result.insert(result.end(),sizeDataBytes->begin(),sizeDataBytes->end());
            delete sizeDataBytes;

            for (size_t i=0;i<data.size();i++){ 
                int16_t valType = getTypeId(data[i]);
                std::vector<uint8_t>* valTypeBytes = marshalInt16_t(valType);
                result.insert(result.end(),valTypeBytes->begin(),valTypeBytes->end());
                delete valTypeBytes;

                std::vector<uint8_t> valBytes = getBytes(data[i]);
                int32_t valSize = getSizeByVal(data[i]);
                std::vector<uint8_t>* valSizeBytes = marshalInt32_t(valSize);
                result.insert(result.end(),valSizeBytes->begin(),valSizeBytes->end());
                delete valSizeBytes;

                result.insert(result.end(),valBytes.begin(),valBytes.end());
            }
            
            return result; 
        }
        
        std::vector<bool> getBitMap() const { return bitMap; }
        std::vector<allVars> getData() const { return dataValues; }
};

#endif