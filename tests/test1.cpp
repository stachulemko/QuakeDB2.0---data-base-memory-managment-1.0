#include <gtest/gtest.h>
#include <iostream>

// Prosty test przykładowy
TEST(SampleTest, BasicTest) {
    EXPECT_EQ(1, 1);
    EXPECT_TRUE(true);
}

TEST(SampleTest, StringTest) {
    std::string hello = "Hello";
    std::string world = "World";
    EXPECT_NE(hello, world);
}

// Test dla QuakeDB
TEST(QuakeDBTest, InitializationTest) {
    // Tu będą testy dla twojej bazy danych
    EXPECT_TRUE(true); // Placeholder
}