// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <fifi/fifi_utils.hpp>

namespace kodo
{

    /// @ingroup symbol_id_layers
    /// @brief Base class for the plain symbol id reader and writer
    template<class SuperCoder>
    class plain_symbol_id : public SuperCoder
    {
    public:

        /// @copydoc layer::field_type;
        typedef typename SuperCoder::field_type field_type;

    public:

        /// @ingroup factory_layers
        /// The factory layer associated with this coder.
        class factory : public SuperCoder::factory
        {
        public:

            /// @copydoc layer::factory::factory(uint32_t,uint32_t)
            factory(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory(max_symbols, max_symbol_size)
            { }

            /// @copydoc layer::factory::max_id_size() const
            uint32_t max_id_size() const
            {
                return SuperCoder::factory::max_coefficient_vector_size();
            }
        };

    public:

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_id_size = SuperCoder::coefficient_vector_size();
            assert(m_id_size > 0);
        }

        /// @copydoc layer::id_size()
        uint32_t id_size() const
        {
            return m_id_size;
        }

    protected:

        /// The number of bytes needed to store the symbol id
        /// i.e. the coding coefficients
        uint32_t m_id_size;

    };

}


