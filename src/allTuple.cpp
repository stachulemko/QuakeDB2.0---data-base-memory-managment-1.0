#include "allTuple.h"

tuple::tuple(){

}
tuple::~tuple(){

}
std::vector<uint8_t> tuple::marshallTuple(int64_t xmin, int64_t xmax, int32_t cid, int32_t infomask, int16_t hoff, bool bitmap, int64_t oid, std::vector<bool>bitMap,std::vector<allVars>data){ 
    std::vector<uint8_t> result;
    std::vector<uint8_t> *typeBin = marshalInt16_t(tupleIndetification);
    std::vector<uint8_t>headerBin = header.marshallHeaderTuple(xmin,xmax,cid,infomask,hoff,bitmap,oid);
    std::vector<uint8_t>dataBin = dataNullBitMap.marshallDataNullBitMapTuple(bitMap,data);
    std::vector<uint8_t> *sizeBin = marshalInt32_t(dataBin.size()+headerBin.size());
    std::cout<<"sizeTypeBin : "<<typeBin->size()<<std::endl;
    std::cout<<"sizeHeaderBin : "<<headerBin.size()<<std::endl;
    std::cout<<"sizeDataBin : "<<dataBin.size()<<std::endl;
    result.insert(result.end(),typeBin->begin(),typeBin->end());
    result.insert(result.end(),sizeBin->begin(),sizeBin->end());  //new 
    result.insert(result.end(),headerBin.begin(),headerBin.end());
    result.insert(result.end(),dataBin.begin(),dataBin.end());
    delete typeBin;
    delete sizeBin;
    return result;
}
void tuple::unmarshallTuple(const std::vector<uint8_t>& data) {
    if (data.size() < 2) {
        return;
    }
    int16_t type=0;
    int32_t offset = 0;
    std::vector<uint8_t>typeBytes;
    typeBytes.insert(typeBytes.end(),data.begin(),data.begin()+2);
    std::cout<<"unmarshallTypeBytes : "<<typeBytes.size()<<std::endl;
    offset += 2;
    UnmarshalInt16_t(&type,&typeBytes);
    if(type == tupleIndetification){
        offset+=4;
        std::vector<uint8_t>headerBytes;
        headerBytes.insert(headerBytes.end(),data.begin()+offset,data.begin()+offset+header.getSize());
        std::cout<<"unmarshallheaderBytes : "<<headerBytes.size()<<std::endl;
        offset += header.getSize();
        header.unmarshallHeaderTuple(headerBytes);
        //header.showData();
        std::vector<uint8_t>dataBytes;
        dataBytes.insert(dataBytes.end(),data.begin()+offset,data.end());
        std::cout<<"unmarshalldataBytes : "<<dataBytes.size()<<std::endl;
        dataNullBitMap.unmarshallDataNullBitMapTuple(dataBytes);
    }
}

std::vector<uint8_t> tuple::marshallTupleWithData(){
    return marshallTuple(header.getTxMin(),header.getTxMax(),header.getTCid(),header.getTInfomask(),header.getTHoff(),header.getNullBitmap(),header.getOptionalOid(),dataNullBitMap.getBitMap(),dataNullBitMap.getData());
}