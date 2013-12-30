// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>

#include <fifi/simple_online.hpp>
#include <fifi/field_types.hpp>

#include "expected_results.hpp"

// ----- Tests for binary8 -----

TEST(test_simple_online, binary8fieldDegree)
{
    EXPECT_EQ( fifi::find_degree<fifi::binary8>(2), 1);
    EXPECT_EQ( fifi::find_degree<fifi::binary8>(17), 4);
}

TEST(test_simple_online, binary8multiply)
{
    check_results_multiply<fifi::simple_online<fifi::binary8> >();
}

TEST(test_simple_online, binary8invert)
{
    check_results_invert<fifi::simple_online<fifi::binary8> >();
}

TEST(test_simple_online, binary8divide)
{
    check_results_divide<fifi::simple_online<fifi::binary8> >();
}

TEST(test_simple_online, binary8add)
{
    check_results_add<fifi::simple_online<fifi::binary8> >();
}

TEST(test_simple_online, binary8subtract)
{
    check_results_subtract<fifi::simple_online<fifi::binary8> >();
}

TEST(test_simple_online, binary8randCheck)
{
    check_random<fifi::simple_online<fifi::binary8> >();
}

// ----- Tests for binary16 -----

TEST(test_simple_online, binary16multiply)
{
    check_results_multiply<fifi::simple_online<fifi::binary16> >();
}

TEST(test_simple_online, binary16invert)
{
    check_results_invert<fifi::simple_online<fifi::binary16> >();
}

TEST(test_simple_online, binary16divide)
{
    check_results_divide<fifi::simple_online<fifi::binary16> >();
}

TEST(test_simple_online, binary16add)
{
    check_results_add<fifi::simple_online<fifi::binary16> >();
}

TEST(test_simple_online, binary16subtract)
{
    check_results_subtract<fifi::simple_online<fifi::binary16> >();
}

TEST(test_simple_online, binary16randCheck)
{
    check_random<fifi::simple_online<fifi::binary16> >();
}

