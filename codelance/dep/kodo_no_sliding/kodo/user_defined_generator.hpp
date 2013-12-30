// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <algorithm>

namespace kodo
{

    /// @ingroup coefficient_generator_layers
    /// @brief This layer allows the user to specify the coefficients which
    ///        be used for the coding operation.
    template<class SuperCoder>
    class user_defined_generator : public SuperCoder
    {
    public:

        /// Constructor
        user_defined_generator()
            : m_coefficients(nullptr)
        { }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory &the_factory)
        {
            SuperCoder::initialize(the_factory);
            m_coefficients = nullptr;
        }

        /// @copydoc layer::generate(uint8_t*)
        void generate(uint8_t *coefficients)
        {
            assert(coefficients != 0);
            assert(m_coefficients != 0);
            assert(SuperCoder::rank() == SuperCoder::symbols());

            uint32_t size = SuperCoder::coefficient_vector_size();
            std::copy_n(m_coefficients, size, coefficients);
        }

        /// @param coefficients The coefficients which should
        ///        be used next time generate is called
        void set_user_coefficients(const uint8_t* coefficients)
        {
            assert(coefficients != 0);
            m_coefficients = coefficients;
        }

        /// @return The user specified coefficients
        const uint8_t* user_coefficients() const
        {
            return m_coefficients;
        }

    private:

        /// The user defined coding coefficients
        const uint8_t* m_coefficients;

    };
}


