// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{

    /// @ingroup payload_codec_layers state_api
    /// @brief The payload rank recoder writes the rank of the encoder into
    ///        the payload buffer.
    ///
    /// The payload rank recoder will forward rank returned by the
    /// seen_encoder_rank() function. This is done to avoid problems where
    /// a recoder has seen a rank of e.g. 3 but itself has rank 1. If the
    /// recoder would forward its own rank another decoder might falsely
    /// detect is_partially_complete as it also sees rank 1.
    template<class SuperCoder>
    class payload_rank_writer : public SuperCoder
    {
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

        /// @copydoc layer::encode(uint8_t*)
        uint32_t encode(uint8_t* payload)
        {
            assert(payload != 0);

            // Write the encoder rank to the payload
            SuperCoder::write_rank(payload);

            uint32_t written = SuperCoder::rank_size();
            return SuperCoder::encode(payload + written) + written;
        }

        /// @copydoc layer::payload_size() const
        uint32_t payload_size() const
        {
            return SuperCoder::payload_size() + SuperCoder::rank_size();
        }

    };

}

