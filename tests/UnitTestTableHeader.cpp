#include <gtest/gtest.h>
#include <vector>
#include <climits>
#include "tableHeader.h"

TEST(TableHeaderTest, DefaultConstructor) {
    tableHeader header;
    
    EXPECT_EQ(header.getOid(), 0);
    EXPECT_EQ(header.getContainToast(), 0);
    EXPECT_EQ(header.getNumberOfColumns(), 0);
    EXPECT_EQ(header.getOwner(), 0);
    EXPECT_EQ(header.getPgNamespace(), 0);
    EXPECT_EQ(header.getPgConstraint(), 0);
    EXPECT_EQ(header.getRights(), 0);
    EXPECT_EQ(header.getFreeSpace(), 0);
    EXPECT_TRUE(header.getTypesWithAllowNull().empty());
    EXPECT_TRUE(header.getColumnNames().empty());
}

TEST(TableHeaderTest, ParametricConstructorEmptyVectors) {
    tableHeader header(42, 1, 0, 9999, 2, 555, 3, 2048, {}, {}, {});
    
    EXPECT_EQ(header.getOid(), 42);
    EXPECT_EQ(header.getContainToast(), 1);
    EXPECT_EQ(header.getNumberOfColumns(), 0);
    EXPECT_EQ(header.getOwner(), 9999);
    EXPECT_EQ(header.getPgNamespace(), 2);
    EXPECT_EQ(header.getPgConstraint(), 555);
    EXPECT_EQ(header.getRights(), 3);
    EXPECT_EQ(header.getFreeSpace(), 2048);
    EXPECT_TRUE(header.getTypesWithAllowNull().empty());
    EXPECT_TRUE(header.getColumnNames().empty());
}

TEST(TableHeaderTest, ParametricConstructorFullData) {
    std::vector<int8_t> types = {1, 2, 3};
    std::vector<int8_t> allowNull = {0, 1, 0};
    std::vector<std::string> names = {"id", "name", "age"};
    
    tableHeader header(100, 1, 3, 5000, 1, 200, 7, 1024, types, allowNull, names);
    
    EXPECT_EQ(header.getOid(), 100);
    EXPECT_EQ(header.getContainToast(), 1);
    EXPECT_EQ(header.getNumberOfColumns(), 3);
    EXPECT_EQ(header.getOwner(), 5000);
    EXPECT_EQ(header.getPgNamespace(), 1);
    EXPECT_EQ(header.getPgConstraint(), 200);
    EXPECT_EQ(header.getRights(), 7);
    EXPECT_EQ(header.getFreeSpace(), 1024);
    
    ASSERT_EQ(header.getTypesWithAllowNull().size(), 3);
    EXPECT_EQ(header.getTypesWithAllowNull()[0], 0);
    EXPECT_EQ(header.getTypesWithAllowNull()[1], 1);
    EXPECT_EQ(header.getTypesWithAllowNull()[2], 0);
    
    ASSERT_EQ(header.getColumnNames().size(), 3);
    EXPECT_EQ(header.getColumnNames()[0], "id");
    EXPECT_EQ(header.getColumnNames()[1], "name");
    EXPECT_EQ(header.getColumnNames()[2], "age");
}

TEST(TableHeaderTest, SetDataBasic) {
    tableHeader header;
    std::vector<int8_t> types = {1, 2};
    std::vector<int8_t> allowNull = {1, 0};
    std::vector<std::string> names = {"col1", "col2"};
    
    header.setData(50, 0, 2, 7777, 3, 888, 5, 512, types, allowNull, names);
    
    EXPECT_EQ(header.getOid(), 50);
    EXPECT_EQ(header.getContainToast(), 0);
    EXPECT_EQ(header.getNumberOfColumns(), 2);
    EXPECT_EQ(header.getOwner(), 7777);
    EXPECT_EQ(header.getPgNamespace(), 3);
    EXPECT_EQ(header.getPgConstraint(), 888);
    EXPECT_EQ(header.getRights(), 5);
    EXPECT_EQ(header.getFreeSpace(), 512);
}

