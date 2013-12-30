// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <fifi/fifi_utils.hpp>
#include <sak/storage.hpp>

namespace kodo
{

    /// @ingroup coefficient_storage_layers
    /// @brief Provides storage and access to the coding coefficients
    ///        used during encoding and decoding.
    template<class SuperCoder>
    class coefficient_storage : public SuperCoder
    {
    public:

        /// @copydoc layer::field_type
        typedef typename SuperCoder::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename field_type::value_type value_type;

    public:

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory &the_factory)
        {
            SuperCoder::construct(the_factory);

            m_coefficients_storage.resize(
                the_factory.max_coefficient_vectors());

            for(auto& storage : m_coefficients_storage)
            {
                storage.resize(the_factory.max_coefficient_vector_size());
            }

        }

        /// @copydoc layer::coefficient_vector_data(uint32_t)
        uint8_t* coefficient_vector_data(uint32_t index)
        {
            assert(index < SuperCoder::symbols());
            return &(m_coefficients_storage[index])[0];
        }

        /// @copydoc layer::coefficient_vector_data(uint32_t) const
        const uint8_t* coefficient_vector_data(uint32_t index) const
        {
            assert(index < SuperCoder::symbols());
            return &(m_coefficients_storage[index])[0];
        }

        /// @copydoc layer::coefficient_vector_value(uint32_t)
        value_type* coefficient_vector_values(uint32_t index)
        {
            return reinterpret_cast<value_type*>(
                coefficient_vector_data(index));
        }

        /// @copydoc layer::coefficient_vector_value(uint32_t) const
        const value_type* coefficient_vector_values(uint32_t index) const
        {
            return reinterpret_cast<const value_type*>(
                coefficient_vector_data(index));
        }

        /// @copydoc layer::set_coefficient_vector_data(
        ///              uint32_t,const sak::const_storage&)
        void set_coefficient_vector_data(uint32_t index,
                                         const sak::const_storage &storage)
        {
            assert(storage.m_size == SuperCoder::coefficient_vector_size());
            assert(storage.m_data != 0);

            auto dest = sak::storage(
                coefficient_vector_data(index),
                SuperCoder::coefficient_vector_size());

            sak::copy_storage(dest, storage);
        }

    private:

        /// Stores the encoding vectors, we do a vector of vectors
        /// to maximize chances of proper alignment of individual
        /// coefficient vectors. This is needed when using SSE etc.
        /// instructions for fast computations with the coefficients
        std::vector< std::vector<uint8_t> > m_coefficients_storage;

    };
}


