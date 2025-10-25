#include <gtest/gtest.h>
#include "alltable.h"
#include <vector>
#include <limits>
#include <chrono>

#include <gtest/gtest.h>
#include "alltable.h"
#include "tableHeader.h"
#include "allTuple.h"
#include "block8kb.h"
#include <vector>
#include <limits>
#include <chrono>

// Helper function do tworzenia tableHeader z danymi
tableHeader* createTestTableHeader() {
    return new tableHeader(1, 0, 3, 1001, 0, 0, 7, 1024, {1, 2, 3}, {1, 2, 3}, {"col1", "col2", "col3"});
}

// ==================== TESTY KONSTRUKTORÓW ====================

TEST(AllTableTest, DefaultConstructor) {
    allTable table;
    
    // Test że konstruktor domyślny nie rzuca wyjątków
    EXPECT_NO_THROW(table.showData());
}

TEST(AllTableTest, ParameterizedConstructor) {
    tableHeader* header = createTestTableHeader();
    allTable table(1024, header);
    
    // Test że konstruktor z parametrami działa
    EXPECT_NO_THROW(table.showData());
    
    delete header;
}

TEST(AllTableTest, ConstructorWithNullHeader) {
    allTable table(1024, nullptr);
    
    // Test że konstruktor z nullptr header nie rzuca wyjątków podczas tworzenia
    EXPECT_NO_THROW(allTable(1024, nullptr));
}

TEST(AllTableTest, ConstructorWithZeroFreeSpace) {
    tableHeader* header = createTestTableHeader();
    allTable table(0, header);
    
    EXPECT_NO_THROW(table.showData());
    
    delete header;
}

TEST(AllTableTest, ConstructorWithMaxFreeSpace) {
    tableHeader* header = createTestTableHeader();
    allTable table(std::numeric_limits<int32_t>::max(), header);
    
    EXPECT_NO_THROW(table.showData());
    
    delete header;
}

// ==================== TESTY DODAWANIA BLOKÓW ====================

TEST(AllTableTest, AddNewBlock) {
    tableHeader* header = createTestTableHeader();
    allTable table(1024, header);
    
    EXPECT_NO_THROW(table.addNewBlock(1024, 100, 200, 300, 400, 500, 1));
    EXPECT_NO_THROW(table.showData());
    
    delete header;
}

TEST(AllTableTest, AddMultipleBlocks) {
    tableHeader* header = createTestTableHeader();
    allTable table(2048, header);
    
    for (int i = 0; i < 3; i++) {
        EXPECT_NO_THROW(table.addNewBlock(1024 + i * 100, 100 + i, 200 + i, 300 + i, 400 + i, 500 + i, i % 2));
    }
    
    EXPECT_NO_THROW(table.showData());
    
    delete header;
}

TEST(AllTableTest, AddBlockWithZeroValues) {
    tableHeader* header = createTestTableHeader();
    allTable table(1024, header);
    
    EXPECT_NO_THROW(table.addNewBlock(0, 0, 0, 0, 0, 0, 0));
    EXPECT_NO_THROW(table.showData());
    
    delete header;
}

TEST(AllTableTest, AddBlockWithMaxValues) {
    tableHeader* header = createTestTableHeader();
    allTable table(1024, header);
    
    EXPECT_NO_THROW(table.addNewBlock(
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int16_t>::max(),
        std::numeric_limits<int16_t>::max(),
        std::numeric_limits<int8_t>::max()
    ));
    
    EXPECT_NO_THROW(table.showData());
    
    delete header;
}

TEST(AllTableTest, AddBlockWithNegativeValues) {
    tableHeader* header = createTestTableHeader();
    allTable table(1024, header);
    
    EXPECT_NO_THROW(table.addNewBlock(-1000, -100, -200, -300, -400, -500, -1));
    EXPECT_NO_THROW(table.showData());
    
    delete header;
}

// ==================== TESTY DODAWANIA DANYCH DO BLOKÓW ====================

