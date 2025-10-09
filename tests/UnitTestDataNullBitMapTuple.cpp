#include <gtest/gtest.h>
#include "dataNullBitMapTuple.h"
#include "typeManagerAllVarsTypes.h"
#include <vector>
#include <string>
#include <limits>

// ==================== TESTY KONSTRUKTORA ====================

TEST(DataNullBitMapTupleTest, DefaultConstructor) {
    DataNullBitMapTuple tuple;
    EXPECT_TRUE(tuple.getBitMap().empty());
    EXPECT_TRUE(tuple.getData().empty());
    EXPECT_EQ(tuple.getSize(), 0);
}

// ==================== TESTY MARSHALLING ====================

TEST(DataNullBitMapTupleTest, MarshallEmptyData) {
    DataNullBitMapTuple tuple;
    std::vector<bool> bitmap = {};
    std::vector<allVars> data = {};
    
    std::vector<uint8_t> result = tuple.marshallDataNullBitMapTuple(bitmap, data);
    
    // Rozmiar: 2 (type) + 2 (bitmapType) + 4 (sizeBitMap) + 2 (dataType) + 4 (sizeData) = 14 bajtów
    EXPECT_EQ(result.size(), 14);
}

TEST(DataNullBitMapTupleTest, MarshallSingleInt32) {
    DataNullBitMapTuple tuple;
    std::vector<bool> bitmap = {true};
    std::vector<allVars> data = {int32_t(12345)};
    
    std::vector<uint8_t> result = tuple.marshallDataNullBitMapTuple(bitmap, data);
    
    EXPECT_FALSE(result.empty());
    // 14 (header) + 1 (bitmap) + 2 (valType) + 4 (valSize) + 4 (int32) = 25 bajtów
    EXPECT_EQ(result.size(), 25);
}

TEST(DataNullBitMapTupleTest, MarshallSingleInt64) {
    DataNullBitMapTuple tuple;
    std::vector<bool> bitmap = {true};
    std::vector<allVars> data = {int64_t(9876543210LL)};
    
    std::vector<uint8_t> result = tuple.marshallDataNullBitMapTuple(bitmap, data);
    
    EXPECT_FALSE(result.empty());
    // 14 (header) + 1 (bitmap) + 2 (valType) + 4 (valSize) + 8 (int64) = 29 bajtów
    EXPECT_EQ(result.size(), 29);
}

TEST(DataNullBitMapTupleTest, MarshallSingleString) {
    DataNullBitMapTuple tuple;
    std::vector<bool> bitmap = {true};
    std::vector<allVars> data = {std::string("Test")};
    
    std::vector<uint8_t> result = tuple.marshallDataNullBitMapTuple(bitmap, data);
    
    EXPECT_FALSE(result.empty());
    // 14 (header) + 1 (bitmap) + 2 (valType) + 4 (valSize) + 4 (string size) + 4 (string content) = 29 bajtów
    EXPECT_GT(result.size(), 20);
}

TEST(DataNullBitMapTupleTest, MarshallMultipleInt32) {
    DataNullBitMapTuple tuple;
    std::vector<bool> bitmap = {true, true, true};
    std::vector<allVars> data = {int32_t(10), int32_t(20), int32_t(30)};
    
    std::vector<uint8_t> result = tuple.marshallDataNullBitMapTuple(bitmap, data);
    
    EXPECT_FALSE(result.empty());
    // 14 (header) + 3 (bitmap) + 3*(2+4+4) = 14 + 3 + 30 = 47 bajtów
    EXPECT_EQ(result.size(), 47);
}

TEST(DataNullBitMapTupleTest, MarshallMixedTypes) {
    DataNullBitMapTuple tuple;
    std::vector<bool> bitmap = {true, false, true};
    std::vector<allVars> data = {
        int32_t(123), 
        int64_t(456789LL), 
        std::string("Mixed")
    };
    
    std::vector<uint8_t> result = tuple.marshallDataNullBitMapTuple(bitmap, data);
    
    EXPECT_FALSE(result.empty());
    EXPECT_GT(result.size(), 30);
}

