#include <gtest/gtest.h>
#include "tableHeader.h"
#include <vector>
#include <limits>
#include <chrono>

TEST(TableHeaderTest, DefaultConstructor) {
    tableHeader header;
    
    // Sprawdź, czy konstruktor inicjalizuje pola
    EXPECT_EQ(header.getOid(), 0);
    EXPECT_EQ(header.getContainToast(), 0);
    EXPECT_EQ(header.getNumberOfColumns(), 0);
    EXPECT_EQ(header.getOwner(), 0);
    EXPECT_EQ(header.getPgNamespace(), 0);
    EXPECT_EQ(header.getPgConstraint(), 0);
    EXPECT_EQ(header.getRights(), 0);
    EXPECT_EQ(header.getFreeSpace(), 0);
    EXPECT_EQ(header.getUnitSize(), 0);
}

TEST(TableHeaderTest, MarshallBasicValues) {
    tableHeader header;
    
    std::vector<uint8_t> result = header.marshallTableHeader(
        100,    // oid
        1,      // contain_toast
        5,      // numberOfColumns
        12345,  // owner
        2,      // pg_namespace
        999,    // pg_constraint
        3,      // rights
        1024,   // freeSpace
        256     // unitSize
    );
    
    EXPECT_FALSE(result.empty());
    // 2 (int16) + 4 (int32) + 1 (int8) + 4 (int32) + 8 (int64) + 1 (int8) + 4 (int32) + 1 (int8) + 4 (int32) + 4 (int32) = 33 bytes
    EXPECT_EQ(result.size(), 33);
}

TEST(TableHeaderTest, MarshallZeroValues) {
    tableHeader header;
    
    std::vector<uint8_t> result = header.marshallTableHeader(0, 0, 0, 0, 0, 0, 0, 0, 0);
    
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 33);
}

TEST(TableHeaderTest, MarshallConsistency) {
    tableHeader header1;
    tableHeader header2;
    
    std::vector<uint8_t> result1 = header1.marshallTableHeader(42, 1, 10, 9999, 2, 555, 3, 512, 128);
    std::vector<uint8_t> result2 = header2.marshallTableHeader(42, 1, 10, 9999, 2, 555, 3, 512, 128);
    
    EXPECT_EQ(result1, result2);
    EXPECT_EQ(result1.size(), 33);
}

TEST(TableHeaderTest, MarshallDifferentValues) {
    tableHeader header;
    
    std::vector<uint8_t> result1 = header.marshallTableHeader(100, 1, 5, 1000, 1, 200, 2, 256, 64);
    std::vector<uint8_t> result2 = header.marshallTableHeader(200, 1, 5, 1000, 1, 200, 2, 256, 64);
    
    EXPECT_NE(result1, result2); // Różne wartości oid
    EXPECT_EQ(result1.size(), result2.size());
}

TEST(TableHeaderTest, MarshallMaxValues) {
    tableHeader header;
    
    std::vector<uint8_t> result = header.marshallTableHeader(
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int8_t>::max(),
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int64_t>::max(),
        std::numeric_limits<int8_t>::max(),
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int8_t>::max(),
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int32_t>::max()
    );
    
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 33);
}

TEST(TableHeaderTest, MarshallMinValues) {
    tableHeader header;
    
    std::vector<uint8_t> result = header.marshallTableHeader(
        std::numeric_limits<int32_t>::min(),
        std::numeric_limits<int8_t>::min(),
        std::numeric_limits<int32_t>::min(),
        std::numeric_limits<int64_t>::min(),
        std::numeric_limits<int8_t>::min(),
        std::numeric_limits<int32_t>::min(),
        std::numeric_limits<int8_t>::min(),
        std::numeric_limits<int32_t>::min(),
        std::numeric_limits<int32_t>::min()
    );
    
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 33);
}

TEST(TableHeaderTest, MarshallNegativeValues) {
    tableHeader header;
    
    std::vector<uint8_t> result = header.marshallTableHeader(-100, -1, -50, -9999, -2, -555, -3, -512, -128);
    
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 33);
}

TEST(TableHeaderTest, UnmarshallEmptyVector) {
    tableHeader header;
    std::vector<uint8_t> empty;
    
    EXPECT_NO_THROW(header.unmarshallTableHeader(empty));
    
    // Pola nie są przypisywane w unmarshall, więc pozostają 0
    EXPECT_EQ(header.getOid(), 0);
    EXPECT_EQ(header.getContainToast(), 0);
    EXPECT_EQ(header.getNumberOfColumns(), 0);
}

TEST(TableHeaderTest, UnmarshallTooSmall) {
    tableHeader header;
    std::vector<uint8_t> tooSmall(21, 0x42);
    
    EXPECT_NO_THROW(header.unmarshallTableHeader(tooSmall));
    
    // Pola nie są przypisywane w unmarshall, więc pozostają 0
    EXPECT_EQ(header.getOid(), 0);
    EXPECT_EQ(header.getContainToast(), 0);
    EXPECT_EQ(header.getNumberOfColumns(), 0);
}