TEST(AllTableTest, AddDataToBlockSingleTuple) {
    tableHeader* header = createTestTableHeader();
    allTable table(4096, header);
    
    tuple testTuple;
    std::vector<bool> bitMap = {true, false, true};
    std::vector<allVars> data = {allVars(123), allVars(std::string("test")), allVars(43)};
    testTuple.setData(100, 200, 1, 0, 24, true, 5000, bitMap, data);
    
    EXPECT_NO_THROW(table.addDataToBlock(testTuple, 2048, 100, 200, 300, 400, 500, 1));
    EXPECT_NO_THROW(table.showData());
    
    delete header;
}

TEST(AllTableTest, AddDataToBlockMultipleTuples) {
    tableHeader* header = createTestTableHeader();
    allTable table(6000, header);
    
    for (int i = 0; i < 3; i++) {
        tuple testTuple;
        std::vector<bool> bitMap = {true, false, true};
        std::vector<allVars> data = {allVars(123 + i), allVars(std::string("test") + std::to_string(i)), allVars(43 + i)};
        testTuple.setData(100 + i, 200 + i, 1, 0, 24, i % 2 == 0, 5000 + i, bitMap, data);
        
        EXPECT_NO_THROW(table.addDataToBlock(testTuple, 3000, 100 + i, 200 + i, 300 + i, 400 + i, 500 + i, i % 2));
    }
    
    EXPECT_NO_THROW(table.showData());
    
    delete header;
}

TEST(AllTableTest, AddDataToBlockEmptyTuple) {
    tableHeader* header = createTestTableHeader();
    allTable table(4096, header);
    
    tuple emptyTuple;
    
    EXPECT_NO_THROW(table.addDataToBlock(emptyTuple, 2048, 100, 200, 300, 400, 500, 1));
    EXPECT_NO_THROW(table.showData());
    
    delete header;
}

TEST(AllTableTest, AddDataToBlockWithExistingTmpBlock) {
    tableHeader* header = createTestTableHeader();
    allTable table(4096, header);
    
    // Dodaj pierwszy blok
    table.addNewBlock(2048, 100, 200, 300, 400, 500, 1);
    
    // Dodaj tuple do istniejącego bloku tmp
    tuple testTuple;
    std::vector<bool> bitMap = {true, false, true};
    std::vector<allVars> data = {allVars(42), allVars(std::string("data")), allVars(84)};
    testTuple.setData(100, 200, 1, 0, 24, true, 5000, bitMap, data);
    
    EXPECT_NO_THROW(table.addDataToBlock(testTuple, 2048, 100, 200, 300, 400, 500, 1));
    EXPECT_NO_THROW(table.showData());
    
    delete header;
}

TEST(AllTableTest, AddDataToBlockCausingNewBlock) {
    tableHeader* header = createTestTableHeader();
    allTable table(2048, header);
    
    // Dodaj dużo tupli, aby spowodować utworzenie nowego bloku
    for (int i = 0; i < 5; i++) {
        tuple largeTuple;
        std::vector<bool> bitMap = {true, false, true};
        std::vector<allVars> data;
        
        for (int j = 0; j < 3; j++) {
            data.push_back(allVars(std::string("Large data string " + std::to_string(i) + "_" + std::to_string(j))));
        }
        
        largeTuple.setData(100 + i, 200 + i, 1, 0, 24, true, 5000 + i, bitMap, data);
        
        EXPECT_NO_THROW(table.addDataToBlock(largeTuple, 1000, 100 + i, 200 + i, 300 + i, 400 + i, 500 + i, i % 2));
    }
    
    EXPECT_NO_THROW(table.showData());
    
    delete header;
}

// ==================== TESTY MARSHALLING ====================

TEST(AllTableTest, MarshallEmptyTable) {
    tableHeader* header = createTestTableHeader();
    allTable table(1024, header);
    
    std::vector<uint8_t> result = table.marshallAllTable();
    
    EXPECT_FALSE(result.empty());
    EXPECT_GE(result.size(), 33); // Przynajmniej rozmiar header tabeli
    
    delete header;
}

