
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
    tableHeader th(1,0,3,1001,0,0,7,1024,{1,2,3},{1,2,3},{"col1","col2","col3"});
    allTable table(8192, &th);
    tuple tupleObj;
    tupleObj.setData(100,200,1,0,24,true,5000,{false,true,false},{allVars(123),allVars("test"),allVars(43)});
    table.addDataToBlock(tupleObj, 0, 0, 0, 0, 0, 0);

    allTable table2();
    table.unmarshallAllTable(table.marshallAllTable());
    table.showData();
    return 0;
}