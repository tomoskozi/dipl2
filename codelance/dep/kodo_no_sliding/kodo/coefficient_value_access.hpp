// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <fifi/fifi_utils.hpp>

namespace kodo
{

    /// @brief Provides functions to access and manipulate values in
    ///        an array of coefficients
    template<class SuperCoder>
    class coefficient_value_access : public SuperCoder
    {
    public:

        /// @copydoc layer::field_type
        typedef typename SuperCoder::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename SuperCoder::value_type value_type;

    public:

        /// @copydoc layer::coefficient_value(const value_type*, uint32_t) const
        value_type coefficient_value(const value_type* coefficients,
                                     uint32_t index) const
        {
            assert(coefficients != 0);
            return fifi::get_value<field_type>(coefficients, index);
        }

        /// @copydoc layer::set_coefficient_value(value_type*,
        ///              uint32_t, value_type) const
        void set_coefficient_value(value_type* coefficients, uint32_t index,
                                   value_type value) const
        {
            assert(coefficients != 0);
            fifi::set_value<field_type>(coefficients, index, value);
        }

    };

}


