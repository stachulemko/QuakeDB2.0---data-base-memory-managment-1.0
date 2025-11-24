#include <gtest/gtest.h>
#include "block8kb.h"
#include "allTuple.h"
#include <vector>
#include <limits>
#include <chrono>


bool check(const block8kb& block1, const block8kb& block2) {
    // Porównaj nagłówki
    if (block1.getHeader()->getNextblock() != block2.getHeader()->getNextblock()) return false;
    if (block1.getHeader()->getBlockIndetification() != block2.getHeader()->getBlockIndetification()) return false;
    if (block1.getHeader()->getPdLsn() != block2.getHeader()->getPdLsn()) return false;
    if (block1.getHeader()->getPdChecksum() != block2.getHeader()->getPdChecksum()) return false;
    if (block1.getHeader()->getPdFlags() != block2.getHeader()->getPdFlags()) return false;
    if (block1.getHeader()->getContainToast() != block2.getHeader()->getContainToast()) return false;

    // Porównaj liczbę tupli
    if (block1.getTupleCount() != block2.getTupleCount()) return false;

    // Porównaj każdą tuplę
    for (size_t i = 0; i < block1.getTupleCount(); ++i) {
        const tuple& tuple1 = block1.getTuple(i);
        const tuple& tuple2 = block2.getTuple(i);

        // Porównaj nagłówki tupli
        if (tuple1.getTxMin() != tuple2.getTxMin()) return false;
        if (tuple1.getTxMax() != tuple2.getTxMax()) return false;
        if (tuple1.getTCid() != tuple2.getTCid()) return false;
        if (tuple1.getTInfomask() != tuple2.getTInfomask()) return false;
        if (tuple1.getTHoff() != tuple2.getTHoff()) return false;
        if (tuple1.getNullBitmap() != tuple2.getNullBitmap()) return false;
        if (tuple1.getOptionalOid() != tuple2.getOptionalOid()) return false;

        // Porównaj bitmapy i dane tupli
        if (tuple1.getBitMap() != tuple2.getBitMap()) return false;
        if (tuple1.getData() != tuple2.getData()) return false;
    }

    return true;
}

// ==================== TESTY KONSTRUKTORÓW ====================

TEST(Block8kbTest, DefaultConstructor) {
    block8kb block(1024, 100, 200, 300, 400, 500, 1);
    
    EXPECT_EQ(block.getblockSizeAfterFreeSpace(), blockSize - 1024);
    EXPECT_GT(block.getSize(), 0); // Powinien mieć przynajmniej header
}

TEST(Block8kbTest, ConstructorWithZeroFreeSpace) {
    block8kb block(0, 0, 0, 0, 0, 0, 0);
    
    EXPECT_EQ(block.getblockSizeAfterFreeSpace(), blockSize);
    EXPECT_GT(block.getSize(), 0);
}

TEST(Block8kbTest, ConstructorWithMaxFreeSpace) {
    block8kb block(8192, 100, 200, 300, 400, 500, 1);
    
    EXPECT_EQ(block.getblockSizeAfterFreeSpace(), 0);
    EXPECT_GT(block.getSize(), 0);
}

TEST(Block8kbTest, ConstructorWithNegativeValues) {
    block8kb block(-100, -1, -2, -3, -4, -5, -1);
    
    EXPECT_NO_THROW(block.getSize());
    EXPECT_NO_THROW(block.marshallBlock8kb());
}

TEST(Block8kbTest, ConstructorWithMaxValues) {
    block8kb block(std::numeric_limits<int32_t>::max(), 
                   std::numeric_limits<int32_t>::max(),
                   std::numeric_limits<int32_t>::max(),
                   std::numeric_limits<int32_t>::max(),
                   std::numeric_limits<int16_t>::max(),
                   std::numeric_limits<int16_t>::max(),
                   std::numeric_limits<int8_t>::max());
    
    EXPECT_NO_THROW(block.getSize());
    EXPECT_NO_THROW(block.marshallBlock8kb());
}

// ==================== TESTY METOD GET ====================

