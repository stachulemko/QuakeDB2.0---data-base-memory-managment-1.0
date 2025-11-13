
#include <iostream>
#include <gtest/gtest.h>
#include "typesDataConverter.h"
#include "headerTuple.h"
#include "dataNullBitMapTuple.h"
#include "allTuple.h"
#include "tableHeader.h"
#include "allTableWithTableHeader.h"
#include "alltable.h"
int main() {
    /*
    tableHeader* th = new tableHeader(1,0,3,1001,0,0,7,1024,{1,2,3},{1,2,3},{"col1","col2","col3"});
    allTable table(200, th);
    tuple tupleObj;
    tupleObj.setData(100,200,1,0,24,true,5000,{false,true,false},{allVars(123),allVars("test"),allVars(43)});
    table.addDataToBlock(tupleObj, 1, 2, 3, 4, 5, 6);

    allTable table2;
    table2.unmarshallAllTable(table.marshallAllTable());
    table2.showData();
    return 0;
    */
        

    
    tuple tupleObj;
    tupleObj.setData(100,200,1,0,24,true,5000,{false,true,false},{allVars(123),allVars("test"),allVars(43)});
    std::vector<uint8_t> marshalledData = tupleObj.marshallTupleWithData();

    tuple tupleObj2;
    tupleObj2.unmarshallTuple(marshalledData);
    tupleObj2.showData();
    
    
}