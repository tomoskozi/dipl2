// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <fifi/fifi_utils.hpp>

#include "coefficient_value_access.hpp"

namespace kodo
{

    /// @brief Provides functions to access and manipulate values in
    ///        an array of coefficients, but with an added offset to the
    ///        indexes.
    ///
    /// This is useful in cases where we want to create e.g. an non-square
    /// decoding matrix. E.g. imagine this matrix:
    ///
    ///    1 0 1 0 1 1
    ///    0 1 1 1 0 0
    ///    0 1 0 1 0 1
    ///
    /// By default we will always access the coding coefficients from the
    /// beginning of the encoding vector. Which enables us to decode e.g.
    /// a follows:
    ///
    ///    1 0 0 0 1 0
    ///    0 1 0 1 0 1
    ///    0 0 1 0 0 1
    ///
    /// This has nicely create 3 pivot positions from in the first 3 columns.
    /// Now, lets imagine that we wanted to shift those pivot positions so that
    /// they would appear in the last three columns. This could be done by
    /// adding a offset to the index when inspecting the coefficient values.
    template<class SuperCoder>
    class offset_coefficient_value_access :
        public coefficient_value_access<SuperCoder>
    {
    public:

        /// The actual super class, we exploit that most of the functionality
        /// needed is already implemented in the coefficient_value_access
        /// layer
        typedef coefficient_value_access<SuperCoder> Super;

        /// @copydoc layer::value_type;
        typedef typename Super::value_type value_type;

    public:

        /// Constructor
        offset_coefficient_value_access()
            : m_offset(0)
        { }

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            Super::initialize(the_factory);
            m_offset = 0U;
        }

        /// @copydoc coefficient_value_access::coefficient_value(
        ///             const value_type*, uint32_t) const
        value_type coefficient_value(const value_type* coefficients,
                                     uint32_t index) const
        {
            index += m_offset;
            assert(index < SuperCoder::coefficients_elements());

            return Super::coefficient_value(coefficients, index);
        }

        /// @copydoc coefficient_value_access::coefficient_value(
        ///             const value_type*, uint32_t, value_type) const
        void set_coefficient_value(value_type* coefficients, uint32_t index,
                                   value_type value) const
        {
            index += m_offset;
            assert(index < SuperCoder::coefficients_elements());

            Super::set_coefficient_value(coefficients, index, value);
        }

        /// Sets the coefficient offset which will be added when accessing
        /// coefficient values
        /// @param offset The offset to add
        void set_coefficient_offset(uint32_t offset)
        {
            assert(offset < SuperCoder::coefficients_elements());
            m_offset = offset;
        }

        /// @return The offset currently used
        uint32_t coefficient_offset() const
        {
            return m_offset;
        }

    private:

        /// Stores the offset values used to offset index access
        uint32_t m_offset;

    };

}