TEST(AllTableTest, MarshallTableWithBlocks) {
    tableHeader* header = createTestTableHeader();
    allTable table(4096, header);
    
    // Dodaj blok
    table.addNewBlock(2048, 100, 200, 300, 400, 500, 1);
    
    std::vector<uint8_t> result = table.marshallAllTable();
    
    EXPECT_FALSE(result.empty());
    EXPECT_GE(result.size(), 33 + blockSize); // Header + przynajmniej jeden blok
    
    delete header;
}

TEST(AllTableTest, MarshallTableWithData) {
    tableHeader* header = createTestTableHeader();
    allTable table(4096, header);
    
    // Dodaj tuple do tabeli
    tuple testTuple;
    std::vector<bool> bitMap = {true, false, true};
    std::vector<allVars> data = {allVars(123), allVars(std::string("test")), allVars(43)};
    testTuple.setData(100, 200, 1, 0, 24, true, 5000, bitMap, data);
    
    table.addDataToBlock(testTuple, 2048, 100, 200, 300, 400, 500, 1);
    
    std::vector<uint8_t> result = table.marshallAllTable();
    
    EXPECT_FALSE(result.empty());
    EXPECT_GE(result.size(), 33 + blockSize);
    
    delete header;
}

TEST(AllTableTest, MarshallTableWithMultipleBlocks) {
    tableHeader* header = createTestTableHeader();
    allTable table(6000, header);
    
    // Dodaj wiele tupli, aby utworzyć wiele bloków
    for (int i = 0; i < 10; i++) {
        tuple testTuple;
        std::vector<bool> bitMap = {true, false, true};
        std::vector<allVars> data = {
            allVars(std::string("Data_" + std::to_string(i) + "_0")),
            allVars(std::string("Data_" + std::to_string(i) + "_1")),
            allVars(std::string("Data_" + std::to_string(i) + "_2"))
        };
        
        testTuple.setData(100 + i, 200 + i, 1, 0, 24, i % 2 == 0, 5000 + i, bitMap, data);
        table.addDataToBlock(testTuple, 1000, 100 + i, 200 + i, 300 + i, 400 + i, 500 + i, i % 2);
    }
    
    std::vector<uint8_t> result = table.marshallAllTable();
    
    EXPECT_FALSE(result.empty());
    EXPECT_GE(result.size(), 33); // Przynajmniej header
    
    delete header;
}

TEST(AllTableTest, MarshallConsistency) {
    tableHeader* header1 = createTestTableHeader();
    tableHeader* header2 = createTestTableHeader();
    allTable table1(1024, header1);
    allTable table2(1024, header2);
    
    std::vector<uint8_t> result1 = table1.marshallAllTable();
    std::vector<uint8_t> result2 = table2.marshallAllTable();
    
    EXPECT_FALSE(result1.empty());
    EXPECT_FALSE(result2.empty());
    EXPECT_GE(result1.size(), 33);
    EXPECT_GE(result2.size(), 33);
    
    delete header1;
    delete header2;
}

// ==================== TESTY UNMARSHALLING ====================

TEST(AllTableTest, UnmarshallEmptyVector) {
    tableHeader* header = createTestTableHeader();
    allTable table(1024, header);
    std::vector<uint8_t> empty;
    
    // Test że unmarshall pustego wektora jest obsługiwany
    // (assert może być wywołany, ale nie powinien crashować w release)
    EXPECT_DEATH(table.unmarshallAllTable(empty),"");
    
    delete header;
}

TEST(AllTableTest, UnmarshallTooSmallVector) {
    tableHeader* header = createTestTableHeader();
    allTable table(1024, header);
    std::vector<uint8_t> tooSmall(10, 0x00);
    
    // Test że unmarshall za małego wektora jest obsługiwany
    EXPECT_THROW(table.unmarshallAllTable(tooSmall), std::exception);
    
    delete header;
}

TEST(AllTableTest, UnmarshallMinimalValidSize) {
    tableHeader* header = createTestTableHeader();
    allTable table(1024, header);
    std::vector<uint8_t> minimal(33, 0x00);
    
    EXPECT_THROW(table.unmarshallAllTable(minimal), std::exception);
    
    delete header;
}

