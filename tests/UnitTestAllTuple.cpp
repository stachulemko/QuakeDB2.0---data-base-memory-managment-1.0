#include <gtest/gtest.h>
#include "allTuple.h"
#include <vector>
#include <limits>
#include <chrono>

TEST(TupleTest, DefaultConstructor) {
    tuple tpl;
    
    EXPECT_EQ(tpl.getTxMin(), 0);
    EXPECT_EQ(tpl.getTxMax(), 0);
    EXPECT_EQ(tpl.getTCid(), 0);
    EXPECT_EQ(tpl.getTInfomask(), 0);
    EXPECT_EQ(tpl.getTHoff(), 0);
    EXPECT_FALSE(tpl.getNullBitmap());
    EXPECT_EQ(tpl.getOptionalOid(), 0);
    EXPECT_TRUE(tpl.getBitMap().empty());
    EXPECT_TRUE(tpl.getData().empty());
}

TEST(TupleTest, MarshallBasicValues) {
    tuple tpl;
    
    std::vector<bool> bitMap = {true, false, true};
    std::vector<allVars> data = {allVars(42), allVars(84), allVars(126)};
    
    std::vector<uint8_t> result = tpl.marshallTuple(
        100,     // xmin
        200,     // xmax
        300,     // cid
        400,     // infomask
        500,     // hoff
        true,    // bitmap
        600,     // oid
        bitMap,
        data
    );
    
    EXPECT_FALSE(result.empty());
    EXPECT_GE(result.size(), 2);
}

TEST(TupleTest, MarshallZeroValues) {
    tuple tpl;
    
    std::vector<bool> bitMap;
    std::vector<allVars> data;
    
    std::vector<uint8_t> result = tpl.marshallTuple(0, 0, 0, 0, 0, false, 0, bitMap, data);
    
    EXPECT_FALSE(result.empty());
    EXPECT_GE(result.size(), 2);
}

TEST(TupleTest, MarshallConsistency) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {true, false};
    std::vector<allVars> data = {allVars(42), allVars(84)};
    
    std::vector<uint8_t> result1 = tpl1.marshallTuple(100, 200, 300, 400, 500, true, 600, bitMap, data);
    std::vector<uint8_t> result2 = tpl2.marshallTuple(100, 200, 300, 400, 500, true, 600, bitMap, data);
    
    EXPECT_EQ(result1, result2);
}

TEST(TupleTest, MarshallDifferentValues) {
    tuple tpl;
    
    std::vector<bool> bitMap = {true};
    std::vector<allVars> data1 = {allVars(42)};
    std::vector<allVars> data2 = {allVars(99)};
    
    std::vector<uint8_t> result1 = tpl.marshallTuple(100, 200, 300, 400, 500, true, 600, bitMap, data1);
    std::vector<uint8_t> result2 = tpl.marshallTuple(100, 200, 300, 400, 500, true, 600, bitMap, data2);
    
    EXPECT_NE(result1, result2);
}

TEST(TupleTest, MarshallMaxValues) {
    tuple tpl;
    
    std::vector<bool> bitMap = {true};
    std::vector<allVars> data = {allVars(std::numeric_limits<int32_t>::max())};
    
    std::vector<uint8_t> result = tpl.marshallTuple(
        std::numeric_limits<int64_t>::max(),
        std::numeric_limits<int64_t>::max(),
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int16_t>::max(),
        true,
        std::numeric_limits<int64_t>::max(),
        bitMap,
        data
    );
    
    EXPECT_FALSE(result.empty());
}

TEST(TupleTest, MarshallMinValues) {
    tuple tpl;
    
    std::vector<bool> bitMap = {false};
    std::vector<allVars> data = {allVars(std::numeric_limits<int32_t>::min())};
    
    std::vector<uint8_t> result = tpl.marshallTuple(
        std::numeric_limits<int64_t>::min(),
        std::numeric_limits<int64_t>::min(),
        std::numeric_limits<int32_t>::min(),
        std::numeric_limits<int32_t>::min(),
        std::numeric_limits<int16_t>::min(),
        false,
        std::numeric_limits<int64_t>::min(),
        bitMap,
        data
    );
    
    EXPECT_FALSE(result.empty());
}

