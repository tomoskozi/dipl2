// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>

#include <fifi/finite_field.hpp>
#include <fifi/simple_online.hpp>
#include <fifi/full_table.hpp>
#include <fifi/extended_log_table.hpp>
#include <fifi/log_table.hpp>
#include <fifi/optimal_prime.hpp>
#include <fifi/element.hpp>
#include <fifi/is_binary.hpp>

template<class FiniteField>
void element_construct_helper(FiniteField &finite_field)
{

    typedef typename FiniteField::field_type field_type;
    typedef typename FiniteField::field_impl field_impl;
    typedef typename FiniteField::value_type value_type;

    // Just two random values
    value_type one = rand() % (field_type::max_value + 1);
    value_type two = rand() % (field_type::max_value + 1);

    fifi::element<FiniteField> e1(one, finite_field);
    fifi::element<FiniteField> e2(two, finite_field);

    fifi::element<FiniteField> e3(finite_field);

    e3 = e1 + e2;
    e3 += e2;

    e3 = e1 - e2;
    e3 -= e2;

    e3 = e1 * e2;
    e3 *= e2;

    if(e2.value() == 0)
        e2 += fifi::element<FiniteField>(1, finite_field);

    // Since the binary field operates on vectors
    // of elements and not single bits it is not
    // compatible with this test. Example:
    // We select the values 'one' and 'two' to
    // be either 0 or 1. That means that the
    // value_type will contain 0x000..0 or 0x000..1
    // since every element is a bit this means that
    // we will divide by zero in the cases where the
    // bit is zero - which is of course not allowed.
    // Therefore we have to either update the test
    // or not test inversion/division in the binary
    // field
    if(fifi::is_binary<field_type>::value == false)
    {
        e3 = e1 / e2;
        e3 /= e2;
    }

    //Maybe some actual testing should be done here?
    EXPECT_TRUE(true);
}


TEST(test_element, Simple_Online_Binary)
{
    typedef fifi::simple_online<fifi::binary> field_type;
    fifi::finite_field<field_type> finite_field;
    element_construct_helper(finite_field);
}

TEST(test_element, Simple_Online_Binary8)
{
    typedef fifi::simple_online<fifi::binary8> field_type;
    fifi::finite_field<field_type> finite_field;
    element_construct_helper(finite_field);
}

TEST(test_element, Simple_Online_Binary16)
{
    typedef fifi::simple_online<fifi::binary16> field_type;
    fifi::finite_field<field_type> finite_field;
    element_construct_helper(finite_field);
}

TEST(test_element, Full_Table_Binary8)
{
    typedef fifi::full_table<fifi::binary8> field_type;
    fifi::finite_field<field_type> finite_field;
    element_construct_helper(finite_field);
}

TEST(test_element, Log_Table_Binary8)
{
    typedef fifi::log_table<fifi::binary8> field_type;
    fifi::finite_field<field_type> finite_field;
    element_construct_helper(finite_field);
}
TEST(test_element, Log_Table_Binary16)
{
    typedef fifi::log_table<fifi::binary16> field_type;
    fifi::finite_field<field_type> finite_field;
    element_construct_helper(finite_field);
}

TEST(test_element, Extended_Log_Table_Binary8)
{
    typedef fifi::extended_log_table<fifi::binary8> field_type;
    fifi::finite_field<field_type> finite_field;
    element_construct_helper(finite_field);
}
TEST(test_element, Extended_Log_Table_Binary16)
{
    typedef fifi::extended_log_table<fifi::binary16> field_type;
    fifi::finite_field<field_type> finite_field;
    element_construct_helper(finite_field);
}

TEST(test_element, Optimal_Prime_Prime2325)
{
    typedef fifi::optimal_prime<fifi::prime2325> field_type;
    fifi::finite_field<field_type> finite_field;
    element_construct_helper(finite_field);
}

