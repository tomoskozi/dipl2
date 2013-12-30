// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <fifi/fifi_utils.hpp>

namespace kodo
{

    /// @ingroup coefficient_storage_layers
    /// @brief Provides info about the coding coefficients
    ///        used during encoding and decoding.
    ///
    template<class SuperCoder>
    class coefficient_info : public SuperCoder
    {
    public:

        /// @copydoc layer::field_type
        typedef typename SuperCoder::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename field_type::value_type value_type;

    public:

        /// @ingroup factory_layers
        /// The factory layer associated with this coder.
        class factory : public SuperCoder::factory
        {
        public:

            /// @copydoc layer::factory::factory(uint32_t, uint32_t)
            factory(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory(max_symbols, max_symbol_size)
            { }

            /// @copydoc layer::factory::max_coefficient_vector_size() const
            uint32_t max_coefficient_vector_size() const
            {
                return fifi::elements_to_size<field_type>(
                    SuperCoder::factory::max_symbols());
            }

            /// @copydoc layer::factory::max_coefficient_vectors() const
            uint32_t max_coefficient_vectors() const
            {
                return SuperCoder::factory::max_symbols();
            }

        };

    public:

        /// Constructor
        coefficient_info()
            : m_coefficient_vector_length(0),
              m_coefficient_vector_size(0)
        { }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_coefficient_vector_length =
                fifi::elements_to_length<field_type>(the_factory.symbols());

            m_coefficient_vector_size =
                fifi::elements_to_size<field_type>(the_factory.symbols());

            assert(m_coefficient_vector_length > 0);
            assert(m_coefficient_vector_size > 0);
        }

        /// @copydoc layer::coefficient_vectors() const
        uint32_t coefficient_vectors() const
        {
            return SuperCoder::symbols();
        }

        /// @copydoc layer::coefficient_vector_elements() const
        uint32_t coefficient_vector_elements() const
        {
            return SuperCoder::symbols();
        }

        /// @copydoc layer::coefficient_vector_length() const
        uint32_t coefficient_vector_length() const
        {
            assert(m_coefficient_vector_length > 0);
            return m_coefficient_vector_length;
        }

        /// @copydoc layer::coefficient_vector_size() const
        uint32_t coefficient_vector_size() const
        {
            assert(m_coefficient_vector_size > 0);
            return m_coefficient_vector_size;
        }

    private:

        /// The length of coefficients in value_type elements
        uint32_t m_coefficient_vector_length;

        /// The size of coefficients in bytes
        uint32_t m_coefficient_vector_size;
    };

}


