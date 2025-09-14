#include <gtest/gtest.h>
#include "typesDataConverter.h"
#include <cstring>
#include <limits>


// ================================
// TESTY DLA BOOL
// ================================

TEST(TypesDataConverterTest, BoolMarshalTrue) {
    bool original = true;
    std::vector<uint8_t>* result = marshalBool(original);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->size(), 1);
    EXPECT_EQ((*result)[0], 1);
    
    delete result;
}

TEST(TypesDataConverterTest, BoolMarshalFalse) {
    bool original = false;
    std::vector<uint8_t>* result = marshalBool(original);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->size(), 1);
    EXPECT_EQ((*result)[0], 0);
    
    delete result;
}

TEST(TypesDataConverterTest, BoolUnmarshalTrue) {
    std::vector<uint8_t>* binaryData = new std::vector<uint8_t>{1};
    bool result;
    
    UnmarshalBool(&result, binaryData);
    EXPECT_TRUE(result);
    
    delete binaryData;
}

TEST(TypesDataConverterTest, BoolUnmarshalFalse) {
    std::vector<uint8_t>* binaryData = new std::vector<uint8_t>{0};
    bool result;
    
    UnmarshalBool(&result, binaryData);
    EXPECT_FALSE(result);
    
    delete binaryData;
}

TEST(TypesDataConverterTest, BoolUnmarshalNullPtr) {
    bool result = true;
    UnmarshalBool(&result, nullptr);
    EXPECT_FALSE(result); // Powinno ustawić na false
}

TEST(TypesDataConverterTest, BoolUnmarshalEmptyVector) {
    std::vector<uint8_t>* emptyVector = new std::vector<uint8_t>();
    bool result = true;
    
    UnmarshalBool(&result, emptyVector);
    EXPECT_FALSE(result); // Powinno ustawić na false
    
    delete emptyVector;
}

TEST(TypesDataConverterTest, BoolRoundTrip) {
    bool original = true;
    std::vector<uint8_t>* marshaled = marshalBool(original);
    bool unmarshaled;
    
    UnmarshalBool(&unmarshaled, marshaled);
    EXPECT_EQ(original, unmarshaled);
    
    delete marshaled;
}

// ================================
// TESTY DLA INT16_T
// ================================

TEST(TypesDataConverterTest, Int16MarshalPositive) {
    int16_t original = 12345;
    std::vector<uint8_t>* result = marshalInt16_t(original);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->size(), sizeof(int16_t));
    
    delete result;
}

TEST(TypesDataConverterTest, Int16MarshalNegative) {
    int16_t original = -12345;
    std::vector<uint8_t>* result = marshalInt16_t(original);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->size(), sizeof(int16_t));
    
    delete result;
}

TEST(TypesDataConverterTest, Int16MarshalZero) {
    int16_t original = 0;
    std::vector<uint8_t>* result = marshalInt16_t(original);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->size(), sizeof(int16_t));
    
    delete result;
}

TEST(TypesDataConverterTest, Int16MarshalMaxValue) {
    int16_t original = std::numeric_limits<int16_t>::max();
    std::vector<uint8_t>* result = marshalInt16_t(original);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->size(), sizeof(int16_t));
    
    delete result;
}

TEST(TypesDataConverterTest, Int16MarshalMinValue) {
    int16_t original = std::numeric_limits<int16_t>::min();
    std::vector<uint8_t>* result = marshalInt16_t(original);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->size(), sizeof(int16_t));
    
    delete result;
}

TEST(TypesDataConverterTest, Int16RoundTrip) {
    int16_t original = 42;
    std::vector<uint8_t>* marshaled = marshalInt16_t(original);
    int16_t unmarshaled;
    
    UnmarshalInt16_t(&unmarshaled, marshaled);
    EXPECT_EQ(original, unmarshaled);
    
    delete marshaled;
}

TEST(TypesDataConverterTest, Int16UnmarshalNullPtr) {
    int16_t result = 123;
    int16_t original = result;
    
    UnmarshalInt16_t(&result, nullptr);
    EXPECT_EQ(result, original); // Nie powinno się zmienić
}