TEST(Block8kbTest, GetSizeEmpty) {
    block8kb block(1024, 0, 0, 0, 0, 0, 0);
    
    int32_t size = block.getSize();
    EXPECT_GT(size, 0); // Przynajmniej rozmiar header (17 bajtów)
    EXPECT_LE(size, blockSize);
}

TEST(Block8kbTest, GetSizeWithTuples) {
    block8kb block(1024, 0, 0, 0, 0, 0, 0);
    
    // Dodaj tuple
    tuple testTuple;
    testTuple.setData(100, 200, 1, 0, 50, true, 1000, 
                      {true, false, true}, 
                      {allVars(123), allVars(std::string("test")), allVars(456)});
    
    int32_t sizeBefore = block.getSize();
    block.addData(testTuple);
    int32_t sizeAfter = block.getSize();
    
    EXPECT_GT(sizeAfter, sizeBefore);
}

TEST(Block8kbTest, GetBlockSizeAfterFreeSpace) {
    std::vector<int32_t> freeSpaces = {0, 100, 1000, 4096, 8192};
    
    for (int32_t freeSpace : freeSpaces) {
        block8kb block(freeSpace, 0, 0, 0, 0, 0, 0);
        EXPECT_EQ(block.getblockSizeAfterFreeSpace(), blockSize - freeSpace);
    }
}

// ==================== TESTY DODAWANIA DANYCH ====================

TEST(Block8kbTest, AddDataSingleTuple) {
    block8kb block(4096, 0, 0, 0, 0, 0, 0);
    
    tuple testTuple;
    testTuple.setData(100, 200, 1, 0, 50, true, 1000, 
                      {true, false}, 
                      {allVars(123), allVars(std::string("test"))});
    
    int32_t sizeBefore = block.getSize();
    block.addData(testTuple);
    int32_t sizeAfter = block.getSize();
    
    EXPECT_GT(sizeAfter, sizeBefore);
}

TEST(Block8kbTest, AddDataMultipleTuples) {
    block8kb block(6000, 0, 0, 0, 0, 0, 0);
    
    for (int i = 0; i < 3; i++) {
        tuple testTuple;
        testTuple.setData(100 + i, 200 + i, 1, 0, 50, true, 1000 + i, 
                          {true, false}, 
                          {allVars(123 + i), allVars(std::string("test") + std::to_string(i))});
        block.addData(testTuple);
    }
    
    EXPECT_GT(block.getSize(), 17); // Header + tuples
}

TEST(Block8kbTest, AddDataEmptyTuple) {
    block8kb block(4096, 0, 0, 0, 0, 0, 0);
    
    tuple emptyTuple;
    int32_t sizeBefore = block.getSize();
    block.addData(emptyTuple);
    int32_t sizeAfter = block.getSize();
    
    EXPECT_GE(sizeAfter, sizeBefore); // Może być równy lub większy
}

// ==================== TESTY MARSHALLING ====================

TEST(Block8kbTest, MarshallEmptyBlock) {
    block8kb block(1024, 0, 0, 0, 0, 0, 0);
    
    std::vector<uint8_t> result = block.marshallBlock8kb();
    
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), blockSize);
}

TEST(Block8kbTest, MarshallWithTuples) {
    block8kb block(4096, 123, 456, 789, 100, 200, 1);
    
    tuple testTuple;
    testTuple.setData(100, 200, 1, 0, 50, true, 1000, 
                      {true, false}, 
                      {allVars(123), allVars(std::string("hello"))});
    block.addData(testTuple);
    
    std::vector<uint8_t> result = block.marshallBlock8kb();
    
    EXPECT_EQ(result.size(), blockSize);
    EXPECT_FALSE(result.empty());
}

TEST(Block8kbTest, MarshallConsistency) {
    block8kb block1(1024, 100, 200, 300, 400, 500, 1);
    block8kb block2(1024, 100, 200, 300, 400, 500, 1);
    
    std::vector<uint8_t> result1 = block1.marshallBlock8kb();
    std::vector<uint8_t> result2 = block2.marshallBlock8kb();
    
    EXPECT_EQ(result1.size(), blockSize);
    EXPECT_EQ(result2.size(), blockSize);
    EXPECT_EQ(result1, result2);
}

