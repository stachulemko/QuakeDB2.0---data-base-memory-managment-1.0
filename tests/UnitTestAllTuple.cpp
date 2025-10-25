#include <gtest/gtest.h>
#include "allTuple.h"
#include <vector>
#include <limits>
#include <chrono>

// ==================== TESTY KONSTRUKTORÓW ====================

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
    EXPECT_GT(tpl.getSize(), 0); // Powinien mieć przynajmniej rozmiar header
}

TEST(TupleTest, ConstructorInitialization) {
    tuple tpl;
    
    // Test że wszystkie gettery działają bez rzucania wyjątków
    EXPECT_NO_THROW(tpl.getTxMin());
    EXPECT_NO_THROW(tpl.getTxMax());
    EXPECT_NO_THROW(tpl.getTCid());
    EXPECT_NO_THROW(tpl.getTInfomask());
    EXPECT_NO_THROW(tpl.getTHoff());
    EXPECT_NO_THROW(tpl.getNullBitmap());
    EXPECT_NO_THROW(tpl.getOptionalOid());
    EXPECT_NO_THROW(tpl.getBitMap());
    EXPECT_NO_THROW(tpl.getData());
    EXPECT_NO_THROW(tpl.getSize());
}

// ==================== TESTY METODY setData ====================

TEST(TupleTest, SetDataBasicValues) {
    tuple tpl;
    
    std::vector<bool> bitMap = {true, false, true};
    std::vector<allVars> data = {allVars(42), allVars(84), allVars(126)};
    
    EXPECT_NO_THROW(tpl.setData(100, 200, 300, 400, 500, true, 600, bitMap, data));
    
    EXPECT_EQ(tpl.getTxMin(), 100);
    EXPECT_EQ(tpl.getTxMax(), 200);
    EXPECT_EQ(tpl.getTCid(), 300);
    EXPECT_EQ(tpl.getTInfomask(), 400);
    EXPECT_EQ(tpl.getTHoff(), 500);
    EXPECT_TRUE(tpl.getNullBitmap());
    EXPECT_EQ(tpl.getOptionalOid(), 600);
    
    EXPECT_EQ(tpl.getBitMap().size(), 3);
    EXPECT_EQ(tpl.getData().size(), 3);
    EXPECT_TRUE(tpl.getBitMap()[0]);
    EXPECT_FALSE(tpl.getBitMap()[1]);
    EXPECT_TRUE(tpl.getBitMap()[2]);
}