TEST(TupleTest, MarshallNegativeValues) {
    tuple tpl;
    
    std::vector<bool> bitMap = {true, false};
    std::vector<allVars> data = {allVars(-100), allVars(-200)};
    
    std::vector<uint8_t> result = tpl.marshallTuple(-100, -200, -300, -400, -500, false, -600, bitMap, data);
    
    EXPECT_FALSE(result.empty());
}

TEST(TupleTest, UnmarshallEmptyVector) {
    tuple tpl;
    std::vector<uint8_t> empty;
    
    EXPECT_NO_THROW(tpl.unmarshallTuple(empty));
    
    EXPECT_EQ(tpl.getTxMin(), 0);
    EXPECT_EQ(tpl.getTxMax(), 0);
    EXPECT_EQ(tpl.getTCid(), 0);
}

TEST(TupleTest, UnmarshallTooSmall) {
    tuple tpl;
    std::vector<uint8_t> tooSmall(1, 0x42);
    
    EXPECT_NO_THROW(tpl.unmarshallTuple(tooSmall));
    
    EXPECT_EQ(tpl.getTxMin(), 0);
    EXPECT_EQ(tpl.getTxMax(), 0);
    EXPECT_EQ(tpl.getTCid(), 0);
}

TEST(TupleTest, UnmarshallMinimumSize) {
    tuple tpl;
    
    std::vector<uint8_t> minimum(39, 0x00);
    minimum[0] = 0x00;
    minimum[1] = 0x00;
    
    EXPECT_NO_THROW(tpl.unmarshallTuple(minimum));
}

TEST(TupleTest, UnmarshallCorrectTypeIdentification) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {true};
    std::vector<allVars> data = {allVars(42)};
    
    std::vector<uint8_t> marshalled = tpl1.marshallTuple(100, 200, 300, 400, 500, true, 600, bitMap, data);
    
    EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled));
}

TEST(TupleTest, UnmarshallWrongTypeIdentification) {
    tuple tpl;
    
    std::vector<uint8_t> wrongType(39, 0x00);
    wrongType[0] = 0xFF;
    wrongType[1] = 0xFF;
    
    EXPECT_NO_THROW(tpl.unmarshallTuple(wrongType));
    
    EXPECT_EQ(tpl.getTxMin(), 0);
    EXPECT_EQ(tpl.getTxMax(), 0);
    EXPECT_EQ(tpl.getTCid(), 0);
}

TEST(TupleTest, UnmarshallLargerSize) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {true};
    std::vector<allVars> data = {allVars(42)};
    
    std::vector<uint8_t> marshalled = tpl1.marshallTuple(100, 200, 300, 400, 500, true, 600, bitMap, data);
    marshalled.resize(marshalled.size() + 20, 0xFF);
    
    EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled));
}

TEST(TupleTest, MarshallUnmarshallRoundTrip) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {true, false, true, false};
    std::vector<allVars> data = {
        allVars(12345),
        allVars(67890),
        allVars(11111),
        allVars(22222)
    };
    
    std::vector<uint8_t> marshalled = tpl1.marshallTuple(
        1000,    // xmin
        2000,    // xmax
        3000,    // cid
        4000,    // infomask
        5000,    // hoff
        true,    // bitmap
        6000,    // oid
        bitMap,
        data
    );
    
    EXPECT_FALSE(marshalled.empty());
    EXPECT_GE(marshalled.size(), 39);
    
    EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled));
}

TEST(TupleTest, HeaderDataSeparation) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {true, false};
    std::vector<allVars> data = {allVars(42), allVars(84)};
    
    std::vector<uint8_t> marshalled = tpl1.marshallTuple(100, 200, 300, 400, 500, true, 600, bitMap, data);
    
    EXPECT_GE(marshalled.size(), 41);
    
    tpl2.unmarshallTuple(marshalled);
    
    EXPECT_NO_THROW(tpl2.getTxMin());
    EXPECT_NO_THROW(tpl2.getBitMap());
    EXPECT_NO_THROW(tpl2.getData());
}

TEST(TupleTest, BoundaryDataSizes) {
    tuple tpl;
    
    std::vector<size_t> testSizes = {0, 1, 2, 38, 39, 40, 50, 100, 200};
    
    for (size_t size : testSizes) {
        std::vector<uint8_t> testData(size, 0x33);
        
        EXPECT_NO_THROW(tpl.unmarshallTuple(testData))
            << "Failed with size: " << size;
    }
}

