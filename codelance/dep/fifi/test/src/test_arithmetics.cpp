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
class test_arithmetics
{
public:

    typedef typename FieldImpl::value_type value_type;
    typedef typename FieldImpl::field_type field_type;

    /// @param length the length of the data buffers
    /// @param start_offset the start offset into the data buffers this
    ///        allows us to ensure that the functions work even in cases
    ///        where the addresses are not aligned on any specific memory
    ///        addresses
    test_arithmetics(uint32_t length, uint32_t start_offset)
        : m_length(length),
          m_start_offset(start_offset),
          m_data_length(length + start_offset)
        {
            m_dest.resize(m_data_length);
            m_temp.resize(m_data_length);
            m_src.resize(m_data_length);
            m_dest_check.resize(m_data_length);
            m_src_check.resize(m_data_length);
        }

    void setup()
        {
            for(uint32_t i = 0; i < m_data_length; ++i)
            {
                value_type d = rand() % field_type::order;
                value_type s = rand() % field_type::order;

                m_dest[i] = d;
                m_src[i]  = s;

                m_dest_check[i] = d;
                m_src_check[i]  = s;
            }
        }

    void test_add()
        {
            setup();

            fifi::add(m_impl,
                      &m_dest[0] + m_start_offset,
                      &m_src[0]  + m_start_offset, m_length);

            // Now we verify that we have computed:
            // dest[i] = dest[i] + src[i]
            for(uint32_t i = 0; i < m_length; ++i)
            {
                value_type result =
                    m_impl.add(m_dest_check[i + m_start_offset],
                               m_src_check[i + m_start_offset]);

                ASSERT_EQ(result, m_dest[i + m_start_offset]);
            }

        }


    void test_subtract()
        {
            setup();

            fifi::subtract(m_impl,
                           &m_dest[0] + m_start_offset,
                           &m_src[0] + m_start_offset,
                           m_length);

            // Now we verify that we have computed:
            // dest[i] = dest[i] + src[i]
            for(uint32_t i = 0; i < m_length; ++i)
            {
                value_type result =
                    m_impl.subtract(m_dest_check[i + m_start_offset],
                                    m_src_check[i + m_start_offset]);

                ASSERT_EQ(result, m_dest[i + m_start_offset]);
            }
        }

    void test_multiply()
        {
            setup();

            fifi::multiply(m_impl,
                           &m_dest[0] + m_start_offset,
                           &m_src[0] + m_start_offset,
                           m_length);

            // Now we verify that we have computed:
            // dest[i] = dest[i] + src[i]
            for(uint32_t i = 0; i < m_length; ++i)
            {
                value_type result =
                    m_impl.multiply(m_dest_check[i + m_start_offset],
                                    m_src_check[i + m_start_offset]);

                ASSERT_EQ(result, m_dest[i + m_start_offset]);
            }
        }


    void test_multiply_constant()
        {
            setup();

            value_type coefficient = rand() % field_type::order;

            fifi::multiply_constant(m_impl,
                                    coefficient,
                                    &m_dest[0] + m_start_offset,
                                    m_length);

            // Now we verify that we have computed:
            // dest[i] = dest[i] + src[i]
            for(uint32_t i = 0; i < m_length; ++i)
            {
                value_type result =
                    m_impl.multiply(coefficient,
                                    m_dest_check[i + m_start_offset]);

                ASSERT_EQ(result, m_dest[i + m_start_offset]);
            }
        }


    void test_multiply_add()
        {
            setup();

            value_type coefficient = rand() % field_type::order;

            fifi::multiply_add(m_impl, coefficient,
                               &m_dest[0] + m_start_offset,
                               &m_src[0] + m_start_offset,
                               &m_temp[0] + m_start_offset,
                               m_length);

            // Now we verify that we have computed:
            // dest[i] = dest[i] + (src[i]*c)
            for(uint32_t i = 0; i < m_length; ++i)
            {
                value_type multiplied =
                    m_impl.multiply(m_src_check[i + m_start_offset],
                                    coefficient);

                value_type result =
                    m_impl.add(m_dest_check[i + m_start_offset],
                               multiplied);

                ASSERT_EQ(result, m_dest[i + m_start_offset]);
            }
        }


    void test_multiply_subtract()
        {
            setup();

            value_type coefficient = rand() % field_type::order;

            fifi::multiply_subtract(m_impl, coefficient,
                                    &m_dest[0] + m_start_offset,
                                    &m_src[0] + m_start_offset,
                                    &m_temp[0] + m_start_offset,
                                    m_length);

            // Now we verify that we have computed:
            // dest[i] = dest[i] + (src[i]*c)
            for(uint32_t i = 0; i < m_length; ++i)
            {
                value_type multiplied =
                    m_impl.multiply(m_src_check[i + m_start_offset],
                                    coefficient);

                value_type result =
                    m_impl.subtract(m_dest_check[i + m_start_offset],
                                    multiplied);

                ASSERT_EQ(result, m_dest[i + m_start_offset]);
            }
        }



private:

    uint32_t m_length;
    uint32_t m_start_offset;
    uint32_t m_data_length;

    std::vector<value_type> m_dest;
    std::vector<value_type> m_src;
    std::vector<value_type> m_temp;

    std::vector<value_type> m_dest_check;
    std::vector<value_type> m_src_check;

    FieldImpl m_impl;

};

template<class FieldImpl>
void test_functions(uint32_t length, uint32_t start_offset)
{
    test_arithmetics<FieldImpl> s(length, start_offset);
    s.test_add();
    s.test_subtract();
    s.test_multiply_add();
    s.test_multiply_subtract();
    s.test_multiply();
    s.test_multiply_constant();
}

void test_functions(uint32_t length, uint32_t start_offset)
{
    test_functions<fifi::simple_online<fifi::binary> >(
        length, start_offset);

    test_functions< fifi::simple_online<fifi::binary8> >(
        length, start_offset);

    test_functions< fifi::simple_online<fifi::binary16> >(
        length, start_offset);

    test_functions< fifi::full_table<fifi::binary8> >(
        length, start_offset);

    test_functions< fifi::log_table<fifi::binary8> >(
        length, start_offset);

    test_functions< fifi::log_table<fifi::binary16> >(
        length, start_offset);

    test_functions< fifi::extended_log_table<fifi::binary8> >(
        length, start_offset);

    test_functions< fifi::extended_log_table<fifi::binary16> >(
        length, start_offset);

    test_functions< fifi::optimal_prime<fifi::prime2325> >(
        length, start_offset);

    // Just also run the default fields
    test_functions< fifi::default_field<fifi::binary>::type >(
        length, start_offset);

    test_functions< fifi::default_field<fifi::binary8>::type >(
        length, start_offset);

    test_functions< fifi::default_field<fifi::binary16>::type >(
        length, start_offset);
}


TEST(test_arithmetics, RunFunctions)
{
    for(uint32_t i = 0; i <= 16; ++i)
    {
        test_functions(100, i);
    }

    uint32_t data_length = (rand() % 1000) + 1;
    for(uint32_t i = 0; i <= 16; ++i)
    {
        test_functions(data_length, i);
    }

}