TEST(TupleTest, SetDataZeroValues) {
    tuple tpl;
    
    std::vector<bool> emptyBitMap;
    std::vector<allVars> emptyData;
    
    EXPECT_NO_THROW(tpl.setData(0, 0, 0, 0, 0, false, 0, emptyBitMap, emptyData));
    
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

TEST(TupleTest, SetDataMaxValues) {
    tuple tpl;
    
    std::vector<bool> bitMap = {true, true, true};
    std::vector<allVars> data = {
        allVars(std::numeric_limits<int32_t>::max()),
        allVars(std::numeric_limits<int64_t>::max()),
        allVars(std::string("max_test"))
    };
    
    EXPECT_NO_THROW(tpl.setData(
        std::numeric_limits<int64_t>::max(),
        std::numeric_limits<int64_t>::max(),
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int16_t>::max(),
        true,
        std::numeric_limits<int64_t>::max(),
        bitMap,
        data
    ));
    
    EXPECT_EQ(tpl.getTxMin(), std::numeric_limits<int64_t>::max());
    EXPECT_EQ(tpl.getTxMax(), std::numeric_limits<int64_t>::max());
    EXPECT_EQ(tpl.getTCid(), std::numeric_limits<int32_t>::max());
    EXPECT_EQ(tpl.getTInfomask(), std::numeric_limits<int32_t>::max());
    EXPECT_EQ(tpl.getTHoff(), std::numeric_limits<int16_t>::max());
    EXPECT_TRUE(tpl.getNullBitmap());
    EXPECT_EQ(tpl.getOptionalOid(), std::numeric_limits<int64_t>::max());
}

TEST(TupleTest, SetDataNegativeValues) {
    tuple tpl;
    
    std::vector<bool> bitMap = {false, true, false};
    std::vector<allVars> data = {allVars(-100), allVars(-200), allVars(-300)};
    
    EXPECT_NO_THROW(tpl.setData(-1000, -2000, -3000, -4000, -5000, false, -6000, bitMap, data));
    
    EXPECT_EQ(tpl.getTxMin(), -1000);
    EXPECT_EQ(tpl.getTxMax(), -2000);
    EXPECT_EQ(tpl.getTCid(), -3000);
    EXPECT_EQ(tpl.getTInfomask(), -4000);
    EXPECT_EQ(tpl.getTHoff(), -5000);
    EXPECT_FALSE(tpl.getNullBitmap());
    EXPECT_EQ(tpl.getOptionalOid(), -6000);
}

// ==================== TESTY MARSHALLING ====================

TEST(TupleTest, MarshallBasicValues) {
    tuple tpl;
    
    std::vector<bool> bitMap = {true, false, true};
    std::vector<allVars> data = {allVars(42), allVars(84), allVars(126)};
    
    std::vector<uint8_t> result = tpl.marshallTuple(
        100, 200, 300, 400, 500, true, 600, bitMap, data
    );
    
    EXPECT_FALSE(result.empty());
    EXPECT_GE(result.size(), 2); // Przynajmniej identyfikator typu
}

TEST(TupleTest, MarshallWithSetData) {
    tuple tpl;
    
    std::vector<bool> bitMap = {true, false};
    std::vector<allVars> data = {allVars(123), allVars(std::string("test"))};
    
    tpl.setData(100, 200, 300, 400, 500, true, 600, bitMap, data);
    
    std::vector<uint8_t> result = tpl.marshallTupleWithData();
    
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

TEST(TupleTest, MarshallEmptyData) {
    tuple tpl;
    
    std::vector<bool> emptyBitMap;
    std::vector<allVars> emptyData;
    
    std::vector<uint8_t> result = tpl.marshallTuple(100, 200, 300, 400, 500, false, 600, emptyBitMap, emptyData);
    
    EXPECT_FALSE(result.empty());
    EXPECT_GE(result.size(), 2);
}

// ==================== TESTY UNMARSHALLING ====================

TEST(TupleTest, UnmarshallEmptyVector) {
    tuple tpl;
    std::vector<uint8_t> empty;
    
    EXPECT_NO_THROW(tpl.unmarshallTuple(empty));
    
    // Po unmarshall pustego wektora, wartości powinny pozostać niezmienione
    EXPECT_EQ(tpl.getTxMin(), 0);
    EXPECT_EQ(tpl.getTxMax(), 0);
    EXPECT_EQ(tpl.getTCid(), 0);
}

TEST(TupleTest, UnmarshallTooSmall) {
    tuple tpl;
    std::vector<uint8_t> tooSmall(1, 0x42);
    
    EXPECT_NO_THROW(tpl.unmarshallTuple(tooSmall));
    
    // Dane powinny pozostać niezmienione
    EXPECT_EQ(tpl.getTxMin(), 0);
    EXPECT_EQ(tpl.getTxMax(), 0);
    EXPECT_EQ(tpl.getTCid(), 0);
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
    wrongType[0] = 0xFF; // Nieprawidłowy identyfikator
    wrongType[1] = 0xFF;
    
    EXPECT_NO_THROW(tpl.unmarshallTuple(wrongType));
    
    // Dane powinny pozostać niezmienione
    EXPECT_EQ(tpl.getTxMin(), 0);
    EXPECT_EQ(tpl.getTxMax(), 0);
    EXPECT_EQ(tpl.getTCid(), 0);
}

// ==================== TESTY ROUND-TRIP ====================

TEST(TupleTest, RoundTripBasic) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {true, false, true};
    std::vector<allVars> data = {allVars(123), allVars(456), allVars(789)};
    
    // Ustaw dane w pierwszym obiekcie
    tpl1.setData(1000, 2000, 3000, 4000, 5000, true, 6000, bitMap, data);
    
    // Marshall z pierwszego obiektu
    std::vector<uint8_t> marshalled = tpl1.marshallTupleWithData();
    EXPECT_FALSE(marshalled.empty());
    
    // Unmarshall do drugiego obiektu
    EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled));
    
    // Sprawdź czy dane zostały przeniesione
    EXPECT_EQ(tpl2.getTxMin(), 1000);
    EXPECT_EQ(tpl2.getTxMax(), 2000);
    EXPECT_EQ(tpl2.getTCid(), 3000);
    EXPECT_EQ(tpl2.getTInfomask(), 4000);
    EXPECT_EQ(tpl2.getTHoff(), 5000);
    EXPECT_TRUE(tpl2.getNullBitmap());
    EXPECT_EQ(tpl2.getOptionalOid(), 6000);
    
    EXPECT_EQ(tpl2.getBitMap().size(), 3);
    EXPECT_EQ(tpl2.getData().size(), 3);
}

