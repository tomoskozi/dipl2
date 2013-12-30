// Copyright Steinwurf ApS 2011-2013.
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

#include <kodo/has_linear_block_decoder_delayed.hpp>

namespace kodo
{

    /// @ingroup payload_codec
    ///
    /// @brief This layer can be added to a decoding stack to update
    /// the status of the different decoding symbols. The
    /// functionality makes it possible for a user to detect when
    /// partial decoding is possible i.e. when symbols in decoder are
    /// fully decoded even though the full data block has not been
    /// sent.
    ///
    /// Detecting whether data has been partially decoded can be done by
    /// tracking whether encoder and decoder has the same rank. This of course
    /// means that we depend on the decoder performs full Gaussian elimination
    /// on the incoming symbols.
    ///
    /// To figure out which symbols have been partially decoded the decoder's
    /// layer::is_symbol_decoded(uint32_t) const function can be used.
    template<class SuperCoder>
    class rank_symbol_decoding_status_updater : public SuperCoder
    {
    public:

        /// @copydoc layer::rank_type
        typedef typename SuperCoder::rank_type rank_type;

    public:

        /// The linear_block_decoder_delayed is not compatible with this
        /// layer since it does not perform the full Gaussian Elimination.
        /// This means that even though the encoder and decoder rank matches
        /// the backward substitution step will not be performed until the
        /// decoder reaches full rank.
        static_assert(!has_linear_block_decoder_delayed<SuperCoder>::value,
                      "Layer not compatible with decoders which do not "
                      "perform full backward substitution on each incoming "
                      "symbol");

    public:

        /// @copydoc layer::decode(uint8_t*)
        void decode(uint8_t *payload)
        {
            SuperCoder::decode(payload);

            rank_type decoder_rank = SuperCoder::rank();
            rank_type remote_rank = SuperCoder::remote_rank();

            // If the decoder and encoder rank matches we know that when
            // using Gaussian Elimination with backward substitution that
            // symbols stored must be fully decoded
            if(decoder_rank == remote_rank)
            {
                for(uint32_t i = 0; i < SuperCoder::symbols(); ++i)
                {
                    if(SuperCoder::is_symbol_seen(i))
                    {
                        SuperCoder::set_symbol_decoded(i);
                    }
                }

                assert(SuperCoder::rank() == SuperCoder::symbols_decoded());
            }
        }

    };

}

