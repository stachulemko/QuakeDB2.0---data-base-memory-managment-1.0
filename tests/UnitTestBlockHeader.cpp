/*
#include <gtest/gtest.h>
#include "blockHeader.h"
#include <vector>
#include <limits>
#include <chrono>

TEST(BlockHeaderTest, DefaultConstructor) {
    blockHeader header;
    
    EXPECT_EQ(header.getNextblock(), 0);
    EXPECT_EQ(header.getBlockIndetification(), 0);
    EXPECT_EQ(header.getPdLsn(), 0);
    EXPECT_EQ(header.getPdChecksum(), 0);
    EXPECT_EQ(header.getPdFlags(), 0);
    EXPECT_EQ(header.getContainToast(), 0);
}

TEST(BlockHeaderTest, MarshallBasicValues) {
    blockHeader header;
    
    std::vector<uint8_t> result = header.marshallBlockHeader(
        100,    // nextblock
        200,    // blockIndetification
        300,    // pd_lsn
        400,    // pd_checksum
        500,    // pd_flags
        1       // contain_toast
    );
    
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 17);
}

TEST(BlockHeaderTest, MarshallZeroValues) {
    blockHeader header;
    
    std::vector<uint8_t> result = header.marshallBlockHeader(0, 0, 0, 0, 0, 0);
    
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 17);
}

TEST(BlockHeaderTest, MarshallConsistency) {
    blockHeader header1;
    blockHeader header2;
    
    std::vector<uint8_t> result1 = header1.marshallBlockHeader(42, 123, 456, 789, 101, 1);
    std::vector<uint8_t> result2 = header2.marshallBlockHeader(42, 123, 456, 789, 101, 1);
    
    EXPECT_EQ(result1, result2);
    EXPECT_EQ(result1.size(), 17);
}

TEST(BlockHeaderTest, MarshallDifferentValues) {
    blockHeader header;
    
    std::vector<uint8_t> result1 = header.marshallBlockHeader(100, 200, 300, 400, 500, 1);
    std::vector<uint8_t> result2 = header.marshallBlockHeader(999, 200, 300, 400, 500, 1);
    
    EXPECT_NE(result1, result2);
    EXPECT_EQ(result1.size(), result2.size());
}

TEST(BlockHeaderTest, MarshallMaxValues) {
    blockHeader header;
    
    std::vector<uint8_t> result = header.marshallBlockHeader(
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int16_t>::max(),
        std::numeric_limits<int16_t>::max(),
        std::numeric_limits<int8_t>::max()
    );
    
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 17);
}

TEST(BlockHeaderTest, MarshallMinValues) {
    blockHeader header;
    
    std::vector<uint8_t> result = header.marshallBlockHeader(
        std::numeric_limits<int32_t>::min(),
        std::numeric_limits<int32_t>::min(),
        std::numeric_limits<int32_t>::min(),
        std::numeric_limits<int16_t>::min(),
        std::numeric_limits<int16_t>::min(),
        std::numeric_limits<int8_t>::min()
    );
    
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 17);
}

TEST(BlockHeaderTest, MarshallNegativeValues) {
    blockHeader header;
    
    std::vector<uint8_t> result = header.marshallBlockHeader(-100, -200, -300, -400, -500, -1);
    
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 17);
}

TEST(BlockHeaderTest, UnmarshallEmptyVector) {
    blockHeader header;
    std::vector<uint8_t> empty;
    
    EXPECT_NO_THROW(header.unmarshallBlockHeader(empty));
    
    EXPECT_EQ(header.getNextblock(), 0);
    EXPECT_EQ(header.getBlockIndetification(), 0);
    EXPECT_EQ(header.getPdLsn(), 0);
}

TEST(BlockHeaderTest, UnmarshallTooSmall) {
    blockHeader header;
    std::vector<uint8_t> tooSmall(16, 0x42);
    
    EXPECT_NO_THROW(header.unmarshallBlockHeader(tooSmall));
    
    EXPECT_EQ(header.getNextblock(), 0);
    EXPECT_EQ(header.getBlockIndetification(), 0);
    EXPECT_EQ(header.getPdLsn(), 0);
}

TEST(BlockHeaderTest, UnmarshallMinimumSize) {
    blockHeader header1;
    blockHeader header2;
    
    std::vector<uint8_t> marshalled = header1.marshallBlockHeader(123, 456, 789, 100, 200, 1);
    std::vector<uint8_t> minimum(marshalled.begin(), marshalled.begin() + 17);
    
    EXPECT_NO_THROW(header2.unmarshallBlockHeader(minimum));
    
    EXPECT_EQ(header2.getNextblock(), 123);
    EXPECT_EQ(header2.getBlockIndetification(), 456);
    EXPECT_EQ(header2.getPdLsn(), 789);
    EXPECT_EQ(header2.getPdChecksum(), 100);
    EXPECT_EQ(header2.getPdFlags(), 200);
    EXPECT_EQ(header2.getContainToast(), 1);
}

TEST(BlockHeaderTest, UnmarshallExactSize) {
    blockHeader header1;
    blockHeader header2;
    
    std::vector<uint8_t> marshalled = header1.marshallBlockHeader(123, 456, 789, 100, 200, 1);
    
    EXPECT_NO_THROW(header2.unmarshallBlockHeader(marshalled));
    
    EXPECT_EQ(header2.getNextblock(), 123);
    EXPECT_EQ(header2.getBlockIndetification(), 456);
    EXPECT_EQ(header2.getPdLsn(), 789);
    EXPECT_EQ(header2.getPdChecksum(), 100);
    EXPECT_EQ(header2.getPdFlags(), 200);
    EXPECT_EQ(header2.getContainToast(), 1);
}

TEST(BlockHeaderTest, UnmarshallLargerSize) {
    blockHeader header1;
    blockHeader header2;
    
    std::vector<uint8_t> marshalled = header1.marshallBlockHeader(123, 456, 789, 100, 200, 1);
    marshalled.resize(30, 0xFF);
    
    EXPECT_NO_THROW(header2.unmarshallBlockHeader(marshalled));
    
    EXPECT_EQ(header2.getNextblock(), 123);
    EXPECT_EQ(header2.getBlockIndetification(), 456);
    EXPECT_EQ(header2.getPdLsn(), 789);
}

TEST(BlockHeaderTest, MarshallUnmarshallRoundTrip) {
    blockHeader header1;
    blockHeader header2;
    
    std::vector<uint8_t> marshalled = header1.marshallBlockHeader(
        12345,   // nextblock
        67890,   // blockIndetification
        11111,   // pd_lsn
        2222,    // pd_checksum
        3333,    // pd_flags
        7        // contain_toast
    );
    
    EXPECT_EQ(marshalled.size(), 17);
    
    header2.unmarshallBlockHeader(marshalled);
    
    EXPECT_EQ(header2.getNextblock(), 12345);
    EXPECT_EQ(header2.getBlockIndetification(), 67890);
    EXPECT_EQ(header2.getPdLsn(), 11111);
    EXPECT_EQ(header2.getPdChecksum(), 2222);
    EXPECT_EQ(header2.getPdFlags(), 3333);
    EXPECT_EQ(header2.getContainToast(), 7);
}

TEST(BlockHeaderTest, BoundaryDataSizes) {
    blockHeader header;
    
    std::vector<size_t> testSizes = {0, 1, 16, 17, 18, 25, 50, 100};
    
    for (size_t size : testSizes) {
        std::vector<uint8_t> testData(size, 0x33);
        
        EXPECT_NO_THROW(header.unmarshallBlockHeader(testData))
            << "Failed with size: " << size;
    }
}

TEST(BlockHeaderTest, ConsecutiveOperations) {
    blockHeader header1;
    blockHeader header2;
    
    for (int i = 0; i < 10; i++) {
        std::vector<uint8_t> marshalled = header1.marshallBlockHeader(i, i+10, i+20, i+30, i+40, i%2);
        header2.unmarshallBlockHeader(marshalled);
        
        EXPECT_EQ(header2.getNextblock(), i);
        EXPECT_EQ(header2.getBlockIndetification(), i+10);
        EXPECT_EQ(header2.getPdLsn(), i+20);
        EXPECT_EQ(header2.getPdChecksum(), i+30);
        EXPECT_EQ(header2.getPdFlags(), i+40);
        EXPECT_EQ(header2.getContainToast(), i%2);
    }
}

TEST(BlockHeaderTest, PerformanceMarshall) {
    const int iterations = 1000;
    blockHeader header;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        std::vector<uint8_t> result = header.marshallBlockHeader(i, i+1, i+2, i+3, i+4, i%2);
        volatile size_t dummy = result.size();
        (void)dummy;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    EXPECT_LT(duration.count(), 1000000);
}

TEST(BlockHeaderTest, PerformanceUnmarshall) {
    const int iterations = 1000;
    blockHeader header1;
    blockHeader header2;
    
    std::vector<uint8_t> data = header1.marshallBlockHeader(42, 123, 456, 789, 101, 1);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        header2.unmarshallBlockHeader(data);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    EXPECT_LT(duration.count(), 1000000);
}

TEST(BlockHeaderTest, DestructorTest) {
    for (int i = 0; i < 10; i++) {
        blockHeader* header1 = new blockHeader();
        blockHeader* header2 = new blockHeader();
        
        std::vector<uint8_t> marshalled = header1->marshallBlockHeader(i, i+1, i+2, i+3, i+4, i%2);
        header2->unmarshallBlockHeader(marshalled);
        
        EXPECT_EQ(header2->getNextblock(), i);
        
        delete header1;
        delete header2;
    }
}

TEST(BlockHeaderTest, MemoryConsistency) {
    blockHeader sourceHeader;
    std::vector<blockHeader> headers(10);
    
    std::vector<uint8_t> data = sourceHeader.marshallBlockHeader(42, 123, 456, 789, 101, 1);
    
    for (auto& header : headers) {
        header.unmarshallBlockHeader(data);
        
        EXPECT_EQ(header.getNextblock(), 42);
        EXPECT_EQ(header.getBlockIndetification(), 123);
        EXPECT_EQ(header.getPdLsn(), 456);
        EXPECT_EQ(header.getPdChecksum(), 789);
        EXPECT_EQ(header.getPdFlags(), 101);
        EXPECT_EQ(header.getContainToast(), 1);
    }
}

TEST(BlockHeaderTest, EdgeCaseValues) {
    blockHeader header1;
    blockHeader header2;
    blockHeader header3;
    blockHeader header4;
    
    std::vector<uint8_t> result1 = header1.marshallBlockHeader(-1, -1, -1, -1, -1, -1);
    std::vector<uint8_t> result2 = header2.marshallBlockHeader(0, 0, 0, 0, 0, 0);
    std::vector<uint8_t> result3 = header3.marshallBlockHeader(1, 1, 1, 1, 1, 1);
    
    EXPECT_NE(result1, result2);
    EXPECT_NE(result2, result3);
    EXPECT_NE(result1, result3);
    
    EXPECT_EQ(result1.size(), 17);
    EXPECT_EQ(result2.size(), 17);
    EXPECT_EQ(result3.size(), 17);
    
    header4.unmarshallBlockHeader(result1);
    EXPECT_EQ(header4.getNextblock(), -1);
    EXPECT_EQ(header4.getBlockIndetification(), -1);
    EXPECT_EQ(header4.getPdLsn(), -1);
    EXPECT_EQ(header4.getPdChecksum(), -1);
    EXPECT_EQ(header4.getPdFlags(), -1);
    EXPECT_EQ(header4.getContainToast(), -1);
    
    header4.unmarshallBlockHeader(result2);
    EXPECT_EQ(header4.getNextblock(), 0);
    EXPECT_EQ(header4.getBlockIndetification(), 0);
    EXPECT_EQ(header4.getPdLsn(), 0);
    
    header4.unmarshallBlockHeader(result3);
    EXPECT_EQ(header4.getNextblock(), 1);
    EXPECT_EQ(header4.getBlockIndetification(), 1);
    EXPECT_EQ(header4.getPdLsn(), 1);
}

TEST(BlockHeaderTest, DataIntegrityCheck) {
    blockHeader header1;
    blockHeader header2;
    
    std::vector<uint8_t> result = header1.marshallBlockHeader(
        0x12345678,  // nextblock
        0x9ABCDEF0,  // blockIndetification
        0x11223344,  // pd_lsn
        0x5566,      // pd_checksum
        0x7788,      // pd_flags
        0x99         // contain_toast
    );
    
    EXPECT_EQ(result.size(), 17);
    
    header2.unmarshallBlockHeader(result);
    
    EXPECT_EQ(header2.getNextblock(), 0x12345678);
    EXPECT_EQ(header2.getBlockIndetification(), 0x9ABCDEF0);
    EXPECT_EQ(header2.getPdLsn(), 0x11223344);
    EXPECT_EQ(header2.getPdChecksum(), 0x5566);
    EXPECT_EQ(header2.getPdFlags(), 0x7788);
    EXPECT_EQ(header2.getContainToast(), (int8_t)0x99);
}

TEST(BlockHeaderTest, MarshallInternalConsistency) {
    blockHeader header;
    
    std::vector<uint8_t> result1 = header.marshallBlockHeader(123, 456, 789, 100, 200, 1);
    std::vector<uint8_t> result2 = header.marshallBlockHeader(123, 456, 789, 100, 200, 1);
    std::vector<uint8_t> result3 = header.marshallBlockHeader(123, 456, 789, 100, 200, 1);
    
    EXPECT_EQ(result1, result2);
    EXPECT_EQ(result2, result3);
    EXPECT_EQ(result1, result3);
    
    EXPECT_EQ(result1.size(), 17);
    EXPECT_EQ(result2.size(), 17);
    EXPECT_EQ(result3.size(), 17);
}

TEST(BlockHeaderTest, MarshallSizeConsistency) {
    blockHeader header;
    
    std::vector<uint8_t> result1 = header.marshallBlockHeader(0, 0, 0, 0, 0, 0);
    std::vector<uint8_t> result2 = header.marshallBlockHeader(999999, 888888, 777777, 32767, 32767, 127);
    std::vector<uint8_t> result3 = header.marshallBlockHeader(-999999, -888888, -777777, -32768, -32768, -128);
    
    EXPECT_EQ(result1.size(), 17);
    EXPECT_EQ(result2.size(), 17);
    EXPECT_EQ(result3.size(), 17);
}
/*
*/


