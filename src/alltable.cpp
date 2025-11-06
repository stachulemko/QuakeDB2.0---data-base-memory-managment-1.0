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
        for (int i = 0; i < 2; ++i) {
            for (int b = 7; b >= 0; --b)
                std::cout << ((result[i] >> b) & 1);
            }
    }
    return result;
}

void allTable::unmarshallAllTable(const std::vector<uint8_t>& data){
    if(data.size()<blockSize){
        assert("error : the data is not complete");
    }
    std::vector<uint8_t> headerBytes;
    headerBytes.insert(headerBytes.end(),data.begin(),data.begin()+blockSize);
    header->unmarshallTableHeader(headerBytes);
    //std::vector<uint8_t> tuplesBytes;
    //tuplesBytes.insert(tuplesBytes.end(),data.begin()+39,data.end());
    for (int i=blockSize;i<data.size();i+=blockSize){
        std::vector<uint8_t>block8kbBytes;
        block8kbBytes.insert(block8kbBytes.end(),data.begin()+i,data.begin()+i+blockSize);
        std::cout<<"+++++++++++++="<<std::endl;
        for (int i = 0; i < 2; ++i) {
                for (int b = 7; b >= 0; --b)
                    std::cout << ((block8kbBytes[i] >> b) & 1);
                }
        std::cout<<std::endl;
        std::cout<<"+++++++++++++="<<std::endl;
        
        block8kb *block8kbObj = new block8kb(0,0,0,0,0,0,0);
        
        //block8kbObj->setBlockNum();
        block8kbObj->unmarshallBlock8kb(block8kbBytes);
        blocks.push_back(block8kbObj);
    }
    //tuples.unmarshallTuple(tuplesBytes);
}