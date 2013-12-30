// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cmath>
#include <stdexcept>

#include <gtest/gtest.h>

#include <sak/storage.hpp>

#include <fifi/optimal_prime.hpp>
#include <fifi/field_types.hpp>
#include <fifi/prime2325_bitmap.hpp>
#include <fifi/prime2325_binary_search.hpp>
#include <fifi/prime2325_apply_prefix.hpp>

void print_bits_header()
{
        for(uint32_t i = 32; i--> 0;)
    {
        std::cout << std::setw(2) << i << " ";
    }

    std::cout << std::endl;
}

void print_bits(uint32_t v)
{
    for(uint32_t i = 32; i--> 0;)
    {
        std::cout << std::setw(2) << (v >> i & 0x1) << " ";
    }
    std::cout << std::endl;
}

TEST(test_prime2325, prefix_length)
{
    // 2^y => 1 << y

    // In 8 bits we can write 256 distinct bit values so 8 bit should
    // be enough for a block length of 255 etc.
    EXPECT_EQ(fifi::prime2325::prefix_length(127), 7U);
    EXPECT_EQ(fifi::prime2325::prefix_length(128), 8U);
    EXPECT_EQ(fifi::prime2325::prefix_length(255), 8U);
    EXPECT_EQ(fifi::prime2325::prefix_length(256), 9U);
    EXPECT_EQ(fifi::prime2325::prefix_length(511), 9U);
    EXPECT_EQ(fifi::prime2325::prefix_length(512), 10U);
    EXPECT_EQ(fifi::prime2325::prefix_length((1<<29) - 1), 29U);
}

/// Very simple test we just set a few values and check that we are fine
TEST(test_prime2325, prefix_bitmap)
{
    uint32_t block_length = 255;
    std::vector<uint32_t> data(block_length, 0);

    // Set some of the top 8 bits
    data[0] = 0x01000000;
    data[1] = 0x02000000;

    fifi::prime2325_bitmap p(block_length);

    EXPECT_EQ(p.m_mapping_bits, 32U);
    EXPECT_EQ(p.m_shift_prefix, 24U);

    uint32_t prefix = p.find_prefix(sak::storage(data));

    // Our data contains three values
    EXPECT_TRUE(prefix != 0x00000000U);
    EXPECT_TRUE(prefix != 0x01000000U);
    EXPECT_TRUE(prefix != 0x02000000U);
}

template<class Algorithm>
void test_find_one_prefix(uint32_t prefix_bits)
{
    // If we have x bits then 2^x gives us the possible
    // values then we make block length 2^x - 1, then there
    // is exactly one missing prefix
    uint32_t block_length = (1 << prefix_bits) - 1;

    std::vector<uint32_t> data(block_length);

    uint8_t skip_value = rand() % block_length;
    uint32_t prefix_length = fifi::prime2325::prefix_length(block_length);

    EXPECT_EQ(prefix_bits, prefix_length);

    uint32_t shift_prefix = 32 - prefix_length;

    for(uint32_t i = 0; i < block_length; ++i)
    {
        if(i >= skip_value)
        {
            data[i] = (i+1) << shift_prefix;
        }
        else
        {
            data[i] = i << shift_prefix;
        }
    }

    uint32_t missing_prefix = skip_value << shift_prefix;

    Algorithm p(block_length);

    uint32_t prefix = p.find_prefix(sak::storage(data));

    EXPECT_EQ(missing_prefix, prefix);
}

void test_find_one_prefix(uint32_t prefix_bits)
{
    test_find_one_prefix<fifi::prime2325_bitmap>(prefix_bits);
    test_find_one_prefix<fifi::prime2325_binary_search>(prefix_bits);
}

/// Select a random value and set all but that one
TEST(test_prime2325, find_one_prefix)
{
    test_find_one_prefix(7);
    test_find_one_prefix(8);
    test_find_one_prefix(9);
    test_find_one_prefix(10);

    uint32_t bits = (rand() % 20) + 1;
    test_find_one_prefix(bits);
}