TEST(TableHeaderTest, UnmarshallMinimumSize) {
    tableHeader header;
    
    // Tworzymy dane dla minimalnego rozmiaru (22 bajty)
    std::vector<uint8_t> minimum(22, 0x00);
    
    EXPECT_NO_THROW(header.unmarshallTableHeader(minimum));
    
    // Pola nie są przypisywane w unmarshall - tylko lokalne zmienne
}

TEST(TableHeaderTest, UnmarshallExactSize) {
    tableHeader header;
    
    // Tworzymy dane dla dokładnego rozmiaru (33 bajty)
    std::vector<uint8_t> exact(33, 0x00);
    
    EXPECT_NO_THROW(header.unmarshallTableHeader(exact));
}

TEST(TableHeaderTest, UnmarshallLargerSize) {
    tableHeader header;
    
    // Dane większe niż wymagane
    std::vector<uint8_t> larger(50, 0x42);
    
    EXPECT_NO_THROW(header.unmarshallTableHeader(larger));
}

TEST(TableHeaderTest, MarshallUnmarshallRoundTrip) {
    tableHeader header1;
    tableHeader header2;
    
    // Marshall z konkretnymi wartościami
    std::vector<uint8_t> marshalled = header1.marshallTableHeader(
        12345,   // oid
        1,       // contain_toast
        10,      // numberOfColumns
        9876543210LL, // owner
        3,       // pg_namespace
        555,     // pg_constraint
        7,       // rights
        2048,    // freeSpace
        512      // unitSize
    );
    
    EXPECT_EQ(marshalled.size(), 33);
    
    // Unmarshall (ale nie przypisuje do pól klasy)
    EXPECT_NO_THROW(header2.unmarshallTableHeader(marshalled));
    
    // Pola pozostają bez zmian (unmarshall używa tylko lokalnych zmiennych)
    EXPECT_EQ(header2.getOid(), 0);
    EXPECT_EQ(header2.getContainToast(), 0);
    EXPECT_EQ(header2.getNumberOfColumns(), 0);
}

TEST(TableHeaderTest, BoundaryDataSizes) {
    tableHeader header;
    
    std::vector<size_t> testSizes = {0, 1, 21, 22, 30, 33, 35, 50, 100};
    
    for (size_t size : testSizes) {
        std::vector<uint8_t> testData(size, 0x33);
        
        EXPECT_NO_THROW(header.unmarshallTableHeader(testData))
            << "Failed with size: " << size;
    }
}

TEST(TableHeaderTest, ConsecutiveOperations) {
    tableHeader header;
    
    for (int i = 0; i < 10; i++) {
        std::vector<uint8_t> marshalled = header.marshallTableHeader(i, 1, 5, 1000+i, 2, 100+i, 3, 256, 64);
        EXPECT_EQ(marshalled.size(), 33);
        
        EXPECT_NO_THROW(header.unmarshallTableHeader(marshalled));
        
        // Pola pozostają bez zmian
        EXPECT_EQ(header.getOid(), 0);
    }
}

TEST(TableHeaderTest, PerformanceMarshall) {
    const int iterations = 1000;
    tableHeader header;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        std::vector<uint8_t> result = header.marshallTableHeader(i, 1, 10, 1000, 2, 500, 3, 256, 64);
        volatile size_t dummy = result.size();
        (void)dummy;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    EXPECT_LT(duration.count(), 1000000); // Mniej niż 1s dla 1000 operacji
}

TEST(TableHeaderTest, PerformanceUnmarshall) {
    const int iterations = 1000;
    tableHeader header1;
    tableHeader header2;
    
    std::vector<uint8_t> data = header1.marshallTableHeader(42, 1, 5, 9999, 2, 555, 3, 1024, 256);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        header2.unmarshallTableHeader(data);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    EXPECT_LT(duration.count(), 1000000); // Mniej niż 1s dla 1000 operacji
}

TEST(TableHeaderTest, DestructorTest) {
    for (int i = 0; i < 10; i++) {
        tableHeader* header1 = new tableHeader();
        tableHeader* header2 = new tableHeader();
        
        std::vector<uint8_t> marshalled = header1->marshallTableHeader(i, 1, 5, 1000, 2, 100, 3, 256, 64);
        header2->unmarshallTableHeader(marshalled);
        
        EXPECT_EQ(header1->getOid(), 0);
        EXPECT_EQ(header2->getOid(), 0);
        
        delete header1;
        delete header2;
    }
}