TEST(TableHeaderTest, SetDataOverwrite) {
    tableHeader header;
    std::vector<int8_t> types1 = {1};
    std::vector<int8_t> allowNull1 = {0};
    std::vector<std::string> names1 = {"old"};
    
    header.setData(10, 0, 1, 100, 0, 10, 1, 200, types1, allowNull1, names1);
    EXPECT_EQ(header.getOid(), 10);
    EXPECT_EQ(header.getColumnNames()[0], "old");
    
    std::vector<int8_t> types2 = {2, 3};
    std::vector<int8_t> allowNull2 = {1, 1};
    std::vector<std::string> names2 = {"new1", "new2"};
    
    header.setData(20, 1, 2, 200, 1, 20, 2, 400, types2, allowNull2, names2);
    EXPECT_EQ(header.getOid(), 20);
    EXPECT_EQ(header.getContainToast(), 1);
    EXPECT_EQ(header.getNumberOfColumns(), 2);
    ASSERT_EQ(header.getColumnNames().size(), 2);
    EXPECT_EQ(header.getColumnNames()[0], "new1");
    EXPECT_EQ(header.getColumnNames()[1], "new2");
}

TEST(TableHeaderTest, MarshallUnmarshallBasic) {
    tableHeader header;
    std::vector<int8_t> types = {1, 2, 3};
    std::vector<int8_t> allowNull = {0, 1, 0};
    std::vector<std::string> names = {"id", "name", "age"};
    
    header.setData(42, 1, 3, 9999, 2, 555, 3, 2048, types, allowNull, names);
    
    std::vector<uint8_t> data = header.marshallTableHeaderWithData();
    EXPECT_GT(data.size(), 0);
    
    tableHeader newHeader;
    newHeader.unmarshallTableHeader(data);
    
    EXPECT_EQ(newHeader.getOid(), 42);
    EXPECT_EQ(newHeader.getContainToast(), 1);
    EXPECT_EQ(newHeader.getNumberOfColumns(), 3);
    EXPECT_EQ(newHeader.getOwner(), 9999);
    EXPECT_EQ(newHeader.getPgNamespace(), 2);
    EXPECT_EQ(newHeader.getPgConstraint(), 555);
    EXPECT_EQ(newHeader.getRights(), 3);
    EXPECT_EQ(newHeader.getFreeSpace(), 2048);
    
    ASSERT_EQ(newHeader.getTypesWithAllowNull().size(), 3);
    EXPECT_EQ(newHeader.getTypesWithAllowNull()[0], 0);
    EXPECT_EQ(newHeader.getTypesWithAllowNull()[1], 1);
    EXPECT_EQ(newHeader.getTypesWithAllowNull()[2], 0);
    
    ASSERT_EQ(newHeader.getColumnNames().size(), 3);
    EXPECT_EQ(newHeader.getColumnNames()[0], "id");
    EXPECT_EQ(newHeader.getColumnNames()[1], "name");
    EXPECT_EQ(newHeader.getColumnNames()[2], "age");
}

TEST(TableHeaderTest, MarshallUnmarshallEmptyVectors) {
    tableHeader header;
    header.setData(100, 0, 0, 5000, 0, 100, 0, 1024, {}, {}, {});
    
    std::vector<uint8_t> data = header.marshallTableHeaderWithData();
    
    tableHeader newHeader;
    newHeader.unmarshallTableHeader(data);
    
    EXPECT_EQ(newHeader.getOid(), 100);
    EXPECT_EQ(newHeader.getContainToast(), 0);
    EXPECT_EQ(newHeader.getNumberOfColumns(), 0);
    EXPECT_TRUE(newHeader.getTypesWithAllowNull().empty());
    EXPECT_TRUE(newHeader.getColumnNames().empty());
}