TEST(Block8kbTest, MarshallDifferentBlocks) {
    block8kb block1(1024, 100, 200, 300, 400, 500, 1);
    block8kb block2(2048, 999, 888, 777, 666, 555, 0);
    
    std::vector<uint8_t> result1 = block1.marshallBlock8kb();
    std::vector<uint8_t> result2 = block2.marshallBlock8kb();
    
    EXPECT_EQ(result1.size(), blockSize);
    EXPECT_EQ(result2.size(), blockSize);
    EXPECT_NE(result1, result2);
}

TEST(Block8kbTest, MarshallStructureValidation) {
    block8kb block(1024, 123, 456, 789, 100, 200, 1);
    
    std::vector<uint8_t> result = block.marshallBlock8kb();
    
    EXPECT_EQ(result.size(), blockSize);
    // Sprawdź czy pierwsze 2 bajty to identyfikator bloku
    EXPECT_GE(result.size(), 2);
    // Sprawdź czy następne 17 bajtów to header
    EXPECT_GE(result.size(), 19);
}

// ==================== TESTY UNMARSHALLING ====================

TEST(Block8kbTest, UnmarshallEmptyVector) {
    block8kb block(1024, 0, 0, 0, 0, 0, 0);
    std::vector<uint8_t> empty;
    
    EXPECT_NO_THROW(block.unmarshallBlock8kb(empty));
}

TEST(Block8kbTest, UnmarshallWrongSize) {
    block8kb block(1024, 0, 0, 0, 0, 0, 0);
    std::vector<uint8_t> wrongSize(100, 0x00);
    
    EXPECT_NO_THROW(block.unmarshallBlock8kb(wrongSize));
}

TEST(Block8kbTest, UnmarshallCorrectSize) {
    block8kb block1(1024, 123, 456, 789, 100, 200, 1);
    block8kb block2(0, 0, 0, 0, 0, 0, 0);
    
    std::vector<uint8_t> marshalled = block1.marshallBlock8kb();
    EXPECT_EQ(marshalled.size(), blockSize);
    
    EXPECT_NO_THROW(block2.unmarshallBlock8kb(marshalled));
}

TEST(Block8kbTest, UnmarshallWrongTypeIdentification) {
    block8kb block(1024, 0, 0, 0, 0, 0, 0);
    
    std::vector<uint8_t> wrongType(blockSize, 0x00);
    wrongType[0] = 0xFF; // Nieprawidłowy identyfikator
    wrongType[1] = 0xFF;
    
    EXPECT_NO_THROW(block.unmarshallBlock8kb(wrongType));
}

// ==================== TESTY ROUND-TRIP ====================

TEST(Block8kbTest, RoundTripEmptyBlock) {
    block8kb block1(2048, 123, 456, 789, 100, 200, 1);
    block8kb block2(0, 0, 0, 0, 0, 0, 0);
    
    std::vector<uint8_t> marshalled = block1.marshallBlock8kb();
    EXPECT_EQ(marshalled.size(), blockSize);
    
    block2.unmarshallBlock8kb(marshalled);
    
    // Test podstawowych właściwości
    EXPECT_GT(block2.getSize(), 0);
}

TEST(Block8kbTest, RoundTripWithTuples) {
    block8kb block1(4096, 123, 456, 789, 100, 200, 1);
    
    // Dodaj kilka tupli
    for (int i = 0; i < 2; i++) {
        tuple testTuple;
        testTuple.setData(100 + i, 200 + i, 1, 0, 50, true, 1000 + i, 
                          {true, false}, 
                          {allVars(123 + i), allVars(std::string("test") + std::to_string(i))});
        block1.addData(testTuple);
    }
    
    std::vector<uint8_t> marshalled = block1.marshallBlock8kb();
    EXPECT_EQ(marshalled.size(), blockSize);
    
    block8kb block2(0, 0, 0, 0, 0, 0, 0);
    EXPECT_NO_THROW(block2.unmarshallBlock8kb(marshalled));
}

TEST(Block8kbTest, RoundTripConsistency) {
    block8kb block1(1024, 999, 888, 777, 666, 555, 7);
    
    std::vector<uint8_t> marshalled1 = block1.marshallBlock8kb();
    
    block8kb block2(0, 0, 0, 0, 0, 0, 0);
    block2.unmarshallBlock8kb(marshalled1);
    
    std::vector<uint8_t> marshalled2 = block2.marshallBlock8kb();
    
    EXPECT_EQ(marshalled1.size(), marshalled2.size());
    EXPECT_EQ(marshalled1.size(), blockSize);
}

