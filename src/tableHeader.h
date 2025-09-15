#ifndef TABLEHEADER_H
#define TABLEHEADER_H

#include <iostream>
#include <vector>
#include "typesDataConverter.h"
#include "typeManagerAllVarsTypes.h"

/*
___________________________________
|             table               | 
|types with allow Null            |   - size = number of raws * 4 bit + number of raws * 1 bit
|oid indetification               |   - 32 bit
|contain toast                    |   - 1 bit
|column names                     |   - ~
|owner                            |   - int64_t
|pg_namespace shema_status(public)|   - int8_t
|pg_constraint - keys information |   - int32_t
|rights                           |   - int8_t
|free space                       |   - optional with large number of updates
-----------------------------------
*/

class tableHeader{
    private:
        int32_t oid=0;
        int8_t contain_toast=0;
        int32_t numberOfColumns=0;
        int64_t owner=0;
        int8_t pg_namespace=0;
        int32_t pg_constraint=0;
        int8_t rights=0;
        int32_t freeSpace=0;
        int32_t unitSize=0;
    public:
        tableHeader();

        ~tableHeader();

        void setData(int32_t oid, int8_t contain_toast, int32_t numberOfColumns, int64_t owner, int8_t pg_namespace, int32_t pg_constraint, int8_t rights, int32_t freeSpace, int32_t unitSize){
            this->oid = oid;
            this->contain_toast = contain_toast;
            this->numberOfColumns = numberOfColumns;
            this->owner = owner;
            this->pg_namespace = pg_namespace;
            this->pg_constraint = pg_constraint;
            this->rights = rights;
            this->freeSpace = freeSpace;
            this->unitSize = unitSize;
        }

        std::vector<uint8_t> marshallTableHeaderWithData(){
            return marshallTableHeader(oid,contain_toast,numberOfColumns,owner,pg_namespace,pg_constraint,rights,freeSpace,unitSize);
        }

        std::vector<uint8_t> marshallTableHeader(int32_t oid, int8_t contain_toast, int32_t numberOfColumns, int64_t owner, int8_t pg_namespace, int32_t pg_constraint, int8_t rights, int32_t freeSpace, int32_t unitSize);

        void unmarshallTableHeader(const std::vector<uint8_t>& data);

        // Gettery
        int32_t getOid()  { return oid; }
        int8_t getContainToast()  { return contain_toast; }
        int32_t getNumberOfColumns()  { return numberOfColumns; }  
        int64_t getOwner()  { return owner; }
        int8_t getPgNamespace()  { return pg_namespace; }
        int32_t getPgConstraint()  { return pg_constraint; }
        int8_t getRights()  { return rights; }
        int32_t getFreeSpace()  { return freeSpace; }
        int32_t getUnitSize()  { return unitSize; }


};








#endif