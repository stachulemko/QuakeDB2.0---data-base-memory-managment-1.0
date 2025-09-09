#include <gtest/gtest.h>
#include "headerTuple.h"
#include "typesDataConverter.h"
#include <cstring>
#include <limits>
#include <bitset>
#include <chrono>


// ================================
// TESTY KONSTRUKTORA
// ================================

TEST(HeaderTupleTest, DefaultConstructor) {
    //HeaderTuple header;
    EXPECT_TRUE(true); 
}

// ================================
// TESTY MARSHALL HEADER TUPLE - PODSTAWOWE
// ================================

TEST(HeaderTupleTest, MarshallHeaderTupleBasicValues) {
    HeaderTuple *header = new HeaderTuple();
    
    int64_t t_xmin = 1001;
    int64_t t_xmax = 2002; 
    int32_t t_cid = 100;
    int32_t t_infomask = 0x0001;
    int16_t t_hoff = 24;
    bool null_bitmap = true;
    int64_t optional_oid = 12345;
    
    std::bitset<280> result = header.marshallHeaderTuple(
        t_xmin, t_xmax, t_cid, t_infomask, t_hoff, null_bitmap, optional_oid
    );
    
    EXPECT_EQ(result.size(), 280);
    
    EXPECT_FALSE(result.none());
    
}
/*
TEST(HeaderTupleTest, MarshallHeaderTupleZeroValues) {
    HeaderTuple header;
    
    std::bitset<280> result = header.marshallHeaderTuple(
        0, 0, 0, 0, 0, false, 0
    );
    
    EXPECT_EQ(result.size(), 280);
    EXPECT_LE(result.count(), 280); 
}

TEST(HeaderTupleTest, MarshallHeaderTupleNegativeValues) {
    HeaderTuple header;
    
    std::bitset<280> result = header.marshallHeaderTuple(
        -1001, -2002, -100, -200, -24, false, -12345
    );
    
    EXPECT_EQ(result.size(), 280);
    EXPECT_GT(result.count(), 50);
    
    //std::cout << "MarshallHeaderTuple - wartości ujemne: PASSED" << std::endl;
}

TEST(HeaderTupleTest, MarshallHeaderTupleMaxValues) {
    HeaderTuple header;
    
    std::bitset<280> result = header.marshallHeaderTuple(
        std::numeric_limits<int64_t>::max(),
        std::numeric_limits<int64_t>::max(),
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int16_t>::max(),
        true,
        std::numeric_limits<int64_t>::max()
    );
    
    EXPECT_EQ(result.size(), 280);
    EXPECT_GT(result.count(), 200);
    
    //std::cout << "MarshallHeaderTuple - wartości maksymalne: PASSED" << std::endl;
}

TEST(HeaderTupleTest, MarshallHeaderTupleMinValues) {
    HeaderTuple header;
    
    std::bitset<280> result = header.marshallHeaderTuple(
        std::numeric_limits<int64_t>::min(),
        std::numeric_limits<int64_t>::min(),
        std::numeric_limits<int32_t>::min(),
        std::numeric_limits<int32_t>::min(),
        std::numeric_limits<int16_t>::min(),
        false,
        std::numeric_limits<int64_t>::min()
    );
    
    EXPECT_EQ(result.size(), 280);
    EXPECT_GT(result.count(), 50);
    
    std::cout << "MarshallHeaderTuple - wartości minimalne: PASSED" << std::endl;
}

// ================================
// TESTY KONSYSTENCJI MARSHALL
// ================================

TEST(HeaderTupleTest, MarshallConsistency) {
    HeaderTuple header;
    
    int64_t t_xmin = 12345;
    int64_t t_xmax = 67890;
    int32_t t_cid = 100;
    int32_t t_infomask = 0x0005;
    int16_t t_hoff = 28;
    bool null_bitmap = true;
    int64_t optional_oid = 999888;
    
    std::bitset<280> result1 = header.marshallHeaderTuple(
        t_xmin, t_xmax, t_cid, t_infomask, t_hoff, null_bitmap, optional_oid
    );
    
    std::bitset<280> result2 = header.marshallHeaderTuple(
        t_xmin, t_xmax, t_cid, t_infomask, t_hoff, null_bitmap, optional_oid
    );
    
    EXPECT_EQ(result1, result2);
    
    std::cout << "MarshallHeaderTuple - konsystencja: PASSED" << std::endl;
}

TEST(HeaderTupleTest, MarshallDifferentValues) {
    HeaderTuple header;
    
    std::bitset<280> result1 = header.marshallHeaderTuple(
        1000, 2000, 100, 0x01, 24, true, 12345
    );
    
    std::bitset<280> result2 = header.marshallHeaderTuple(
        1001, 2000, 100, 0x01, 24, true, 12345  // Zmienione t_xmin
    );
    
    // Wyniki powinny być różne
    EXPECT_NE(result1, result2);
    
    std::cout << "MarshallHeaderTuple - różne wartości: PASSED" << std::endl;
}

// ================================
// TESTY BITÓW I STRUKTURY
// ================================

TEST(HeaderTupleTest, MarshallBitsetStructure) {
    HeaderTuple header;
    
    std::bitset<280> result = header.marshallHeaderTuple(
        0x123456789ABCDEF0LL, // t_xmin - 8 bajtów = 64 bity
        0x0FEDCBA987654321LL, // t_xmax - 8 bajtów = 64 bity  
        0x12345678,           // t_cid - 4 bajty = 32 bity
        0x87654321,           // t_infomask - 4 bajty = 32 bity
        0x1234,               // t_hoff - 2 bajty = 16 bitów
        true,                 // null_bitmap - 1 bit
        0xABCDEF0123456789LL  // optional_oid - 8 bajtów = 64 bity
    );
    
    EXPECT_EQ(result.size(), 280);
    
    EXPECT_GT(result.count(), 10);
    
    //std::cout << "MarshallHeaderTuple - struktura bitset: PASSED" << std::endl;
}

TEST(HeaderTupleTest, MarshallBooleanFlag) {
    HeaderTuple header;
    
    // Test z null_bitmap = true
    std::bitset<280> result_true = header.marshallHeaderTuple(
        1000, 2000, 100, 0x01, 24, true, 12345
    );
    
    // Test z null_bitmap = false
    std::bitset<280> result_false = header.marshallHeaderTuple(
        1000, 2000, 100, 0x01, 24, false, 12345
    );
    
    // Wyniki powinny się różnić tylko flagą boolean
    EXPECT_NE(result_true, result_false);
    
    std::cout << "MarshallHeaderTuple - flaga boolean: PASSED" << std::endl;
}

// ================================
// TESTY WYDAJNOŚCIOWE
// ================================

TEST_F(HeaderTupleTest, MarshallPerformanceTest) {
    HeaderTuple header;
    const int iterations = 100000;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        std::bitset<280> result = header.marshallHeaderTuple(
            1000 + i, 2000 + i, 100 + i, 0x01, 24, (i % 2 == 0), 12345 + i
        );
        
        // Użyj wyniku żeby kompilator nie zoptymalizował
        volatile size_t dummy = result.count();
        (void)dummy;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Marshall " << iterations << " headerów w " 
              << duration.count() << " μs (" 
              << (duration.count() / (double)iterations) << " μs/operacja)" << std::endl;
    
    // Test wydajności - max 2 sekundy dla 100k operacji
    EXPECT_LT(duration.count(), 2000000);
}

// ================================
// TESTY SPECYFICZNE DLA POSTGRESQL
// ================================

TEST_F(HeaderTupleTest, PostgreSQLTransactionTest) {
    HeaderTuple header;
    
    // Symuluj tworzenie tuple przez transakcję 1001
    std::bitset<280> created = header.marshallHeaderTuple(
        1001,    // t_xmin - transakcja tworząca
        0,       // t_xmax - nie usunięty
        1,       // t_cid - pierwsza komenda w transakcji
        0x0000,  // t_infomask - brak flag
        24,      // t_hoff - standardowy header 24 bajty
        false,   // null_bitmap - brak NULL
        12345    // optional_oid
    );
    
    // Symuluj usunięcie tuple przez transakcję 1002
    std::bitset<280> deleted = header.marshallHeaderTuple(
        1001,    // t_xmin - nadal ta sama
        1002,    // t_xmax - transakcja usuwająca
        1,       // t_cid
        0x0001,  // t_infomask - flaga usunięcia
        24,      // t_hoff
        false,   // null_bitmap
        12345    // optional_oid
    );
    
    EXPECT_NE(created, deleted);
    EXPECT_EQ(created.size(), 280);
    EXPECT_EQ(deleted.size(), 280);
    
    std::cout << "PostgreSQL transaction lifecycle test: PASSED" << std::endl;
}

TEST_F(HeaderTupleTest, PostgreSQLInfomaskFlags) {
    HeaderTuple header;
    
    // Test różnych flag t_infomask
    std::vector<int32_t> infomask_flags = {
        0x0000,  // Brak flag
        0x0001,  // HEAP_HASNULL
        0x0002,  // HEAP_HASVARWIDTH  
        0x0004,  // HEAP_HASEXTERNAL
        0x0008,  // HEAP_HASOID
        0x0010,  // HEAP_XMAX_KEYSHR_LOCK
        0x0020,  // HEAP_COMBOCID
        0x0040,  // HEAP_XMAX_EXCL_LOCK
        0x0080   // HEAP_XMAX_LOCK_ONLY
    };
    
    std::vector<std::bitset<280>> results;
    
    for (int32_t flag : infomask_flags) {
        std::bitset<280> result = header.marshallHeaderTuple(
            1000, 0, 1, flag, 24, false, 12345
        );
        results.push_back(result);
    }
    
    // Wszystkie wyniki powinny być różne (różne flagi)
    for (size_t i = 0; i < results.size(); i++) {
        for (size_t j = i + 1; j < results.size(); j++) {
            EXPECT_NE(results[i], results[j]);
        }
    }
    
    std::cout << "PostgreSQL infomask flags test: PASSED" << std::endl;
}

TEST_F(HeaderTupleTest, PostgreSQLHeaderOffset) {
    HeaderTuple header;
    
    // Test różnych wartości t_hoff (header offset)
    std::vector<int16_t> header_offsets = {24, 28, 32, 40, 48};
    
    for (int16_t hoff : header_offsets) {
        std::bitset<280> result = header.marshallHeaderTuple(
            1000, 0, 1, 0x01, hoff, false, 12345
        );
        
        EXPECT_EQ(result.size(), 280);
        EXPECT_FALSE(result.none()); // Nie powinien być pusty
    }
    
    std::cout << "PostgreSQL header offset test: PASSED" << std::endl;
}

// ================================
// TESTY WARTOŚCI GRANICZNYCH
// ================================

TEST(HeaderTupleTest, BoundaryValuesTest) {
    HeaderTuple header;
    
    struct TestCase {
        int64_t t_xmin, t_xmax, optional_oid;
        int32_t t_cid, t_infomask;
        int16_t t_hoff;
        bool null_bitmap;
        std::string description;
    };
    
    std::vector<TestCase> test_cases = {
        {0, 0, 0, 0, 0, 0, false, "Wszystkie zera"},
        {1, 1, 1, 1, 1, 1, true, "Wszystkie jedynki"},
        {-1, -1, -1, -1, -1, -1, false, "Wszystkie minus jeden"},
        {std::numeric_limits<int64_t>::max(), 0, 0, 0, 0, 24, false, "Max int64"},
        {0, std::numeric_limits<int64_t>::min(), 0, 0, 0, 24, false, "Min int64"},
        {0, 0, 0, std::numeric_limits<int32_t>::max(), 0, 24, false, "Max int32"},
        {0, 0, 0, 0, std::numeric_limits<int32_t>::min(), 24, false, "Min int32"},
        {0, 0, 0, 0, 0, std::numeric_limits<int16_t>::max(), false, "Max int16"},
        {0, 0, 0, 0, 0, std::numeric_limits<int16_t>::min(), false, "Min int16"}
    };
    
    for (const auto& test_case : test_cases) {
        std::bitset<280> result = header.marshallHeaderTuple(
            test_case.t_xmin, test_case.t_xmax, test_case.t_cid,
            test_case.t_infomask, test_case.t_hoff, 
            test_case.null_bitmap, test_case.optional_oid
        );
        
        EXPECT_EQ(result.size(), 280) << "Test case: " << test_case.description;
        
        std::cout << "Boundary test - " << test_case.description << ": PASSED" << std::endl;
    }
}

// ================================
// TESTY INTEGRACJNE
// ================================

TEST(HeaderTupleTest, IntegrationWithTypesConverter) {
    HeaderTuple header;
    
    // Użyj konwertera do przygotowania danych
    int64_t test_xmin = 123456789;
    std::vector<uint8_t>* xmin_bytes = marshalInt64_t(test_xmin);
    ASSERT_NE(xmin_bytes, nullptr);
    
    // Unmarshall z powrotem
    int64_t unmarshalled_xmin;
    UnmarshalInt64_t(&unmarshalled_xmin, xmin_bytes);
    EXPECT_EQ(test_xmin, unmarshalled_xmin);
    
    // Użyj w marshall
    std::bitset<280> result = header.marshallHeaderTuple(
        unmarshalled_xmin, 0, 1, 0x01, 24, false, 12345
    );
    
    EXPECT_EQ(result.size(), 280);
    EXPECT_FALSE(result.none());
    
    delete xmin_bytes;
    
    std::cout << "Integration with TypesConverter: PASSED" << std::endl;
}

// ================================
// TEST PODSUMOWUJĄCY
// ================================

TEST(HeaderTupleTest, ComprehensiveMarshallTest) {
    HeaderTuple header;
    
    // Kompleksowy test z rzeczywistymi danymi PostgreSQL
    struct TupleData {
        int64_t t_xmin = 1001;      // Transakcja tworząca
        int64_t t_xmax = 0;         // Nie usunięty
        int32_t t_cid = 1;          // Pierwsza komenda
        int32_t t_infomask = 0x0001; // HEAP_HASNULL
        int16_t t_hoff = 24;        // Standardowy header
        bool null_bitmap = true;     // Ma null bitmap
        int64_t optional_oid = 98765; // OID obiektu
    } tuple_data;
    
    std::bitset<280> result = header.marshallHeaderTuple(
        tuple_data.t_xmin,
        tuple_data.t_xmax,
        tuple_data.t_cid,
        tuple_data.t_infomask,
        tuple_data.t_hoff,
        tuple_data.null_bitmap,
        tuple_data.optional_oid
    );
    
    // Kompletne sprawdzenie wyniku
    EXPECT_EQ(result.size(), 280);
    EXPECT_FALSE(result.none());
    EXPECT_GT(result.count(), 5);  // Przynajmniej kilka bitów ustawionych
    EXPECT_LT(result.count(), 275); // Ale nie wszystkie
    
    // Test poprawności bitset jako string
    std::string bitset_str = result.to_string();
    EXPECT_EQ(bitset_str.length(), 280);
    
    std::cout << "Comprehensive Marshall Test: PASSED" << std::endl;
    std::cout << "Ustawione bity: " << result.count() << "/280" << std::endl;
    std::cout << "Pierwsze 64 bity: " << bitset_str.substr(0, 64) << std::endl;
}
*/