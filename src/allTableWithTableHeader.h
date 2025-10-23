/*
#ifndef ALLTABLEWITHTABLEHEADER_H
#define ALLTABLEWITHTABLEHEADER_H

#include <iostream>
#include "alltable.h"
#include "tableHeader.h"
#include  "fileManager.h"

class AllTableWithTableHeader {
    private:
        tableHeader *header = nullptr;
        allTable *alltable = nullptr;

        //std::vector<int8_t> typesWithAllowNull={}; 
        //std::vector<std::string> columnNames={};
    public:
        AllTableWithTableHeader(int32_t oid,int8_t contain_toast,int32_t numberOfColumns,int64_t owner,int8_t pg_namespace,int32_t pg_constraint,int8_t rights,int32_t freeSpace,std::vector<int8_t>types,std::vector<int8_t> typesWithAllowNull,std::vector<std::string> columnNames){
            if(types.size()==0 || typesWithAllowNull.size()==0 || columnNames.size()==0 || types.size()!=typesWithAllowNull.size() || types.size()!=columnNames.size()){
                //throw std::invalid_argument("Invalid types or columnNames or typesWithAllowNull size");
            }
            //header = new tableHeader(oid, contain_toast, numberOfColumns, owner, pg_namespace, pg_constraint, rights, freeSpace,types,typesWithAllowNull,columnNames);
            //alltable = new allTable(freeSpace);
            //createBinFile(PathTableHeaders, std::to_string(oid));
            //createBinFile(PathTableRows, std::to_string(oid));
            //saveBlock(PathTableHeaders, std::to_string(oid), header->marshallTableHeaderWithData());
        }
        void addDataToBlock(tuple& tupleObj){
            alltable->addDataToBlock(tupleObj);
        }

        ~AllTableWithTableHeader() {
            delete header;
            delete alltable;
        }

        std::vector<uint8_t>marshallHeader(){
            return header->marshallTableHeaderWithData();
        }

        std::vector<uint8_t>marshallAllTable(){
            return alltable->marshallAllTable();
        }
        void unmarshallHeader(const std::vector<uint8_t>& data){
            header->unmarshallTableHeader(data);
        }
        void marshallAllTable(const std::vector<uint8_t>& data){
            alltable->unmarshallAllTable(data);
        }

        void showData(){
            header->showData();
            alltable->showData();
        }
};




#endif
*/