// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <sak/convert_endian.hpp>
#include <sak/storage.hpp>

#include <boost/type_traits/is_base_of.hpp>

#include "systematic_base_coder.hpp"

namespace kodo
{

    /// @ingroup codec_header_layers
    /// @brief Systematic encoding layer.
    ///
    /// Specifying the SystematicOn template argument true will initialize
    /// the systematic encoder to produce systematic packets. If
    /// SystematicOn is false a user must first call the set_systematic_on()
    /// function to start producing systematic packets.
    template<class SuperCoder>
    class systematic_encoder : public SuperCoder
    {
    public:

        /// The symbol count type
        typedef typename systematic_base_coder::counter_type
            counter_type;

        /// The flag type
        typedef typename systematic_base_coder::flag_type
            flag_type;

    public:

        /// @ingroup factory_layers
        /// The factory layer associated with this coder.
        /// In this case only needed to provide the max_payload_size()
        /// function.
        class factory : public SuperCoder::factory
        {
        public:

            /// @copydoc layer::factory::factory(uint32_t,uint32_t)
            factory(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory(max_symbols, max_symbol_size)
            { }

            /// @copydoc layer::max_header_size() const
            uint32_t max_header_size() const
            {
                return SuperCoder::factory::max_header_size() +
                    sizeof(flag_type) + sizeof(counter_type);
            }
        };

    public:

        /// @copydoc layer::encode(uint8_t*, uint8_t*)
        uint32_t encode(uint8_t *symbol_data, uint8_t *symbol_header)
        {
            assert(symbol_data != 0);
            assert(symbol_header != 0);

            // The systematic phase takes places when we still haven't
            // generated more symbols than what currently accessible
            // to the encoder

            if(SuperCoder::in_systematic_phase())
            {
                return encode_systematic(symbol_data, symbol_header);
            }
            else
            {
                return encode_non_systematic(symbol_data, symbol_header);
            }
        }

        /// @copydoc layer::header_size() const
        uint32_t header_size() const
        {
            return SuperCoder::header_size() +
                sizeof(flag_type) + sizeof(counter_type);
        }

    protected:

        /// Encodes a systematic packet
        /// @copydoc layer::encode(uint8_t*,uint8_t*)
        uint32_t encode_systematic(uint8_t *symbol_data,
                                   uint8_t *symbol_header)
        {
            assert(symbol_data != 0);
            assert(symbol_header != 0);

            /// Flag systematic packet
            sak::big_endian::put<flag_type>(
                systematic_base_coder::systematic_flag, symbol_header);

            uint32_t next_symbol = SuperCoder::next_systematic_symbol();

            /// Set the symbol id
            sak::big_endian::put<counter_type>(
                next_symbol, symbol_header + sizeof(flag_type));

            SuperCoder::encode_symbol(symbol_data, next_symbol);

            return sizeof(flag_type) + sizeof(counter_type);
        }

        /// Encodes a non-systematic packets
        /// @copydoc layer::encode(uint8_t*,uint8_t*)
        uint32_t encode_non_systematic(uint8_t *symbol_data,
                                       uint8_t *symbol_header)
        {
            assert(symbol_data != 0);
            assert(symbol_header != 0);

            /// Flag non_systematic packet
            sak::big_endian::put<flag_type>(
                systematic_base_coder::non_systematic_flag,
                symbol_header);

            uint32_t bytes_consumed = SuperCoder::encode(
                symbol_data, symbol_header + sizeof(flag_type));

            return bytes_consumed + sizeof(flag_type);
        }

    };

}