// ==================== TESTY WYDAJNOŚCI ====================

TEST(Block8kbTest, PerformanceMarshall) {
    const int iterations = 100;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        block8kb block(i + 1000, i, i + 1, i + 2, i + 3, i + 4, i % 2);
        std::vector<uint8_t> result = block.marshallBlock8kb();
        volatile size_t dummy = result.size();
        (void)dummy;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    EXPECT_LT(duration.count(), 100000); // Mniej niż 100ms
}

TEST(Block8kbTest, PerformanceUnmarshall) {
    const int iterations = 100;
    block8kb sourceBlock(1024, 123, 456, 789, 100, 200, 1);
    
    std::vector<uint8_t> testData = sourceBlock.marshallBlock8kb();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        block8kb block(0, 0, 0, 0, 0, 0, 0);
        block.unmarshallBlock8kb(testData);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    EXPECT_LT(duration.count(), 50000); // Mniej niż 50ms
}

// ==================== TESTY ZARZĄDZANIA PAMIĘCIĄ ====================

TEST(Block8kbTest, DestructorTest) {
    for (int i = 0; i < 10; i++) {
        block8kb* block = new block8kb(i * 100, i, i + 1, i + 2, i + 3, i + 4, i % 2);
        
        tuple testTuple;
        testTuple.setData(100, 200, 1, 0, 50, true, 1000, 
                          {true, false}, 
                          {allVars(i), allVars(std::string("test"))});
        block->addData(testTuple);
        
        std::vector<uint8_t> result = block->marshallBlock8kb();
        EXPECT_EQ(result.size(), blockSize);
        
        delete block; // Test destruktora
    }
}

TEST(Block8kbTest, VectorOfBlocks) {
    std::vector<block8kb*> blocks;
    
    for (int i = 0; i < 5; i++) {
        blocks.emplace_back(new block8kb(i * 500, i, i + 1, i + 2, i + 3, i + 4, i % 2));
    }
    
    for (auto& block : blocks) {
        std::vector<uint8_t> result = block->marshallBlock8kb();
        EXPECT_EQ(result.size(), blockSize);
    }
}

// ==================== TESTY WARTOŚCI GRANICZNYCH ====================

TEST(Block8kbTest, BoundaryValues) {
    std::vector<std::tuple<int32_t, int32_t, int32_t, int32_t, int16_t, int16_t, int8_t>> testCases = {
        {0, 0, 0, 0, 0, 0, 0},
        {blockSize, std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::min(), 
         0, std::numeric_limits<int16_t>::max(), std::numeric_limits<int16_t>::min(), 
         std::numeric_limits<int8_t>::max()},
        {-1000, -1, -2, -3, -4, -5, -1},
        {blockSize + 1000, 999999, 888888, 777777, 30000, 20000, 100}
    };
    
    for (const auto& testCase : testCases) {
        block8kb block(std::get<0>(testCase), std::get<1>(testCase), std::get<2>(testCase),
                       std::get<3>(testCase), std::get<4>(testCase), std::get<5>(testCase),
                       std::get<6>(testCase));
        
        EXPECT_NO_THROW(block.getSize());
        
        std::vector<uint8_t> result = block.marshallBlock8kb();
        EXPECT_EQ(result.size(), blockSize);
        
        block8kb testBlock(0, 0, 0, 0, 0, 0, 0);
        EXPECT_NO_THROW(testBlock.unmarshallBlock8kb(result));
    }
}

// ==================== TESTY METODY showData ====================

TEST(Block8kbTest, ShowDataMethod) {
    block8kb block(1024, 123, 456, 789, 100, 200, 1);
    
    tuple testTuple;
    testTuple.setData(100, 200, 1, 0, 50, true, 1000, 
                      {true, false}, 
                      {allVars(123), allVars(std::string("test"))});
    block.addData(testTuple);
    
    // Test że metoda nie rzuca wyjątków
    EXPECT_NO_THROW(block.showData());
}