// ================================
// TESTY DLA INT32_T
// ================================

TEST(TypesDataConverterTest, Int32MarshalPositive) {
    int32_t original = 1234567890;
    std::vector<uint8_t>* result = marshalInt32_t(original);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->size(), sizeof(int32_t));
    
    delete result;
}

TEST(TypesDataConverterTest, Int32MarshalNegative) {
    int32_t original = -1234567890;
    std::vector<uint8_t>* result = marshalInt32_t(original);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->size(), sizeof(int32_t));
    
    delete result;
}

TEST(TypesDataConverterTest, Int32RoundTrip) {
    int32_t original = 987654321;
    std::vector<uint8_t>* marshaled = marshalInt32_t(original);
    int32_t unmarshaled;
    
    UnmarshalInt32_t(&unmarshaled, marshaled);
    EXPECT_EQ(original, unmarshaled);
    
    delete marshaled;
}

TEST(TypesDataConverterTest, Int32MaxMinValues) {
    // Test max value
    int32_t maxVal = std::numeric_limits<int32_t>::max();
    std::vector<uint8_t>* marshaledMax = marshalInt32_t(maxVal);
    int32_t unmarshaledMax;
    UnmarshalInt32_t(&unmarshaledMax, marshaledMax);
    EXPECT_EQ(maxVal, unmarshaledMax);
    delete marshaledMax;
    
    // Test min value
    int32_t minVal = std::numeric_limits<int32_t>::min();
    std::vector<uint8_t>* marshaledMin = marshalInt32_t(minVal);
    int32_t unmarshaledMin;
    UnmarshalInt32_t(&unmarshaledMin, marshaledMin);
    EXPECT_EQ(minVal, unmarshaledMin);
    delete marshaledMin;
}

// ================================
// TESTY DLA INT64_T
// ================================

TEST(TypesDataConverterTest, Int64MarshalPositive) {
    int64_t original = 12345678901234567LL;
    std::vector<uint8_t>* result = marshalInt64_t(original);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->size(), sizeof(int64_t));
    
    delete result;
}

TEST(TypesDataConverterTest, Int64RoundTrip) {
    int64_t original = -9876543210987654LL;
    std::vector<uint8_t>* marshaled = marshalInt64_t(original);
    int64_t unmarshaled;
    
    UnmarshalInt64_t(&unmarshaled, marshaled);
    EXPECT_EQ(original, unmarshaled);
    
    delete marshaled;
}

TEST(TypesDataConverterTest, Int64MaxMinValues) {
    // Test max value
    int64_t maxVal = std::numeric_limits<int64_t>::max();
    std::vector<uint8_t>* marshaledMax = marshalInt64_t(maxVal);
    int64_t unmarshaledMax;
    UnmarshalInt64_t(&unmarshaledMax, marshaledMax);
    EXPECT_EQ(maxVal, unmarshaledMax);
    delete marshaledMax;
    
    // Test min value
    int64_t minVal = std::numeric_limits<int64_t>::min();
    std::vector<uint8_t>* marshaledMin = marshalInt64_t(minVal);
    int64_t unmarshaledMin;
    UnmarshalInt64_t(&unmarshaledMin, marshaledMin);
    EXPECT_EQ(minVal, unmarshaledMin);
    delete marshaledMin;
}

// ================================
// TESTY DLA STRING
// ================================

TEST(TypesDataConverterTest, StringMarshalEmpty) {
    std::string original = "";
    std::vector<uint8_t>* result = marshalString(original);
    
    ASSERT_NE(result, nullptr);
    //EXPECT_EQ(result->size(), 1); // null terminator
    //EXPECT_EQ((*result)[0], 0);
    
    delete result;
}

