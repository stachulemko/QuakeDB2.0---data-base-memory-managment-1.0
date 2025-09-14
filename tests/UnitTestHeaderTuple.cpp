#include <gtest/gtest.h>
#include "headerTuple.h"
#include "typesDataConverter.h"
#include <cstring>
#include <limits>
#include <vector>
#include <chrono>

TEST(HeaderTupleTest, DefaultConstructor) {
    HeaderTuple header;
    EXPECT_EQ(header.getTxMin(), 0);
    EXPECT_EQ(header.getTxMax(), 0);
    EXPECT_EQ(header.getTCid(), 0);
    EXPECT_EQ(header.getTInfomask(), 0);
    EXPECT_EQ(header.getTHoff(), 0);
    EXPECT_EQ(header.getNullBitmap(), false);
    EXPECT_EQ(header.getOptionalOid(), 0);
}

TEST(HeaderTupleTest, ParameterizedConstructor) {
    HeaderTuple header(1001, 2002, 100, 0x0001, 24, true, 12345);
    EXPECT_EQ(header.getTxMin(), 1001);
    EXPECT_EQ(header.getTxMax(), 2002);
    EXPECT_EQ(header.getTCid(), 100);
    EXPECT_EQ(header.getTInfomask(), 0x0001);
    EXPECT_EQ(header.getTHoff(), 24);
    EXPECT_EQ(header.getNullBitmap(), true);
    EXPECT_EQ(header.getOptionalOid(), 12345);
}

TEST(HeaderTupleTest, SettersAndGetters) {
    HeaderTuple header;
    header.setTxMin(999);
    header.setTxMax(1000);
    header.setTCid(50);
    header.setTInfomask(0x0002);
    header.setTHoff(28);
    header.setNullBitmap(true);
    header.setOptionalOid(54321);
    
    EXPECT_EQ(header.getTxMin(), 999);
    EXPECT_EQ(header.getTxMax(), 1000);
    EXPECT_EQ(header.getTCid(), 50);
    EXPECT_EQ(header.getTInfomask(), 0x0002);
    EXPECT_EQ(header.getTHoff(), 28);
    EXPECT_EQ(header.getNullBitmap(), true);
    EXPECT_EQ(header.getOptionalOid(), 54321);
}

TEST(HeaderTupleTest, MarshallHeaderTupleBasicValues) {
    HeaderTuple header;
    std::vector<uint8_t> result = header.marshallHeaderTuple(
        1001, 2002, 100, 0x0001, 24, true, 12345
    );
    EXPECT_FALSE(result.empty());
    EXPECT_GE(result.size(), 35);
}

TEST(HeaderTupleTest, MarshallHeaderTupleZeroValues) {
    HeaderTuple header;
    std::vector<uint8_t> result = header.marshallHeaderTuple(
        0, 0, 0, 0, 0, false, 0
    );
    EXPECT_FALSE(result.empty());
    EXPECT_GE(result.size(), 35);
}

TEST(HeaderTupleTest, MarshallHeaderTupleNegativeValues) {
    HeaderTuple header;
    std::vector<uint8_t> result = header.marshallHeaderTuple(
        -1001, -2002, -100, -200, -24, false, -12345
    );
    EXPECT_FALSE(result.empty());
    EXPECT_GE(result.size(), 35);
}

TEST(HeaderTupleTest, MarshallHeaderTupleMaxValues) {
    HeaderTuple header;
    std::vector<uint8_t> result = header.marshallHeaderTuple(
        std::numeric_limits<int64_t>::max(),
        std::numeric_limits<int64_t>::max(),
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int16_t>::max(),
        true,
        std::numeric_limits<int64_t>::max()
    );
    EXPECT_FALSE(result.empty());
    EXPECT_GE(result.size(), 35);
}

TEST(HeaderTupleTest, MarshallHeaderTupleMinValues) {
    HeaderTuple header;
    std::vector<uint8_t> result = header.marshallHeaderTuple(
        std::numeric_limits<int64_t>::min(),
        std::numeric_limits<int64_t>::min(),
        std::numeric_limits<int32_t>::min(),
        std::numeric_limits<int32_t>::min(),
        std::numeric_limits<int16_t>::min(),
        false,
        std::numeric_limits<int64_t>::min()
    );
    EXPECT_FALSE(result.empty());
    EXPECT_GE(result.size(), 35);
}

TEST(HeaderTupleTest, MarshallConsistency) {
    HeaderTuple header;
    std::vector<uint8_t> result1 = header.marshallHeaderTuple(
        1000, 2000, 100, 0x01, 24, true, 12345
    );
    std::vector<uint8_t> result2 = header.marshallHeaderTuple(
        1000, 2000, 100, 0x01, 24, true, 12345
    );
    EXPECT_EQ(result1, result2);
}

TEST(HeaderTupleTest, MarshallDifferentValues) {
    HeaderTuple header;
    std::vector<uint8_t> result1 = header.marshallHeaderTuple(
        1000, 2000, 100, 0x01, 24, true, 12345
    );
    std::vector<uint8_t> result2 = header.marshallHeaderTuple(
        1001, 2000, 100, 0x01, 24, true, 12345
    );
    EXPECT_NE(result1, result2);
}