TEST(AllTableTest, UnmarshallCorrectSize) {
    tableHeader* header1 = createTestTableHeader();
    tableHeader* header2 = createTestTableHeader();
    allTable table1(1024, header1);
    allTable table2(1024, header2);
    
    // Dodaj dane do pierwszej tabeli
    tuple testTuple;
    std::vector<bool> bitMap = {true, false, true};
    std::vector<allVars> data = {allVars(42), allVars(std::string("test")), allVars(84)};
    testTuple.setData(100, 200, 1, 0, 24, true, 5000, bitMap, data);
    table1.addDataToBlock(testTuple, 2048, 100, 200, 300, 400, 500, 1);
    
    std::vector<uint8_t> marshalled = table1.marshallAllTable();
    EXPECT_FALSE(marshalled.empty());
    
    EXPECT_NO_THROW(table2.unmarshallAllTable(marshalled));
    
    delete header1;
    delete header2;
}

TEST(AllTableTest, UnmarshallWithBlockSizeData) {
    tableHeader* header = createTestTableHeader();
    allTable table(1024, header);
    
    // Utwórz dane o rozmiarze blockSize + header
    std::vector<uint8_t> data(33 + blockSize, 0x00);
    
    EXPECT_NO_THROW(table.unmarshallAllTable(data));
    
    delete header;
}

// ==================== TESTY ROUND-TRIP ====================

TEST(AllTableTest, RoundTripEmptyTable) {
    tableHeader* header1 = createTestTableHeader();
    tableHeader* header2 = createTestTableHeader();
    allTable table1(1024, header1);
    allTable table2(1024, header2);
    
    std::vector<uint8_t> marshalled = table1.marshallAllTable();
    EXPECT_FALSE(marshalled.empty());
    
    EXPECT_NO_THROW(table2.unmarshallAllTable(marshalled));
    
    // Test że można ponownie zmarshallować
    std::vector<uint8_t> remarshalled = table2.marshallAllTable();
    EXPECT_FALSE(remarshalled.empty());
    
    delete header1;
    delete header2;
}

TEST(AllTableTest, RoundTripWithData) {
    tableHeader* header1 = createTestTableHeader();
    tableHeader* header2 = createTestTableHeader();
    allTable table1(4096, header1);
    allTable table2(4096, header2);
    
    // Dodaj dane do pierwszej tabeli
    tuple testTuple;
    std::vector<bool> bitMap = {true, false, true};
    std::vector<allVars> data = {allVars(123), allVars(std::string("data")), allVars(789)};
    testTuple.setData(1000, 2000, 1, 0, 24, true, 6000, bitMap, data);
    
    table1.addDataToBlock(testTuple, 2048, 100, 200, 300, 400, 500, 1);
    
    std::vector<uint8_t> marshalled = table1.marshallAllTable();
    EXPECT_FALSE(marshalled.empty());
    
    EXPECT_NO_THROW(table2.unmarshallAllTable(marshalled));
    
    std::vector<uint8_t> remarshalled = table2.marshallAllTable();
    EXPECT_FALSE(remarshalled.empty());
    EXPECT_GE(remarshalled.size(), 33);
    
    delete header1;
    delete header2;
}

