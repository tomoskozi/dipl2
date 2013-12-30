// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <boost/type_traits.hpp>

#include <gtest/gtest.h>

#include <fifi/finite_field.hpp>
#include <fifi/simple_online.hpp>
#include <fifi/full_table.hpp>
#include <fifi/extended_log_table.hpp>
#include <fifi/log_table.hpp>
#include <fifi/optimal_prime.hpp>

template<class FiniteField>
void finite_field_construct_helper(FiniteField &finite_field)
{
    typedef typename FiniteField::field_type field_type;
    typedef typename FiniteField::field_impl field_impl;
    typedef typename FiniteField::value_type value_type;

    // Just two random values
    value_type one = 128;
    value_type two = 54;

    value_type res;
    res = finite_field.multiply(one, two);

    // The binary field does not support divide and invert
    if(!boost::is_same<field_type, fifi::binary>::value)
    {
        res = finite_field.divide(one, two);
        res = finite_field.invert(one);
    }

    res = finite_field.add(one, two);
    res = finite_field.subtract(one, two);

    field_impl &field_reference = finite_field.impl();

    res = field_reference.multiply(one, two);

    // The binary field does not support divide
    if(!boost::is_same<field_type, fifi::binary>::value)
    {
        res = field_reference.divide(one, two);
        res = field_reference.invert(one);
    }

    res = field_reference.add(one, two);
    res = field_reference.subtract(one, two);

    (void)res; // quiet "set but not used" warning

    ASSERT_TRUE(true);
}


TEST(test_finite_field, Simple_Online_Binary)
{
    typedef fifi::simple_online<fifi::binary> field_type;
    fifi::finite_field<field_type> finite_field;
    finite_field_construct_helper(finite_field);

}
TEST(test_finite_field, Simple_Online_Binary8)
{
    typedef fifi::simple_online<fifi::binary8> field_type;
    fifi::finite_field<field_type> finite_field;
    finite_field_construct_helper(finite_field);
}
TEST(test_finite_field, Simple_Online_Binary16)
{
    typedef fifi::simple_online<fifi::binary16> field_type;
    fifi::finite_field<field_type> finite_field;
    finite_field_construct_helper(finite_field);
}

TEST(test_finite_field, Full_Table_Binary8)
{
    typedef fifi::full_table<fifi::binary8> field_type;
    fifi::finite_field<field_type> finite_field;
    finite_field_construct_helper(finite_field);
}

TEST(test_finite_field, Log_Table_Binary8)
{
    typedef fifi::log_table<fifi::binary8> field_type;
    fifi::finite_field<field_type> finite_field;
    finite_field_construct_helper(finite_field);
}
TEST(test_finite_field, Log_Table_Binary16)
{
    typedef fifi::log_table<fifi::binary16> field_type;
    fifi::finite_field<field_type> finite_field;
    finite_field_construct_helper(finite_field);
}

TEST(test_finite_field, Extended_Log_Table_Binary8)
{
    typedef fifi::extended_log_table<fifi::binary8> field_type;
    fifi::finite_field<field_type> finite_field;
    finite_field_construct_helper(finite_field);
}
TEST(test_finite_field, Extended_Log_Table_Binary16)
{
    typedef fifi::extended_log_table<fifi::binary16> field_type;
    fifi::finite_field<field_type> finite_field;
    finite_field_construct_helper(finite_field);
}

TEST(test_finite_field, Optimal_Prime_Prime2325)
{
    typedef fifi::optimal_prime<fifi::prime2325> field_type;
    fifi::finite_field<field_type> finite_field;
    finite_field_construct_helper(finite_field);
}