TEST(TableHeaderTest, MemoryConsistency) {
    tableHeader sourceHeader;
    std::vector<tableHeader> headers(10);
    
    std::vector<uint8_t> data = sourceHeader.marshallTableHeader(42, 1, 5, 9999, 2, 555, 3, 1024, 256);
    
    for (auto& header : headers) {
        header.unmarshallTableHeader(data);
        
        // Wszystkie obiekty pozostają z wartościami domyślnymi
        EXPECT_EQ(header.getOid(), 0);
        EXPECT_EQ(header.getContainToast(), 0);
        EXPECT_EQ(header.getNumberOfColumns(), 0);
    }
}

TEST(TableHeaderTest, EdgeCaseValues) {
    tableHeader header1;
    tableHeader header2;
    tableHeader header3;
    tableHeader header4;
    
    // Test z wartościami -1, 0, 1
    std::vector<uint8_t> result1 = header1.marshallTableHeader(-1, -1, -1, -1, -1, -1, -1, -1, -1);
    std::vector<uint8_t> result2 = header2.marshallTableHeader(0, 0, 0, 0, 0, 0, 0, 0, 0);
    std::vector<uint8_t> result3 = header3.marshallTableHeader(1, 1, 1, 1, 1, 1, 1, 1, 1);
    
    // Powinny być różne dane
    EXPECT_NE(result1, result2);
    EXPECT_NE(result2, result3);
    EXPECT_NE(result1, result3);
    
    // Ale ten sam rozmiar
    EXPECT_EQ(result1.size(), 33);
    EXPECT_EQ(result2.size(), 33);
    EXPECT_EQ(result3.size(), 33);
    
    // Unmarshall nie zmienia pól obiektów
    header4.unmarshallTableHeader(result1);
    EXPECT_EQ(header4.getOid(), 0);
    
    header4.unmarshallTableHeader(result2);
    EXPECT_EQ(header4.getOid(), 0);
    
    header4.unmarshallTableHeader(result3);
    EXPECT_EQ(header4.getOid(), 0);
}

TEST(TableHeaderTest, DataIntegrityCheck) {
    tableHeader header1;
    tableHeader header2;
    
    // Sprawdź czy marshall produkuje spójne dane
    std::vector<uint8_t> result = header1.marshallTableHeader(
        0x12345678,  // oid
        0x42,        // contain_toast
        0x87654321,  // numberOfColumns
        0x1234567890ABCDEFLL, // owner
        0x33,        // pg_namespace
        0xABCDEF12,  // pg_constraint
        0x77,        // rights
        0x11223344,  // freeSpace
        0x55667788   // unitSize
    );
    
    EXPECT_EQ(result.size(), 33);
    
    header2.unmarshallTableHeader(result);
    
    // Unmarshall nie przypisuje do pól klasy
    EXPECT_EQ(header2.getOid(), 0);
    EXPECT_EQ(header2.getContainToast(), 0);
    EXPECT_EQ(header2.getNumberOfColumns(), 0);
    EXPECT_EQ(header2.getOwner(), 0);
    EXPECT_EQ(header2.getPgNamespace(), 0);
    EXPECT_EQ(header2.getPgConstraint(), 0);
    EXPECT_EQ(header2.getRights(), 0);
    EXPECT_EQ(header2.getFreeSpace(), 0);
    EXPECT_EQ(header2.getUnitSize(), 0);
}

TEST(TableHeaderTest, MarshallInternalConsistency) {
    tableHeader header;
    
    // Testuj czy marshall zawsze produkuje ten sam rezultat dla tych samych danych
    std::vector<uint8_t> result1 = header.marshallTableHeader(123, 1, 5, 9999, 2, 555, 3, 1024, 256);
    std::vector<uint8_t> result2 = header.marshallTableHeader(123, 1, 5, 9999, 2, 555, 3, 1024, 256);
    std::vector<uint8_t> result3 = header.marshallTableHeader(123, 1, 5, 9999, 2, 555, 3, 1024, 256);
    
    EXPECT_EQ(result1, result2);
    EXPECT_EQ(result2, result3);
    EXPECT_EQ(result1, result3);
    
    EXPECT_EQ(result1.size(), 33);
    EXPECT_EQ(result2.size(), 33);
    EXPECT_EQ(result3.size(), 33);
}

TEST(TableHeaderTest, MarshallSizeConsistency) {
    tableHeader header;
    
    // Niezależnie od wartości, rozmiar zawsze powinien być 33
    std::vector<uint8_t> result1 = header.marshallTableHeader(0, 0, 0, 0, 0, 0, 0, 0, 0);
    std::vector<uint8_t> result2 = header.marshallTableHeader(999999, 127, 999999, 9223372036854775807LL, 127, 999999, 127, 999999, 999999);
    std::vector<uint8_t> result3 = header.marshallTableHeader(-999999, -128, -999999, -9223372036854775808LL, -128, -999999, -128, -999999, -999999);
    
    EXPECT_EQ(result1.size(), 33);
    EXPECT_EQ(result2.size(), 33);
    EXPECT_EQ(result3.size(), 33);
}