// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#ifndef FIFI_LOG_TABLE_HPP
#define FIFI_LOG_TABLE_HPP

#include <algorithm>
#include <cmath>
#include <cassert>
#include <stdint.h>
#include <vector>

#include "simple_online.hpp"
#include "binary_extension_field.hpp"

namespace fifi
{
    /// Produces an extended log table for multiplication and division.
    template<class Field>
    class log_table : public binary_extension_field<Field>
    {
    public:

        /// Typedef of the data type used for each field element
        typedef typename Field::value_type value_type;

        /// Typedef of the data type used for each field element
        typedef typename Field::order_type order_type;

        /// Typedef of the field type used
        typedef Field field_type;

    public:

        /// Constructor
        log_table();


        /// @copydoc finite_field::multiply()
        value_type multiply(value_type element_one, value_type element_two) const;

        /// @copydoc finite_field::divide()
        value_type divide(value_type numerator, value_type denominator) const;

        /// @copydoc finite_field::invert()
        value_type invert(value_type element) const;

    public:

        /// The Log table
        std::vector<value_type> m_log;

        /// The AntiLog table
        std::vector<value_type> m_antilog;

        /// Assess to the simple online algorithms
        using binary_extension_field<Field>::m_simple_online;
    };

    template<class Field>
    log_table<Field>::log_table()
    {
        m_log.resize(Field::order, '\0');
        m_antilog.resize(Field::order, '\0');

        /// inital value corresponds x^0
        value_type power = 1;

        for(order_type i = 0; i < Field::order - 1; ++i)
        {
            m_log[power] = i;
            m_antilog[i] = power;

            // The decimal value 2 corresponds to the
            // polynomial element 'x'
            power = m_simple_online.multiply(2U, power);
        }

        // This handles the special case where the sum of two
        // exponents hit the maximum value and should be reduced
        // to zero. Instead of doing this we map this entry to
        // the same entry as if the sum had beem zero.
        m_antilog[Field::max_value] = 1;
    }

    template<class Field>
    typename log_table<Field>::value_type
    log_table<Field>::multiply(value_type element_one, value_type element_two) const
    {
        if(element_one == 0 || element_two == 0)
            return 0;

        value_type one = m_log[element_one];
        value_type two = m_log[element_two];

        // This might be a bit tricky, but what we do here is to
        // check for an integer overflow. If an overflow has happened
        // we have to +1 to sum: we may write the sum as (2^m * h) + l,
        // where h is the high-half of the sum and l is the low half.
        // Now we realize that 2^m is congurent to +1 when working mod (2^m -1)
        // So we may write (1 * h) + l, now we realize that h must also be 1 since
        // we had an overflow thus: If we had an overflow all we have to do is to
        // +1 to the sum.
        value_type sum = one + two;
        sum = sum < one ? sum + 1 : sum;

        // Notice that we handle the case where sum is equal to 2^m - 1, by remapping this
        // entry in the AntiLog table to 1
        return m_antilog[sum];
    }

    template<class Field>
    typename log_table<Field>::value_type
    log_table<Field>::divide(value_type numerator, value_type denominator) const
    {
        assert(denominator != 0);

        if(numerator == 0)
            return 0;

        value_type one = m_log[numerator];
        value_type two = m_log[denominator];

        // Again we have to do a bit of trickery, since we do not have a good
        // way to handle negative numbers when subtracting them we do the following
        // we realize the inverse element of power representation must be
        // inverse exponent = (2^W - 1) - exponent since
        // inverse exponent + exponent = 2^W - 1 and order mod 2^W - 1 = 0

        // Get inverse of denominator
        two = Field::max_value - two;

        // Now we may simply multiply like before
        value_type sum = one + two;
        sum = sum < one ? sum + 1 : sum;

        // Notice that we handle the case where sum is equal to 2^m - 1, by remapping this
        // entry in the AntiLog table to 1
        return m_antilog[sum];
    }

    template<class Field>
    typename log_table<Field>::value_type
    log_table<Field>::invert(value_type element) const
    {
        value_type power = m_log[element];

        // See log_table<Field>::divide
        power = Field::max_value - power;

        return m_antilog[power];
    }
}

#endif