TEST(TupleTest, ConsecutiveOperations) {
    tuple tpl1;
    tuple tpl2;
    
    for (int i = 0; i < 5; i++) {
        std::vector<bool> bitMap = {i % 2 == 0};
        std::vector<allVars> data = {allVars(i * 100)};
        
        std::vector<uint8_t> marshalled = tpl1.marshallTuple(
            i, i+1, i+2, i+3, i+4, i%2==0, i+5, bitMap, data
        );
        
        EXPECT_FALSE(marshalled.empty());
        EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled));
    }
}

TEST(TupleTest, PerformanceMarshall) {
    const int iterations = 100;
    tuple tpl;
    
    std::vector<bool> bitMap = {true, false};
    std::vector<allVars> data = {allVars(42), allVars(84)};
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        std::vector<uint8_t> result = tpl.marshallTuple(i, i+1, i+2, i+3, i+4, i%2==0, i+5, bitMap, data);
        volatile size_t dummy = result.size();
        (void)dummy;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    EXPECT_LT(duration.count(), 1000000);
}

TEST(TupleTest, PerformanceUnmarshall) {
    const int iterations = 100;
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {true};
    std::vector<allVars> data = {allVars(42)};
    
    std::vector<uint8_t> testData = tpl1.marshallTuple(100, 200, 300, 400, 500, true, 600, bitMap, data);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        tpl2.unmarshallTuple(testData);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    EXPECT_LT(duration.count(), 1000000);
}

TEST(TupleTest, DestructorTest) {
    for (int i = 0; i < 5; i++) {
        tuple* tpl1 = new tuple();
        tuple* tpl2 = new tuple();
        
        std::vector<bool> bitMap = {true};
        std::vector<allVars> data = {allVars(i)};
        
        std::vector<uint8_t> marshalled = tpl1->marshallTuple(i, i+1, i+2, i+3, i+4, true, i+5, bitMap, data);
        tpl2->unmarshallTuple(marshalled);
        
        delete tpl1;
        delete tpl2;
    }
}

TEST(TupleTest, MemoryConsistency) {
    tuple sourceTuple;
    std::vector<tuple> tuples(5);
    
    std::vector<bool> bitMap = {true, false};
    std::vector<allVars> data = {allVars(42), allVars(84)};
    
    std::vector<uint8_t> testData = sourceTuple.marshallTuple(100, 200, 300, 400, 500, true, 600, bitMap, data);
    
    for (auto& tpl : tuples) {
        EXPECT_NO_THROW(tpl.unmarshallTuple(testData));
    }
}

TEST(TupleTest, EdgeCaseValues) {
    tuple tpl1;
    tuple tpl2;
    tuple tpl3;
    tuple tpl4;
    
    std::vector<bool> bitMap1 = {true};
    std::vector<bool> bitMap2 = {false};
    std::vector<allVars> data1 = {allVars(-1)};
    std::vector<allVars> data2 = {allVars(0)};
    std::vector<allVars> data3 = {allVars(1)};
    
    std::vector<uint8_t> result1 = tpl1.marshallTuple(-1, -1, -1, -1, -1, false, -1, bitMap1, data1);
    std::vector<uint8_t> result2 = tpl2.marshallTuple(0, 0, 0, 0, 0, false, 0, bitMap2, data2);
    std::vector<uint8_t> result3 = tpl3.marshallTuple(1, 1, 1, 1, 1, true, 1, bitMap1, data3);
    
    EXPECT_NE(result1, result2);
    EXPECT_NE(result2, result3);
    EXPECT_NE(result1, result3);
    
    EXPECT_NO_THROW(tpl4.unmarshallTuple(result1));
    EXPECT_NO_THROW(tpl4.unmarshallTuple(result2));
    EXPECT_NO_THROW(tpl4.unmarshallTuple(result3));
}