template<class Algorithm>
void test_find_a_prefix(std::vector<uint32_t> data)
{
    uint32_t block_length = data.size();
    Algorithm p(block_length);

    uint32_t prefix = p.find_prefix(sak::storage(data));

    // Check that the prefix does not appear in the data
    for(uint32_t i = 0; i < block_length; ++i)
    {
        EXPECT_TRUE(data[i] != prefix);
    }

    // Apply the prefix and test that all values are below the prime
    fifi::apply_prefix(sak::storage(data), ~prefix);

    // Check that all values are below the prefix
    for(uint32_t i = 0; i < block_length; ++i)
    {
        EXPECT_TRUE(data[i] < fifi::prime2325::prime);
    }


}



void test_find_a_prefix(uint32_t block_length)
{
    {
        std::vector<uint32_t> data(block_length);

        for(uint32_t i = 0; i < block_length; ++i)
        {
            data[i] = rand();
        }

        test_find_a_prefix<fifi::prime2325_bitmap>(data);
        test_find_a_prefix<fifi::prime2325_binary_search>(data);
    }

    {
        std::vector<uint32_t> data(block_length, 0xffffffffU);

        test_find_a_prefix<fifi::prime2325_bitmap>(data);
        test_find_a_prefix<fifi::prime2325_binary_search>(data);
    }

}


/// For different block sizes find a prefix
TEST(test_prime2325, find_a_prefix)
{

    test_find_a_prefix(7);
    test_find_a_prefix(8);
    test_find_a_prefix(9);
    test_find_a_prefix(512);

    uint32_t bits = (rand() % 200000) + 1;
    test_find_a_prefix(bits);
}


/// Tests the size_needed function for the prime2325_bitmap
TEST(test_prime2325, size_needed_bitmap)
{
    // The size returned is in bytes. We need one bit
    // per possible field value. For a block length of 255
    // we need 8 bits that gives 256 different field values
    // that is 256 / 8 = 32
    EXPECT_EQ(fifi::prime2325_bitmap::size_needed(127), 16U);
    EXPECT_EQ(fifi::prime2325_bitmap::size_needed(128), 32U);
    EXPECT_EQ(fifi::prime2325_bitmap::size_needed(254), 32U);
    EXPECT_EQ(fifi::prime2325_bitmap::size_needed(255), 32U);

    // 512 = need 10 bits = 1024 different field values, this gives
    // 1024 / 8 = 128 bytes
    EXPECT_EQ(fifi::prime2325_bitmap::size_needed(512), 128U);
}

/// Tests the size_needed() function for the prime2325_binary_search
TEST(test_prime2325, binary_search_size_needed)
{
    // The size needed for the binary search depends on the
    // block length, the number of passes and the size of the
    // counter. In the below it is assumed that the counter is
    // uint32_t if this changes in the future the test has to
    // be updated

    uint32_t size_of_counter = sizeof(uint32_t);

    // The prefix length for 127 is 7 so space needed will be
    // 2^ceil(7/k_pass) * sizeof(uint32_t)
    EXPECT_EQ(fifi::prime2325_binary_search::size_needed(127, 1),
              128*size_of_counter);

    EXPECT_EQ(fifi::prime2325_binary_search::size_needed(127, 2),
              16*size_of_counter);

    EXPECT_EQ(fifi::prime2325_binary_search::size_needed(127, 3),
              8*size_of_counter);

    EXPECT_EQ(fifi::prime2325_binary_search::size_needed(127, 4),
              4*size_of_counter);

    EXPECT_EQ(fifi::prime2325_binary_search::size_needed(127, 5),
              4*size_of_counter);

    EXPECT_EQ(fifi::prime2325_binary_search::size_needed(127, 6),
              4*size_of_counter);

    EXPECT_EQ(fifi::prime2325_binary_search::size_needed(127, 7),
              2*size_of_counter);

    EXPECT_EQ(fifi::prime2325_binary_search::size_needed(127, 8),
              2*size_of_counter);

    EXPECT_EQ(fifi::prime2325_binary_search::size_needed(127, 20),
              2*size_of_counter);
}

