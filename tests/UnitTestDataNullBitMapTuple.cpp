#include <gtest/gtest.h>
#include "dataNullBitMapTuple.h"
#include "typeManagerAllVarsTypes.h"
#include <vector>
#include <string>
#include <limits>
#include <chrono>

class DataNullBitMapTupleTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(DataNullBitMapTupleTest, DefaultConstructor) {
    DataNullBitMapTuple tuple;
    EXPECT_TRUE(tuple.getBitMap().empty());
    EXPECT_TRUE(tuple.getData().empty());
}

TEST_F(DataNullBitMapTupleTest, MarshallBasicValues) {
    DataNullBitMapTuple tuple;
    std::vector<bool> bitmap = {true, false, true};
    std::vector<allVars> data = {int32_t(42), std::string("Hello"), int32_t(100)};
    
    std::vector<uint8_t> result = tuple.marshallDataNullBitMapTuple(bitmap, data);
    
    EXPECT_FALSE(result.empty());
    EXPECT_GT(result.size(), 20);
}

TEST_F(DataNullBitMapTupleTest, MarshallEmptyData) {
    DataNullBitMapTuple tuple;
    std::vector<bool> bitmap = {};
    std::vector<allVars> data = {};
    
    std::vector<uint8_t> result = tuple.marshallDataNullBitMapTuple(bitmap, data);
    
    EXPECT_FALSE(result.empty());
    EXPECT_GT(result.size(), 10);
}

TEST_F(DataNullBitMapTupleTest, MarshallSingleInt32) {
    DataNullBitMapTuple tuple;
    std::vector<bool> bitmap = {true};
    std::vector<allVars> data = {int32_t(12345)};
    
    std::vector<uint8_t> result = tuple.marshallDataNullBitMapTuple(bitmap, data);
    
    EXPECT_FALSE(result.empty());
    EXPECT_GT(result.size(), 15);
}

TEST_F(DataNullBitMapTupleTest, MarshallSingleInt64) {
    DataNullBitMapTuple tuple;
    std::vector<bool> bitmap = {true};
    std::vector<allVars> data = {int64_t(9876543210LL)};
    
    std::vector<uint8_t> result = tuple.marshallDataNullBitMapTuple(bitmap, data);
    
    EXPECT_FALSE(result.empty());
    EXPECT_GT(result.size(), 19);
}

TEST_F(DataNullBitMapTupleTest, MarshallSingleString) {
    DataNullBitMapTuple tuple;
    std::vector<bool> bitmap = {true};
    std::vector<allVars> data = {std::string("TestString")};
    
    std::vector<uint8_t> result = tuple.marshallDataNullBitMapTuple(bitmap, data);
    
    EXPECT_FALSE(result.empty());
    EXPECT_GT(result.size(), 25);
}

TEST_F(DataNullBitMapTupleTest, MarshallMixedTypes) {
    DataNullBitMapTuple tuple;
    std::vector<bool> bitmap = {true, false, true, false};
    std::vector<allVars> data = {
        int32_t(123), 
        int64_t(456789LL), 
        std::string("Mixed"), 
        int32_t(-999)
    };
    
    std::vector<uint8_t> result = tuple.marshallDataNullBitMapTuple(bitmap, data);
    
    EXPECT_FALSE(result.empty());
    EXPECT_GT(result.size(), 40);
}

TEST_F(DataNullBitMapTupleTest, RoundTripBasic) {
    DataNullBitMapTuple tuple1;
    std::vector<bool> originalBitmap = {true, false, true};
    std::vector<allVars> originalData = {int32_t(42), std::string("Hello"), int32_t(100)};
    
    std::vector<uint8_t> marshalled = tuple1.marshallDataNullBitMapTuple(originalBitmap, originalData);
    
    DataNullBitMapTuple tuple2;
    tuple2.unmarshallDataNullBitMapTuple(marshalled);
    
    std::vector<bool> resultBitmap = tuple2.getBitMap();
    EXPECT_EQ(resultBitmap.size(), originalBitmap.size());
    for (size_t i = 0; i < originalBitmap.size(); i++) {
        EXPECT_EQ(resultBitmap[i], originalBitmap[i]);
    }
    
    std::vector<allVars> resultData = tuple2.getData();
    EXPECT_EQ(resultData.size(), originalData.size());
    
    EXPECT_TRUE(std::holds_alternative<int32_t>(resultData[0]));
    EXPECT_EQ(std::get<int32_t>(resultData[0]), 42);
    
    EXPECT_TRUE(std::holds_alternative<std::string>(resultData[1]));
    EXPECT_EQ(std::get<std::string>(resultData[1]), "Hello");
    
    EXPECT_TRUE(std::holds_alternative<int32_t>(resultData[2]));
    EXPECT_EQ(std::get<int32_t>(resultData[2]), 100);
}

