// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#ifndef FIFI_TEST_EXPECTED_RESULTS_HPP
#define FIFI_TEST_EXPECTED_RESULTS_HPP

#include <cstdlib>

#include <fifi/field_types.hpp>

// The expected result of an arithmetic operation
// e.g. result == operation(arg1, arg2).
// Where operation can be add, subtract, multiply
// and divide.
//
// Note, we can use the same results for all algorithms
// since they are over the same field they should
// produce the same results.
//
// Feel free to add more results to the tables.
//

template<class Field>
struct expected_result_1
{
    typename Field::value_type arg1;
    typename Field::value_type result;
};

template<class Field>
struct expected_result_2
{
    typename Field::value_type arg1;
    typename Field::value_type arg2;
    typename Field::value_type result;
};

template<class Field>
struct multiply_results;

/// Specialized struct which contains the multiplication
/// results for the binary8 field
template<>
struct multiply_results<fifi::binary8>
{
    static const expected_result_2<fifi::binary8> m_results[];
    static const uint32_t m_size;
};

/// Specialized struct which contains the multiplication
/// results for the binary16 field
template<>
struct multiply_results<fifi::binary16>
{
    static const expected_result_2<fifi::binary16> m_results[];
    static const uint32_t m_size;
};

/// Specialized struct which contains the multiplication
/// results for the prime2325 field
template<>
struct multiply_results<fifi::prime2325>
{
    static const expected_result_2<fifi::prime2325> m_results[];
    static const uint32_t m_size;
};

/// Helper template function which takes a FieldImpl as its
/// template argument and runs its multiply function
template<class FieldImpl>
inline void check_results_multiply()
{
    typedef typename FieldImpl::field_type field_type;

    FieldImpl field;

    uint32_t size = multiply_results<field_type>::m_size;

    for(uint32_t i = 0; i < size; ++i)
    {
        expected_result_2<field_type> res =
            multiply_results<field_type>::m_results[i];

        EXPECT_EQ(field.multiply(res.arg1, res.arg2), res.result);
    }
}


template<class Field>
struct divide_results;

/// Specialized struct which contains the multiplication
/// results for the binary8 field
template<>
struct divide_results<fifi::binary8>
{
    static const expected_result_2<fifi::binary8> m_results[];
    static const uint32_t m_size;
};

/// Specialized struct which contains the multiplication
/// results for the binary16 field
template<>
struct divide_results<fifi::binary16>
{
    static const expected_result_2<fifi::binary16> m_results[];
    static const uint32_t m_size;
};

/// Specialized struct which contains the multiplication
/// results for the prime2325 field
template<>
struct divide_results<fifi::prime2325>
{
    static const expected_result_2<fifi::prime2325> m_results[];
    static const uint32_t m_size;
};

/// Helper template function which takes a FieldImpl as its
/// template argument and runs its divide function
template<class FieldImpl>
inline void check_results_divide()
{
    typedef typename FieldImpl::field_type field_type;

    FieldImpl field;

    uint32_t size = divide_results<field_type>::m_size;

    for(uint32_t i = 0; i < size; ++i)
    {
        expected_result_2<field_type> res =
            divide_results<field_type>::m_results[i];

        EXPECT_EQ(field.divide(res.arg1, res.arg2), res.result);
    }
}


template<class Field>
struct add_results;

/// Specialized struct which contains the multiplication
/// results for the binary8 field
template<>
struct add_results<fifi::binary8>
{
    static const expected_result_2<fifi::binary8> m_results[];
    static const uint32_t m_size;
};

/// Specialized struct which contains the multiplication
/// results for the binary16 field
template<>
struct add_results<fifi::binary16>
{
    static const expected_result_2<fifi::binary16> m_results[];
    static const uint32_t m_size;
};

/// Specialized struct which contains the multiplication
/// results for the prime2325 field
template<>
struct add_results<fifi::prime2325>
{
    static const expected_result_2<fifi::prime2325> m_results[];
    static const uint32_t m_size;
};

/// Helper template function which takes a FieldImpl as its
/// template argument and runs its add function
template<class FieldImpl>
inline void check_results_add()
{
    typedef typename FieldImpl::field_type field_type;

    FieldImpl field;

    uint32_t size = add_results<field_type>::m_size;

    for(uint32_t i = 0; i < size; ++i)
    {
        expected_result_2<field_type> res =
            add_results<field_type>::m_results[i];

        EXPECT_EQ(field.add(res.arg1, res.arg2), res.result);
    }
}

template<class Field>
struct subtract_results;

/// Specialized struct which contains the multiplication
/// results for the binary8 field
template<>
struct subtract_results<fifi::binary8>
{
    static const expected_result_2<fifi::binary8> m_results[];
    static const uint32_t m_size;
};

/// Specialized struct which contains the multiplication
/// results for the binary16 field
template<>
struct subtract_results<fifi::binary16>
{
    static const expected_result_2<fifi::binary16> m_results[];
    static const uint32_t m_size;
};

/// Specialized struct which contains the multiplication
/// results for the prime2325 field
template<>
struct subtract_results<fifi::prime2325>
{
    static const expected_result_2<fifi::prime2325> m_results[];
    static const uint32_t m_size;
};

/// Helper template function which takes a FieldImpl as its
/// template argument and runs its subtract function
template<class FieldImpl>
inline void check_results_subtract()
{
    typedef typename FieldImpl::field_type field_type;

    FieldImpl field;

    uint32_t size = subtract_results<field_type>::m_size;

    for(uint32_t i = 0; i < size; ++i)
    {
        expected_result_2<field_type> res =
            subtract_results<field_type>::m_results[i];

        EXPECT_EQ(field.subtract(res.arg1, res.arg2), res.result);
    }
}

template<class Field>
struct invert_results;

/// Specialized struct which contains the multiplication
/// results for the binary8 field
template<>
struct invert_results<fifi::binary8>
{
    static const expected_result_1<fifi::binary8> m_results[];
    static const uint32_t m_size;
};

/// Specialized struct which contains the multiplication
/// results for the binary16 field
template<>
struct invert_results<fifi::binary16>
{
    static const expected_result_1<fifi::binary16> m_results[];
    static const uint32_t m_size;
};

/// Specialized struct which contains the multiplication
/// results for the prime2325 field
template<>
struct invert_results<fifi::prime2325>
{
    static const expected_result_1<fifi::prime2325> m_results[];
    static const uint32_t m_size;
};

/// Helper template function which takes a FieldImpl as its
/// template argument and runs its invert function
template<class FieldImpl>
inline void check_results_invert()
{
    typedef typename FieldImpl::field_type field_type;

    FieldImpl field;

    uint32_t size = invert_results<field_type>::m_size;

    for(uint32_t i = 0; i < size; ++i)
    {
        expected_result_1<field_type> res =
            invert_results<field_type>::m_results[i];

        EXPECT_EQ(field.invert(res.arg1), res.result);
    }
}

template<class FieldImpl>
inline void check_random()
{
    typedef typename FieldImpl::field_type field_type;

    FieldImpl field;

    uint32_t elements = 100;

    for(uint32_t i = 0; i < elements; ++i)
    {
        typename field_type::value_type v = rand() % field_type::order;

        if(v == 0)
            ++v;

        EXPECT_EQ( field.multiply(v, field.invert(v)), 1U);
        EXPECT_EQ( field.multiply(v, field.divide(1U, v)), 1U);
    }
}

#endif
