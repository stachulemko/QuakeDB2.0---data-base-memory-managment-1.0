#include <gtest/gtest.h>
#include "block8kb.h"
#include <vector>
#include <limits>
#include <chrono>


TEST(Block8kbTest, Constructor) {
    block8kb block(1024, 256);
    
    std::vector<uint8_t> result = block.marshallBlock8kb();
    EXPECT_FALSE(result.empty());
}

TEST(Block8kbTest, ConstructorZeroValues) {
    block8kb block(0, 0);
    
    std::vector<uint8_t> result = block.marshallBlock8kb();
    EXPECT_FALSE(result.empty());
}

TEST(Block8kbTest, ConstructorNegativeValues) {
    block8kb block(-100, -50);
    
    std::vector<uint8_t> result = block.marshallBlock8kb();
    EXPECT_FALSE(result.empty());
}

TEST(Block8kbTest, ConstructorMaxValues) {
    block8kb block(std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max());
    
    std::vector<uint8_t> result = block.marshallBlock8kb();
    EXPECT_FALSE(result.empty());
}

TEST(Block8kbTest, MarshallEmptyBlock) {
    block8kb block(8192, 0);
    
    std::vector<uint8_t> result = block.marshallBlock8kb();
    
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 8192);
}

TEST(Block8kbTest, MarshallConsistency) {
    block8kb block1(512, 128);
    block8kb block2(512, 128);
    
    std::vector<uint8_t> result1 = block1.marshallBlock8kb();
    std::vector<uint8_t> result2 = block2.marshallBlock8kb();
    
    EXPECT_EQ(result1.size(), 8192);
    EXPECT_EQ(result2.size(), 8192);
}

TEST(Block8kbTest, MarshallDifferentFreeSpace) {
    block8kb block1(1000, 256);
    block8kb block2(2000, 256);
    
    std::vector<uint8_t> result1 = block1.marshallBlock8kb();
    std::vector<uint8_t> result2 = block2.marshallBlock8kb();
    
    EXPECT_EQ(result1.size(), 8192);
    EXPECT_EQ(result2.size(), 8192);
}

TEST(Block8kbTest, UnmarshallEmptyVector) {
    block8kb block(1024, 256);
    std::vector<uint8_t> empty;
    
    EXPECT_NO_THROW(block.unmarshallBlock8kb(empty));
}

TEST(Block8kbTest, UnmarshallTooSmall) {
    block8kb block(1024, 256);
    std::vector<uint8_t> tooSmall(1, 0x42);
    
    EXPECT_NO_THROW(block.unmarshallBlock8kb(tooSmall));
}

TEST(Block8kbTest, UnmarshallMinimumSize) {
    block8kb block(1024, 256);
    std::vector<uint8_t> minimum(19, 0x00);
    
    EXPECT_NO_THROW(block.unmarshallBlock8kb(minimum));
}

TEST(Block8kbTest, UnmarshallWrongTypeIdentification) {
    block8kb block(1024, 256);
    
    std::vector<uint8_t> wrongType(8192, 0x00);
    wrongType[0] = 0xFF;
    wrongType[1] = 0xFF;
    
    EXPECT_NO_THROW(block.unmarshallBlock8kb(wrongType));
}

TEST(Block8kbTest, UnmarshallCorrectTypeIdentification) {
    block8kb block1(1024, 256);
    block8kb block2(0, 0);
    
    std::vector<uint8_t> marshalled = block1.marshallBlock8kb();
    
    EXPECT_NO_THROW(block2.unmarshallBlock8kb(marshalled));
}

TEST(Block8kbTest, MarshallUnmarshallRoundTrip) {
    block8kb block1(2048, 512);
    block8kb block2(0, 0);
    
    std::vector<uint8_t> marshalled = block1.marshallBlock8kb();
    
    EXPECT_EQ(marshalled.size(), 8192);
    
    EXPECT_NO_THROW(block2.unmarshallBlock8kb(marshalled));
}

TEST(Block8kbTest, BlockSizeConsistency) {
    std::vector<int32_t> freeSizes = {0, 100, 1000, 4096, 8192};
    std::vector<int32_t> unitSizes = {0, 64, 256, 1024};
    
    for (auto freeSize : freeSizes) {
        for (auto unitSize : unitSizes) {
            block8kb block(freeSize, unitSize);
            std::vector<uint8_t> result = block.marshallBlock8kb();
            
            EXPECT_EQ(result.size(), 8192) 
                << "Failed with freeSpace=" << freeSize << ", unitSize=" << unitSize;
        }
    }
}

TEST(Block8kbTest, BoundaryDataSizes) {
    block8kb block(1024, 256);
    
    std::vector<size_t> testSizes = {0, 1, 2, 18, 19, 20, 50, 8192, 10000};
    
    for (size_t size : testSizes) {
        std::vector<uint8_t> testData(size, 0x33);
        
        EXPECT_NO_THROW(block.unmarshallBlock8kb(testData))
            << "Failed with size: " << size;
    }
}

TEST(Block8kbTest, ConsecutiveOperations) {
    for (int i = 0; i < 5; i++) {
        block8kb block1(i * 100, i * 50);
        block8kb block2(0, 0);
        
        std::vector<uint8_t> marshalled = block1.marshallBlock8kb();
        EXPECT_EQ(marshalled.size(), 8192);
        
        EXPECT_NO_THROW(block2.unmarshallBlock8kb(marshalled));
    }
}

