// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#ifndef FIFI_SIMPLE_ONLINE_HPP
#define FIFI_SIMPLE_ONLINE_HPP

#include <cstdlib>
#include <limits>
#include <cassert>

#include <boost/noncopyable.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_unsigned.hpp>

#include "field_types.hpp"

namespace fifi
{
    template<class Field>
    int find_degree(typename Field::value_type element)
    {
        int degree = 0;
        // Add a check for unsigned or arithmetic shifts
        element >>= 1;

        while(element > 0)
        {
            ++degree;
            element >>= 1;
        }

        return degree;
    }

    /// Simple online finite field algorithms - computes the results
    /// on the fly without relying on pre-computed look-up tables etc.
    template<class Field>
    class simple_online : public boost::noncopyable
    {
    public:

        /// Typedef of the data type used for each field element
        typedef typename Field::value_type value_type;

        /// Typedef of the field type used
        typedef Field field_type;

    public:

        /// Constructor
        simple_online()
        {}

        /// @copydoc finite_field::multiply()
        value_type multiply(value_type element_one, value_type element_two) const;

        /// @copydoc finite_field::divide()
        value_type divide(value_type numerator, value_type denominator) const;

        /// @copydoc finite_field::invert()
        value_type invert(value_type element) const;

        /// @copydoc finite_field::add()
        value_type add(value_type element_one, value_type element_two) const;

        /// @copydoc finite_field::subtract()
        value_type subtract(value_type element_one, value_type element_two) const;
    };

    template<class Field>
    inline typename simple_online<Field>::value_type
    simple_online<Field>::multiply(value_type element_one, value_type element_two) const
    {
        if((element_one == 0) || (element_two == 0))
            return 0;

        value_type high_bit_flag = 0;
        value_type low_bit_flag = 0;

        value_type high_bit_mask = 1 << (Field::degree - 1);

        value_type result = 0;

        for(unsigned i = 0; i < Field::degree; ++i)
        {
            low_bit_flag = element_two & 0x1;

            if( low_bit_flag != 0 )
            {
                result ^= element_one;
            }

            high_bit_flag = element_one & high_bit_mask;

            element_one <<= 1;
            element_two >>= 1;

            if( high_bit_flag != 0 )
            {
                element_one ^= Field::prime;
            }
        }
        return result;
    }

    template<class Field>
    inline typename simple_online<Field>::value_type
    simple_online<Field>::divide(value_type numerator, value_type denominator) const
    {
        return multiply(invert(denominator), numerator);
    }

    template<class Field>
    inline typename simple_online<Field>::value_type
    simple_online<Field>::invert(value_type element) const
    {
        assert(element != 0);

        // If element is 1 the inverse is 1, since we had to 'unwrap' the
        // first iteration (see below), we make an explicit check here.
        if(element == 1)
            return 1;

        value_type r_large = Field::prime;
        value_type r_small = element;

        value_type y_large = 0;
        value_type y_small = 1;

        // Since we cannot use the element data type to represent the
        // field polynomial we would not get a correct degree calculation
        // in the first iteration. Thus we perform the first interation
        // of the algorithm here with the correct prime degree. In the
        // following iterations of the algorithm the degree of r_large, which
        // initially holds our polynomial will be representable in value_type
        int j = Field::degree - find_degree<Field>(r_small);

        assert(j > 0);

        r_large ^= (r_small << j);
        y_large ^= (y_small << j);

        while(r_large != 1)
        {
            assert(r_large > 1);

            j = find_degree<Field>(r_large) - find_degree<Field>(r_small);

            if(j < 0)
            {
                std::swap(r_large, r_small);
                std::swap(y_large, y_small);

                j = std::abs(j);
            }

            r_large ^= (r_small << j);
            y_large ^= (y_small << j);
        }

        return y_large;
    }

    template<class Field>
    inline typename simple_online<Field>::value_type
    simple_online<Field>::add(value_type element_one, value_type element_two) const
    {
        return element_one ^ element_two;
    }

    template<class Field>
    inline typename simple_online<Field>::value_type
    simple_online<Field>::subtract(value_type element_one, value_type element_two) const
    {
        return element_one ^ element_two;
    }

    /// @copydoc simple_online::multiply()
    template<>
    inline simple_online<binary>::value_type
    simple_online<binary>::multiply(value_type element_one, value_type element_two) const
    {
        return element_one & element_two;
    }

    /// @copydoc simple_online::divide()
    template<>
    inline simple_online<binary>::value_type
    simple_online<binary>::divide(value_type numerator, value_type denominator) const
    {
        // Since we consider value_type a vector of single bit elements we would
        // have to only allow division by denominator 0xFFFF...FF since we cannot
        // divide by zero. The two asserts should check that we only divide with
        // all ones. We know the usefulness of this is limited but include it
        // for completeness
        BOOST_STATIC_ASSERT((boost::is_unsigned<value_type>::value == true));
        assert(denominator == std::numeric_limits<value_type>::max());

        return numerator & denominator;
    }

    /// @copydoc simple_online::invert()
    template<>
    inline simple_online<binary>::value_type
    simple_online<binary>::invert(value_type element) const
    {
        // As with division we do not really have a good way of reason to
        // implement the inverse function for the binary field. There is only
        // one non-zero element which is its own inverse. Anyway again for the
        // sake of completeness.
        BOOST_STATIC_ASSERT((boost::is_unsigned<value_type>::value == true));
        assert(element == std::numeric_limits<value_type>::max());

        return element;
    }
}

#endif

