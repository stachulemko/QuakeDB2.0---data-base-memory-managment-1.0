#include <gtest/gtest.h>
#include "blockHeader.h"
#include <vector>
#include <limits>
#include <chrono>

// ==================== TESTY KONSTRUKTORÓW ====================

TEST(BlockHeaderTest, DefaultConstructor) {
    blockHeader header;
    
    EXPECT_EQ(header.getNextblock(), 0);
    EXPECT_EQ(header.getBlockIndetification(), 0);
    EXPECT_EQ(header.getPdLsn(), 0);
    EXPECT_EQ(header.getPdChecksum(), 0);
    EXPECT_EQ(header.getPdFlags(), 0);
    EXPECT_EQ(header.getContainToast(), 0);
    EXPECT_EQ(header.getSize(), 17);
}

// ==================== TESTY METODY setData ====================

TEST(BlockHeaderTest, SetDataBasic) {
    blockHeader header;
    
    header.setData(100, 200, 300, 400, 500, 1);
    
    EXPECT_EQ(header.getNextblock(), 100);
    EXPECT_EQ(header.getBlockIndetification(), 200);
    EXPECT_EQ(header.getPdLsn(), 300);
    EXPECT_EQ(header.getPdChecksum(), 400);
    EXPECT_EQ(header.getPdFlags(), 500);
    EXPECT_EQ(header.getContainToast(), 1);
}

TEST(BlockHeaderTest, SetDataOverwrite) {
    blockHeader header;
    
    // Pierwsze ustawienie
    header.setData(10, 20, 30, 40, 50, 1);
    EXPECT_EQ(header.getNextblock(), 10);
    EXPECT_EQ(header.getBlockIndetification(), 20);
    
    // Nadpisanie
    header.setData(100, 200, 300, 400, 500, 0);
    EXPECT_EQ(header.getNextblock(), 100);
    EXPECT_EQ(header.getBlockIndetification(), 200);
    EXPECT_EQ(header.getPdLsn(), 300);
    EXPECT_EQ(header.getPdChecksum(), 400);
    EXPECT_EQ(header.getPdFlags(), 500);
    EXPECT_EQ(header.getContainToast(), 0);
}

TEST(BlockHeaderTest, SetDataZeroValues) {
    blockHeader header;
    
    header.setData(0, 0, 0, 0, 0, 0);
    
    EXPECT_EQ(header.getNextblock(), 0);
    EXPECT_EQ(header.getBlockIndetification(), 0);
    EXPECT_EQ(header.getPdLsn(), 0);
    EXPECT_EQ(header.getPdChecksum(), 0);
    EXPECT_EQ(header.getPdFlags(), 0);
    EXPECT_EQ(header.getContainToast(), 0);
}

TEST(BlockHeaderTest, SetDataNegativeValues) {
    blockHeader header;
    
    header.setData(-100, -200, -300, -400, -500, -1);
    
    EXPECT_EQ(header.getNextblock(), -100);
    EXPECT_EQ(header.getBlockIndetification(), -200);
    EXPECT_EQ(header.getPdLsn(), -300);
    EXPECT_EQ(header.getPdChecksum(), -400);
    EXPECT_EQ(header.getPdFlags(), -500);
    EXPECT_EQ(header.getContainToast(), -1);
}

// ==================== TESTY MARSHALLING ====================

