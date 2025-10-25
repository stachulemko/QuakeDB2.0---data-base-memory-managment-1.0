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
    typeBytes.insert(typeBytes.end(),data.begin(),data.begin()+2);
    int16_t blockType =0 ;
    UnmarshalInt16_t(&blockType,&typeBytes);
    if(blockType == allBlockIndetification){
        std::vector<uint8_t> headerBytes;
        headerBytes.insert(headerBytes.end(),data.begin()+2,data.begin()+19);
        header->unmarshallBlockHeader(headerBytes);
        std::vector<uint8_t> tuplesBytes;
        tuplesBytes.insert(tuplesBytes.end(),data.begin()+19,data.end());
        int32_t tmpSize = 0;
        while(tmpSize < tuplesBytes.size()){
            int16_t tupleType = 0;
            std::vector<uint8_t> tupleTypeBytes;
            tupleTypeBytes.insert(tupleTypeBytes.end(),tuplesBytes.begin()+tmpSize,tuplesBytes.begin()+tmpSize+2);
            UnmarshalInt16_t(&tupleType,&tupleTypeBytes);
            if(tupleType == tupleIndetification){
                std::vector<uint8_t> tupleData;
                int32_t tupleSize = 0 ;
                std::vector<uint8_t> tupleSizeBytes;
                tupleSizeBytes.insert(tupleSizeBytes.end(),tuplesBytes.begin()+tmpSize+2,tuplesBytes.begin()+tmpSize+6);
                UnmarshalInt32_t(&tupleSize,&tupleSizeBytes);

                std::vector<uint8_t>tupleBytes;
                tupleBytes.insert(tupleBytes.end(),tuplesBytes.begin()+tmpSize+6,tuplesBytes.begin()+tmpSize+tupleSize+6);
                //tupleTypeBytes.insert(tupleTypeBytes.end(),tuplesBytes.begin()+tmpSize,tuplesBytes.begin()+tmpSize+2);
                //tupleData.insert(tupleData.end(),tuplesBytes.begin()+tmpSize,tuplesBytes.begin()+tmpSize+100);
                tuple tmpTuple;
                tmpTuple.unmarshallTuple(tupleBytes);
                tuples.push_back(tmpTuple);
                tmpSize+=tupleSize+6;
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