TEST(DataNullBitMapTupleTest, MarshallWithNullBitmap) {
    DataNullBitMapTuple tuple;
    std::vector<bool> bitmap = {false, false, false};
    std::vector<allVars> data = {int32_t(1), int32_t(2), int32_t(3)};
    
    std::vector<uint8_t> result = tuple.marshallDataNullBitMapTuple(bitmap, data);
    
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 47);
}

// ==================== TESTY UNMARSHALLING ====================

TEST(DataNullBitMapTupleTest, UnmarshallEmptyVector) {
    DataNullBitMapTuple tuple;
    std::vector<uint8_t> empty;
    
    EXPECT_NO_THROW(tuple.unmarshallDataNullBitMapTuple(empty));
    EXPECT_TRUE(tuple.getBitMap().empty());
    EXPECT_TRUE(tuple.getData().empty());
}

TEST(DataNullBitMapTupleTest, UnmarshallInvalidData) {
    DataNullBitMapTuple tuple;
    std::vector<uint8_t> invalid = {0x01, 0x02, 0x03};
    
    EXPECT_NO_THROW(tuple.unmarshallDataNullBitMapTuple(invalid));
}

// ==================== TESTY ROUND-TRIP ====================

TEST(DataNullBitMapTupleTest, RoundTripEmptyData) {
    DataNullBitMapTuple tuple1;
    std::vector<bool> originalBitmap = {};
    std::vector<allVars> originalData = {};
    
    std::vector<uint8_t> marshalled = tuple1.marshallDataNullBitMapTuple(originalBitmap, originalData);
    
    DataNullBitMapTuple tuple2;
    tuple2.unmarshallDataNullBitMapTuple(marshalled);
    
    EXPECT_TRUE(tuple2.getBitMap().empty());
    EXPECT_TRUE(tuple2.getData().empty());
}

TEST(DataNullBitMapTupleTest, RoundTripSingleInt32) {
    DataNullBitMapTuple tuple1;
    std::vector<bool> originalBitmap = {true};
    std::vector<allVars> originalData = {int32_t(42)};
    
    std::vector<uint8_t> marshalled = tuple1.marshallDataNullBitMapTuple(originalBitmap, originalData);
    
    DataNullBitMapTuple tuple2;
    tuple2.unmarshallDataNullBitMapTuple(marshalled);
    
    std::vector<bool> resultBitmap = tuple2.getBitMap();
    std::vector<allVars> resultData = tuple2.getData();
    
    EXPECT_EQ(resultBitmap.size(), 1);
    EXPECT_TRUE(resultBitmap[0]);
    
    EXPECT_EQ(resultData.size(), 1);
    EXPECT_TRUE(std::holds_alternative<int32_t>(resultData[0]));
    EXPECT_EQ(std::get<int32_t>(resultData[0]), 42);
}

TEST(DataNullBitMapTupleTest, RoundTripSingleInt64) {
    DataNullBitMapTuple tuple1;
    std::vector<bool> originalBitmap = {true};
    std::vector<allVars> originalData = {int64_t(9876543210LL)};
    
    std::vector<uint8_t> marshalled = tuple1.marshallDataNullBitMapTuple(originalBitmap, originalData);
    
    DataNullBitMapTuple tuple2;
    tuple2.unmarshallDataNullBitMapTuple(marshalled);
    
    std::vector<allVars> resultData = tuple2.getData();
    
    EXPECT_EQ(resultData.size(), 1);
    EXPECT_TRUE(std::holds_alternative<int64_t>(resultData[0]));
    EXPECT_EQ(std::get<int64_t>(resultData[0]), 9876543210LL);
}

TEST(DataNullBitMapTupleTest, RoundTripSingleString) {
    DataNullBitMapTuple tuple1;
    std::vector<bool> originalBitmap = {true};
    std::vector<allVars> originalData = {std::string("Hello")};
    
    std::vector<uint8_t> marshalled = tuple1.marshallDataNullBitMapTuple(originalBitmap, originalData);
    
    DataNullBitMapTuple tuple2;
    tuple2.unmarshallDataNullBitMapTuple(marshalled);
    
    std::vector<allVars> resultData = tuple2.getData();
    
    EXPECT_EQ(resultData.size(), 1);
    EXPECT_TRUE(std::holds_alternative<std::string>(resultData[0]));
    EXPECT_EQ(std::get<std::string>(resultData[0]), "Hello");
}