TEST(AllTableTest, RoundTripConsistency) {
    tableHeader* header1 = createTestTableHeader();
    tableHeader* header2 = createTestTableHeader();
    tableHeader* header3 = createTestTableHeader();
    allTable table1(2048, header1);
    allTable table2(2048, header2);
    allTable table3(2048, header3);
    
    // Dodaj kilka tupli
    for (int i = 0; i < 3; i++) {
        tuple testTuple;
        std::vector<bool> bitMap = {true, false, true};
        std::vector<allVars> data = {allVars(i * 100), allVars(std::string("item") + std::to_string(i)), allVars(i * 200)};
        testTuple.setData(100 + i, 200 + i, 1, 0, 24, i % 2 == 0, 5000 + i, bitMap, data);
        
        table1.addDataToBlock(testTuple, 1024, 100 + i, 200 + i, 300 + i, 400 + i, 500 + i, i % 2);
    }
    
    std::vector<uint8_t> marshalled1 = table1.marshallAllTable();
    
    table2.unmarshallAllTable(marshalled1);
    std::vector<uint8_t> marshalled2 = table2.marshallAllTable();
    
    table3.unmarshallAllTable(marshalled2);
    std::vector<uint8_t> marshalled3 = table3.marshallAllTable();
    
    EXPECT_FALSE(marshalled1.empty());
    EXPECT_FALSE(marshalled2.empty());
    EXPECT_FALSE(marshalled3.empty());
    
    EXPECT_GE(marshalled1.size(), 33);
    EXPECT_GE(marshalled2.size(), 33);
    EXPECT_GE(marshalled3.size(), 33);
    
    delete header1;
    delete header2;
    delete header3;
}

// ==================== TESTY METODY showData ====================

TEST(AllTableTest, ShowDataEmptyTable) {
    tableHeader* header = createTestTableHeader();
    allTable table(1024, header);
    
    EXPECT_NO_THROW(table.showData());
    
    delete header;
}

TEST(AllTableTest, ShowDataWithBlocks) {
    tableHeader* header = createTestTableHeader();
    allTable table(4096, header);
    
    table.addNewBlock(2048, 100, 200, 300, 400, 500, 1);
    
    EXPECT_NO_THROW(table.showData());
    
    delete header;
}

TEST(AllTableTest, ShowDataWithData) {
    tableHeader* header = createTestTableHeader();
    allTable table(4096, header);
    
    tuple testTuple;
    std::vector<bool> bitMap = {true, false, true};
    std::vector<allVars> data = {allVars(42), allVars(std::string("test")), allVars(84)};
    testTuple.setData(100, 200, 1, 0, 24, true, 5000, bitMap, data);
    
    table.addDataToBlock(testTuple, 2048, 100, 200, 300, 400, 500, 1);
    
    EXPECT_NO_THROW(table.showData());
    
    delete header;
}

TEST(AllTableTest, ShowDataWithNullHeader) {
    allTable table(1024, nullptr);
    
    // Test że showData z null header nie crashuje
    EXPECT_NO_THROW(table.showData());
}

// ==================== TESTY WYDAJNOŚCI ====================

TEST(AllTableTest, PerformanceMarshall) {
    const int iterations = 10;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        tableHeader* header = createTestTableHeader();
        allTable table(1024 + i, header);
        
        table.addNewBlock(512 + i, 100 + i, 200 + i, 300 + i, 400 + i, 500 + i, i % 2);
        
        std::vector<uint8_t> result = table.marshallAllTable();
        volatile size_t dummy = result.size();
        (void)dummy;
        
        delete header;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    EXPECT_LT(duration.count(), 100000); // Mniej niż 100ms
}
/*
TEST(AllTableTest, PerformanceUnmarshall) {
    const int iterations = 10;
    
    tableHeader* sourceHeader = createTestTableHeader();
    allTable sourceTable(1024, sourceHeader);
    sourceTable.addNewBlock(512, 100, 200, 300, 400, 500, 1);
    
    std::vector<uint8_t> testData = sourceTable.marshallAllTable();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        tableHeader* header = createTestTableHeader();
        allTable table(1024, header);
        table.unmarshallAllTable(testData);
        delete header;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    EXPECT_LT(duration.count(), 50000); // Mniej niż 50ms
    
    delete sourceHeader;
}
*/

// ==================== TESTY ZARZĄDZANIA PAMIĘCIĄ ====================