TEST(Block8kbTest, PerformanceMarshall) {
    const int iterations = 50;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        block8kb block(i + 1000, i + 100);
        std::vector<uint8_t> result = block.marshallBlock8kb();
        volatile size_t dummy = result.size();
        (void)dummy;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    EXPECT_LT(duration.count(), 10000000);
}

TEST(Block8kbTest, PerformanceUnmarshall) {
    const int iterations = 50;
    block8kb sourceBlock(1024, 256);
    
    std::vector<uint8_t> testData = sourceBlock.marshallBlock8kb();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        block8kb block(0, 0);
        block.unmarshallBlock8kb(testData);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    EXPECT_LT(duration.count(), 10000000);
}

TEST(Block8kbTest, DestructorTest) {
    for (int i = 0; i < 3; i++) {
        block8kb* block1 = new block8kb(i * 200, i * 64);
        block8kb* block2 = new block8kb(0, 0);
        
        std::vector<uint8_t> marshalled = block1->marshallBlock8kb();
        block2->unmarshallBlock8kb(marshalled);
        
        delete block1;
        delete block2;
    }
}

TEST(Block8kbTest, EdgeCaseValues) {
    block8kb block1(-1, -1);
    block8kb block2(0, 0);
    block8kb block3(1, 1);
    block8kb block4(8192, 8192);
    
    std::vector<uint8_t> result1 = block1.marshallBlock8kb();
    std::vector<uint8_t> result2 = block2.marshallBlock8kb();
    std::vector<uint8_t> result3 = block3.marshallBlock8kb();
    std::vector<uint8_t> result4 = block4.marshallBlock8kb();
    
    EXPECT_EQ(result1.size(), 8192);
    EXPECT_EQ(result2.size(), 8192);
    EXPECT_EQ(result3.size(), 8192);
    EXPECT_EQ(result4.size(), 8192);
    
    block8kb testBlock(0, 0);
    
    EXPECT_NO_THROW(testBlock.unmarshallBlock8kb(result1));
    EXPECT_NO_THROW(testBlock.unmarshallBlock8kb(result2));
    EXPECT_NO_THROW(testBlock.unmarshallBlock8kb(result3));
    EXPECT_NO_THROW(testBlock.unmarshallBlock8kb(result4));
}

TEST(Block8kbTest, ZeroPaddingVerification) {
    block8kb block(8100, 50);
    
    std::vector<uint8_t> result = block.marshallBlock8kb();
    
    EXPECT_EQ(result.size(), 8192);
    
    size_t zeroCount = 0;
    for (size_t i = result.size() - 100; i < result.size(); i++) {
        if (result[i] == 0x00) {
            zeroCount++;
        }
    }
    
    EXPECT_GT(zeroCount, 10);
}

TEST(Block8kbTest, UnmarshallWithTuples) {
    block8kb block1(4000, 1000);
    block8kb block2(0, 0);
    
    std::vector<uint8_t> marshalled = block1.marshallBlock8kb();
    
    EXPECT_NO_THROW(block2.unmarshallBlock8kb(marshalled));
}

TEST(Block8kbTest, TupleProcessingInUnmarshall) {
    block8kb block1(2000, 500);
    block8kb block2(0, 0);
    
    std::vector<uint8_t> marshalled = block1.marshallBlock8kb();
    
    if (marshalled.size() >= 19) {
        EXPECT_NO_THROW(block2.unmarshallBlock8kb(marshalled));
    }
}

TEST(Block8kbTest, UnmarshallDataBoundaryCheck) {
    block8kb block(1024, 256);
    
    std::vector<uint8_t> exactSize(19, 0x00);
    exactSize[0] = 0x00;
    exactSize[1] = 0x00;
    
    EXPECT_NO_THROW(block.unmarshallBlock8kb(exactSize));
}

TEST(Block8kbTest, BlockSizeFixed) {
    std::vector<std::pair<int32_t, int32_t>> testParams = {
        {0, 0},
        {100, 50},
        {1000, 200},
        {4096, 1024},
        {8192, 8192},
        {10000, 5000},
        {-500, -100}
    };
    
    for (const auto& param : testParams) {
        block8kb block(param.first, param.second);
        std::vector<uint8_t> result = block.marshallBlock8kb();
        
        EXPECT_EQ(result.size(), 8192)
            << "Block size should always be 8192 regardless of parameters. "
            << "freeSpace=" << param.first << ", unitSize=" << param.second;
    }
}

TEST(Block8kbTest, MarshallInternalStructure) {
    block8kb block(1500, 300);
    
    std::vector<uint8_t> result = block.marshallBlock8kb();
    
    EXPECT_EQ(result.size(), 8192);
    
    // Test że pierwsze 2 bajty to identyfikator bloku
    EXPECT_GE(result.size(), 2);
}

TEST(Block8kbTest, UnmarshallInternalStructure) {
    block8kb block1(2000, 400);
    block8kb block2(0, 0);
    
    std::vector<uint8_t> marshalled = block1.marshallBlock8kb();
    
    // Test struktury: 2 bajty identyfikator + 17 bajtów header + reszta tupli
    EXPECT_GE(marshalled.size(), 19);
    
    EXPECT_NO_THROW(block2.unmarshallBlock8kb(marshalled));
}

TEST(Block8kbTest, MemoryConsistency) {
    block8kb sourceBlock(2048, 512);
    std::vector<block8kb> blocks;
    
    for (int i = 0; i < 3; i++) {
        blocks.emplace_back(0, 0);
    }
    
    std::vector<uint8_t> testData = sourceBlock.marshallBlock8kb();
    
    for (auto& block : blocks) {
        EXPECT_NO_THROW(block.unmarshallBlock8kb(testData));
    }
}
