// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>

#include <fifi/is_prime2325.hpp>

TEST(TestIsPrime2325, is_prime2325)
{
    EXPECT_TRUE(fifi::is_prime2325<fifi::binary>::value == false);
    EXPECT_TRUE(fifi::is_prime2325<fifi::binary8>::value == false);
    EXPECT_TRUE(fifi::is_prime2325<fifi::binary16>::value == false);
    EXPECT_TRUE(fifi::is_prime2325<fifi::prime2325>::value == true);
}