TEST(AllTableTest, DestructorTest) {
    for (int i = 0; i < 5; i++) {
        tableHeader* header = createTestTableHeader();
        allTable* table = new allTable(1024 + i * 100, header);
        
        table->addNewBlock(512 + i, 100 + i, 200 + i, 300 + i, 400 + i, 500 + i, i % 2);
        
        tuple testTuple;
        std::vector<bool> bitMap = {true, false, true};
        std::vector<allVars> data = {allVars(i), allVars(std::string("test")), allVars(i + 100)};
        testTuple.setData(i, i + 1, 1, 0, 24, true, i + 5, bitMap, data);
        
        table->addDataToBlock(testTuple, 512 + i, 100 + i, 200 + i, 300 + i, 400 + i, 500 + i, i % 2);
        
        std::vector<uint8_t> result = table->marshallAllTable();
        EXPECT_FALSE(result.empty());
        
        delete table; // Test destruktora
        delete header; // Header musi być usunięty osobno
    }
}

TEST(AllTableTest, VectorOfTables) {
    std::vector<tableHeader*> headers;
    std::vector<allTable*> tables;
    
    for (int i = 0; i < 3; i++) {
        headers.push_back(createTestTableHeader());
        tables.push_back(new allTable(1024 + i * 100, headers[i]));
        
        tables[i]->addNewBlock(512 + i, 100 + i, 200 + i, 300 + i, 400 + i, 500 + i, i % 2);
        
        std::vector<uint8_t> result = tables[i]->marshallAllTable();
        EXPECT_FALSE(result.empty());
    }
    
    // Cleanup
    for (int i = 0; i < 3; i++) {
        delete tables[i];
        delete headers[i];
    }
}

// ==================== TESTY KOMPLEKSOWYCH SCENARIUSZY ====================

TEST(AllTableTest, ComplexScenario) {
    tableHeader* header = createTestTableHeader();
    allTable table(4096, header);
    
    // Dodaj różne typy danych
    std::vector<std::string> testStrings = {"short", "medium length string", "very long string with many characters"};
    std::vector<int32_t> testInts = {0, -1000, 1000, std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::min()};
    
    for (size_t i = 0; i < testStrings.size(); i++) {
        tuple testTuple;
        std::vector<bool> bitMap = {true, false, true};
        std::vector<allVars> data = {
            allVars(testInts[i % testInts.size()]),
            allVars(testStrings[i]),
            allVars(static_cast<int64_t>(i * 1000))
        };
        
        testTuple.setData(i * 100, i * 200, 1, 0, 24, i % 2 == 0, i * 600, bitMap, data);
        table.addDataToBlock(testTuple, 2048, i * 10, i * 20, i * 30, i * 40, i * 50, i % 2);
    }
    
    std::vector<uint8_t> marshalled = table.marshallAllTable();
    EXPECT_FALSE(marshalled.empty());
    
    tableHeader* header2 = createTestTableHeader();
    allTable table2(4096, header2);
    EXPECT_NO_THROW(table2.unmarshallAllTable(marshalled));
    
    EXPECT_NO_THROW(table.showData());
    EXPECT_NO_THROW(table2.showData());
    
    delete header;
    delete header2;
}

TEST(AllTableTest, StressTestManyTuples) {
    tableHeader* header = createTestTableHeader();
    allTable table(6000, header);
    
    // Dodaj wiele tupli
    for (int i = 0; i < 20; i++) {
        tuple testTuple;
        std::vector<bool> bitMap = {true, false, true};
        std::vector<allVars> data;
        
        // 3 kolumny zgodnie z tableHeader
        data.push_back(allVars(i * 100));
        data.push_back(allVars(std::string("data_") + std::to_string(i)));
        data.push_back(allVars(static_cast<int64_t>(i * 1000)));
        
        testTuple.setData(i * 10, i * 20, 1, 0, 24, i % 2 == 0, i * 60, bitMap, data);
        table.addDataToBlock(testTuple, 1000 + i * 10, i, i + 1, i + 2, i + 3, i + 4, i % 2);
    }
    
    std::vector<uint8_t> marshalled = table.marshallAllTable();
    EXPECT_FALSE(marshalled.empty());
    
    tableHeader* header2 = createTestTableHeader();
    allTable table2(6000, header2);
    EXPECT_NO_THROW(table2.unmarshallAllTable(marshalled));
    
    delete header;
    delete header2;
}