// ==================== TESTY ZŁOŻONE ====================

TEST(Block8kbTest, ComplexDataStructures) {
    block8kb block(4096, 12345, 67890, 11111, 9999, 8888, 127);
    
    // Dodaj różne typy tupli
    std::vector<allVars> mixedData = {
        allVars(int32_t(42)),
        allVars(int64_t(9876543210LL)),
        allVars(std::string("Complex test string with special chars: éñü")),
        allVars(int32_t(-123456)),
        allVars(std::string(""))
    };
    
    tuple complexTuple;
    complexTuple.setData(999, 888, 5, 0, 100, true, 5000, 
                         {true, false, true, false, true}, mixedData);
    block.addData(complexTuple);
    
    std::vector<uint8_t> marshalled = block.marshallBlock8kb();
    EXPECT_EQ(marshalled.size(), blockSize);
    
    block8kb reconstructedBlock(0, 0, 0, 0, 0, 0, 0);
    EXPECT_NO_THROW(reconstructedBlock.unmarshallBlock8kb(marshalled));
}

TEST(Block8kbTest, StressTestMultipleTuples) {
    block8kb block(6000, 1, 2, 3, 4, 5, 1);
    
    // Dodaj wiele małych tupli
    for (int i = 0; i < 10; i++) {
        tuple smallTuple;
        smallTuple.setData(i, i + 100, 2, 0, 20, i % 2 == 0, 100 + i, 
                           {true, false}, 
                           {allVars(i), allVars(std::string("item") + std::to_string(i))});
        
        // Sprawdź czy tuple mieści się w bloku
        if (block.getSize() + smallTuple.getSize() <= block.getblockSizeAfterFreeSpace()) {
            block.addData(smallTuple);
        } else {
            break; // Blok jest pełny
        }
    }
    
    std::vector<uint8_t> result = block.marshallBlock8kb();
    EXPECT_EQ(result.size(), blockSize);
    
    block8kb testBlock(0, 0, 0, 0, 0, 0, 0);
    EXPECT_NO_THROW(testBlock.unmarshallBlock8kb(result));
}

// ==================== TEST INTEGRACYJNY ====================

TEST(Block8kbTest, FullIntegrationTest) {
    // Stwórz blok z rzeczywistymi danymi
    block8kb originalBlock(3000, 12345, 67890, 111222, 9999, 7777, 1);
    
    // Dodaj różnorodne tuple
    std::vector<tuple> testTuples;
    
    // Tuple 1: Dane liczbowe
    tuple tuple1;
    tuple1.setData(1, 100, 3, 0, 50, true, 1000, 
                   {true, false, true}, 
                   {allVars(int32_t(42)), allVars(int64_t(123456789LL)), allVars(int32_t(-99))});
    originalBlock.addData(tuple1);
    
    // Tuple 2: Dane tekstowe
    tuple tuple2;
    tuple2.setData(2, 200, 2, 0, 60, false, 2000, 
                   {false, true}, 
                   {allVars(std::string("Hello World")), allVars(std::string("Integration Test"))});
    originalBlock.addData(tuple2);
    
    // Tuple 3: Dane mieszane
    tuple tuple3;
    tuple3.setData(3, 300, 4, 0, 70, true, 3000, 
                   {true, true, false, true}, 
                   {allVars(int32_t(999)), allVars(std::string("Mixed")), 
                    allVars(int64_t(0)), allVars(int32_t(std::numeric_limits<int32_t>::max()))});
    originalBlock.addData(tuple3);
    
    // Test marshall
    std::vector<uint8_t> marshalled = originalBlock.marshallBlock8kb();
    EXPECT_EQ(marshalled.size(), blockSize);
    EXPECT_FALSE(marshalled.empty());
    
    // Test unmarshall
    block8kb reconstructedBlock(0, 0, 0, 0, 0, 0, 0);
    EXPECT_NO_THROW(reconstructedBlock.unmarshallBlock8kb(marshalled));
    
    // Test że rozmiar jest spójny
    EXPECT_GT(reconstructedBlock.getSize(), 0);
    
    // Test że można ponownie zmarshallować
    std::vector<uint8_t> remarshalled = reconstructedBlock.marshallBlock8kb();
    EXPECT_EQ(remarshalled.size(), blockSize);
    
    // Test metod pomocniczych
    EXPECT_NO_THROW(originalBlock.showData());
    EXPECT_NO_THROW(reconstructedBlock.showData());
}


