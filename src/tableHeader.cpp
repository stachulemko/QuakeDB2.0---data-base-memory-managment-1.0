#include "tableHeader.h"


tableHeader::tableHeader(){
            oid = 0;
            contain_toast = 0;
            numberOfColumns = 0;
            owner = 0;
            pg_namespace = 0;
            pg_constraint = 0;
            rights = 0;
        }

tableHeader::~tableHeader(){}

std::vector<uint8_t> tableHeader::marshallTableHeader(int32_t oid,int8_t contain_toast,int32_t numberOfColumns,int64_t owner,int8_t pg_namespace,int32_t pg_constraint,int8_t rights,int32_t freeSpace,std::vector<int8_t> types,std::vector<int8_t> typesWithAllowNull , std::vector<std::string> columnNames){
    std::vector<uint8_t> result;
    int32_t sizeAll =33;
    std::vector<uint8_t>* typeTableHeader = marshalInt16_t(tableHeaderIndetification);
    result.insert(result.end(),typeTableHeader->begin(),typeTableHeader->end());
    delete typeTableHeader;

    std::vector<uint8_t>* oidBytes = marshalInt32_t(oid);
    result.insert(result.end(),oidBytes->begin(),oidBytes->end());
    delete oidBytes;

    std::vector<uint8_t>* toastBytes = marshalInt8_t(contain_toast);
    result.insert(result.end(),toastBytes->begin(),toastBytes->end());
    delete toastBytes;

    std::vector<uint8_t>* numberOfColumnsBytes = marshalInt32_t(numberOfColumns);
    result.insert(result.end(),numberOfColumnsBytes->begin(),numberOfColumnsBytes->end());
    delete numberOfColumnsBytes;

    std::vector<uint8_t>* ownerBytes = marshalInt64_t(owner);
    result.insert(result.end(),ownerBytes->begin(),ownerBytes->end());
    delete ownerBytes;

    std::vector<uint8_t>* pg_namespaceBytes = marshalInt8_t(pg_namespace);
    result.insert(result.end(),pg_namespaceBytes->begin(),pg_namespaceBytes->end());
    delete pg_namespaceBytes;

    std::vector<uint8_t>* pg_constraintBytes = marshalInt32_t(pg_constraint);
    result.insert(result.end(),pg_constraintBytes->begin(),pg_constraintBytes->end());
    delete pg_constraintBytes;

    std::vector<uint8_t>* rightsBytes = marshalInt8_t(rights);
    result.insert(result.end(),rightsBytes->begin(),rightsBytes->end());
    delete rightsBytes;

    //std::vector<uint8_t>* freeSpaceTableHeaderBytes = marshalInt32_t(freeSpaceTableHeader);
    //result.insert(result.end(),freeSpaceTableHeaderBytes->begin(),freeSpaceTableHeaderBytes->end());
    //delete freeSpaceTableHeaderBytes;

    std::vector<uint8_t>* freeSpaceBytes = marshalInt32_t(freeSpace);
    result.insert(result.end(),freeSpaceBytes->begin(),freeSpaceBytes->end());
    delete freeSpaceBytes;

    std::vector<uint8_t>* typeTableBytes = marshalInt16_t(typeTableIndetification);
    result.insert(result.end(),typeTableBytes->begin(),typeTableBytes->end());
    delete typeTableBytes;

    std::vector<uint8_t>* sizeTypesBytes = marshalInt32_t(typesWithAllowNull.size());
    result.insert(result.end(),sizeTypesBytes->begin(),sizeTypesBytes->end());
    delete sizeTypesBytes;

    std::vector<uint8_t>* sizeTypesAllBytes = {};
    for (int i=0;i<types.size();i++){
        std::vector<uint8_t>* typeColumnBytes = marshalInt8_t(types[i]);
        result.insert(result.end(),typeColumnBytes->begin(),typeColumnBytes->end());
        delete typeColumnBytes;
    }

    if(typesWithAllowNull.size()==0){
        //return;
    }
    else{
        std::vector<uint8_t>* int16_tType = marshalInt16_t(dataNullTableHeader);
        result.insert(result.end(),int16_tType->begin(),int16_tType->end());
        sizeAll += 2;
        delete int16_tType;
        std::vector<uint8_t>* numberOfColumnsTypeBytes = marshalInt32_t(typesWithAllowNull.size());
        result.insert(result.end(),numberOfColumnsTypeBytes->begin(),numberOfColumnsTypeBytes->end());
        sizeAll += 4;
        delete numberOfColumnsTypeBytes;
        for (int i =0;i<typesWithAllowNull.size();i++){
            std::vector<uint8_t>* typeColumnBytes = marshalInt8_t(typesWithAllowNull[i]);
            result.insert(result.end(),typeColumnBytes->begin(),typeColumnBytes->end());
            sizeAll += 1;
            delete typeColumnBytes;
        }
        std::vector<uint8_t>* columnNamesTableHeaderBytes = marshalInt16_t(columnNamesTableHeader);
        result.insert(result.end(),columnNamesTableHeaderBytes->begin(),columnNamesTableHeaderBytes->end());
        sizeAll += 2;
        delete columnNamesTableHeaderBytes;
        std::vector<uint8_t>* numberOfColumnsNameBytes = marshalInt32_t(columnNames.size());
        result.insert(result.end(),numberOfColumnsNameBytes->begin(),numberOfColumnsNameBytes->end());
        
    
        sizeAll += 4;
        delete numberOfColumnsNameBytes;
        for (int i =0;i<columnNames.size();i++){
            std::vector<uint8_t>* stringTypeBytes = marshalInt16_t(stringIndetification);
            std::vector<uint8_t>* sizeStringBytes = marshalInt32_t(columnNames[i].size());
            std::vector<uint8_t>* nameColumnBytes = marshalString(columnNames[i]);
            result.insert(result.end(),stringTypeBytes->begin(),stringTypeBytes->end());
            result.insert(result.end(),sizeStringBytes->begin(),sizeStringBytes->end());
            result.insert(result.end(),nameColumnBytes->begin(),nameColumnBytes->end());
            sizeAll += 2+4+columnNames[i].size();
            delete stringTypeBytes;
            delete sizeStringBytes;
            delete nameColumnBytes;

        }
        
    }
    return result;
}

