/*
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



//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------




TEST(TupleTest, MarshallUnmarshallDataIntegrity) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {true, false, true, false};
    std::vector<allVars> data = {allVars(42), allVars(84L), allVars(std::string("test")), allVars(126)};
    
    // Marshall w pierwszym obiekcie
    std::vector<uint8_t> marshalled = tpl1.marshallTuple(1000, 2000, 3000, 4000, 5000, true, 6000, bitMap, data);
    
    EXPECT_FALSE(marshalled.empty());
    EXPECT_GE(marshalled.size(), 39);
    
    // Unmarshall w drugim obiekcie
    EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled));
    
    // Sprawdź czy wszystkie dane zostały poprawnie przeniesione
    EXPECT_EQ(tpl2.getTxMin(), 1000);
    EXPECT_EQ(tpl2.getTxMax(), 2000);
    EXPECT_EQ(tpl2.getTCid(), 3000);
    EXPECT_EQ(tpl2.getTInfomask(), 4000);
    EXPECT_EQ(tpl2.getTHoff(), 5000);
    EXPECT_TRUE(tpl2.getNullBitmap());
    EXPECT_EQ(tpl2.getOptionalOid(), 6000);
    
    EXPECT_EQ(tpl2.getBitMap().size(), 4);
    EXPECT_TRUE(tpl2.getBitMap()[0]);
    EXPECT_FALSE(tpl2.getBitMap()[1]);
    EXPECT_TRUE(tpl2.getBitMap()[2]);
    EXPECT_FALSE(tpl2.getBitMap()[3]);
    
    EXPECT_EQ(tpl2.getData().size(), 4);
}

TEST(TupleTest, MarshallUnmarshallNegativeValues) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {false, true, false};
    std::vector<allVars> data = {allVars(-100), allVars(-200L), allVars(-300)};
    
    std::vector<uint8_t> marshalled = tpl1.marshallTuple(-1000, -2000, -3000, -4000, -5000, false, -6000, bitMap, data);
    
    EXPECT_FALSE(marshalled.empty());
    
    EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled));
    
    EXPECT_EQ(tpl2.getTxMin(), -1000);
    EXPECT_EQ(tpl2.getTxMax(), -2000);
    EXPECT_EQ(tpl2.getTCid(), -3000);
    EXPECT_EQ(tpl2.getTInfomask(), -4000);
    EXPECT_EQ(tpl2.getTHoff(), -5000);
    EXPECT_FALSE(tpl2.getNullBitmap());
    EXPECT_EQ(tpl2.getOptionalOid(), -6000);
    
    EXPECT_EQ(tpl2.getBitMap().size(), 3);
    EXPECT_FALSE(tpl2.getBitMap()[0]);
    EXPECT_TRUE(tpl2.getBitMap()[1]);
    EXPECT_FALSE(tpl2.getBitMap()[2]);
    
    EXPECT_EQ(tpl2.getData().size(), 3);
}

TEST(TupleTest, MarshallUnmarshallMaxValues) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {true, true, true};
    std::vector<allVars> data = {
        allVars(std::numeric_limits<int>::max()),
        allVars(std::numeric_limits<long>::max()),
        allVars(std::string("max_test"))
    };
    
    std::vector<uint8_t> marshalled = tpl1.marshallTuple(
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
    
    EXPECT_FALSE(marshalled.empty());
    
    EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled));
    
    EXPECT_EQ(tpl2.getTxMin(), std::numeric_limits<int64_t>::max());
    EXPECT_EQ(tpl2.getTxMax(), std::numeric_limits<int64_t>::max());
    EXPECT_EQ(tpl2.getTCid(), std::numeric_limits<int32_t>::max());
    EXPECT_EQ(tpl2.getTInfomask(), std::numeric_limits<int32_t>::max());
    EXPECT_EQ(tpl2.getTHoff(), std::numeric_limits<int16_t>::max());
    EXPECT_TRUE(tpl2.getNullBitmap());
    EXPECT_EQ(tpl2.getOptionalOid(), std::numeric_limits<int64_t>::max());
    
    EXPECT_EQ(tpl2.getBitMap().size(), 3);
    EXPECT_EQ(tpl2.getData().size(), 3);
}

TEST(TupleTest, MarshallUnmarshallMinValues) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {false, false, false};
    std::vector<allVars> data = {
        allVars(std::numeric_limits<int>::min()),
        allVars(std::numeric_limits<long>::min()),
        allVars(std::string("min_test"))
    };
    
    std::vector<uint8_t> marshalled = tpl1.marshallTuple(
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
    
    EXPECT_FALSE(marshalled.empty());
    
    EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled));
    
    EXPECT_EQ(tpl2.getTxMin(), std::numeric_limits<int64_t>::min());
    EXPECT_EQ(tpl2.getTxMax(), std::numeric_limits<int64_t>::min());
    EXPECT_EQ(tpl2.getTCid(), std::numeric_limits<int32_t>::min());
    EXPECT_EQ(tpl2.getTInfomask(), std::numeric_limits<int32_t>::min());
    EXPECT_EQ(tpl2.getTHoff(), std::numeric_limits<int16_t>::min());
    EXPECT_FALSE(tpl2.getNullBitmap());
    EXPECT_EQ(tpl2.getOptionalOid(), std::numeric_limits<int64_t>::min());
    
    EXPECT_EQ(tpl2.getBitMap().size(), 3);
    EXPECT_EQ(tpl2.getData().size(), 3);
}

TEST(TupleTest, MarshallUnmarshallEmptyData) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> emptyBitMap;
    std::vector<allVars> emptyData;
    
    std::vector<uint8_t> marshalled = tpl1.marshallTuple(100, 200, 300, 400, 500, false, 600, emptyBitMap, emptyData);
    
    EXPECT_FALSE(marshalled.empty());
    
    EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled));
    
    EXPECT_EQ(tpl2.getTxMin(), 100);
    EXPECT_EQ(tpl2.getTxMax(), 200);
    EXPECT_EQ(tpl2.getTCid(), 300);
    EXPECT_EQ(tpl2.getTInfomask(), 400);
    EXPECT_EQ(tpl2.getTHoff(), 500);
    EXPECT_FALSE(tpl2.getNullBitmap());
    EXPECT_EQ(tpl2.getOptionalOid(), 600);
    
    EXPECT_TRUE(tpl2.getBitMap().empty());
    EXPECT_TRUE(tpl2.getData().empty());
}

TEST(TupleTest, MarshallUnmarshallLargeDataSet) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap;
    std::vector<allVars> data;
    
    for (int i = 0; i < 20; i++) {
        bitMap.push_back(i % 2 == 0);
        if (i % 3 == 0) {
            data.push_back(allVars(i * 100));
        } else if (i % 3 == 1) {
            data.push_back(allVars(static_cast<long>(i * 1000)));
        } else {
            data.push_back(allVars(std::string("item_" + std::to_string(i))));
        }
    }
    
    std::vector<uint8_t> marshalled = tpl1.marshallTuple(10000, 20000, 30000, 40000, 1000, true, 60000, bitMap, data);
    
    EXPECT_FALSE(marshalled.empty());
    
    EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled));
    
    EXPECT_EQ(tpl2.getTxMin(), 10000);
    EXPECT_EQ(tpl2.getTxMax(), 20000);
    EXPECT_EQ(tpl2.getTCid(), 30000);
    EXPECT_EQ(tpl2.getTInfomask(), 40000);
    EXPECT_EQ(tpl2.getTHoff(), 1000);
    EXPECT_TRUE(tpl2.getNullBitmap());
    EXPECT_EQ(tpl2.getOptionalOid(), 60000);
    
    EXPECT_EQ(tpl2.getBitMap().size(), 20);
    EXPECT_EQ(tpl2.getData().size(), 20);
    
    for (int i = 0; i < 20; i++) {
        EXPECT_EQ(tpl2.getBitMap()[i], i % 2 == 0) << "BitMap mismatch at index " << i;
    }
}

TEST(TupleTest, MarshallUnmarshallStringData) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {true, true, true, true, true};
    std::vector<allVars> data = {
        allVars(std::string("")),
        allVars(std::string("a")),
        allVars(std::string("Hello World!")),
        allVars(std::string("Special chars: !@#$%^&*()")),
        allVars(std::string("Very long string with lots of characters"))
    };
    
    std::vector<uint8_t> marshalled = tpl1.marshallTuple(7000, 8000, 9000, 10000, 11000, true, 12000, bitMap, data);
    
    EXPECT_FALSE(marshalled.empty());
    
    EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled));
    
    EXPECT_EQ(tpl2.getTxMin(), 7000);
    EXPECT_EQ(tpl2.getTxMax(), 8000);
    EXPECT_EQ(tpl2.getTCid(), 9000);
    EXPECT_EQ(tpl2.getTInfomask(), 10000);
    EXPECT_EQ(tpl2.getTHoff(), 11000);
    EXPECT_TRUE(tpl2.getNullBitmap());
    EXPECT_EQ(tpl2.getOptionalOid(), 12000);
    
    EXPECT_EQ(tpl2.getBitMap().size(), 5);
    EXPECT_EQ(tpl2.getData().size(), 5);
    
    for (int i = 0; i < 5; i++) {
        EXPECT_TRUE(tpl2.getBitMap()[i]) << "BitMap should be true at index " << i;
    }
}

TEST(TupleTest, MarshallUnmarshallZeroValues) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {false, false, false};
    std::vector<allVars> data = {allVars(0), allVars(0L), allVars(std::string(""))};
    
    std::vector<uint8_t> marshalled = tpl1.marshallTuple(0, 0, 0, 0, 0, false, 0, bitMap, data);
    
    EXPECT_FALSE(marshalled.empty());
    
    EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled));
    
    EXPECT_EQ(tpl2.getTxMin(), 0);
    EXPECT_EQ(tpl2.getTxMax(), 0);
    EXPECT_EQ(tpl2.getTCid(), 0);
    EXPECT_EQ(tpl2.getTInfomask(), 0);
    EXPECT_EQ(tpl2.getTHoff(), 0);
    EXPECT_FALSE(tpl2.getNullBitmap());
    EXPECT_EQ(tpl2.getOptionalOid(), 0);
    
    EXPECT_EQ(tpl2.getBitMap().size(), 3);
    EXPECT_EQ(tpl2.getData().size(), 3);
    
    for (int i = 0; i < 3; i++) {
        EXPECT_FALSE(tpl2.getBitMap()[i]) << "BitMap should be false at index " << i;
    }
}

TEST(TupleTest, MarshallUnmarshallMultipleRoundTrips) {
    tuple tpl1;
    tuple tpl2;
    tuple tpl3;
    
    std::vector<bool> bitMap = {true, false, true};
    std::vector<allVars> data = {allVars(123), allVars(456L), allVars(std::string("roundtrip"))};
    
    // Pierwszy round trip: tpl1 -> marshall -> tpl2 -> unmarshall
    std::vector<uint8_t> marshalled1 = tpl1.marshallTuple(1000, 2000, 3000, 4000, 5000, true, 6000, bitMap, data);
    
    EXPECT_FALSE(marshalled1.empty());
    EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled1));
    
    // Sprawdź dane w tpl2
    EXPECT_EQ(tpl2.getTxMin(), 1000);
    EXPECT_EQ(tpl2.getTxMax(), 2000);
    EXPECT_EQ(tpl2.getTCid(), 3000);
    EXPECT_TRUE(tpl2.getNullBitmap());
    EXPECT_EQ(tpl2.getBitMap().size(), 3);
    EXPECT_EQ(tpl2.getData().size(), 3);
    
    // Drugi round trip: tpl2 -> marshallTupleWithData -> tpl3 -> unmarshall
    std::vector<uint8_t> marshalled2 = tpl2.marshallTupleWithData();
    
    EXPECT_FALSE(marshalled2.empty());
    EXPECT_NO_THROW(tpl3.unmarshallTuple(marshalled2));
    
    // Sprawdź czy dane zostały zachowane w tpl3
    EXPECT_EQ(tpl3.getTxMin(), 1000);
    EXPECT_EQ(tpl3.getTxMax(), 2000);
    EXPECT_EQ(tpl3.getTCid(), 3000);
    EXPECT_EQ(tpl3.getTInfomask(), 4000);
    EXPECT_EQ(tpl3.getTHoff(), 5000);
    EXPECT_TRUE(tpl3.getNullBitmap());
    EXPECT_EQ(tpl3.getOptionalOid(), 6000);
    
    EXPECT_EQ(tpl3.getBitMap().size(), 3);
    EXPECT_EQ(tpl3.getData().size(), 3);
    
    EXPECT_TRUE(tpl3.getBitMap()[0]);
    EXPECT_FALSE(tpl3.getBitMap()[1]);
    EXPECT_TRUE(tpl3.getBitMap()[2]);
}

TEST(TupleTest, MarshallUnmarshallConsecutiveObjects) {
    std::vector<tuple> tuples(10);
    
    std::vector<bool> bitMap = {true, false};
    std::vector<allVars> data = {allVars(42), allVars(std::string("consecutive"))};
    
    // Marshall w pierwszym obiekcie
    std::vector<uint8_t> marshalled = tuples[0].marshallTuple(999, 888, 777, 666, 555, true, 444, bitMap, data);
    
    EXPECT_FALSE(marshalled.empty());
    
    // Unmarshall w pozostałych obiektach
    for (int i = 1; i < 10; i++) {
        EXPECT_NO_THROW(tuples[i].unmarshallTuple(marshalled));
        
        EXPECT_EQ(tuples[i].getTxMin(), 999);
        EXPECT_EQ(tuples[i].getTxMax(), 888);
        EXPECT_EQ(tuples[i].getTCid(), 777);
        EXPECT_EQ(tuples[i].getTInfomask(), 666);
        EXPECT_EQ(tuples[i].getTHoff(), 555);
        EXPECT_TRUE(tuples[i].getNullBitmap());
        EXPECT_EQ(tuples[i].getOptionalOid(), 444);
        
        EXPECT_EQ(tuples[i].getBitMap().size(), 2);
        EXPECT_EQ(tuples[i].getData().size(), 2);
        
        EXPECT_TRUE(tuples[i].getBitMap()[0]);
        EXPECT_FALSE(tuples[i].getBitMap()[1]);
    }
}

TEST(TupleTest, MarshallUnmarshallComplexDataTypes) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {true, false, true, false, true, false};
    std::vector<allVars> data = {
        allVars(std::numeric_limits<int>::max()),
        allVars(std::numeric_limits<long>::min()),
        allVars(std::string("complex_test_string_with_many_characters")),
        allVars(0),
        allVars(-999999L),
        allVars(std::string(""))
    };
    
    std::vector<uint8_t> marshalled = tpl1.marshallTuple(
        0x123456789ABCDEFLL,
        -0x123456789ABCDEFLL,
        0x12345678,
        -0x12345678,
        0x1234,
        true,
        0xFEDCBA9876543210LL,
        bitMap,
        data
    );
    
    EXPECT_FALSE(marshalled.empty());
    
    EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled));
    
    EXPECT_EQ(tpl2.getTxMin(), 0x123456789ABCDEFLL);
    EXPECT_EQ(tpl2.getTxMax(), -0x123456789ABCDEFLL);
    EXPECT_EQ(tpl2.getTCid(), 0x12345678);
    EXPECT_EQ(tpl2.getTInfomask(), -0x12345678);
    EXPECT_EQ(tpl2.getTHoff(), 0x1234);
    EXPECT_TRUE(tpl2.getNullBitmap());
    EXPECT_EQ(tpl2.getOptionalOid(), 0xFEDCBA9876543210LL);
    
    EXPECT_EQ(tpl2.getBitMap().size(), 6);
    EXPECT_EQ(tpl2.getData().size(), 6);
    
    for (int i = 0; i < 6; i++) {
        EXPECT_EQ(tpl2.getBitMap()[i], i % 2 == 0) << "BitMap mismatch at index " << i;
    }
}

TEST(TupleTest, MarshallUnmarshallPerformanceStress) {
    const int iterations = 20;
    
    for (int iter = 0; iter < iterations; iter++) {
        tuple tpl1;
        tuple tpl2;
        
        std::vector<bool> bitMap;
        std::vector<allVars> data;
        
        for (int i = 0; i < 5; i++) {
            bitMap.push_back(i % 2 == 0);
            data.push_back(allVars(iter * 100 + i));
        }
        
        std::vector<uint8_t> marshalled = tpl1.marshallTuple(
            iter * 1000,
            iter * 1000 + 1,
            iter * 1000 + 2,
            iter * 1000 + 3,
            iter * 1000 + 4,
            iter % 2 == 0,
            iter * 1000 + 5,
            bitMap,
            data
        );
        
        EXPECT_FALSE(marshalled.empty());
        
        EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled));
        
        EXPECT_EQ(tpl2.getTxMin(), iter * 1000);
        EXPECT_EQ(tpl2.getTxMax(), iter * 1000 + 1);
        EXPECT_EQ(tpl2.getTCid(), iter * 1000 + 2);
        EXPECT_EQ(tpl2.getTInfomask(), iter * 1000 + 3);
        EXPECT_EQ(tpl2.getTHoff(), iter * 1000 + 4);
        EXPECT_EQ(tpl2.getNullBitmap(), iter % 2 == 0);
        EXPECT_EQ(tpl2.getOptionalOid(), iter * 1000 + 5);
        
        EXPECT_EQ(tpl2.getBitMap().size(), 5);
        EXPECT_EQ(tpl2.getData().size(), 5);
    }
}


*/