TEST(HeaderTupleTest, PostgreSQLTransactionTest) {
    HeaderTuple header;
    
    std::vector<uint8_t> created = header.marshallHeaderTuple(
        1001, 0, 1, 0x0000, 24, false, 12345
    );
    
    std::vector<uint8_t> deleted = header.marshallHeaderTuple(
        1001, 1002, 1, 0x0001, 24, false, 12345
    );
    
    EXPECT_NE(created, deleted);
    EXPECT_FALSE(created.empty());
    EXPECT_FALSE(deleted.empty());
}

TEST(HeaderTupleTest, PostgreSQLInfomaskFlags) {
    HeaderTuple header;
    std::vector<int32_t> infomask_flags = {
        0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080
    };
    
    std::vector<std::vector<uint8_t>> results;
    for (int32_t flag : infomask_flags) {
        std::vector<uint8_t> result = header.marshallHeaderTuple(
            1000, 0, 1, flag, 24, false, 12345
        );
        results.push_back(result);
    }
    
    for (size_t i = 0; i < results.size(); i++) {
        for (size_t j = i + 1; j < results.size(); j++) {
            EXPECT_NE(results[i], results[j]);
        }
    }
}

TEST(HeaderTupleTest, PostgreSQLHeaderOffset) {
    HeaderTuple header;
    std::vector<int16_t> header_offsets = {24, 28, 32, 40, 48};
    
    for (int16_t hoff : header_offsets) {
        std::vector<uint8_t> result = header.marshallHeaderTuple(
            1000, 0, 1, 0x01, hoff, false, 12345
        );
        EXPECT_FALSE(result.empty());
        EXPECT_GE(result.size(), 35);
    }
}

TEST(HeaderTupleTest, EmptyVectorUnmarshall) {
    HeaderTuple header;
    std::vector<uint8_t> empty_vector;
    header.unmarshallHeaderTuple(empty_vector);
    
    EXPECT_EQ(header.getTxMin(), 0);
    EXPECT_EQ(header.getTxMax(), 0);
    EXPECT_EQ(header.getTCid(), 0);
    EXPECT_EQ(header.getTInfomask(), 0);
    EXPECT_EQ(header.getTHoff(), 0);
    EXPECT_EQ(header.getNullBitmap(), false);
    EXPECT_EQ(header.getOptionalOid(), 0);
}

TEST(HeaderTupleTest, PerformanceTest) {
    HeaderTuple header;
    const int iterations = 10000;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        std::vector<uint8_t> result = header.marshallHeaderTuple(
            1000 + i, 2000 + i, 100 + i, 0x01, 24, (i % 2 == 0), 12345 + i
        );
        volatile size_t dummy = result.size();
        (void)dummy;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    EXPECT_LT(duration.count(), 1000000);
}

TEST(HeaderTupleTest, MarshallSizeConsistency) {
    HeaderTuple header;
    
    std::vector<uint8_t> result1 = header.marshallHeaderTuple(0, 0, 0, 0, 0, false, 0);
    std::vector<uint8_t> result2 = header.marshallHeaderTuple(1000, 2000, 100, 0x01, 24, true, 12345);
    
    EXPECT_EQ(result1.size(), result2.size());
    EXPECT_FALSE(result1.empty());
    EXPECT_FALSE(result2.empty());
}

TEST(HeaderTupleTest, BooleanFlagTest) {
    HeaderTuple header;
    
    std::vector<uint8_t> result_true = header.marshallHeaderTuple(
        1000, 2000, 100, 0x01, 24, true, 12345
    );
    
    std::vector<uint8_t> result_false = header.marshallHeaderTuple(
        1000, 2000, 100, 0x01, 24, false, 12345
    );
    
    EXPECT_NE(result_true, result_false);
    EXPECT_EQ(result_true.size(), result_false.size());
}

TEST(HeaderTupleTest, DataIntegrity) {
    HeaderTuple header;
    
    std::vector<uint8_t> result = header.marshallHeaderTuple(
        0x123456789ABCDEF0LL, 0x0FEDCBA987654321LL, 0x12345678, 0x87654321, 0x1234, true, 0xABCDEF0123456789LL
    );
    
    EXPECT_FALSE(result.empty());
    EXPECT_GE(result.size(), 35);
    
    bool has_non_zero = false;
    for (uint8_t byte : result) {
        if (byte != 0) {
            has_non_zero = true;
            break;
        }
    }
    EXPECT_TRUE(has_non_zero);
}

TEST(HeaderTupleTest, BoundaryValuesBasic) {
    HeaderTuple header;
    
    std::vector<uint8_t> result_min = header.marshallHeaderTuple(
        std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::min(),
        std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::min(),
        std::numeric_limits<int16_t>::min(), false, std::numeric_limits<int64_t>::min()
    );
    
    std::vector<uint8_t> result_max = header.marshallHeaderTuple(
        std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::max(),
        std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int16_t>::max(), true, std::numeric_limits<int64_t>::max()
    );
    
    EXPECT_FALSE(result_min.empty());
    EXPECT_FALSE(result_max.empty());
    EXPECT_EQ(result_min.size(), result_max.size());
    EXPECT_NE(result_min, result_max);
}

TEST(HeaderTupleTest, ComprehensiveBasic) {
    HeaderTuple header(1001, 0, 1, 0x0001, 24, true, 98765);
    
    std::vector<uint8_t> result = header.marshallHeaderTuple(
        header.getTxMin(), header.getTxMax(), header.getTCid(),
        header.getTInfomask(), header.getTHoff(),
        header.getNullBitmap(), header.getOptionalOid()
    );
    
    EXPECT_FALSE(result.empty());
    EXPECT_GE(result.size(), 35);
}