void tableHeader::unmarshallTableHeader(const std::vector<uint8_t>& data){
    if(data.size() < 22) return;

    int16_t typeTableHeader=0;
    int32_t tmpSize = 0;
    std::vector<uint8_t> typeTableBin = std::vector<uint8_t>(data.begin(), data.begin() + tmpSize+2);
    UnmarshalInt16_t(&typeTableHeader, &typeTableBin);
    tmpSize+=2;
    //int32_t oid=0;
    std::vector<uint8_t> oidBin = std::vector<uint8_t>(data.begin()+tmpSize, data.begin() + tmpSize + 4);
    UnmarshalInt32_t(&oid, &oidBin);
    tmpSize+=4;

    //int8_t contain_toast=0;
    std::vector<uint8_t> toastBin = std::vector<uint8_t>(data.begin() + tmpSize, data.begin() + tmpSize + 1);
    UnmarshalInt8_t(&contain_toast, &toastBin);
    tmpSize+=1;
    //std::cout<<contain_toast<<std::endl;
    //int32_t numberOfColumns=0;
    std::vector<uint8_t> numberOfColumnsBin = std::vector<uint8_t>(data.begin() + tmpSize, data.begin() + tmpSize + 4);
    UnmarshalInt32_t(&numberOfColumns, &numberOfColumnsBin);
    tmpSize+=4;

    //int64_t owner=0;
    std::vector<uint8_t> ownerBin = std::vector<uint8_t>(data.begin() + tmpSize, data.begin() + tmpSize + 8);
    UnmarshalInt64_t(&owner, &ownerBin);
    tmpSize+=8;

    //int8_t pg_namespace=0;
    std::vector<uint8_t> pg_namespaceBin = std::vector<uint8_t>(data.begin() + tmpSize, data.begin() + tmpSize + 1);
    UnmarshalInt8_t(&pg_namespace, &pg_namespaceBin);
    tmpSize+=1;

    //int32_t pg_constraint=0;
    std::vector<uint8_t> pg_constraintBin = std::vector<uint8_t>( data.begin() + tmpSize, data.begin() + tmpSize + 4);
    UnmarshalInt32_t(&pg_constraint, &pg_constraintBin);
    tmpSize+=4;

    //int8_t rights=0;
    std::vector<uint8_t> rightsBin = std::vector<uint8_t>(data.begin() + tmpSize, data.begin() + tmpSize + 1);
    UnmarshalInt8_t(&rights, &rightsBin);
    tmpSize+=1;

    //int32_t freeSpaceTableHeader=0;
    //std::vector<uint8_t> freeSpaceTableHeaderBin = std::vector<uint8_t>( data.begin() + tmpSize, data.begin() + tmpSize + 4);
    //std::vector<uint8_t> freeSpaceTableHeaderBin = std::vector<uint8_t>( data.begin() + 25, data.begin() + 29);
    //UnmarshalInt32_t(&freeSpaceTableHeader, &freeSpaceTableHeaderBin);
    //tmpSize+=4;



    //int32_t freeSpace=0;
    std::vector<uint8_t> freeSpaceBin = std::vector<uint8_t>( data.begin() + tmpSize, data.begin() + tmpSize + 4);
    UnmarshalInt32_t(&freeSpace, &freeSpaceBin);
    tmpSize+=4;

    std::vector<uint8_t> typesTypeBin = std::vector<uint8_t>(data.begin() + tmpSize, data.begin() + tmpSize + 2);
    int16_t typesType=0;
    UnmarshalInt16_t(&typesType, &typesTypeBin);
    if(typesType != typeTableIndetification) {
        return;
    }
    
    tmpSize+=2;
    
    std::vector<uint8_t> sizeTypesBin = std::vector<uint8_t>(data.begin() + tmpSize, data.begin() + tmpSize + 4);
    int32_t sizeTypes=0;
    UnmarshalInt32_t(&sizeTypes, &sizeTypesBin);
    tmpSize+=4;
    
    for (int i=0;i<sizeTypes;i++){
        int8_t typeCol=0;
        std::vector<uint8_t> typeColBin = std::vector<uint8_t>(data.begin() + tmpSize, data.begin() + tmpSize + 1);
        UnmarshalInt8_t(&typeCol, &typeColBin);
        types.push_back(typeCol);
        tmpSize+=1;
    }

    if(data.size()<=41){
        return;
    }
    else{
        
        int16_t dataNullType = 0;
        std::vector<uint8_t> dataNullTypeBytes = std::vector<uint8_t>(data.begin() + tmpSize, data.begin() + tmpSize + 2);
        UnmarshalInt16_t(&dataNullType, &dataNullTypeBytes);
        tmpSize+=2; 
        if(dataNullType != dataNullTableHeader) {
            return;
        }
        else{
            
            int32_t numberOfColumnsAllowNull = 0;
            std::vector<uint8_t> numberOfColumnsAllowNullBytes = std::vector<uint8_t>(data.begin() + tmpSize, data.begin() + tmpSize + 4);
            UnmarshalInt32_t(&numberOfColumnsAllowNull, &numberOfColumnsAllowNullBytes);
            tmpSize+=4;
            int32_t i = numberOfColumnsAllowNull;
            while(i>0){
                int8_t allowNull=0;
                std::vector<uint8_t> allowNullBytes = std::vector<uint8_t>(data.begin() + tmpSize, data.begin() + tmpSize + 1);
                UnmarshalInt8_t(&allowNull, &allowNullBytes);
                tmpSize+=1;
                typesWithAllowNull.push_back(allowNull);
                i--;
            }
            int16_t columnsNameTypes = 0;
            std::vector<uint8_t> columnsNameTypesBytes = std::vector<uint8_t>(data.begin() + tmpSize, data.begin() + tmpSize + 2);
            UnmarshalInt16_t(&columnsNameTypes, &columnsNameTypesBytes);
            tmpSize+=2;
            //std::cout<<numberOfColumnsAllowNull<<std::endl;
            if(columnsNameTypes != columnNamesTableHeader) {
                return;
            }
            else{
                int32_t numberOfColumnsNames = 0;
                std::vector<uint8_t> numberOfColumnsNamesBytes = std::vector<uint8_t>(data.begin() + tmpSize , data.begin() + tmpSize + 4);
                UnmarshalInt32_t(&numberOfColumnsNames, &numberOfColumnsNamesBytes);
                tmpSize+=4;
                int32_t j = numberOfColumnsNames;
                int32_t currentIndex = tmpSize;
                while(j>0){
                    int16_t stringType=0;
                    std::vector<uint8_t> stringTypeBytes = std::vector<uint8_t>(data.begin() + currentIndex, data.begin() + currentIndex + 2);
                    UnmarshalInt16_t(&stringType, &stringTypeBytes);
                    if(stringType != stringIndetification) {
                        return;
                    }
                    currentIndex += 2;

                    int32_t sizeString=0;
                    std::vector<uint8_t> sizeStringBytes = std::vector<uint8_t>(data.begin() + currentIndex, data.begin() + currentIndex + 4);
                    UnmarshalInt32_t(&sizeString, &sizeStringBytes);
                    currentIndex += 4;

                    std::vector<uint8_t> nameColumnBytes = std::vector<uint8_t>(data.begin() + currentIndex, data.begin() + currentIndex + sizeString);
                    std::string nameColumn;
                    UnmarshalString(&nameColumn, &nameColumnBytes);
                    columnNames.push_back(nameColumn);
                    currentIndex += sizeString;

                    j--;
                }
            }

        
        }
    }

}