TEST(TableHeaderTest, MarshallUnmarshallLargeData) {
    tableHeader header;
    std::vector<int8_t> types(100, 1);
    std::vector<int8_t> allowNull(100, 0);
    std::vector<std::string> names;
    for (int i = 0; i < 100; i++) {
        names.push_back("column_" + std::to_string(i));
    }
    
    header.setData(999, 1, 100, 123456, 5, 9999, 7, 4096, types, allowNull, names);
    
    std::vector<uint8_t> data = header.marshallTableHeaderWithData();
    
    tableHeader newHeader;
    newHeader.unmarshallTableHeader(data);
    
    EXPECT_EQ(newHeader.getOid(), 999);
    EXPECT_EQ(newHeader.getNumberOfColumns(), 100);
    ASSERT_EQ(newHeader.getColumnNames().size(), 100);
    EXPECT_EQ(newHeader.getColumnNames()[0], "column_0");
    EXPECT_EQ(newHeader.getColumnNames()[99], "column_99");
}

TEST(TableHeaderTest, MarshallUnmarshallSpecialCharacters) {
    tableHeader header;
    std::vector<int8_t> types = {1, 2};
    std::vector<int8_t> allowNull = {1, 1};
    std::vector<std::string> names = {"col_with_underscore", "col-with-dash"};
    
    header.setData(1, 0, 2, 100, 0, 10, 0, 200, types, allowNull, names);
    
    std::vector<uint8_t> data = header.marshallTableHeaderWithData();
    
    tableHeader newHeader;
    newHeader.unmarshallTableHeader(data);
    
    ASSERT_EQ(newHeader.getColumnNames().size(), 2);
    EXPECT_EQ(newHeader.getColumnNames()[0], "col_with_underscore");
    EXPECT_EQ(newHeader.getColumnNames()[1], "col-with-dash");
}

TEST(TableHeaderTest, MarshallUnmarshallLongColumnNames) {
    tableHeader header;
    std::vector<int8_t> types = {1};
    std::vector<int8_t> allowNull = {0};
    std::string longName(1000, 'A');
    std::vector<std::string> names = {longName};
    
    header.setData(1, 0, 1, 100, 0, 10, 0, 200, types, allowNull, names);
    
    std::vector<uint8_t> data = header.marshallTableHeaderWithData();
    
    tableHeader newHeader;
    newHeader.unmarshallTableHeader(data);
    
    ASSERT_EQ(newHeader.getColumnNames().size(), 1);
    EXPECT_EQ(newHeader.getColumnNames()[0].length(), 1000);
    EXPECT_EQ(newHeader.getColumnNames()[0], longName);
}

TEST(TableHeaderTest, EdgeValuesInt32) {
    tableHeader header;
    header.setData(INT32_MAX, 0, INT32_MAX, 0, 0, INT32_MAX, 0, INT32_MAX, {}, {}, {});
    
    std::vector<uint8_t> data = header.marshallTableHeaderWithData();
    
    tableHeader newHeader;
    newHeader.unmarshallTableHeader(data);
    
    EXPECT_EQ(newHeader.getOid(), INT32_MAX);
    EXPECT_EQ(newHeader.getNumberOfColumns(), INT32_MAX);
    EXPECT_EQ(newHeader.getPgConstraint(), INT32_MAX);
    EXPECT_EQ(newHeader.getFreeSpace(), INT32_MAX);
}

TEST(TableHeaderTest, EdgeValuesInt32Negative) {
    tableHeader header;
    header.setData(INT32_MIN, 0, INT32_MIN, 0, 0, INT32_MIN, 0, INT32_MIN, {}, {}, {});
    
    std::vector<uint8_t> data = header.marshallTableHeaderWithData();
    
    tableHeader newHeader;
    newHeader.unmarshallTableHeader(data);
    
    EXPECT_EQ(newHeader.getOid(), INT32_MIN);
    EXPECT_EQ(newHeader.getNumberOfColumns(), INT32_MIN);
    EXPECT_EQ(newHeader.getPgConstraint(), INT32_MIN);
}

