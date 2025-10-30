#include "alltable.h"
#include <cassert>


allTable::~allTable(){
    // Usuń wszystkie bloki
    for(auto* block : blocks) {
        if(block != nullptr) {
            delete block;
        }
    }
    blocks.clear();
    
    // Usuń tymczasowy blok
    if(blockTmp != nullptr) {
        delete blockTmp;
        blockTmp = nullptr;
    }
    
    // NIE usuwaj header - to wskaźnik przekazany z main()!
    // header jest obiektem na stosie z main.cpp
    header = nullptr;
}

std::vector<uint8_t> allTable::marshallAllTable(){
    std::vector<uint8_t> result;
    std::vector<uint8_t> tableBytes = header->marshallTableHeaderWithData();
    result.insert(result.end(),tableBytes.begin(),tableBytes.end());
    for (int i=0;i<blocks.size();i++){
        std::vector<uint8_t> blockBytes = blocks[i]->marshallBlock8kb();
        result.insert(result.end(), blockBytes.begin(), blockBytes.end());
    }
    if(blockTmp!=nullptr){
        std::vector<uint8_t> blockBytes = blockTmp->marshallBlock8kb();
        result.insert(result.end(), blockBytes.begin(), blockBytes.end());
    }
    return result;
}

void allTable::unmarshallAllTable(const std::vector<uint8_t>& data,std::vector<std::pair<int64_t,int64_t>> pos){
    if(data.size()<33){
        assert("error : the data is not complete");
    }
    std::vector<uint8_t> headerBytes;
    headerBytes.insert(headerBytes.end(),data.begin(),data.begin()+33);
    header->unmarshallTableHeader(headerBytes);
    std::vector<uint8_t> tuplesBytes;
    tuplesBytes.insert(tuplesBytes.end(),data.begin()+39,data.end());
    for (int i=0;i<data.size();i+=blockSize){
        std::vector<uint8_t>block8kbBytes;
        block8kbBytes.insert(block8kbBytes.end(),data.begin()+i,data.begin()+i+blockSize);
        block8kb *block8kbObj = new block8kb(freeSpace,0,0,0,0,0,0);
        block8kbObj->unmarshallBlock8kb(block8kbBytes);
        blocks.push_back(block8kbObj);
    }
    //tuples.unmarshallTuple(tuplesBytes);
}