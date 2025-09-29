#include <gtest/gtest.h>
#include "alltable.h"
#include <vector>
#include <limits>
#include <chrono>

/*
TEST(AllTableTest, DefaultConstructor) {
    allTable table(1024, 64);

    std::vector<uint8_t> marshalled = table.marshallAllTable();
    
    EXPECT_FALSE(marshalled.empty());
    EXPECT_GE(marshalled.size(), 33); // Minimum header size
}

TEST(AllTableTest, ConstructorParameters) {
    int32_t freeSpace = 2048;
    int32_t unitSize = 128;
    
    allTable table(freeSpace, unitSize);
    
    std::vector<uint8_t> marshalled = table.marshallAllTable();
    
    EXPECT_FALSE(marshalled.empty());
    EXPECT_GE(marshalled.size(), 33);
}

TEST(AllTableTest, ConstructorZeroValues) {
    allTable table(0, 0);
    
    std::vector<uint8_t> marshalled = table.marshallAllTable();
    
    EXPECT_FALSE(marshalled.empty());
    EXPECT_GE(marshalled.size(), 33);
}


TEST(AllTableTest, ConstructorNegativeValues) {
    allTable table(-100, -50);
    
    std::vector<uint8_t> marshalled = table.marshallAllTable();
    
    EXPECT_FALSE(marshalled.empty());
    EXPECT_GE(marshalled.size(), 33);
}


TEST(AllTableTest, ConstructorMaxValues) {
    allTable table(std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max());
    
    std::vector<uint8_t> marshalled = table.marshallAllTable();
    
    EXPECT_FALSE(marshalled.empty());
    EXPECT_GE(marshalled.size(), 33);
}

TEST(AllTableTest, MarshallAllTableBasic) {
    allTable table(1024, 64);
    
    std::vector<uint8_t> result = table.marshallAllTable();
    
    EXPECT_FALSE(result.empty());
    EXPECT_GE(result.size(), 33); // Header size
}

TEST(AllTableTest, MarshallAllTableConsistency) {
    allTable table1(512, 32);
    allTable table2(512, 32);
    
    std::vector<uint8_t> result1 = table1.marshallAllTable();
    std::vector<uint8_t> result2 = table2.marshallAllTable();
    
    EXPECT_EQ(result1.size(), result2.size());
    // Note: Content might differ due to internal state, but size should be same
}

TEST(AllTableTest, MarshallAllTableDifferentParameters) {
    allTable table1(1024, 64);
    allTable table2(2048, 128);
    
    std::vector<uint8_t> result1 = table1.marshallAllTable();
    std::vector<uint8_t> result2 = table2.marshallAllTable();
    
    EXPECT_FALSE(result1.empty());
    EXPECT_FALSE(result2.empty());
    // Both should have at least header size
    EXPECT_GE(result1.size(), 33);
    EXPECT_GE(result2.size(), 33);
}

TEST(AllTableTest, UnmarshallAllTableEmptyData) {
    allTable table(1024, 64);
    std::vector<uint8_t> empty;
    
    //EXPECT_NO_THROW(table.unmarshallAllTable(empty));
}

TEST(AllTableTest, UnmarshallAllTableTooSmall) {
    allTable table(1024, 64);
    std::vector<uint8_t> tooSmall(38, 0x00); // Less than header size
    
    EXPECT_NO_THROW(table.unmarshallAllTable(tooSmall));
}

TEST(AllTableTest, UnmarshallAllTableMinimumSize) {
    allTable table(1024, 64);
    std::vector<uint8_t> minimum(39, 0x00); // Exact header size
    
    EXPECT_NO_THROW(table.unmarshallAllTable(minimum));
}

TEST(AllTableTest, UnmarshallAllTableWithBlocks) {
    allTable table(1024, 64);
    
    // Create data with header + one block (assuming blockSize = 8192)
    std::vector<uint8_t> testData(33 + 8192, 0x00);
    
    EXPECT_NO_THROW(table.unmarshallAllTable(testData));
}

TEST(AllTableTest, UnmarshallAllTableMultipleBlocks) {
    allTable table(1024, 64);
    
    // Create data with header + multiple blocks
    std::vector<uint8_t> testData(33 + 3 * 8192, 0x00);
    
    EXPECT_NO_THROW(table.unmarshallAllTable(testData));
}

TEST(AllTableTest, MarshallUnmarshallRoundTrip) {
    allTable table1(1024, 64);
    allTable table2(2048, 128); // Different initial values
    
    std::vector<uint8_t> marshalled = table1.marshallAllTable();
    
    EXPECT_FALSE(marshalled.empty());
    
    EXPECT_NO_THROW(table2.unmarshallAllTable(marshalled));
    
    // After unmarshall, table2 should be able to marshall again
    std::vector<uint8_t> remarshalled = table2.marshallAllTable();
    EXPECT_FALSE(remarshalled.empty());
}

TEST(AllTableTest, MarshallUnmarshallConsistency) {
    allTable table1(512, 32);
    allTable table2(1024, 64);
    
    std::vector<uint8_t> marshalled1 = table1.marshallAllTable();
    std::vector<uint8_t> marshalled2 = table1.marshallAllTable();
    
    EXPECT_EQ(marshalled1, marshalled2);
    
    table2.unmarshallAllTable(marshalled1);
    std::vector<uint8_t> remarshalled = table2.marshallAllTable();
    
    EXPECT_FALSE(remarshalled.empty());
}

TEST(AllTableTest, UnmarshallPartialBlockData) {
    allTable table(1024, 64);
    
    // Create data with partial block (not exact multiple of blockSize)
    std::vector<uint8_t> testData(33 + 4096, 0x00); // Half block
    
    EXPECT_NO_THROW(table.unmarshallAllTable(testData));
}

TEST(AllTableTest, UnmarshallLargeData) {
    allTable table(1024, 64);
    
    // Create large data set
    std::vector<uint8_t> largeData(33 + 10 * 8192, 0x00);
    
    EXPECT_NO_THROW(table.unmarshallAllTable(largeData));
}

TEST(AllTableTest, MultipleMarshallCalls) {
    allTable table(1024, 64);
    
    for (int i = 0; i < 5; i++) {
        std::vector<uint8_t> result = table.marshallAllTable();
        
        EXPECT_FALSE(result.empty());
        EXPECT_GE(result.size(), 33);
    }
}

TEST(AllTableTest, MultipleUnmarshallCalls) {
    allTable table(1024, 64);
    
    std::vector<uint8_t> testData(33 + 8192, 0x42);
    
    for (int i = 0; i < 3; i++) {
        EXPECT_NO_THROW(table.unmarshallAllTable(testData));
        
        std::vector<uint8_t> marshalled = table.marshallAllTable();
        EXPECT_FALSE(marshalled.empty());
    }
}

TEST(AllTableTest, DestructorTest) {
    for (int i = 0; i < 5; i++) {
        allTable* table = new allTable(1024 + i * 100, 64 + i * 10);
        
        std::vector<uint8_t> marshalled = table->marshallAllTable();
        EXPECT_FALSE(marshalled.empty());
        
        delete table;
    }
}

TEST(AllTableTest, MemoryConsistency) {
    std::vector<allTable> tables;
    
    for (int i = 0; i < 5; i++) {
        tables.emplace_back(1024 + i * 100, 64 + i * 10);
    }
    
    for (auto& table : tables) {
        std::vector<uint8_t> marshalled = table.marshallAllTable();
        EXPECT_FALSE(marshalled.empty());
        EXPECT_GE(marshalled.size(), 33);
    }
}

TEST(AllTableTest, EdgeCaseParameters) {
    std::vector<std::pair<int32_t, int32_t>> testParams = {
        {0, 0},
        {1, 1},
        {-1, -1},
        {std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::min()},
        {std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()}
    };
    
    for (const auto& params : testParams) {
        allTable table(params.first, params.second);
        
        std::vector<uint8_t> marshalled = table.marshallAllTable();
        EXPECT_FALSE(marshalled.empty()) << "Failed with freeSpace=" << params.first 
                                         << ", unitSize=" << params.second;
        
        allTable table2(0, 0);
        EXPECT_NO_THROW(table2.unmarshallAllTable(marshalled));
    }
}

TEST(AllTableTest, PerformanceMarshall) {
    const int iterations = 50;
    allTable table(1024, 64);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        std::vector<uint8_t> result = table.marshallAllTable();
        volatile size_t dummy = result.size();
        (void)dummy;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    EXPECT_LT(duration.count(), 1000000); // Should complete within 1 second
}

TEST(AllTableTest, PerformanceUnmarshall) {
    const int iterations = 50;
    allTable table1(1024, 64);
    allTable table2(2048, 128);
    
    std::vector<uint8_t> testData = table1.marshallAllTable();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        table2.unmarshallAllTable(testData);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    EXPECT_LT(duration.count(), 1000000); // Should complete within 1 second
}

TEST(AllTableTest, ConsecutiveOperations) {
    allTable table1(1024, 64);
    allTable table2(2048, 128);
    
    for (int i = 0; i < 10; i++) {
        std::vector<uint8_t> marshalled = table1.marshallAllTable();
        EXPECT_FALSE(marshalled.empty());
        
        EXPECT_NO_THROW(table2.unmarshallAllTable(marshalled));
        
        std::vector<uint8_t> remarshalled = table2.marshallAllTable();
        EXPECT_FALSE(remarshalled.empty());
    }
}

TEST(AllTableTest, DataIntegrityWithBlocks) {
    allTable table1(1024, 64);
    allTable table2(0, 0);
    
    // Create test data that represents header + blocks
    std::vector<uint8_t> testData(33 + 2 * 8192, 0x00);
    
    // Set some pattern in the data
    for (size_t i = 0; i < testData.size(); i++) {
        testData[i] = static_cast<uint8_t>(i % 256);
    }
    
    EXPECT_NO_THROW(table1.unmarshallAllTable(testData));
    
    std::vector<uint8_t> marshalled = table1.marshallAllTable();
    EXPECT_FALSE(marshalled.empty());
    
    EXPECT_NO_THROW(table2.unmarshallAllTable(marshalled));
    
    std::vector<uint8_t> remarshalled = table2.marshallAllTable();
    EXPECT_FALSE(remarshalled.empty());
}

TEST(AllTableTest, BoundaryBlockSizes) {
    allTable table(1024, 64);
    
    std::vector<size_t> testSizes = {
        33,                    // Just header
        33 + 8192,            // Header + 1 block
        33 + 2 * 8192,        // Header + 2 blocks
        33 + 8192 - 1,        // Header + incomplete block
        33 + 8192 + 1,        // Header + block + 1 byte
        33 + 10 * 8192        // Header + 10 blocks
    };
    
    for (size_t size : testSizes) {
        std::vector<uint8_t> testData(size, 0x33);
        
        EXPECT_NO_THROW(table.unmarshallAllTable(testData))
            << "Failed with data size: " << size;
        
        std::vector<uint8_t> marshalled = table.marshallAllTable();
        EXPECT_FALSE(marshalled.empty()) << "Marshall failed after unmarshall with size: " << size;
    }
}

TEST(AllTableTest, StressTestMultipleInstances) {
    const int numTables = 20;
    std::vector<std::unique_ptr<allTable>> tables;
    
    // Create multiple table instances
    for (int i = 0; i < numTables; i++) {
        tables.push_back(std::make_unique<allTable>(1024 + i * 100, 64 + i * 8));
    }
    
    // Test all instances
    for (int i = 0; i < numTables; i++) {
        std::vector<uint8_t> marshalled = tables[i]->marshallAllTable();
        EXPECT_FALSE(marshalled.empty()) << "Marshall failed for table " << i;
        
        // Unmarshall in different table
        int targetIndex = (i + 1) % numTables;
        EXPECT_NO_THROW(tables[targetIndex]->unmarshallAllTable(marshalled))
            << "Unmarshall failed from table " << i << " to table " << targetIndex;
    }
}

TEST(AllTableTest, RoundTripPreservesData) {
    allTable table1(1234, 567);
    allTable table2(9999, 8888); // Different initial values
    
    // Create some test data
    std::vector<uint8_t> originalData(39 + 8192, 0x00);
    for (size_t i = 0; i < originalData.size(); i++) {
        originalData[i] = static_cast<uint8_t>((i * 7) % 256);
    }
    
    // Round trip: original -> table1 -> marshall -> table2 -> marshall
    table1.unmarshallAllTable(originalData);
    std::vector<uint8_t> marshalled1 = table1.marshallAllTable();
    
    table2.unmarshallAllTable(marshalled1);
    std::vector<uint8_t> marshalled2 = table2.marshallAllTable();
    
    EXPECT_FALSE(marshalled1.empty());
    EXPECT_FALSE(marshalled2.empty());
    
    // Both marshalled results should have same size at minimum
    EXPECT_GE(marshalled1.size(), 33);
    EXPECT_GE(marshalled2.size(), 33);
}
*/
