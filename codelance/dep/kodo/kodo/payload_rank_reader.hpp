// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <sak/convert_endian.hpp>

#include "rank_reader.hpp"

namespace kodo
{

    /// @ingroup payload_codec_layers
    ///
    /// @brief The payload rank reader is a building block for layers
    ///        that read rank information from the buffer of an
    ///        encoder/decoder payload.
    template<class SuperCoder>
    class payload_rank_reader : public SuperCoder
    {
    public:

        /// @copydoc layer::rank_type
        typedef typename SuperCoder::rank_type rank_type;

    public:

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

            /// @copydoc layer::factory::max_payload_size() const
            uint32_t max_payload_size() const
            {
                return SuperCoder::factory::max_payload_size() +
                    SuperCoder::factory::max_rank_size();
            }
        };

    public:

        /// Unpacks the symbol data and symbol header from the payload
        /// buffer.
        /// @copydoc layer::decode(uint8_t*)
        void decode(uint8_t* payload)
        {
            assert(payload != 0);

            SuperCoder::read_rank(payload);

            // We have read the rank from the payload buffer so we adjust the
            // buffer pointer
            uint32_t read = SuperCoder::rank_size();

            SuperCoder::decode(payload + read);
        }

        /// @copydoc layer::payload_size() const
        uint32_t payload_size() const
        {
            return SuperCoder::payload_size() + SuperCoder::rank_size();
        }

    };
}


