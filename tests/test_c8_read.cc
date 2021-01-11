#include "gtest/gtest.h"
#include <iostream>
#include <fstream>
#include <string>

#include "cummon.h"
#include "c8_read.h"

class c8_read : public ::testing::Test {
protected:
    std::string sample_filepath;
    const uint8_t data[15] = { 0x03, 0xFA, 0xAA, 0x98, 0x90, 0x32, 0x20, 0x34, 0xAB, 0xDE, 0xAD, 0xBE, 0xEF };
    uint8_t buffer[0xFFF];

    c8_read()
    {
        sample_filepath = "example.bin";
    }
    ~c8_read() {}

    virtual void SetUp()
    {
        std::ofstream sample_file(sample_filepath, std::ios::out | std::ios::binary);
        sample_file.write((const char *)data, NELEMS(data));
        sample_file.close();

        memset(buffer, 0x00, NELEMS(buffer));
    }

    virtual void TearDown()
    {
        remove(sample_filepath.c_str());
    }

};

TEST_F(c8_read, wrong_path)
{
    size_t bytes_read = C8_read_program("bad_path.lol", buffer, NELEMS(buffer));
    EXPECT_EQ(0, bytes_read);
}

TEST_F(c8_read, correct_file)
{
    size_t bytes_read = C8_read_program(sample_filepath.c_str(), buffer, NELEMS(buffer));
    EXPECT_EQ(NELEMS(data), bytes_read);
    EXPECT_EQ(0, memcmp(buffer, data, NELEMS(data)));

    memset(buffer, 0x00, NELEMS(buffer));
    bytes_read = C8_read_program(sample_filepath.c_str(), buffer, NELEMS(buffer));
    EXPECT_EQ(NELEMS(data), bytes_read);
    EXPECT_EQ(0, memcmp(buffer, data, NELEMS(data)));
}

TEST_F(c8_read, small_buffer)
{
    uint8_t small_b[5];

    size_t bytes_read = C8_read_program(sample_filepath.c_str(), small_b, NELEMS(small_b));
    EXPECT_EQ(0, bytes_read);
}