TEST(TupleTest, RoundTripWithDirectMarshall) {
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
        1000, 2000, 3000, 4000, 5000, true, 6000, bitMap, data
    );
    
    EXPECT_FALSE(marshalled.empty());
    
    EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled));
    
    // Test podstawowych wartości
    EXPECT_EQ(tpl2.getTxMin(), 1000);
    EXPECT_EQ(tpl2.getTxMax(), 2000);
    EXPECT_EQ(tpl2.getTCid(), 3000);
    EXPECT_EQ(tpl2.getTInfomask(), 4000);
    EXPECT_EQ(tpl2.getTHoff(), 5000);
    EXPECT_TRUE(tpl2.getNullBitmap());
    EXPECT_EQ(tpl2.getOptionalOid(), 6000);
}

TEST(TupleTest, RoundTripConsistency) {
    tuple tpl1;
    tuple tpl2;
    tuple tpl3;
    
    std::vector<bool> bitMap = {true, false};
    std::vector<allVars> data = {allVars(999), allVars(std::string("consistency"))};
    
    tpl1.setData(777, 888, 999, 111, 222, true, 333, bitMap, data);
    
    std::vector<uint8_t> marshalled1 = tpl1.marshallTupleWithData();
    
    tpl2.unmarshallTuple(marshalled1);
    std::vector<uint8_t> marshalled2 = tpl2.marshallTupleWithData();
    
    tpl3.unmarshallTuple(marshalled2);
    
    // Sprawdź czy dane są spójne po wielokrotnych round-tripach
    EXPECT_EQ(tpl3.getTxMin(), 777);
    EXPECT_EQ(tpl3.getTxMax(), 888);
    EXPECT_EQ(tpl3.getTCid(), 999);
    EXPECT_EQ(tpl3.getTInfomask(), 111);
    EXPECT_EQ(tpl3.getTHoff(), 222);
    EXPECT_TRUE(tpl3.getNullBitmap());
    EXPECT_EQ(tpl3.getOptionalOid(), 333);
    
    EXPECT_EQ(marshalled1, marshalled2);
}

// ==================== TESTY METODY getSize ====================

TEST(TupleTest, GetSizeEmpty) {
    tuple tpl;
    
    int32_t size = tpl.getSize();
    EXPECT_GT(size, 0); // Powinien mieć przynajmniej rozmiar header
}

TEST(TupleTest, GetSizeWithData) {
    tuple tpl;
    
    std::vector<bool> bitMap = {true, false, true};
    std::vector<allVars> data = {allVars(42), allVars(84), allVars(126)};
    
    int32_t sizeBefore = tpl.getSize();
    tpl.setData(100, 200, 300, 400, 500, true, 600, bitMap, data);
    int32_t sizeAfter = tpl.getSize();
    
    EXPECT_GE(sizeAfter, sizeBefore); // Rozmiar powinien się zwiększyć lub pozostać taki sam
}

TEST(TupleTest, GetSizeConsistency) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {true, false};
    std::vector<allVars> data = {allVars(123), allVars(456)};
    
    tpl1.setData(100, 200, 300, 400, 500, true, 600, bitMap, data);
    tpl2.setData(100, 200, 300, 400, 500, true, 600, bitMap, data);
    
    EXPECT_EQ(tpl1.getSize(), tpl2.getSize());
}

// ==================== TESTY METODY showData ====================