// ==================== TESTY WARTOŚCI GRANICZNYCH ====================

TEST(AllTableTest, BoundaryValues) {
    std::vector<int32_t> testFreeSpaces = {0, 1, 1023, 1024, 1025, 4095, 4096, 8191, 8192};
    
    for (int32_t freeSpace : testFreeSpaces) {
        tableHeader* header = createTestTableHeader();
        allTable table(freeSpace, header);
        
        EXPECT_NO_THROW(table.addNewBlock(freeSpace / 2, 0, 0, 0, 0, 0, 0));
        
        std::vector<uint8_t> result = table.marshallAllTable();
        EXPECT_FALSE(result.empty());
        
        tableHeader* header2 = createTestTableHeader();
        allTable table2(freeSpace, header2);
        EXPECT_NO_THROW(table2.unmarshallAllTable(result));
        
        delete header;
        delete header2;
    }
}

// ==================== TEST INTEGRACYJNY ====================

TEST(AllTableTest, FullIntegrationTest) {
    tableHeader* originalHeader = createTestTableHeader();
    allTable originalTable(4096, originalHeader);
    
    // Przygotuj kompleksowe dane
    for (int i = 0; i < 5; i++) {
        tuple testTuple;
        std::vector<bool> bitMap = {true, false, true};
        std::vector<allVars> data = {
            allVars(int32_t(i * 1000)),
            allVars(std::string("Integration test data " + std::to_string(i))),
            allVars(int32_t(i * -100))
        };
        
        testTuple.setData(
            i * 1000,
            i * 2000,
            1,
            0,
            24,
            i % 2 == 0,
            i * 6000,
            bitMap,
            data
        );
        
        originalTable.addDataToBlock(testTuple, 2048, i * 10, i * 20, i * 30, i * 40, i * 50, i % 2);
    }
    
    // Test marshalling
    std::vector<uint8_t> marshalled = originalTable.marshallAllTable();
    EXPECT_FALSE(marshalled.empty());
    EXPECT_GE(marshalled.size(), 33);
    
    // Test unmarshalling
    tableHeader* reconstructedHeader = createTestTableHeader();
    allTable reconstructedTable(4096, reconstructedHeader);
    EXPECT_NO_THROW(reconstructedTable.unmarshallAllTable(marshalled));
    
    // Test że można ponownie zmarshallować
    std::vector<uint8_t> remarshalled = reconstructedTable.marshallAllTable();
    EXPECT_FALSE(remarshalled.empty());
    EXPECT_GE(remarshalled.size(), 33);
    
    // Test metod pomocniczych
    EXPECT_NO_THROW(originalTable.showData());
    EXPECT_NO_THROW(reconstructedTable.showData());
    
    delete originalHeader;
    delete reconstructedHeader;
}

// ==================== TEST PRZYKŁADU Z MAIN ====================

TEST(AllTableTest, ExampleFromMain) {
    // Test oparty na przykładzie z main.cpp
    tableHeader* th = new tableHeader(1, 0, 3, 1001, 0, 0, 7, 1024, {1, 2, 3}, {1, 2, 3}, {"col1", "col2", "col3"});
    allTable table(8192, th);
    
    tuple tupleObj;
    tupleObj.setData(100, 200, 1, 0, 24, true, 5000, {false, true, false}, {allVars(123), allVars("test"), allVars(43)});
    
    EXPECT_NO_THROW(table.addDataToBlock(tupleObj, 8192, 0, 0, 0, 0, 0, 0));
    
    std::vector<uint8_t> marshalled = table.marshallAllTable();
    EXPECT_FALSE(marshalled.empty());
    
    tableHeader* th2 = new tableHeader(1, 0, 3, 1001, 0, 0, 7, 1024, {1, 2, 3}, {1, 2, 3}, {"col1", "col2", "col3"});
    allTable table2(8192, th2);
    EXPECT_NO_THROW(table2.unmarshallAllTable(marshalled));
    
    EXPECT_NO_THROW(table.showData());
    EXPECT_NO_THROW(table2.showData());
    
    delete th;
    delete th2;
}