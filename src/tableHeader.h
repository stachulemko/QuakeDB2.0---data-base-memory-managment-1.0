#ifndef TABLEHEADER_H
#define TABLEHEADER_H

#include <iostream>
#include <vector>
#include "typesDataConverter.h"
#include "typeManagerAllVarsTypes.h"

const int32_t tableHeaderSize = 8192;

class tableHeader{
    private:
        int32_t oid=0;
        int64_t xmin =0;
        int64_t xmax = 0;
        int8_t contain_toast=0;
        int32_t numberOfColumns=0;
        int64_t owner=0;
        int8_t pg_namespace=0;
        int32_t pg_constraint=0;
        int8_t rights=0;
        int32_t freeSpace=0;
        std::vector<int8_t> types={};
        std::vector<int8_t> typesWithAllowNull={}; 
        std::vector<std::string> columnNames={};

    public:
        tableHeader();

        ~tableHeader();

        void setData(int32_t oid,int64_t xmin,int64_t xmax, int8_t contain_toast, int32_t numberOfColumns, int64_t owner, int8_t pg_namespace, int32_t pg_constraint, int8_t rights,int32_t freeSpace, std::vector<int8_t> types , std::vector<int8_t> typesWithAllowNull , std::vector<std::string> columnNames) {
            this->oid = oid;
            this->xmin = xmin;
            this->xmax = xmax;
            this->contain_toast = contain_toast;
            this->numberOfColumns = numberOfColumns;
            this->owner = owner;
            this->pg_namespace = pg_namespace;
            this->pg_constraint = pg_constraint;
            this->rights = rights;
            this->freeSpace = freeSpace;
            this->types = types;   
            this->typesWithAllowNull = typesWithAllowNull;
            this->columnNames = columnNames;
        }

        std::vector<uint8_t> marshallTableHeaderWithData(){
            return marshallTableHeader(oid, xmin, xmax, contain_toast, numberOfColumns, owner, pg_namespace, pg_constraint, rights, freeSpace, types, typesWithAllowNull, columnNames);
        }

        std::vector<uint8_t> marshallTableHeader(int32_t oid, int64_t xmin, int64_t xmax, int8_t contain_toast, int32_t numberOfColumns, int64_t owner, int8_t pg_namespace, int32_t pg_constraint, int8_t rights, int32_t freeSpace, std::vector<int8_t> types, std::vector<int8_t> typesWithAllowNull, std::vector<std::string> columnNames);

        void unmarshallTableHeader(const std::vector<uint8_t>& data);

        // Gettery
        int32_t getOid()  { return oid; }
        int64_t getXmin()  { return xmin; }
        int64_t getXmax()  { return xmax; }
        int8_t getContainToast()  { return contain_toast; }
        int32_t getNumberOfColumns()  { return numberOfColumns; }  
        int64_t getOwner()  { return owner; }
        int8_t getPgNamespace()  { return pg_namespace; }
        int32_t getPgConstraint()  { return pg_constraint; }
        int8_t getRights()  { return rights; }
        //int32_t getFreeSpaceTableHeader()  { return freeSpaceTableHeader; }

        int32_t getFreeSpace()  { return freeSpace; }
        std::vector<int8_t> getTypesWithAllowNull()  { return typesWithAllowNull; }
        std::vector<std::string> getColumnNames()  { return columnNames; }
        std::vector<int8_t>getTypes()  { return types; }
        
        void setXmin(int64_t xmin) { this->xmin = xmin; }
        void setXmax(int64_t xmax) { this->xmax = xmax; }
        void setContainToast(int8_t contain_toast) { this->contain_toast = contain_toast; }
        void setNumberOfColumns(int32_t numberOfColumns) { this->numberOfColumns = numberOfColumns; }
        void setOwner(int64_t owner) { this->owner = owner; }
        void setPgNamespace(int8_t pg_namespace) { this->pg_namespace = pg_namespace; }
        void setPgConstraint(int32_t pg_constraint) { this->pg_constraint = pg_constraint; }
        void setRights(int8_t rights) { this->rights = rights; }
        void setFreeSpace(int32_t freeSpace) { this->freeSpace = freeSpace; }
        void setTypes(std::vector<int8_t> types) { this->types = types; }
        void setTypesWithAllowNull(std::vector<int8_t> typesWithAllowNull) { this->typesWithAllowNull = typesWithAllowNull; }
        void setColumnNames(std::vector<std::string> columnNames) { this->columnNames = columnNames; }
        tableHeader(int32_t oid, int64_t xmin, int64_t xmax, int8_t contain_toast, int32_t numberOfColumns, int64_t owner, int8_t pg_namespace, int32_t pg_constraint, int8_t rights, int32_t freeSpace, std::vector<int8_t> types, std::vector<int8_t> typesWithAllowNull, std::vector<std::string> columnNames){
            this->oid = oid;
            this->xmin = xmin;
            this->xmax = xmax;
            this->contain_toast = contain_toast;
            this->numberOfColumns = numberOfColumns;
            this->owner = owner;
            this->pg_namespace = pg_namespace;
            this->pg_constraint = pg_constraint;
            this->rights = rights;
            //this->freeSpaceTableHeader = freeSpaceTableHeader;
            this->freeSpace = freeSpace;
            this->types = types;
            this->typesWithAllowNull = typesWithAllowNull;
            this->columnNames = columnNames;

        }

        void showData(){
            std::cout<<"oid: "<<oid<<std::endl;
            std::cout<<"xmin: "<<xmin<<std::endl;
            std::cout<<"xmax: "<<xmax<<std::endl;
            std::cout<<"contain_toast: "<<(int)contain_toast<<std::endl;
            std::cout<<"numberOfColumns: "<<numberOfColumns<<std::endl;
            std::cout<<"owner: "<<owner<<std::endl;
            std::cout<<"pg_namespace: "<<(int)pg_namespace<<std::endl;
            std::cout<<"pg_constraint: "<<pg_constraint<<std::endl;
            std::cout<<"rights: "<<(int)rights<<std::endl;
            //std::cout<<"freeSpaceTableHeader: "<<freeSpaceTableHeader<<std::endl;
            std::cout<<"freeSpace: "<<freeSpace<<std::endl;

            std::cout<<"types: ";
            for (int i=0;i<types.size();i++){
                std::cout<<(int)types[i]<<" ";
            }
            std::cout<<std::endl;

            std::cout<<"typesWithAllowNull: ";
            for (int i=0;i<typesWithAllowNull.size();i++){
                std::cout<<(int)typesWithAllowNull[i]<<" ";
            }
            std::cout<<std::endl;

            std::cout<<"columnNames: ";
            for (int i=0;i<columnNames.size();i++){
                std::cout<<columnNames[i]<<" ";
            }
            std::cout<<std::endl;
        }


};








#endif