TEST(TableHeaderTest, EdgeValuesInt8) {
    tableHeader header;
    header.setData(0, INT8_MAX, 0, 0, INT8_MAX, 0, INT8_MAX, 0, {}, {}, {});
    
    std::vector<uint8_t> data = header.marshallTableHeaderWithData();
    
    tableHeader newHeader;
    newHeader.unmarshallTableHeader(data);
    
    EXPECT_EQ(newHeader.getContainToast(), INT8_MAX);
    EXPECT_EQ(newHeader.getPgNamespace(), INT8_MAX);
    EXPECT_EQ(newHeader.getRights(), INT8_MAX);
}

TEST(TableHeaderTest, EdgeValuesInt8Negative) {
    tableHeader header;
    header.setData(0, INT8_MIN, 0, 0, INT8_MIN, 0, INT8_MIN, 0, {}, {}, {});
    
    std::vector<uint8_t> data = header.marshallTableHeaderWithData();
    
    tableHeader newHeader;
    newHeader.unmarshallTableHeader(data);
    
    EXPECT_EQ(newHeader.getContainToast(), INT8_MIN);
    EXPECT_EQ(newHeader.getPgNamespace(), INT8_MIN);
    EXPECT_EQ(newHeader.getRights(), INT8_MIN);
}

TEST(TableHeaderTest, EdgeValuesInt64) {
    tableHeader header;
    header.setData(0, 0, 0, INT64_MAX, 0, 0, 0, 0, {}, {}, {});
    
    std::vector<uint8_t> data = header.marshallTableHeaderWithData();
    
    tableHeader newHeader;
    newHeader.unmarshallTableHeader(data);
    
    EXPECT_EQ(newHeader.getOwner(), INT64_MAX);
}

TEST(TableHeaderTest, EdgeValuesInt64Negative) {
    tableHeader header;
    header.setData(0, 0, 0, INT64_MIN, 0, 0, 0, 0, {}, {}, {});
    
    std::vector<uint8_t> data = header.marshallTableHeaderWithData();
    
    tableHeader newHeader;
    newHeader.unmarshallTableHeader(data);
    
    EXPECT_EQ(newHeader.getOwner(), INT64_MIN);
}

TEST(TableHeaderTest, AllZeroValues) {
    tableHeader header;
    header.setData(0, 0, 0, 0, 0, 0, 0, 0, {}, {}, {});
    
    std::vector<uint8_t> data = header.marshallTableHeaderWithData();
    
    tableHeader newHeader;
    newHeader.unmarshallTableHeader(data);
    
    EXPECT_EQ(newHeader.getOid(), 0);
    EXPECT_EQ(newHeader.getContainToast(), 0);
    EXPECT_EQ(newHeader.getNumberOfColumns(), 0);
    EXPECT_EQ(newHeader.getOwner(), 0);
    EXPECT_EQ(newHeader.getPgNamespace(), 0);
    EXPECT_EQ(newHeader.getPgConstraint(), 0);
    EXPECT_EQ(newHeader.getRights(), 0);
    EXPECT_EQ(newHeader.getFreeSpace(), 0);
}

TEST(TableHeaderTest, UnmarshallEmptyData) {
    tableHeader header;
    header.setData(42, 1, 3, 9999, 2, 555, 3, 2048, {1, 2, 3}, {0, 1, 0}, {"id", "name", "age"});
    
    std::vector<uint8_t> emptyData;
    header.unmarshallTableHeader(emptyData);
    
    EXPECT_EQ(header.getOid(), 42);
    EXPECT_EQ(header.getContainToast(), 1);
}

