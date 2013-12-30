// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>

#include <fifi/simple_online.hpp>
#include <fifi/full_table.hpp>
#include <fifi/log_table.hpp>
#include <fifi/extended_log_table.hpp>
#include <fifi/field_types.hpp>
#include <fifi/arithmetics.hpp>
#include <fifi/default_field.hpp>

template<class FieldImpl>
void test_alignment(uint32_t length, uint32_t start_offset)
{

    typedef typename FieldImpl::value_type value_type;
    typedef typename FieldImpl::field_type field_type;

    FieldImpl impl;

    std::vector<uint8_t> dest;
    std::vector<uint8_t> src;
    std::vector<uint8_t> temp;

    // We allocate enough buffer for the value_type elements + the
    // start_offset in bytes
    uint32_t data_length = (length * sizeof(value_type)) + start_offset;

    dest.resize(data_length);
    src.resize(data_length);
    temp.resize(data_length);

    value_type *dest_ptr = reinterpret_cast<value_type*>(&dest[start_offset]);
    value_type *src_ptr = reinterpret_cast<value_type*>(&src[start_offset]);
    value_type *temp_ptr = reinterpret_cast<value_type*>(&temp[start_offset]);

    for(uint32_t i = 0; i < length; ++i)
    {
        dest_ptr[i] = rand() % field_type::order;
        src_ptr[i] = rand() % field_type::order;
        temp_ptr[i] = rand() % field_type::order;
    }

    fifi::add(impl, dest_ptr, src_ptr, length);
    fifi::subtract(impl, dest_ptr, src_ptr, length);
    fifi::multiply(impl, dest_ptr, src_ptr, length);

    value_type coefficient = rand() % field_type::order;
    fifi::multiply_constant(impl, coefficient, dest_ptr, length);
    fifi::multiply_add(impl, coefficient, dest_ptr, src_ptr, temp_ptr, length);
    fifi::multiply_subtract(impl, coefficient, dest_ptr, src_ptr, temp_ptr, length);
}

void test_alignment(uint32_t length, uint32_t start_offset)
{
    test_alignment<fifi::simple_online<fifi::binary> >(
        length, start_offset);

    test_alignment< fifi::simple_online<fifi::binary8> >(
        length, start_offset);

    test_alignment< fifi::simple_online<fifi::binary16> >(
        length, start_offset);

    test_alignment< fifi::full_table<fifi::binary8> >(
        length, start_offset);

    test_alignment< fifi::log_table<fifi::binary8> >(
        length, start_offset);

    test_alignment< fifi::log_table<fifi::binary16> >(
        length, start_offset);

    test_alignment< fifi::extended_log_table<fifi::binary8> >(
        length, start_offset);

    test_alignment< fifi::extended_log_table<fifi::binary16> >(
        length, start_offset);

    test_alignment< fifi::optimal_prime<fifi::prime2325> >(
        length, start_offset);

    // Just also run the default fields
    test_alignment< fifi::default_field<fifi::binary>::type >(
        length, start_offset);

    test_alignment< fifi::default_field<fifi::binary8>::type >(
        length, start_offset);

    test_alignment< fifi::default_field<fifi::binary16>::type >(
        length, start_offset);
}


TEST(test_arithmetics, alignment)
{
    for(uint32_t i = 0; i <= 16; ++i)
    {
        test_alignment(100, i);
    }

    uint32_t data_length = (rand() % 1000) + 1;
    for(uint32_t i = 0; i <= 16; ++i)
    {
        test_alignment(data_length, i);
    }
}


