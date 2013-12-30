// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#ifndef FIFI_EXTENDED_LOG_TABLE_HPP
#define FIFI_EXTENDED_LOG_TABLE_HPP

#include <algorithm>
#include <cmath>
#include <cassert>
#include <stdint.h>

#include "simple_online.hpp"
#include "binary_extension_field.hpp"

namespace fifi
{

    /// Produces an extended log table for multiplication
    /// and division.
    template<class Field>
    class extended_log_table : public binary_extension_field<Field>
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
        extended_log_table();

        /// @copydoc finite_field::multiply()
        value_type multiply(value_type element_one, value_type element_two) const;

        /// @copydoc finite_field::divide()
        value_type divide(value_type numerator, value_type denominator) const;

        /// @copydoc finite_field::invert()
        value_type invert(value_type element) const;

    public:

        /// The Log table
        std::vector<value_type> m_log_data;

        /// Pointer to the Log table data
        value_type *m_log;

        /// The AntiLog table
        std::vector<value_type> m_antilog_data;

        /// Pointer to the AntiLog table data, adjusted so that
        /// negative exponent sums are handled.
        /// I.e. m_antilog[-1] is valid.
        value_type *m_antilog;

        /// Assess to the simple online algorithms
        using binary_extension_field<Field>::m_simple_online;
    };


    template<class Field>
    extended_log_table<Field>::extended_log_table()
    {
        m_log_data.resize(Field::order, '\0');
        m_antilog_data.resize((3 * Field::order) - 5, '\0');

        // Array offsets
        uint32_t low_offset  = 0;
        uint32_t mid_offset  = low_offset + Field::order - 2;
        uint32_t high_offset = mid_offset + Field::order - 1;

        // inital value corresponds x^0
        value_type power = 1;

        for(order_type i = 0; i < Field::order - 1; ++i)
        {
            m_log_data[power] = i;
            m_antilog_data[mid_offset + i] = power;

            // The decimal value 2 corresponds to the
            // polynomial element 'x'
            power = m_simple_online.multiply(2U, power);
        }

        for(order_type i = 0; i < Field::order - 2; ++i)
        {
            m_antilog_data[low_offset  + i] = m_antilog_data[mid_offset + i + 1];
            m_antilog_data[high_offset + i] = m_antilog_data[mid_offset + i];
        }

        // Set the pointers
        m_log = &m_log_data[0];
        m_antilog = &m_antilog_data[0] + Field::order - 2;
    }

    template<class Field>
    typename extended_log_table<Field>::value_type
    extended_log_table<Field>::multiply(value_type element_one, value_type element_two) const
    {

        if(element_one == 0 || element_two == 0)
            return 0;

        value_type one = m_log[element_one];
        value_type two = m_log[element_two];

        return m_antilog[one + two];
    }

    template<class Field>
    typename extended_log_table<Field>::value_type
    extended_log_table<Field>::divide(value_type numerator, value_type denominator) const
    {
        assert(denominator != 0);

        if(numerator == 0)
            return 0;

        value_type one = m_log[numerator];
        value_type two = m_log[denominator];

        return m_antilog[one - two];
    }

    template<class Field>
    inline typename extended_log_table<Field>::value_type
    extended_log_table<Field>::invert(value_type element) const
    {
        return divide(1, element);
    }
}

#endif