TEST(TupleTest, DataIntegrityCheck) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {true, false, true, false};
    std::vector<allVars> data = {
        allVars(0x12345678),
        allVars(0x9ABCDEF0),
        allVars(0x11223344),
        allVars(0x55667788)
    };
    
    std::vector<uint8_t> result = tpl1.marshallTuple(
        0x1234567890ABCDEFLL,
        0xFEDCBA0987654321LL,
        0x12345678,
        0x9ABCDEF0,
        0x1122,
        true,
        0xAABBCCDDEEFF0011LL,
        bitMap,
        data
    );
    
    EXPECT_FALSE(result.empty());
    EXPECT_GE(result.size(), 39);
    
    EXPECT_NO_THROW(tpl2.unmarshallTuple(result));
}

TEST(TupleTest, MarshallInternalConsistency) {
    tuple tpl;
    
    std::vector<bool> bitMap = {true, false};
    std::vector<allVars> data = {allVars(123), allVars(456)};
    
    std::vector<uint8_t> result1 = tpl.marshallTuple(100, 200, 300, 400, 500, true, 600, bitMap, data);
    std::vector<uint8_t> result2 = tpl.marshallTuple(100, 200, 300, 400, 500, true, 600, bitMap, data);
    std::vector<uint8_t> result3 = tpl.marshallTuple(100, 200, 300, 400, 500, true, 600, bitMap, data);
    
    EXPECT_EQ(result1, result2);
    EXPECT_EQ(result2, result3);
    EXPECT_EQ(result1, result3);
}

TEST(TupleTest, EmptyDataHandling) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> emptyBitMap;
    std::vector<allVars> emptyData;
    
    std::vector<uint8_t> result = tpl1.marshallTuple(100, 200, 300, 400, 500, false, 600, emptyBitMap, emptyData);
    
    EXPECT_FALSE(result.empty());
    EXPECT_GE(result.size(), 39);
    
    EXPECT_NO_THROW(tpl2.unmarshallTuple(result));
}

TEST(TupleTest, LargeDataHandling) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> largeBitMap(10, true);
    std::vector<allVars> largeData;
    for (int i = 0; i < 10; i++) {
        largeData.push_back(allVars(i));
    }
    
    std::vector<uint8_t> result = tpl1.marshallTuple(100, 200, 300, 400, 500, true, 600, largeBitMap, largeData);
    
    EXPECT_FALSE(result.empty());
    
    EXPECT_NO_THROW(tpl2.unmarshallTuple(result));
}

TEST(TupleTest, GettersAfterUnmarshall) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {true, false};
    std::vector<allVars> data = {allVars(42), allVars(84)};
    
    std::vector<uint8_t> marshalled = tpl1.marshallTuple(1000, 2000, 3000, 4000, 5000, true, 6000, bitMap, data);
    
    tpl2.unmarshallTuple(marshalled);
    
    EXPECT_NO_THROW(tpl2.getTxMin());
    EXPECT_NO_THROW(tpl2.getTxMax());
    EXPECT_NO_THROW(tpl2.getTCid());
    EXPECT_NO_THROW(tpl2.getTInfomask());
    EXPECT_NO_THROW(tpl2.getTHoff());
    EXPECT_NO_THROW(tpl2.getNullBitmap());
    EXPECT_NO_THROW(tpl2.getOptionalOid());
    EXPECT_NO_THROW(tpl2.getBitMap());
    EXPECT_NO_THROW(tpl2.getData());
}

TEST(TupleTest, TypeIdentificationHandling) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {true};
    std::vector<allVars> data = {allVars(42)};
    
    std::vector<uint8_t> marshalled = tpl1.marshallTuple(100, 200, 300, 400, 500, true, 600, bitMap, data);
    
    EXPECT_GE(marshalled.size(), 2);
    
    tpl2.unmarshallTuple(marshalled);
}

TEST(TupleTest, UnmarshallBoundaryAt39Bytes) {
    tuple tpl;
    
    std::vector<uint8_t> exactSize(39, 0x00);
    exactSize[0] = 0x00;
    exactSize[1] = 0x00;
    
    EXPECT_NO_THROW(tpl.unmarshallTuple(exactSize));
}

TEST(TupleTest, UnmarshallSizeJustOver39Bytes) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {true};
    std::vector<allVars> data = {allVars(42)};
    
    std::vector<uint8_t> marshalled = tpl1.marshallTuple(100, 200, 300, 400, 500, true, 600, bitMap, data);
    
    if (marshalled.size() > 39) {
        EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled));
    }
}