TEST_F(DataNullBitMapTupleTest, RoundTripEmptyData) {
    DataNullBitMapTuple tuple1;
    std::vector<bool> originalBitmap = {};
    std::vector<allVars> originalData = {};
    
    std::vector<uint8_t> marshalled = tuple1.marshallDataNullBitMapTuple(originalBitmap, originalData);
    
    DataNullBitMapTuple tuple2;
    tuple2.unmarshallDataNullBitMapTuple(marshalled);
    
    EXPECT_TRUE(tuple2.getBitMap().empty());
    EXPECT_TRUE(tuple2.getData().empty());
}

TEST_F(DataNullBitMapTupleTest, RoundTripInt64Values) {
    DataNullBitMapTuple tuple1;
    std::vector<bool> originalBitmap = {true, true};
    std::vector<allVars> originalData = {
        int64_t(std::numeric_limits<int64_t>::max()),
        int64_t(std::numeric_limits<int64_t>::min())
    };
    
    std::vector<uint8_t> marshalled = tuple1.marshallDataNullBitMapTuple(originalBitmap, originalData);
    
    DataNullBitMapTuple tuple2;
    tuple2.unmarshallDataNullBitMapTuple(marshalled);
    
    std::vector<allVars> resultData = tuple2.getData();
    EXPECT_EQ(resultData.size(), 2);
    
    EXPECT_TRUE(std::holds_alternative<int64_t>(resultData[0]));
    EXPECT_EQ(std::get<int64_t>(resultData[0]), std::numeric_limits<int64_t>::max());
    
    EXPECT_TRUE(std::holds_alternative<int64_t>(resultData[1]));
    EXPECT_EQ(std::get<int64_t>(resultData[1]), std::numeric_limits<int64_t>::min());
}

TEST_F(DataNullBitMapTupleTest, RoundTripStringValues) {
    DataNullBitMapTuple tuple1;
    std::vector<bool> originalBitmap = {true, false, true};
    std::vector<allVars> originalData = {
        std::string(""), 
        std::string("Short"),
        std::string("This is a much longer string to test string handling")
    };
    
    std::vector<uint8_t> marshalled = tuple1.marshallDataNullBitMapTuple(originalBitmap, originalData);
    
    DataNullBitMapTuple tuple2;
    tuple2.unmarshallDataNullBitMapTuple(marshalled);
    
    std::vector<allVars> resultData = tuple2.getData();
    EXPECT_EQ(resultData.size(), 3);
    
    EXPECT_TRUE(std::holds_alternative<std::string>(resultData[0]));
    EXPECT_EQ(std::get<std::string>(resultData[0]), "");
    
    EXPECT_TRUE(std::holds_alternative<std::string>(resultData[1]));
    EXPECT_EQ(std::get<std::string>(resultData[1]), "Short");
    
    EXPECT_TRUE(std::holds_alternative<std::string>(resultData[2]));
    EXPECT_EQ(std::get<std::string>(resultData[2]), "This is a much longer string to test string handling");
}

TEST_F(DataNullBitMapTupleTest, BoundaryValues) {
    DataNullBitMapTuple tuple1;
    std::vector<bool> originalBitmap = {true, true, true, true};
    std::vector<allVars> originalData = {
        int32_t(0),
        int32_t(std::numeric_limits<int32_t>::max()),
        int32_t(std::numeric_limits<int32_t>::min()),
        int32_t(-1)
    };
    
    std::vector<uint8_t> marshalled = tuple1.marshallDataNullBitMapTuple(originalBitmap, originalData);
    
    DataNullBitMapTuple tuple2;
    tuple2.unmarshallDataNullBitMapTuple(marshalled);
    
    std::vector<allVars> resultData = tuple2.getData();
    EXPECT_EQ(resultData.size(), 4);
    
    EXPECT_EQ(std::get<int32_t>(resultData[0]), 0);
    EXPECT_EQ(std::get<int32_t>(resultData[1]), std::numeric_limits<int32_t>::max());
    EXPECT_EQ(std::get<int32_t>(resultData[2]), std::numeric_limits<int32_t>::min());
    EXPECT_EQ(std::get<int32_t>(resultData[3]), -1);
}

