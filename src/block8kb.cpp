#include "block8kb.h"

block8kb::~block8kb(){
    if(header != nullptr) {
        delete header;
        header = nullptr;
    }
}

std::vector<uint8_t> block8kb::marshallBlock8kb(){
    std::vector<uint8_t> result;
    std::vector<uint8_t> *blockBytes = marshalInt16_t(allBlockIndetification);
    result.insert(result.end(),blockBytes->begin(),blockBytes->end());
    std::cout<<"+++++++++++++="<<std::endl;
    for (int i = 0; i < 2; ++i) {
            for (int b = 7; b >= 0; --b)
                std::cout << ((result[i] >> b) & 1);
            }
    std::cout<<std::endl;
    std::cout<<"+++++++++++++="<<std::endl;
    std::vector<uint8_t> headerBytes = header->marshallBlockHeaderWithData();
    result.insert(result.end(), headerBytes.begin(), headerBytes.end());
    for (int i=0;i<tuples.size();i++){
        std::vector<uint8_t> tupleBytes = tuples[i].marshallTupleWithData();
        result.insert(result.end(), tupleBytes.begin(), tupleBytes.end());

    }
    int32_t zeroNums = blockSize - result.size();
    for (int i=0;i<zeroNums;i++){
        if(result.size() + 1 <= blockSize){
            result.push_back(00000000);
        }
    }
    delete blockBytes;
    //blockBytes.insert(blockBytes.begin(), headerBytes.begin(), headerBytes.end());
    return result;
}

void block8kb::unmarshallBlock8kb(const std::vector<uint8_t>& data){
    if(data.size()!= blockSize) return;
    std::vector<uint8_t>typeBytes ;
    int32_t offset = 0 ;
    typeBytes.insert(typeBytes.end(),data.begin()+offset,data.begin()+offset+2);
    int16_t blockType =0 ;
    UnmarshalInt16_t(&blockType,&typeBytes);
    offset += 2 ;
    if(blockType == allBlockIndetification){
        std::vector<uint8_t> headerBytes;
        headerBytes.insert(headerBytes.end(),data.begin()+offset,data.begin()+offset+header->getSize());
        header->unmarshallBlockHeader(headerBytes);
        header->showData();
        offset += header->getSize();
        std::vector<uint8_t> tuplesBytes;
        tuplesBytes.insert(tuplesBytes.end(),data.begin()+offset,data.end());
        int32_t tmpSize = 0;
        while(tmpSize < tuplesBytes.size()){
            int16_t tupleType = 0;
            std::vector<uint8_t> tupleTypeBytes;
            tupleTypeBytes.insert(tupleTypeBytes.end(),tuplesBytes.begin()+tmpSize,tuplesBytes.begin()+tmpSize+2);
            tmpSize+=2;
            UnmarshalInt16_t(&tupleType,&tupleTypeBytes);
            if(tupleType == tupleIndetification){
                std::vector<uint8_t> tupleData;
                int32_t tupleSize = 0 ;
                std::vector<uint8_t> tupleSizeBytes;
                //tupleSizeBytes.insert(tupleSizeBytes.end(),tuplesBytes.begin()+tmpSize,tuplesBytes.begin()+tmpSize+4);
                //UnmarshalInt32_t(&tupleSize,&tupleSizeBytes);
                //tmpSize+=4;
                std::vector<uint8_t>tupleBytes;
                tupleBytes.insert(tupleBytes.end(),tupleTypeBytes.begin(),tupleTypeBytes.end());   // correct  
                tupleBytes.insert(tupleBytes.end(),tuplesBytes.begin()+tmpSize,tuplesBytes.end()); // 
                //tupleTypeBytes.insert(tupleTypeBytes.end(),tuplesBytes.begin()+tmpSize,tuplesBytes.begin()+tmpSize+2);
                //tupleData.insert(tupleData.end(),tuplesBytes.begin()+tmpSize,tuplesBytes.begin()+tmpSize+100);
                tuple tmpTuple;
                tmpTuple.unmarshallTuple(tupleBytes);
                tuples.push_back(tmpTuple);
                tmpSize+=tupleSize;
            } else {
                break;
            }
        }
    }

}


/*
void block8kb::setData(blockHeader& header , std::vector<tuple>& tuples)
{
    this->header = header;
    this->tuples = tuples;
}
*/