TEST(TupleTest, ShowDataMethod) {
    tuple tpl;
    
    std::vector<bool> bitMap = {true, false};
    std::vector<allVars> data = {allVars(123), allVars(std::string("test"))};
    
    tpl.setData(100, 200, 300, 400, 500, true, 600, bitMap, data);
    
    // Test że metoda nie rzuca wyjątków
    EXPECT_NO_THROW(tpl.showData());
}

// ==================== TESTY WYDAJNOŚCI ====================

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
    
    EXPECT_LT(duration.count(), 100000); // Mniej niż 100ms
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
    
    EXPECT_LT(duration.count(), 50000); // Mniej niż 50ms
}

// ==================== TESTY ZARZĄDZANIA PAMIĘCIĄ ====================

TEST(TupleTest, DestructorTest) {
    for (int i = 0; i < 10; i++) {
        tuple* tpl1 = new tuple();
        tuple* tpl2 = new tuple();
        
        std::vector<bool> bitMap = {true};
        std::vector<allVars> data = {allVars(i)};
        
        tpl1->setData(i, i+1, i+2, i+3, i+4, true, i+5, bitMap, data);
        std::vector<uint8_t> marshalled = tpl1->marshallTupleWithData();
        tpl2->unmarshallTuple(marshalled);
        
        delete tpl1;
        delete tpl2; // Test destruktora
    }
}

TEST(TupleTest, VectorOfTuples) {
    std::vector<tuple> tuples(5);
    
    for (int i = 0; i < 5; i++) {
        std::vector<bool> bitMap = {i % 2 == 0};
        std::vector<allVars> data = {allVars(i * 100)};
        
        tuples[i].setData(i, i+1, i+2, i+3, i+4, i%2==0, i+5, bitMap, data);
        
        std::vector<uint8_t> result = tuples[i].marshallTupleWithData();
        EXPECT_FALSE(result.empty());
    }
}

// ==================== TESTY KOMPLEKSOWYCH DANYCH ====================

TEST(TupleTest, ComplexDataTypes) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap = {true, false, true, false, true};
    std::vector<allVars> data = {
        allVars(int32_t(42)),
        allVars(int64_t(9876543210LL)),
        allVars(std::string("Complex test string")),
        allVars(int32_t(-123456)),
        allVars(std::string("Another string"))
    };
    
    tpl1.setData(999, 888, 777, 666, 555, true, 444, bitMap, data);
    
    std::vector<uint8_t> marshalled = tpl1.marshallTupleWithData();
    EXPECT_FALSE(marshalled.empty());
    
    EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled));
    
    EXPECT_EQ(tpl2.getBitMap().size(), 5);
    EXPECT_EQ(tpl2.getData().size(), 5);
}

TEST(TupleTest, StressTestLargeData) {
    tuple tpl1;
    tuple tpl2;
    
    std::vector<bool> bitMap;
    std::vector<allVars> data;
    
    // Dodaj dużo danych
    for (int i = 0; i < 20; i++) {
        bitMap.push_back(i % 2 == 0);
        if (i % 3 == 0) {
            data.push_back(allVars(i * 100));
        } else if (i % 3 == 1) {
            data.push_back(allVars(static_cast<int64_t>(i * 1000)));
        } else {
            data.push_back(allVars(std::string("item_" + std::to_string(i))));
        }
    }
    
    tpl1.setData(10000, 20000, 30000, 40000, 1000, true, 60000, bitMap, data);
    
    std::vector<uint8_t> marshalled = tpl1.marshallTupleWithData();
    EXPECT_FALSE(marshalled.empty());
    
    EXPECT_NO_THROW(tpl2.unmarshallTuple(marshalled));
    
    EXPECT_EQ(tpl2.getBitMap().size(), 20);
    EXPECT_EQ(tpl2.getData().size(), 20);
}

// ==================== TESTY WARTOŚCI GRANICZNYCH ====================