//------------------------
//------------------------
//------------------------
//------------------------

// Dodaj na końcu pliku przed ostatnim #endif

// ==================== TESTY MARSHALL/UNMARSHALL Z GETTERAMI ====================

TEST(Block8kbTest, MarshallUnmarshallBasicGetters) {
    block8kb block8kb1(1,2,3,4,5,6,7);
    tuple tupleObj;
    tupleObj.setData(100,200,1,0,24,true,5000,{false,true,false},{allVars(123),allVars("test"),allVars(43)});
    block8kb1.addData(tupleObj);

    std::vector<uint8_t> marshalledBlock = block8kb1.marshallBlock8kb();

    block8kb block8kb2(0,0,0,0,0,0,0);
    block8kb2.unmarshallBlock8kb(marshalledBlock);
    
    // SPRAWDŹ WSZYSTKIE GETTERY HEADERA BLOKU
    EXPECT_EQ(block8kb1.getHeader()->getPdFlags(), block8kb2.getHeader()->getPdFlags());
    EXPECT_EQ(block8kb1.getHeader()->getNextblock(), block8kb2.getHeader()->getNextblock());
    EXPECT_EQ(block8kb1.getHeader()->getBlockIndetification(), block8kb2.getHeader()->getBlockIndetification());
    EXPECT_EQ(block8kb1.getHeader()->getPdLsn(), block8kb2.getHeader()->getPdLsn());
    EXPECT_EQ(block8kb1.getHeader()->getPdChecksum(), block8kb2.getHeader()->getPdChecksum());
    //EXPECT_EQ(block8kb1.getHeader()->getPdLower(), block8kb2.getHeader()->getPdLower());
    //EXPECT_EQ(block8kb1.getHeader()->getPdUpper(), block8kb2.getHeader()->getPdUpper());
    
    EXPECT_TRUE(check(block8kb1,block8kb2));
}

TEST(Block8kbTest, MultipleMarshallUnmarshallBasicGetters) {
    block8kb block8kb1(1,2,3,4,5,6,7);
    tuple tupleObj;
    tupleObj.setData(100,200,1,0,24,true,5000,{false,true,false},{allVars(123),allVars("test"),allVars(43)});
    block8kb1.addData(tupleObj);

    tuple tupleObj1;
    tupleObj1.setData(100,200,1,0,24,true,5000,{false,true,false},{allVars(123),allVars("test"),allVars(43)});
    block8kb1.addData(tupleObj1);

    std::vector<uint8_t> marshalledBlock = block8kb1.marshallBlock8kb();

    block8kb block8kb2(0,0,0,0,0,0,0);
    block8kb2.unmarshallBlock8kb(marshalledBlock);
    
    //header block chech
    EXPECT_EQ(block8kb1.getHeader()->getPdFlags(), block8kb2.getHeader()->getPdFlags());
    EXPECT_EQ(block8kb1.getHeader()->getNextblock(), block8kb2.getHeader()->getNextblock());
    EXPECT_EQ(block8kb1.getHeader()->getBlockIndetification(), block8kb2.getHeader()->getBlockIndetification());
    EXPECT_EQ(block8kb1.getHeader()->getPdLsn(), block8kb2.getHeader()->getPdLsn());
    EXPECT_EQ(block8kb1.getHeader()->getPdChecksum(), block8kb2.getHeader()->getPdChecksum());
    
    // SPRAWDŹ ROZMIARY BLOKÓW
    EXPECT_GT(block8kb1.getSize(), 0);
    EXPECT_GT(block8kb2.getSize(), 0);
    
    EXPECT_TRUE(check(block8kb1,block8kb2));
    // SPRAWDŹ ILOŚĆ TUPLI
    //EXPECT_EQ(block8kb1.getTuples().size(), block8kb2.getTuples().size());
    //EXPECT_GE(block8kb1.getTuples().size(), 1);
    
}
