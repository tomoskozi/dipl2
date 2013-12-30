// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <cassert>
#include <limits>

namespace fifi
{
    /// The binary field, containing the two elements {0,1}. We do not provide
    /// the degree() function since this is not an extension field.
    struct binary
    {

        /// The data type used for each element
        typedef uint8_t value_type;

        /// Pointer to a value_type
        typedef value_type* value_ptr;

        /// Reference to a value_type
        typedef value_type& value_ref;

        /// The data type used to hold the order of the field i.e. the number of
        /// elements
        typedef uint8_t order_type;

        /// The maximum decimal value of any field element
        const static value_type max_value = 1;

        /// The minimum decimal value for any field element
        const static value_type min_value = 0;

        /// The field order i.e. number of field elements
        const static order_type order = 2;

        /// The number of bits in a value_type
        const static uint32_t bits_per_value =
            std::numeric_limits<value_type>::digits;

    };

    /// A binary extension field with 2^8 elements
    struct binary8
    {
        /// The data type used for each element
        typedef uint8_t value_type;

        /// Pointer to a value_type
        typedef value_type* value_ptr;

        /// Reference to a value_type
        typedef value_type& value_ref;

        /// The data type used to hold the order of the field i.e. the number of
        /// elements
        typedef uint32_t order_type;

        /// The data type used to hold the degree of the field
        typedef uint32_t degree_type;

        /// The maximum decimal value of any field element
        const static value_type max_value = 255;

        /// The minimum decimal value for any field element
        const static value_type min_value = 0;

        /// The field order i.e. number of field elements
        const static order_type order = 256;

        /// The field degree
        const static degree_type degree = 8;

        /// X^8+X^4+X^3+X^2+1 = 285
        /// We do not need the top bit so we just use: X^4+X^3+X^2+1 = 29
        /// We do not need the top bit since this bit is not representable in
        /// the field. Our algorithms should manage whether it it necessary to
        /// reduce an element, if needed only lower bits of the prime is needed.
        const static value_type prime = 29;

    };

    /// A binary extension field with 2^16 elements
    struct binary16
    {

        /// The data type used for each element
        typedef uint16_t value_type;

        /// Pointer to a value_type
        typedef value_type* value_ptr;

        /// Reference to a value_type
        typedef value_type& value_ref;

        /// The data type used to hold the order of the field
        /// i.e. the number of elements
        typedef uint32_t order_type;

        /// The data type used to hold the degree of the field
        typedef uint32_t degree_type;

        /// The maximum decimal value of any field element
        const static value_type max_value = 65535;

        /// The minimum decimal value for any field element
        const static value_type min_value = 0;

        /// The field order i.e. number of field elements
        const static order_type order = 65536;

        /// The field degree
        const static degree_type degree = 16;

        /// X^16+X^12+X^3+X+1 = 69643
        /// We do not need the top bit so we just use: X^12+X^3+X+1 = 4107
        /// We do not need the top bit since this bit is not representable in
        /// the field. Our algorithms should manage whether it it necessary to
        /// reduce an element, if needed only lower bits of the prime is needed.
        const static value_type prime = 4107;

    };

    /// A binary extension field with 2^32 elements
    struct binary32
    {
        /// value type typedef
        typedef uint32_t value_type;

        /// Pointer to a value_type
        typedef value_type* value_ptr;

        /// Reference to a value_type
        typedef value_type& value_ref;

        /// @return the max value
        static value_type max_value()
        { return 4294967295U; }

        /// @return the min value
        static value_type min_value()
        { return 0; }

        /// @return the order
        static uint64_t order()
        { return 4294967296ULL; }

        /// @return the degree
        static uint32_t degree()
        { return 32; }

    };

    /// Prime field 2^32 - 5
    /// Practical implementations may use the mapping algorithm which was
    /// proposed by Crowley et al. and which allows an efficient mapping of
    /// arbitarty binary data to the 2^32 - 5 prime field.
    struct prime2325
    {

        /// The data type used for each element
        typedef uint32_t value_type;

        /// Pointer to a value_type
        typedef value_type* value_ptr;

        /// Reference to a value_type
        typedef value_type& value_ref;

        /// The data type used to hold the order of the field
        /// i.e. the number of elements
        typedef uint32_t order_type;

        /// The maximum decimal value of any field element
        const static value_type max_value = 4294967290U;

        /// The minimum decimal value for any field element
        const static value_type min_value = 0;

        /// The field order i.e. number of field elements
        const static order_type order = 4294967291U;

        /// The prime number used i.e. (2^32 - 5)
        const static value_type prime = 4294967291U;

        /// When use with the Crowley mapping algorithm
        /// the maximum size in bytes a block can be
        /// (2^29 - 1) * 4 to get bytes
        const static uint32_t max_block_size = 2147483644U;

        /// When use with the Crowley mapping algorithm the maximum size in
        /// bytes a block can be 2^29 - 1 data words (where a word is 32 bits).
        const static uint32_t max_block_length = 536870911U;

        /// @param block_length the length of a block
        /// @return the length of the prefix in bits needed to ensure that given
        ///         the block length (i.e. the number of 32 bit values) there
        ///         exists a bit prefix not available in the data
        static uint32_t prefix_length(uint32_t block_length);

    };


    inline uint32_t prime2325::prefix_length(uint32_t block_length)
    {
        assert(block_length > 0);
        assert(block_length <= prime2325::max_block_length);

        // In the absence of a log2 function we look for the highest top bit.
        uint32_t topbit;
        for(topbit = 31; topbit --> 0;)
        {
            if(block_length >> topbit)
                break;
        }

        // How many values can be represented where the most significant bit is
        // topbit.  Create a mask of 0000000..1111..1 where the MSBit is in the
        // topbit position
        uint32_t prefix_values = ~(~0 << topbit) + 1;

        if(prefix_values > block_length)
        {
            return topbit;
        }
        else
        {
            return topbit + 1;
        }
    }

}