TEST(TupleTest, BoundaryValues) {
    tuple tpl;
    
    std::vector<std::tuple<int64_t, int64_t, int32_t, int32_t, int16_t, bool, int64_t>> testCases = {
        {0, 0, 0, 0, 0, false, 0},
        {std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::min(), 
         std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::min(),
         std::numeric_limits<int16_t>::max(), true, std::numeric_limits<int64_t>::max()},
        {-1000, -2000, -3000, -4000, -5000, false, -6000},
        {1, 2, 3, 4, 5, true, 6}
    };
    
    std::vector<bool> bitMap = {true, false};
    std::vector<allVars> data = {allVars(42), allVars(std::string("test"))};
    
    for (const auto& testCase : testCases) {
        EXPECT_NO_THROW(tpl.setData(
            std::get<0>(testCase), std::get<1>(testCase), std::get<2>(testCase),
            std::get<3>(testCase), std::get<4>(testCase), std::get<5>(testCase),
            std::get<6>(testCase), bitMap, data
        ));
        
        std::vector<uint8_t> result = tpl.marshallTupleWithData();
        EXPECT_FALSE(result.empty());
        
        tuple testTuple;
        EXPECT_NO_THROW(testTuple.unmarshallTuple(result));
    }
}

// ==================== TEST INTEGRACYJNY ====================

TEST(TupleTest, FullIntegrationTest) {
    tuple originalTuple;
    tuple reconstructedTuple;
    
    // Przygotuj kompleksowe dane
    std::vector<bool> bitMap = {true, false, true, false, true, true};
    std::vector<allVars> data = {
        allVars(int32_t(12345)),
        allVars(int64_t(-9876543210LL)),
        allVars(std::string("Integration test string")),
        allVars(int32_t(0)),
        allVars(int64_t(std::numeric_limits<int64_t>::max())),
        allVars(std::string(""))
    };
    
    // Ustaw dane w oryginalnym tuple
    originalTuple.setData(
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
    
    // Test getterów
    EXPECT_EQ(originalTuple.getTxMin(), 0x123456789ABCDEFLL);
    EXPECT_EQ(originalTuple.getTxMax(), -0x123456789ABCDEFLL);
    EXPECT_EQ(originalTuple.getTCid(), 0x12345678);
    EXPECT_EQ(originalTuple.getTInfomask(), -0x12345678);
    EXPECT_EQ(originalTuple.getTHoff(), 0x1234);
    EXPECT_TRUE(originalTuple.getNullBitmap());
    EXPECT_EQ(originalTuple.getOptionalOid(), 0xFEDCBA9876543210LL);
    
    EXPECT_EQ(originalTuple.getBitMap().size(), 6);
    EXPECT_EQ(originalTuple.getData().size(), 6);
    
    // Test rozmiaru
    EXPECT_GT(originalTuple.getSize(), 0);
    
    // Test marshalling
    std::vector<uint8_t> marshalled = originalTuple.marshallTupleWithData();
    EXPECT_FALSE(marshalled.empty());
    
    // Test unmarshalling
    EXPECT_NO_THROW(reconstructedTuple.unmarshallTuple(marshalled));
    
    // Test że dane zostały poprawnie przeniesione
    EXPECT_EQ(reconstructedTuple.getTxMin(), 0x123456789ABCDEFLL);
    EXPECT_EQ(reconstructedTuple.getTxMax(), -0x123456789ABCDEFLL);
    EXPECT_EQ(reconstructedTuple.getTCid(), 0x12345678);
    EXPECT_EQ(reconstructedTuple.getTInfomask(), -0x12345678);
    EXPECT_EQ(reconstructedTuple.getTHoff(), 0x1234);
    EXPECT_TRUE(reconstructedTuple.getNullBitmap());
    EXPECT_EQ(reconstructedTuple.getOptionalOid(), 0xFEDCBA9876543210LL);
    
    EXPECT_EQ(reconstructedTuple.getBitMap().size(), 6);
    EXPECT_EQ(reconstructedTuple.getData().size(), 6);
    
    // Test bitMap
    for (size_t i = 0; i < 6; i++) {
        EXPECT_EQ(reconstructedTuple.getBitMap()[i], bitMap[i]) << "BitMap mismatch at index " << i;
    }
    
    // Test że można ponownie zmarshallować
    std::vector<uint8_t> remarshalled = reconstructedTuple.marshallTupleWithData();
    EXPECT_EQ(remarshalled.size(), marshalled.size());
    
    // Test metod pomocniczych
    EXPECT_NO_THROW(originalTuple.showData());
    EXPECT_NO_THROW(reconstructedTuple.showData());
}