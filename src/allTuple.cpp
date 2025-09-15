#include "allTuple.h"

tuple::tuple(){

}
tuple::~tuple(){

}
std::vector<uint8_t> tuple::marshallTuple(int64_t xmin, int64_t xmax, int32_t cid, int32_t infomask, int16_t hoff, bool bitmap, int64_t oid, std::vector<bool>bitMap,std::vector<allVars>data){ 
    std::vector<uint8_t> result;
    std::vector<uint8_t> *typeBin = marshalInt16_t(tupleIndetification);
    std::vector<uint8_t>hederBin = header.marshallHeaderTuple(xmin,xmax,cid,infomask,hoff,bitmap,oid);
    std::vector<uint8_t>dataBin = dataNullBitMap.marshallDataNullBitMapTuple(bitMap,data);
    result.insert(result.end(),typeBin->begin(),typeBin->end());
    result.insert(result.end(),hederBin.begin(),hederBin.end());
    result.insert(result.end(),dataBin.begin(),dataBin.end());
    return result;
}
void tuple::unmarshallTuple(const std::vector<uint8_t>& data) {
    if (data.size() < 2) {
        return;
    }
    int16_t type=0;
    std::vector<uint8_t>typeBytes;
    typeBytes.insert(typeBytes.end(),data.begin(),data.begin()+2);
    UnmarshalInt16_t(&type,&typeBytes);
    if(type == tupleIndetification){
        std::vector<uint8_t>headerBytes;
        headerBytes.insert(headerBytes.end(),data.begin()+2,data.begin()+39);
        header.unmarshallHeaderTuple(headerBytes);
        std::vector<uint8_t>dataBytes;
        dataBytes.insert(dataBytes.end(),data.begin()+39,data.end());
        dataNullBitMap.unmarshallDataNullBitMapTuple(dataBytes);
    }
}