TEST(TableHeaderTest, UnmarshallPartialData) {
    tableHeader header;
    std::vector<uint8_t> fullData = header.marshallTableHeader(
        42, 1, 3, 9999, 2, 555, 3, 2048, {1, 2, 3}, {0, 1, 0}, {"id", "name", "age"}
    );
    
    std::vector<uint8_t> partialData(fullData.begin(), fullData.begin() + 15);
    
    tableHeader newHeader;
    EXPECT_NO_THROW(newHeader.unmarshallTableHeader(partialData));
}

TEST(TableHeaderTest, UnmarshallCorruptedData) {
    tableHeader header;
    std::vector<uint8_t> data = header.marshallTableHeader(
        42, 1, 3, 9999, 2, 555, 3, 2048, {1, 2, 3}, {0, 1, 0}, {"id", "name", "age"}
    );
    
    for (size_t i = 0; i < data.size(); i += 10) {
        if (i < data.size()) {
            data[i] = 0xFF;
        }
    }
    
    tableHeader newHeader;
    EXPECT_NO_THROW(newHeader.unmarshallTableHeader(data));
}

TEST(TableHeaderTest, MismatchedVectorSizes) {
    tableHeader header;
    std::vector<int8_t> types = {1, 2, 3, 4, 5};
    std::vector<int8_t> allowNull = {0, 1};
    std::vector<std::string> names = {"a", "b", "c"};
    
    EXPECT_NO_THROW(header.setData(1, 0, 5, 100, 0, 10, 0, 200, types, allowNull, names));
    
    std::vector<uint8_t> data = header.marshallTableHeaderWithData();
    
    tableHeader newHeader;
    EXPECT_NO_THROW(newHeader.unmarshallTableHeader(data));
}

TEST(TableHeaderTest, EmptyStringsInColumnNames) {
    tableHeader header;
    std::vector<int8_t> types = {1, 2, 3};
    std::vector<int8_t> allowNull = {0, 1, 0};
    std::vector<std::string> names = {"", "valid_name", ""};
    
    header.setData(1, 0, 3, 100, 0, 10, 0, 200, types, allowNull, names);
    
    std::vector<uint8_t> data = header.marshallTableHeaderWithData();
    
    tableHeader newHeader;
    newHeader.unmarshallTableHeader(data);
    
    ASSERT_EQ(newHeader.getColumnNames().size(), 3);
    EXPECT_EQ(newHeader.getColumnNames()[0], "");
    EXPECT_EQ(newHeader.getColumnNames()[1], "valid_name");
    EXPECT_EQ(newHeader.getColumnNames()[2], "");
}

TEST(TableHeaderTest, MultipleMarshallUnmarshall) {
    tableHeader header;
    
    for (int i = 0; i < 10; i++) {
        std::vector<int8_t> types = {static_cast<int8_t>(i)};
        std::vector<int8_t> allowNull = {static_cast<int8_t>(i % 2)};
        std::vector<std::string> names = {"col_" + std::to_string(i)};
        
        header.setData(i, i % 2, 1, i * 100, i % 3, i * 10, i % 4, i * 100, types, allowNull, names);
        
        std::vector<uint8_t> data = header.marshallTableHeaderWithData();
        
        tableHeader newHeader;
        newHeader.unmarshallTableHeader(data);
        
        EXPECT_EQ(newHeader.getOid(), i);
        EXPECT_EQ(newHeader.getContainToast(), i % 2);
        ASSERT_EQ(newHeader.getColumnNames().size(), 1);
        EXPECT_EQ(newHeader.getColumnNames()[0], "col_" + std::to_string(i));
    }
}