TEST(DataNullBitMapTupleTest, RoundTripMultipleInt32) {
    DataNullBitMapTuple tuple1;
    std::vector<bool> originalBitmap = {true, false, true};
    std::vector<allVars> originalData = {int32_t(10), int32_t(20), int32_t(30)};
    
    std::vector<uint8_t> marshalled = tuple1.marshallDataNullBitMapTuple(originalBitmap, originalData);
    
    DataNullBitMapTuple tuple2;
    tuple2.unmarshallDataNullBitMapTuple(marshalled);
    
    std::vector<bool> resultBitmap = tuple2.getBitMap();
    std::vector<allVars> resultData = tuple2.getData();
    
    EXPECT_EQ(resultBitmap.size(), 3);
    EXPECT_TRUE(resultBitmap[0]);
    EXPECT_FALSE(resultBitmap[1]);
    EXPECT_TRUE(resultBitmap[2]);
    
    EXPECT_EQ(resultData.size(), 3);
    EXPECT_EQ(std::get<int32_t>(resultData[0]), 10);
    EXPECT_EQ(std::get<int32_t>(resultData[1]), 20);
    EXPECT_EQ(std::get<int32_t>(resultData[2]), 30);
}

TEST(DataNullBitMapTupleTest, RoundTripMixedTypes) {
    DataNullBitMapTuple tuple1;
    std::vector<bool> originalBitmap = {true, false, true};
    std::vector<allVars> originalData = {
        int32_t(42), 
        std::string("Hello"), 
        int64_t(100)
    };
    
    std::vector<uint8_t> marshalled = tuple1.marshallDataNullBitMapTuple(originalBitmap, originalData);
    
    DataNullBitMapTuple tuple2;
    tuple2.unmarshallDataNullBitMapTuple(marshalled);
    
    std::vector<bool> resultBitmap = tuple2.getBitMap();
    std::vector<allVars> resultData = tuple2.getData();
    
    EXPECT_EQ(resultBitmap.size(), 3);
    EXPECT_EQ(resultData.size(), 3);
    
    EXPECT_TRUE(std::holds_alternative<int32_t>(resultData[0]));
    EXPECT_EQ(std::get<int32_t>(resultData[0]), 42);
    
    EXPECT_TRUE(std::holds_alternative<std::string>(resultData[1]));
    EXPECT_EQ(std::get<std::string>(resultData[1]), "Hello");
    
    EXPECT_TRUE(std::holds_alternative<int64_t>(resultData[2]));
    EXPECT_EQ(std::get<int64_t>(resultData[2]), 100);
}

