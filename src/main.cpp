
#include <iostream>
#include <gtest/gtest.h>
#include "typesDataConverter.h"
#include "headerTuple.h"
#include "dataNullBitMapTuple.h"
#include "allTuple.h"
#include "tableHeader.h"
#include "allTableWithTableHeader.h"
#include "alltable.h"
#include "block8kb.h"
int main() {
    tableHeader* th = new tableHeader(1,0,3,1001,0,0,7,1024,{1,2,3},{1,2,3},{"col1","col2","col3"});
    allTable table(200, th);
    tuple tupleObj;
    tupleObj.setData(100,200,1,0,24,true,5000,{false,true,false},{allVars(123),allVars("test"),allVars(43)});
    table.addDataToBlock(tupleObj, 1, 2, 3, 4, 5, 6);

    tuple tupleObj1;
    tupleObj1.setData(100,200,1,0,24,true,5000,{false,true,false},{allVars(123),allVars("test"),allVars(43)});
    table.addDataToBlock(tupleObj1, 1, 2, 3, 4, 5, 6);


    tuple tupleObj2;
    tupleObj2.setData(100,200,1,0,24,true,5000,{false,true,false},{allVars(123),allVars("test"),allVars(43)});
    table.addDataToBlock(tupleObj2, 1, 2, 3, 4, 5, 6);

    allTable table2;
    table2.unmarshallAllTable(table.marshallAllTable());

    std::cout<<"---------------------------"<<std::endl;
    table2.showData();
    return 0;
    
        
    /*
    tuple tupleObj;
    tupleObj.setData(100,200,1,0,24,true,5000,{false,true,false},{allVars(123),allVars("test"),allVars(43)});
    std::vector<uint8_t> marshalledData = tupleObj.marshallTupleWithData();

    tuple tupleObj2;
    tupleObj2.unmarshallTuple(marshalledData);
    tupleObj2.showData();
    */
    


    /*
    DataNullBitMapTuple dataNullBitMapTupleTest;
    dataNullBitMapTupleTest.setData({false,true,false},{allVars(123),allVars("test"),allVars(43)});

    std::vector<uint8_t> marshalledData = dataNullBitMapTupleTest.marshallDataNullBitMapTupleWithData();

    DataNullBitMapTuple dataNullBitMapTupleUnmarshall;
    // POPRAWIONE: Przekaż marshalledData jako parametr!
    dataNullBitMapTupleUnmarshall.unmarshallDataNullBitMapTuple(marshalledData);
    dataNullBitMapTupleUnmarshall.showData();
    */
    block8kb block8kb1(1,2,3,4,5,6,7);
    tuple tupleObj;
    tupleObj.setData(100,200,1,0,24,true,5000,{false,true,false},{allVars(123),allVars("test"),allVars(43)});
    block8kb1.addData(tupleObj);

    std::vector<uint8_t> marshalledBlock = block8kb1.marshallBlock8kb();

    block8kb block8kb2(0,0,0,0,0,0,0);
    block8kb2.unmarshallBlock8kb(marshalledBlock);
    

    return 0;



    
}