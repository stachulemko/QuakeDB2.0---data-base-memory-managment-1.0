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

std::vector<uint8_t> tableHeader::marshallTableHeader(int32_t oid, int8_t contain_toast, int32_t numberOfColumns, int64_t owner, int8_t pg_namespace, int32_t pg_constraint, int8_t rights, int32_t freeSpace, int32_t unitSize){
    std::vector<uint8_t> result;

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

    std::vector<uint8_t>* freeSpaceBytes = marshalInt32_t(freeSpace);
    result.insert(result.end(),freeSpaceBytes->begin(),freeSpaceBytes->end());
    delete freeSpaceBytes;

    std::vector<uint8_t>* unitSizeBytes = marshalInt32_t(unitSize);
    result.insert(result.end(),unitSizeBytes->begin(),unitSizeBytes->end());
    delete unitSizeBytes;

    return result;
}

void tableHeader::unmarshallTableHeader(const std::vector<uint8_t>& data){
    if(data.size() < 22) return;

    int16_t typeTableHeader=0;
    std::vector<uint8_t> typeTableBin = std::vector<uint8_t>(data.begin(), data.begin() + 2);
    UnmarshalInt16_t(&typeTableHeader, &typeTableBin);

    int32_t oid=0;
    std::vector<uint8_t> oidBin = std::vector<uint8_t>(data.begin()+2, data.begin() + 6);
    UnmarshalInt32_t(&oid, &oidBin);

    int8_t contain_toast=0;
    std::vector<uint8_t> toastBin = std::vector<uint8_t>(data.begin() + 6, data.begin() + 7);
    UnmarshalInt8_t(&contain_toast, &toastBin);

    int32_t numberOfColumns=0;
    std::vector<uint8_t> numberOfColumnsBin = std::vector<uint8_t>(data.begin() + 7, data.begin() + 11);
    UnmarshalInt32_t(&numberOfColumns, &numberOfColumnsBin);

    int64_t owner=0;
    std::vector<uint8_t> ownerBin = std::vector<uint8_t>(data.begin() + 11, data.begin() + 19);
    UnmarshalInt64_t(&owner, &ownerBin);
    
    int8_t pg_namespace=0;
    std::vector<uint8_t> pg_namespaceBin = std::vector<uint8_t>(data.begin() + 19, data.begin() + 20);
    UnmarshalInt8_t(&pg_namespace, &pg_namespaceBin);

    int32_t pg_constraint=0;
    std::vector<uint8_t> pg_constraintBin = std::vector<uint8_t>( data.begin() + 20, data.begin() + 24);
    UnmarshalInt32_t(&pg_constraint, &pg_constraintBin);

    int8_t rights=0;
    std::vector<uint8_t> rightsBin = std::vector<uint8_t>(data.begin() + 24, data.begin() + 25);
    UnmarshalInt8_t(&rights, &rightsBin);

    int32_t freeSpace=0;
    std::vector<uint8_t> freeSpaceBin = std::vector<uint8_t>(data.begin() + 25, data.begin() + 29);
    UnmarshalInt32_t(&freeSpace, &freeSpaceBin);

    int32_t unitSize=0;
    std::vector<uint8_t> unitSizeBin = std::vector<uint8_t>(data.begin() + 29, data.begin() + 33);
    UnmarshalInt32_t(&unitSize, &unitSizeBin);
}