TEST(TableHeaderTest, ReuseHeaderObject) {
    tableHeader header;
    
    header.setData(1, 0, 1, 100, 0, 10, 0, 200, {1}, {0}, {"first"});
    std::vector<uint8_t> data1 = header.marshallTableHeaderWithData();
    
    header.setData(2, 1, 2, 200, 1, 20, 1, 400, {2, 3}, {1, 0}, {"second", "third"});
    std::vector<uint8_t> data2 = header.marshallTableHeaderWithData();
    
    EXPECT_NE(data1, data2);
    
    tableHeader newHeader1;
    newHeader1.unmarshallTableHeader(data1);
    EXPECT_EQ(newHeader1.getOid(), 1);
    ASSERT_EQ(newHeader1.getColumnNames().size(), 1);
    
    tableHeader newHeader2;
    newHeader2.unmarshallTableHeader(data2);
    EXPECT_EQ(newHeader2.getOid(), 2);
    ASSERT_EQ(newHeader2.getColumnNames().size(), 2);
}

TEST(TableHeaderTest, CompareMarshallMethods) {
    tableHeader header;
    std::vector<int8_t> types = {1, 2};
    std::vector<int8_t> allowNull = {0, 1};
    std::vector<std::string> names = {"col1", "col2"};
    
    header.setData(42, 1, 2, 9999, 2, 555, 3, 2048, types, allowNull, names);
    
    std::vector<uint8_t> dataWithData = header.marshallTableHeaderWithData();
    std::vector<uint8_t> directData = header.marshallTableHeader(42, 1, 2, 9999, 2, 555, 3, 2048, types, allowNull, names);
    
    EXPECT_EQ(dataWithData, directData);
}

TEST(TableHeaderTest, DataConsistencyAfterMultipleOperations) {
    tableHeader header;
    std::vector<int8_t> types = {1, 2, 3};
    std::vector<int8_t> allowNull = {0, 1, 0};
    std::vector<std::string> names = {"id", "name", "age"};
    
    header.setData(42, 1, 3, 9999, 2, 555, 3, 2048, types, allowNull, names);
    
    std::vector<uint8_t> data1 = header.marshallTableHeaderWithData();
    std::vector<uint8_t> data2 = header.marshallTableHeaderWithData();
    
    EXPECT_EQ(data1, data2);
    
    tableHeader newHeader;
    newHeader.unmarshallTableHeader(data1);
    
    std::vector<uint8_t> data3 = newHeader.marshallTableHeaderWithData();
    
    EXPECT_EQ(data1, data3);
}

TEST(TableHeaderTest, ComplexDataRoundTrip) {
    tableHeader header;
    std::vector<int8_t> types = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int8_t> allowNull = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    std::vector<std::string> names = {
        "employee_id", "first_name", "last_name", "email", 
        "phone_number", "hire_date", "job_id", "salary", 
        "commission_pct", "department_id"
    };
    
    header.setData(123, 1, 10, 456789, 5, 999, 7, 8192, types, allowNull, names);
    
    std::vector<uint8_t> data = header.marshallTableHeaderWithData();
    
    tableHeader newHeader;
    newHeader.unmarshallTableHeader(data);
    
    EXPECT_EQ(newHeader.getOid(), 123);
    EXPECT_EQ(newHeader.getContainToast(), 1);
    EXPECT_EQ(newHeader.getNumberOfColumns(), 10);
    EXPECT_EQ(newHeader.getOwner(), 456789);
    EXPECT_EQ(newHeader.getPgNamespace(), 5);
    EXPECT_EQ(newHeader.getPgConstraint(), 999);
    EXPECT_EQ(newHeader.getRights(), 7);
    EXPECT_EQ(newHeader.getFreeSpace(), 8192);
    
    ASSERT_EQ(newHeader.getTypesWithAllowNull().size(), 10);
    ASSERT_EQ(newHeader.getColumnNames().size(), 10);
    
    for (size_t i = 0; i < names.size(); i++) {
        EXPECT_EQ(newHeader.getColumnNames()[i], names[i]);
        EXPECT_EQ(newHeader.getTypesWithAllowNull()[i], allowNull[i]);
    }
}