TEST(TypesDataConverterTest, StringMarshalSimple) {
    std::string original = "Hello";
    std::vector<uint8_t>* result = marshalString(original);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->size(), original.length() );
    
    // Sprawdź zawartość
    for (size_t i = 0; i < original.length(); ++i) {
        EXPECT_EQ((*result)[i], static_cast<uint8_t>(original[i]));
    }
    // Oczekuj, że nie ma null terminatora
    //EXPECT_EQ((*result)[original.length()], 0);

    delete result;
}

TEST(TypesDataConverterTest, StringMarshalSpecialChars) {
    std::string original = "Hello\nWorld\t!";
    std::vector<uint8_t>* result = marshalString(original);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->size(), original.length());
    
    delete result;
}

TEST(TypesDataConverterTest, StringRoundTrip) {
    std::string original = "QuakeDB 2.0 Database";
    std::vector<uint8_t>* marshaled = marshalString(original);
    std::string unmarshaled;
    
    UnmarshalString(&unmarshaled, marshaled);
    EXPECT_EQ(original, unmarshaled);
    
    delete marshaled;
}

TEST(TypesDataConverterTest, StringUnmarshalNullPtr) {
    std::string result = "original";
    std::string original = result;
    //UnmarshalString(&result, nullptr);
    //EXPECT_EQ(returnCheckPoint,true);

    // Sprawdź czy się nie zmieniło lub zostało wyczyszczone
    // (zależy od implementacji)
}

TEST(TypesDataConverterTest, StringUnmarshalEmptyVector) {
    std::vector<uint8_t>* emptyVector = new std::vector<uint8_t>();
    std::string result = "original";
    //UnmarshalString(&result, emptyVector);
    //EXPECT_EQ(returnCheckPoint,true);
    // Powinno być puste lub niezmienione
    
    delete emptyVector;
}

// ================================
// TESTY EDGE CASES
// ================================

TEST(TypesDataConverterTest, AllTypesZeroValues) {
    // Bool
    bool boolZero = false;
    std::vector<uint8_t>* marshaledBool = marshalBool(boolZero);
    bool unmarshaledBool;
    UnmarshalBool(&unmarshaledBool, marshaledBool);
    EXPECT_EQ(boolZero, unmarshaledBool);
    delete marshaledBool;
    
    // Int16
    int16_t int16Zero = 0;
    std::vector<uint8_t>* marshaledInt16 = marshalInt16_t(int16Zero);
    int16_t unmarshaledInt16;
    UnmarshalInt16_t(&unmarshaledInt16, marshaledInt16);
    EXPECT_EQ(int16Zero, unmarshaledInt16);
    delete marshaledInt16;
    
    // Int32
    int32_t int32Zero = 0;
    std::vector<uint8_t>* marshaledInt32 = marshalInt32_t(int32Zero);
    int32_t unmarshaledInt32;
    UnmarshalInt32_t(&unmarshaledInt32, marshaledInt32);
    EXPECT_EQ(int32Zero, unmarshaledInt32);
    delete marshaledInt32;
    
    // Int64
    int64_t int64Zero = 0;
    std::vector<uint8_t>* marshaledInt64 = marshalInt64_t(int64Zero);
    int64_t unmarshaledInt64;
    UnmarshalInt64_t(&unmarshaledInt64, marshaledInt64);
    EXPECT_EQ(int64Zero, unmarshaledInt64);
    delete marshaledInt64;
}

TEST(TypesDataConverterTest, ShowBytesFunction) {
    // Test czy funkcja showBytes nie powoduje crashu
    std::vector<uint8_t> testBytes = {0x01, 0x02, 0x03, 0xFF};
    
    // Przekieruj cout żeby nie zaśmiecać output testów
    std::streambuf* orig = std::cout.rdbuf();
    std::ostringstream buffer;
    std::cout.rdbuf(buffer.rdbuf());
    
    EXPECT_NO_THROW(showBytes(testBytes));
    
    // Przywróć cout
    std::cout.rdbuf(orig);
    
    // Sprawdź czy coś zostało wypisane
    std::string output = buffer.str();
    EXPECT_FALSE(output.empty());
}