TEST(BlockHeaderTest, MarshallBasicValues) {
    blockHeader header;
    
    std::vector<uint8_t> result = header.marshallBlockHeader(100, 200, 300, 400, 500, 1);
    
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

TEST(BlockHeaderTest, MarshallWithDataMethod) {
    blockHeader header;
    
    header.setData(123, 456, 789, 100, 200, 1);
    std::vector<uint8_t> result1 = header.marshallBlockHeaderWithData();
    std::vector<uint8_t> result2 = header.marshallBlockHeader(123, 456, 789, 100, 200, 1);
    
    EXPECT_EQ(result1, result2);
    EXPECT_EQ(result1.size(), 17);
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

// ==================== TESTY UNMARSHALLING ====================

TEST(BlockHeaderTest, UnmarshallEmptyVector) {
    blockHeader header;
    header.setData(123, 456, 789, 100, 200, 1); // Ustaw jakieś dane
    
    std::vector<uint8_t> empty;
    header.unmarshallBlockHeader(empty);
    
    // Dane powinny zostać niezmienione
    EXPECT_EQ(header.getNextblock(), 123);
    EXPECT_EQ(header.getBlockIndetification(), 456);
    EXPECT_EQ(header.getPdLsn(), 789);
}

TEST(BlockHeaderTest, UnmarshallTooSmall) {
    blockHeader header;
    header.setData(123, 456, 789, 100, 200, 1); // Ustaw jakieś dane
    
    std::vector<uint8_t> tooSmall(16, 0x42);
    header.unmarshallBlockHeader(tooSmall);
    
    // Dane powinny zostać niezmienione
    EXPECT_EQ(header.getNextblock(), 123);
    EXPECT_EQ(header.getBlockIndetification(), 456);
    EXPECT_EQ(header.getPdLsn(), 789);
}

TEST(BlockHeaderTest, UnmarshallMinimumSize) {
    blockHeader header1;
    blockHeader header2;
    
    std::vector<uint8_t> marshalled = header1.marshallBlockHeader(123, 456, 789, 100, 200, 1);
    
    EXPECT_EQ(marshalled.size(), 17);
    header2.unmarshallBlockHeader(marshalled);
    
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
    marshalled.resize(50, 0xFF); // Dodaj więcej danych
    
    header2.unmarshallBlockHeader(marshalled);
    
    EXPECT_EQ(header2.getNextblock(), 123);
    EXPECT_EQ(header2.getBlockIndetification(), 456);
    EXPECT_EQ(header2.getPdLsn(), 789);
}

// ==================== TESTY ROUND-TRIP ====================

TEST(BlockHeaderTest, MarshallUnmarshallRoundTripBasic) {
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

TEST(BlockHeaderTest, MarshallUnmarshallRoundTripZero) {
    blockHeader header1;
    blockHeader header2;
    
    std::vector<uint8_t> marshalled = header1.marshallBlockHeader(0, 0, 0, 0, 0, 0);
    header2.unmarshallBlockHeader(marshalled);
    
    EXPECT_EQ(header2.getNextblock(), 0);
    EXPECT_EQ(header2.getBlockIndetification(), 0);
    EXPECT_EQ(header2.getPdLsn(), 0);
    EXPECT_EQ(header2.getPdChecksum(), 0);
    EXPECT_EQ(header2.getPdFlags(), 0);
    EXPECT_EQ(header2.getContainToast(), 0);
}

TEST(BlockHeaderTest, MarshallUnmarshallRoundTripNegative) {
    blockHeader header1;
    blockHeader header2;
    
    std::vector<uint8_t> marshalled = header1.marshallBlockHeader(-100, -200, -300, -400, -500, -1);
    header2.unmarshallBlockHeader(marshalled);
    
    EXPECT_EQ(header2.getNextblock(), -100);
    EXPECT_EQ(header2.getBlockIndetification(), -200);
    EXPECT_EQ(header2.getPdLsn(), -300);
    EXPECT_EQ(header2.getPdChecksum(), -400);
    EXPECT_EQ(header2.getPdFlags(), -500);
    EXPECT_EQ(header2.getContainToast(), -1);
}

TEST(BlockHeaderTest, MarshallUnmarshallRoundTripBoundary) {
    blockHeader header1;
    blockHeader header2;
    
    std::vector<uint8_t> marshalled = header1.marshallBlockHeader(
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int32_t>::min(),
        0,
        std::numeric_limits<int16_t>::max(),
        std::numeric_limits<int16_t>::min(),
        std::numeric_limits<int8_t>::max()
    );
    
    header2.unmarshallBlockHeader(marshalled);
    
    EXPECT_EQ(header2.getNextblock(), std::numeric_limits<int32_t>::max());
    EXPECT_EQ(header2.getBlockIndetification(), std::numeric_limits<int32_t>::min());
    EXPECT_EQ(header2.getPdLsn(), 0);
    EXPECT_EQ(header2.getPdChecksum(), std::numeric_limits<int16_t>::max());
    EXPECT_EQ(header2.getPdFlags(), std::numeric_limits<int16_t>::min());
    EXPECT_EQ(header2.getContainToast(), std::numeric_limits<int8_t>::max());
}

// ==================== TESTY SPÓJNOŚCI DANYCH ====================

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
    EXPECT_EQ(header2.getBlockIndetification(), static_cast<int32_t>(0x9ABCDEF0));
    EXPECT_EQ(header2.getPdLsn(), 0x11223344);
    EXPECT_EQ(header2.getPdChecksum(), 0x5566);
    EXPECT_EQ(header2.getPdFlags(), 0x7788);
    EXPECT_EQ(header2.getContainToast(), static_cast<int8_t>(0x99));
}

TEST(BlockHeaderTest, MultipleMarshallConsistency) {
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

TEST(BlockHeaderTest, SetDataMarshallConsistency) {
    blockHeader header1;
    blockHeader header2;
    
    header1.setData(42, 123, 456, 789, 101, 1);
    
    std::vector<uint8_t> result1 = header1.marshallBlockHeaderWithData();
    std::vector<uint8_t> result2 = header2.marshallBlockHeader(42, 123, 456, 789, 101, 1);
    
    EXPECT_EQ(result1, result2);
}

// ==================== TESTY WYDAJNOŚCI ====================


TEST(BlockHeaderTest, PerformanceUnmarshall) {
    const int iterations = 10000;
    blockHeader header1;
    blockHeader header2;
    
    std::vector<uint8_t> data = header1.marshallBlockHeader(42, 123, 456, 789, 101, 1);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        header2.unmarshallBlockHeader(data);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    EXPECT_LT(duration.count(), 50000); // Mniej niż 50ms dla 10k operacji
}

// ==================== TESTY WIELOKROTNEGO UŻYCIA ====================

TEST(BlockHeaderTest, ConsecutiveOperations) {
    blockHeader header1;
    blockHeader header2;
    
    for (int i = 0; i < 100; i++) {
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

TEST(BlockHeaderTest, ReuseHeaderObject) {
    blockHeader header;
    
    // Pierwsze użycie
    header.setData(1, 2, 3, 4, 5, 1);
    std::vector<uint8_t> result1 = header.marshallBlockHeaderWithData();
    
    // Drugie użycie
    header.setData(10, 20, 30, 40, 50, 0);
    std::vector<uint8_t> result2 = header.marshallBlockHeaderWithData();
    
    EXPECT_NE(result1, result2);
    EXPECT_EQ(result1.size(), result2.size());
}

// ==================== TESTY ZARZĄDZANIA PAMIĘCIĄ ====================

TEST(BlockHeaderTest, MemoryManagement) {
    for (int i = 0; i < 1000; i++) {
        blockHeader* header1 = new blockHeader();
        blockHeader* header2 = new blockHeader();
        
        std::vector<uint8_t> marshalled = header1->marshallBlockHeader(i, i+1, i+2, i+3, i+4, i%2);
        header2->unmarshallBlockHeader(marshalled);
        
        EXPECT_EQ(header2->getNextblock(), i);
        
        delete header1;
        delete header2;
    }
}

TEST(BlockHeaderTest, VectorOfHeaders) {
    std::vector<blockHeader> headers(100);
    blockHeader sourceHeader;
    
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

// ==================== TESTY RÓŻNYCH ROZMIARÓW DANYCH ====================

TEST(BlockHeaderTest, BoundaryDataSizes) {
    blockHeader header;
    
    std::vector<size_t> testSizes = {0, 1, 16, 17, 18, 25, 50, 100, 1000};
    
    for (size_t size : testSizes) {
        std::vector<uint8_t> testData(size, 0x33);
        
        EXPECT_NO_THROW(header.unmarshallBlockHeader(testData))
            << "Failed with size: " << size;
    }
}

TEST(BlockHeaderTest, CorruptedData) {
    blockHeader header1;
    blockHeader header2;
    
    std::vector<uint8_t> original = header1.marshallBlockHeader(123, 456, 789, 100, 200, 1);
    
    // Uszkodź dane
    for (size_t i = 0; i < original.size(); i++) {
        std::vector<uint8_t> corrupted = original;
        corrupted[i] = ~corrupted[i]; // Odwróć bity
        
        EXPECT_NO_THROW(header2.unmarshallBlockHeader(corrupted));
    }
}

// ==================== TEST METODY showData ====================

TEST(BlockHeaderTest, ShowDataMethod) {
    blockHeader header;
    header.setData(123, 456, 789, 100, 200, 1);
    
    // Test że metoda nie rzuca wyjątków
    EXPECT_NO_THROW(header.showData());
}

// ==================== KOMPLEKSOWY TEST INTEGRACYJNY ====================

TEST(BlockHeaderTest, CompleteIntegrationTest) {
    blockHeader originalHeader;
    blockHeader reconstructedHeader;
    
    // Ustaw różnorodne dane
    int32_t nextblock = 12345;
    int32_t blockId = 67890;
    int32_t lsn = 111111;
    int16_t checksum = 9999;
    int16_t flags = 8888;
    int8_t toast = 127;
    
    originalHeader.setData(nextblock, blockId, lsn, checksum, flags, toast);
    
    // Marshall
    std::vector<uint8_t> marshalled = originalHeader.marshallBlockHeaderWithData();
    EXPECT_EQ(marshalled.size(), 17);
    
    // Unmarshall
    reconstructedHeader.unmarshallBlockHeader(marshalled);
    
    // Sprawdź wszystkie pola
    EXPECT_EQ(reconstructedHeader.getNextblock(), nextblock);
    EXPECT_EQ(reconstructedHeader.getBlockIndetification(), blockId);
    EXPECT_EQ(reconstructedHeader.getPdLsn(), lsn);
    EXPECT_EQ(reconstructedHeader.getPdChecksum(), checksum);
    EXPECT_EQ(reconstructedHeader.getPdFlags(), flags);
    EXPECT_EQ(reconstructedHeader.getContainToast(), toast);
    
    // Marshall ponownie i porównaj
    std::vector<uint8_t> remarshalled = reconstructedHeader.marshallBlockHeaderWithData();
    EXPECT_EQ(marshalled, remarshalled);
    
    // Test getSize
    EXPECT_EQ(originalHeader.getSize(), 17);
    EXPECT_EQ(reconstructedHeader.getSize(), 17);
}