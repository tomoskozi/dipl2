// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/make_shared.hpp>
#include <boost/optional.hpp>

#include <sak/storage.hpp>

#include <fifi/is_binary.hpp>
#include <fifi/fifi_utils.hpp>

#include <kodo/forward_linear_block_decoder_policy.hpp>
#include <kodo/backward_linear_block_decoder_policy.hpp>

namespace kodo
{

    /// @ingroup decoder_layers
    /// @ingroup codec_layers
    /// @brief Implements basic linear block decoder.
    ///
    /// The linear block decoder
    /// expects that an encoded symbol is described by a vector of
    /// coefficients. Using these coefficients the block decoder subtracts
    /// incoming symbols until the original data has been recreated.
    template<class DirectionPolicy, class SuperCoder>
    class bidirectional_linear_block_decoder : public SuperCoder
    {
    public:

        /// @copydoc layer::field_type
        typedef typename SuperCoder::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename field_type::value_type value_type;

        /// @copydoc layer::factory
        typedef typename SuperCoder::factory factory;

        /// The direction policy determining the direction we iterate through
        /// the coding coefficients
        typedef DirectionPolicy direction_policy;

    public:

        /// Constructor
        bidirectional_linear_block_decoder()
            : m_maximum_pivot(0)
        { }

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory &the_factory)
        {
            SuperCoder::construct(the_factory);
        }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            // Depending on the policy we either go from 0 to symbols or
            // from symbols to 0.
            m_maximum_pivot =
                direction_policy::min(0, the_factory.symbols() - 1);
        }

        /// @copydoc layer::decode_symbol(uint8_t*,uint8_t*)
        void decode_symbol(uint8_t *symbol_data,
                           uint8_t *symbol_coefficients)
        {
            assert(symbol_data != 0);
            assert(symbol_coefficients != 0);

            value_type *symbol =
                reinterpret_cast<value_type*>(symbol_data);

            value_type *coefficients =
                reinterpret_cast<value_type*>(symbol_coefficients);

            decode_coefficients(symbol, coefficients);

            update_symbol_status();
        }

        /// @copydoc layer::decode_symbol(uint8_t*, uint32_t)
        void decode_symbol(uint8_t *symbol_data,
                           uint32_t symbol_index)
        {
			//printf("%u < %u\n", symbol_index, SuperCoder::symbols());
            assert(symbol_index < SuperCoder::symbols());
            assert(symbol_data != 0);

            if(SuperCoder::is_symbol_decoded(symbol_index))
            {
                return;
            }

            const value_type *symbol =
                reinterpret_cast<value_type*>( symbol_data );

            if(SuperCoder::is_symbol_seen(symbol_index))
            {
                swap_decode(symbol, symbol_index);
            }
            else
            {
                // Stores the symbol and updates the corresponding
                // encoding vector
                store_uncoded_symbol(symbol, symbol_index);

                // Backwards substitution
                value_type *coefficients =
                    SuperCoder::coefficient_vector_values(symbol_index);

                backward_substitute(symbol, coefficients, symbol_index);

                m_maximum_pivot =
                    direction_policy::max(symbol_index, m_maximum_pivot);

            }

            update_symbol_status();
        }

        /// @copydoc layer::is_complete() const
        bool is_complete() const
        {
            return rank() == SuperCoder::symbols();
        }

        /// @copydoc layer::rank() const
        uint32_t rank() const
        {
            return SuperCoder::symbols_seen() + SuperCoder::symbols_decoded();
        }

        /// @copydoc layer::is_symbol_pivot(uint32_t) const
        bool is_symbol_pivot(uint32_t index) const
        {
            assert(index < SuperCoder::symbols());
            return SuperCoder::is_symbol_seen(index) ||
                SuperCoder::is_symbol_decoded(index);
        }

    protected:

        /// Updates the symbol status to decoded if the decoder reaches full
        /// rank
        void update_symbol_status()
        {
            if(!is_complete())
                return;

            // We have finished decoding mark all symbols decoded
            for(uint32_t i = 0; i < SuperCoder::symbols(); ++i)
                SuperCoder::set_symbol_decoded(i);
        }

        /// Decodes a symbol based on the coefficients
        /// @param symbol_data buffer containing the encoding symbol
        /// @param symbol_id buffer containing the encoding vector
        void decode_coefficients(value_type *symbol_data,
                                 value_type *symbol_coefficients)
        {
            assert(symbol_data != 0);
            assert(symbol_coefficients != 0);

            // See if we can find a pivot
            auto pivot_index = forward_substitute_to_pivot(
                symbol_data, symbol_coefficients);

            if(!pivot_index)
                return;

            if(!fifi::is_binary<field_type>::value)
            {
                // Normalize symbol and vector
                normalize(symbol_data,symbol_coefficients,*pivot_index);
            }

            // Reduce the symbol further
            forward_substitute_from_pivot(
                symbol_data, symbol_coefficients, *pivot_index);

            // Now with the found pivot reduce the existing symbols
            backward_substitute(
                symbol_data, symbol_coefficients, *pivot_index);

            // Now save the received symbol
            store_coded_symbol(
                symbol_data, symbol_coefficients, *pivot_index);

            m_maximum_pivot =
                direction_policy::max(*pivot_index, m_maximum_pivot);
        }

        /// When adding a raw symbol (i.e. uncoded) with a specific
        /// pivot id and the decoder already contains a coded symbol
        /// in that position this function performs the proper swap
        /// between the two symbols.
        /// @param symbol_data the data for the raw symbol
        /// @param pivot_index the pivot position of the raw symbol
        void swap_decode(const value_type *symbol_data,
                         uint32_t pivot_index)
        {
            assert(SuperCoder::is_symbol_seen(pivot_index));
            assert(!SuperCoder::is_symbol_decoded(pivot_index));
            assert(!SuperCoder::is_symbol_missing(pivot_index));

            SuperCoder::set_symbol_missing(pivot_index);

            value_type *symbol_i = SuperCoder::symbol_value(pivot_index);

            value_type *vector_i =
                SuperCoder::coefficient_vector_values(pivot_index);

            value_type value =
                SuperCoder::coefficient_value(vector_i, pivot_index);

            assert(value == 1);

            // Subtract the new pivot symbol
            SuperCoder::set_coefficient_value(vector_i, pivot_index, 0U);

            SuperCoder::subtract(symbol_i, symbol_data,
                                 SuperCoder::symbol_length());

            // Now continue our new coded symbol we know that it must
            // if found it will contain a pivot id > that the current.
            decode_coefficients(symbol_i, vector_i);

            // The previous vector may still be in memory
            std::fill_n(vector_i, SuperCoder::coefficient_vector_length(), 0);

            // Stores the symbol and sets the pivot in the vector
            store_uncoded_symbol(symbol_data, pivot_index);

            // No need to backwards substitute since we are
            // replacing an existing symbol. I.e. backwards
            // substitution must already have been done.
        }

        /// Iterates the encoding vector from where a pivot has been
        /// identified and subtracts existing symbols
        /// @param symbol_data the data of the encoded symbol
        /// @param symbol_id the data constituting the encoding vector
        /// @param pivot_index the index of the found pivot element
        void normalize(value_type *symbol_data,
                       value_type *symbol_id,
                       uint32_t pivot_index)
        {

            assert(symbol_id != 0);
            assert(symbol_data != 0);

            assert(pivot_index < SuperCoder::symbols());

            assert(!SuperCoder::is_symbol_seen(pivot_index));
            assert(!SuperCoder::is_symbol_decoded(pivot_index));
            assert(SuperCoder::is_symbol_missing(pivot_index));

            value_type coefficient =
                SuperCoder::coefficient_value(symbol_id, pivot_index);

            assert(coefficient > 0);

            value_type inverted_coefficient =
                SuperCoder::invert(coefficient);

            // Update symbol and corresponding vector
            SuperCoder::multiply(symbol_id, inverted_coefficient,
                                 SuperCoder::coefficient_vector_length());

            SuperCoder::multiply(symbol_data, inverted_coefficient,
                                 SuperCoder::symbol_length());

        }

        /// Iterates the encoding vector and subtracts existing symbols
        /// until a pivot element is found.
        /// @param symbol_data the data of the encoded symbol
        /// @param symbol_id the data constituting the encoding vector
        /// @return the pivot index if found.
        boost::optional<uint32_t> forward_substitute_to_pivot(
            value_type *symbol_data,
            value_type *symbol_id)
        {
            assert(symbol_id != 0);
            assert(symbol_data != 0);

            uint32_t start = direction_policy::min(0, SuperCoder::symbols()-1);
            uint32_t end = direction_policy::max(0, SuperCoder::symbols()-1);

            for(direction_policy p(start, end); !p.at_end(); p.advance())
            {
                uint32_t i = p.index();

                value_type current_coefficient =
                    SuperCoder::coefficient_value(symbol_id, i);

                if(!current_coefficient)
                    continue;

                if(!is_symbol_pivot(i))
                    return boost::optional<uint32_t>( i );

                value_type *vector_i =
                    SuperCoder::coefficient_vector_values( i );

                value_type *symbol_i =
                    SuperCoder::symbol_value( i );

                if(fifi::is_binary<field_type>::value)
                {
                    SuperCoder::subtract(symbol_id, vector_i,
                        SuperCoder::coefficient_vector_length());

                    SuperCoder::subtract(symbol_data, symbol_i,
                        SuperCoder::symbol_length());
                }
                else
                {
                    SuperCoder::multiply_subtract(
                        symbol_id, vector_i,
                        current_coefficient,
                        SuperCoder::coefficient_vector_length());

                    SuperCoder::multiply_subtract(symbol_data, symbol_i,
                        current_coefficient, SuperCoder::symbol_length());
                }

            }

            return boost::none;
        }

        /// Iterates the encoding vector from where a pivot has been
        /// identified and subtracts existing symbols
        /// @param symbol_data the data of the encoded symbol
        /// @param symbol_id the data constituting the encoding vector
        /// @param pivot_index the index of the found pivot element
        void forward_substitute_from_pivot(value_type *symbol_data,
                                           value_type *symbol_id,
                                           uint32_t pivot_index)
        {
            // We have received an encoded symbol - described
            // by the symbol group. We now normalize the
            // the encoding vector according to the symbol id.
            // I.e. we make sure the pivot position has a "1"
            assert(symbol_id != 0);
            assert(symbol_data != 0);

            assert(pivot_index < SuperCoder::symbols());

            assert(!SuperCoder::is_symbol_seen(pivot_index));
            assert(!SuperCoder::is_symbol_decoded(pivot_index));
            assert(SuperCoder::is_symbol_missing(pivot_index));

            // If this pivot index was smaller than the maximum pivot
            // index we have, we might also need to backward
            // substitute the higher pivot values into the new packet
            uint32_t end = direction_policy::max(0, SuperCoder::symbols()-1);

            direction_policy p(pivot_index, end);

            // Jump past the pivot_index position
            p.advance();

            for(; !p.at_end(); p.advance())
            {
                uint32_t i = p.index();

                // Do we have a non-zero value here?
                value_type value =
                    SuperCoder::coefficient_value(symbol_id, i);

                if( !value )
                {
                    // The coefficient is zero
                    continue;
                }

                if( !is_symbol_pivot(i) )
                {
                    // We do not have a pivot here
                    continue;
                }

                value_type *vector_i =
                    SuperCoder::coefficient_vector_values(i);

                value_type *symbol_i =
                    SuperCoder::symbol_value(i);

                if(fifi::is_binary<field_type>::value)
                {
                    SuperCoder::subtract(
                        symbol_id, vector_i,
                        SuperCoder::coefficient_vector_length());

                    SuperCoder::subtract(
                        symbol_data, symbol_i,
                        SuperCoder::symbol_length());
                }
                else
                {
                    SuperCoder::multiply_subtract(
                        symbol_id, vector_i, value,
                        SuperCoder::coefficient_vector_length());

                    SuperCoder::multiply_subtract(
                        symbol_data, symbol_i, value,
                        SuperCoder::symbol_length());
                }
            }
        }

        /// Backward substitute the found symbol into the
        /// existing symbols.
        /// @param symbol_data buffer containing the encoding symbol
        /// @param symbol_id buffer containing the encoding vector
        /// @param pivot_index the pivot index of the symbol in the
        ///        buffers symbol_id and symbol_data
        void backward_substitute(const value_type *symbol_data,
                                 const value_type *symbol_id,
                                 uint32_t pivot_index)
        {
            assert(symbol_id != 0);
            assert(symbol_data != 0);

            assert(pivot_index < SuperCoder::symbols());

            uint32_t from = direction_policy::min(0, SuperCoder::symbols()-1);
            uint32_t to = m_maximum_pivot;

            // We found a "1" that nobody else had as pivot, we now
            // substract this packet from other coded packets
            // - if they have a "1" on our pivot place
            for(direction_policy p(from, to); !p.at_end(); p.advance())
            {
                uint32_t i = p.index();

                if(i == pivot_index)
                {
                    // We cannot backward substitute into our self
                    continue;
                }

                if( SuperCoder::is_symbol_decoded(i) )
                {
                    // We know that we have no non-zero elements
                    // outside the pivot position.
                    continue;
                }

                if( SuperCoder::is_symbol_missing(i) )
                {
                    // We do not have a symbol yet here
                    continue;
                }

                // The symbol must be "seen"
                assert(SuperCoder::is_symbol_seen(i));

                value_type *vector_i =
                    SuperCoder::coefficient_vector_values(i);

                value_type value =
                    SuperCoder::coefficient_value(vector_i, pivot_index);

                if( !value )
                {
                    // The coefficient is zero
                    continue;
                }

                value_type *symbol_i = SuperCoder::symbol_value(i);

                if(fifi::is_binary<field_type>::value)
                {
                    SuperCoder::subtract(vector_i, symbol_id,
                        SuperCoder::coefficient_vector_length());

                    SuperCoder::subtract(symbol_i, symbol_data,
                        SuperCoder::symbol_length());
                }
                else
                {
                    // Update symbol and corresponding vector
                    SuperCoder::multiply_subtract(vector_i, symbol_id, value,
                        SuperCoder::coefficient_vector_length());

                    SuperCoder::multiply_subtract(symbol_i, symbol_data, value,
                        SuperCoder::symbol_length());
                }
            }
        }

        /// Store an encoded symbol and encoding vector with the specified
        /// pivot found.
        /// @param symbol_data buffer containing the encoding symbol
        /// @param symbol_coefficients buffer containing the symbol
        ///        coefficients
        /// @param pivot_index the pivot index
        void store_coded_symbol(const value_type *symbol_data,
                                const value_type *symbol_coefficients,
                                uint32_t pivot_index)
        {
            assert(!SuperCoder::is_symbol_seen(pivot_index));
            assert(!SuperCoder::is_symbol_decoded(pivot_index));
            assert(SuperCoder::is_symbol_missing(pivot_index));

            assert(SuperCoder::is_symbol_available(pivot_index));

            assert(symbol_coefficients != 0);
            assert(symbol_data != 0);

            auto coefficient_storage =
                sak::storage(symbol_coefficients,
                             SuperCoder::coefficient_vector_size());

            SuperCoder::set_coefficient_vector_data(
                pivot_index, coefficient_storage);

            // Mark this symbol seen
            SuperCoder::set_symbol_seen(pivot_index);

            // Copy it into the symbol storage
            sak::const_storage src =
                sak::storage(symbol_data, SuperCoder::symbol_size());

            SuperCoder::copy_into_symbol(pivot_index, src);
        }

        /// Stores an uncoded or fully decoded symbol
        /// @param symbol_data the data for the symbol
        /// @param pivot_index the pivot index of the symbol
        void store_uncoded_symbol(const value_type *symbol_data,
                                  uint32_t pivot_index)
        {
            assert(!SuperCoder::is_symbol_seen(pivot_index));
            assert(!SuperCoder::is_symbol_decoded(pivot_index));
            assert(SuperCoder::is_symbol_missing(pivot_index));

            assert(SuperCoder::is_symbol_available(pivot_index));

            assert(symbol_data != 0);
            assert(pivot_index < SuperCoder::symbols());

            // Update the corresponding vector
            value_type *vector_dest =
                SuperCoder::coefficient_vector_values( pivot_index );

            // Zero out the memory first
            std::fill_n(vector_dest, SuperCoder::coefficient_vector_length(), 0);

            SuperCoder::set_coefficient_value(vector_dest, pivot_index, 1U);

            // Mark this symbol decoded
            SuperCoder::set_symbol_decoded(pivot_index);

            // Copy it into the symbol storage
            sak::const_storage src =
                sak::storage(symbol_data, SuperCoder::symbol_size());

            SuperCoder::copy_into_symbol(pivot_index, src);
        }

    protected:

        /// Stores the current maximum pivot index
        uint32_t m_maximum_pivot;

    };

}