TEST(DataNullBitMapTupleTest, RoundTripInt64Values) {
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

TEST(DataNullBitMapTupleTest, RoundTripStringValues) {
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

// ==================== TESTY WARTOŚCI GRANICZNYCH ====================

TEST(DataNullBitMapTupleTest, BoundaryValuesInt32) {
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

TEST(DataNullBitMapTupleTest, BoundaryValuesInt64) {
    DataNullBitMapTuple tuple1;
    std::vector<bool> originalBitmap = {true, true, true};
    std::vector<allVars> originalData = {
        int64_t(0),
        int64_t(std::numeric_limits<int64_t>::max()),
        int64_t(std::numeric_limits<int64_t>::min())
    };
    
    std::vector<uint8_t> marshalled = tuple1.marshallDataNullBitMapTuple(originalBitmap, originalData);
    
    DataNullBitMapTuple tuple2;
    tuple2.unmarshallDataNullBitMapTuple(marshalled);
    
    std::vector<allVars> resultData = tuple2.getData();
    EXPECT_EQ(resultData.size(), 3);
    
    EXPECT_EQ(std::get<int64_t>(resultData[0]), 0);
    EXPECT_EQ(std::get<int64_t>(resultData[1]), std::numeric_limits<int64_t>::max());
    EXPECT_EQ(std::get<int64_t>(resultData[2]), std::numeric_limits<int64_t>::min());
}

// ==================== TESTY DUŻYCH DANYCH ====================

TEST(DataNullBitMapTupleTest, LargeBitmap) {
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

TEST(DataNullBitMapTupleTest, LargeMixedData) {
    DataNullBitMapTuple tuple1;
    std::vector<bool> originalBitmap;
    std::vector<allVars> originalData;
    
    for (int i = 0; i < 50; i++) {
        originalBitmap.push_back(i % 2 == 0);
        if (i % 3 == 0) {
            originalData.push_back(int32_t(i));
        } else if (i % 3 == 1) {
            originalData.push_back(int64_t(i * 1000LL));
        } else {
            originalData.push_back(std::string("str_") + std::to_string(i));
        }
    }
    
    std::vector<uint8_t> marshalled = tuple1.marshallDataNullBitMapTuple(originalBitmap, originalData);
    
    DataNullBitMapTuple tuple2;
    tuple2.unmarshallDataNullBitMapTuple(marshalled);
    
    std::vector<bool> resultBitmap = tuple2.getBitMap();
    std::vector<allVars> resultData = tuple2.getData();
    
    EXPECT_EQ(resultBitmap.size(), 50);
    EXPECT_EQ(resultData.size(), 50);
}

// ==================== TESTY METOD setData/getData ====================

TEST(DataNullBitMapTupleTest, SetDataBasic) {
    DataNullBitMapTuple tuple;
    std::vector<bool> bitmap = {true, false};
    std::vector<allVars> data = {int32_t(10), int32_t(20)};
    
    tuple.setData(bitmap, data);
    
    EXPECT_EQ(tuple.getBitMap().size(), 2);
    EXPECT_EQ(tuple.getData().size(), 2);
    EXPECT_TRUE(tuple.getBitMap()[0]);
    EXPECT_FALSE(tuple.getBitMap()[1]);
}

TEST(DataNullBitMapTupleTest, GetSizeEmpty) {
    DataNullBitMapTuple tuple;
    EXPECT_EQ(tuple.getSize(), 0);
}

TEST(DataNullBitMapTupleTest, GetSizeWithData) {
    DataNullBitMapTuple tuple;
    std::vector<bool> bitmap = {true, true};
    std::vector<allVars> data = {int32_t(10), int32_t(20)};
    
    tuple.setData(bitmap, data);
    
    // 2 (bitmap size) + 4 + 4 (two int32_t values)
    EXPECT_EQ(tuple.getSize(), 10);
}

// ==================== TESTY SPÓJNOŚCI ====================

TEST(DataNullBitMapTupleTest, MarshallConsistency) {
    DataNullBitMapTuple tuple1, tuple2;
    std::vector<bool> bitmap = {true, false, true};
    std::vector<allVars> data = {int32_t(42), std::string("Test"), int32_t(100)};
    
    std::vector<uint8_t> result1 = tuple1.marshallDataNullBitMapTuple(bitmap, data);
    std::vector<uint8_t> result2 = tuple2.marshallDataNullBitMapTuple(bitmap, data);
    
    EXPECT_EQ(result1, result2);
}

TEST(DataNullBitMapTupleTest, MarshallDifferentValues) {
    DataNullBitMapTuple tuple;
    
    std::vector<uint8_t> result1 = tuple.marshallDataNullBitMapTuple(
        {true}, {int32_t(42)}
    );
    
    std::vector<uint8_t> result2 = tuple.marshallDataNullBitMapTuple(
        {true}, {int32_t(43)}
    );
    
    EXPECT_NE(result1, result2);
}

TEST(DataNullBitMapTupleTest, MultipleRoundTrips) {
    for (int iteration = 0; iteration < 5; iteration++) {
        DataNullBitMapTuple tuple1;
        std::vector<bool> bitmap = {true, false, true};
        std::vector<allVars> data = {
            int32_t(iteration), 
            std::string("iter_") + std::to_string(iteration), 
            int64_t(iteration * 1000LL)
        };
        
        std::vector<uint8_t> marshalled = tuple1.marshallDataNullBitMapTuple(bitmap, data);
        
        DataNullBitMapTuple tuple2;
        tuple2.unmarshallDataNullBitMapTuple(marshalled);
        
        EXPECT_EQ(tuple2.getData().size(), 3);
        EXPECT_EQ(std::get<int32_t>(tuple2.getData()[0]), iteration);
    }
}

// ==================== TEST KOMPLEKSOWY ====================

TEST(DataNullBitMapTupleTest, CompleteIntegrationTest) {
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