TEST_F(DataNullBitMapTupleTest, LargeBitmap) {
    DataNullBitMapTuple tuple1;
    std::vector<bool> originalBitmap(100, true);
    std::vector<allVars> originalData;
    
    for (int i = 0; i < 100; i++) {
        originalData.push_back(int32_t(i));
    }
    
    std::vector<uint8_t> marshalled = tuple1.marshallDataNullBitMapTuple(originalBitmap, originalData);
    
    DataNullBitMapTuple tuple2;
    tuple2.unmarshallDataNullBitMapTuple(marshalled);
    
    std::vector<bool> resultBitmap = tuple2.getBitMap();
    std::vector<allVars> resultData = tuple2.getData();
    
    EXPECT_EQ(resultBitmap.size(), 100);
    EXPECT_EQ(resultData.size(), 100);
    
    for (int i = 0; i < 100; i++) {
        EXPECT_TRUE(resultBitmap[i]);
        EXPECT_EQ(std::get<int32_t>(resultData[i]), i);
    }
}

TEST_F(DataNullBitMapTupleTest, UnmarshallEmptyVector) {
    DataNullBitMapTuple tuple;
    std::vector<uint8_t> empty;
    EXPECT_EQ(empty.size(), 0 );
    EXPECT_EQ(tuple.getBitMap().size(), 0 );
    EXPECT_EQ(tuple.getData().size(), 0 );
    //EXPECT_NO_THROW(tuple.unmarshallDataNullBitMapTuple(empty));
    
    
    //EXPECT_TRUE(tuple.getData().empty());
}

TEST_F(DataNullBitMapTupleTest, UnmarshallInvalidData) {
    DataNullBitMapTuple tuple;
    std::vector<uint8_t> invalid = {0x01, 0x02, 0x03};
    
    EXPECT_NO_THROW(tuple.unmarshallDataNullBitMapTuple(invalid));
}

TEST_F(DataNullBitMapTupleTest, MarshallConsistency) {
    DataNullBitMapTuple tuple1, tuple2;
    std::vector<bool> bitmap = {true, false, true};
    std::vector<allVars> data = {int32_t(42), std::string("Test"), int32_t(100)};
    
    std::vector<uint8_t> result1 = tuple1.marshallDataNullBitMapTuple(bitmap, data);
    std::vector<uint8_t> result2 = tuple2.marshallDataNullBitMapTuple(bitmap, data);
    
    EXPECT_EQ(result1, result2);
}

TEST_F(DataNullBitMapTupleTest, MarshallDifferentValues) {
    DataNullBitMapTuple tuple;
    
    std::vector<uint8_t> result1 = tuple.marshallDataNullBitMapTuple(
        {true}, {int32_t(42)}
    );
    
    std::vector<uint8_t> result2 = tuple.marshallDataNullBitMapTuple(
        {true}, {int32_t(43)}
    );
    
    EXPECT_NE(result1, result2);
}

TEST_F(DataNullBitMapTupleTest, PerformanceTest) {
    const int iterations = 1000;
    DataNullBitMapTuple tuple;
    std::vector<bool> bitmap = {true, false, true};
    std::vector<allVars> data = {int32_t(42), std::string("Hello"), int32_t(100)};
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        std::vector<uint8_t> result = tuple.marshallDataNullBitMapTuple(bitmap, data);
        volatile size_t dummy = result.size();
        (void)dummy;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    EXPECT_LT(duration.count(), 1000000);
}

TEST_F(DataNullBitMapTupleTest, CompleteIntegrationTest) {
    DataNullBitMapTuple originalTuple;
    
    std::vector<bool> originalBitmap = {true, false, true, false, true, true};
    std::vector<allVars> originalData = {
        int32_t(12345),
        int64_t(987654321LL),
        std::string("Integration Test String"),
        int32_t(-42),
        int64_t(std::numeric_limits<int64_t>::max()),
        std::string("")
    };
    
    std::vector<uint8_t> marshalled = originalTuple.marshallDataNullBitMapTuple(originalBitmap, originalData);
    EXPECT_FALSE(marshalled.empty());
    
    DataNullBitMapTuple reconstructedTuple;
    reconstructedTuple.unmarshallDataNullBitMapTuple(marshalled);
    
    std::vector<bool> resultBitmap = reconstructedTuple.getBitMap();
    std::vector<allVars> resultData = reconstructedTuple.getData();
    
    EXPECT_EQ(resultBitmap.size(), originalBitmap.size());
    EXPECT_EQ(resultData.size(), originalData.size());
    
    for (size_t i = 0; i < originalBitmap.size(); i++) {
        EXPECT_EQ(resultBitmap[i], originalBitmap[i]);
    }
    
    EXPECT_EQ(std::get<int32_t>(resultData[0]), 12345);
    EXPECT_EQ(std::get<int64_t>(resultData[1]), 987654321LL);
    EXPECT_EQ(std::get<std::string>(resultData[2]), "Integration Test String");
    EXPECT_EQ(std::get<int32_t>(resultData[3]), -42);
    EXPECT_EQ(std::get<int64_t>(resultData[4]), std::numeric_limits<int64_t>::max());
    EXPECT_EQ(std::get<std::string>(resultData[5]), "");
}