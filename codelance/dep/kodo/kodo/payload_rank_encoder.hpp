// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <sak/convert_endian.hpp>

#include "rank_writer.hpp"
#include "payload_rank_writer.hpp"

namespace kodo
{

    /// @ingroup payload_codec_layers state_layers
    /// @brief The payload rank encoder writes the rank of the encoder into
    ///        the payload buffer.
    ///
    /// Storing the rank of the encoder in the payload buffer allows a decoder
    /// at a receiver to detect early decoding opportunities. This is possible
    /// if the receiver detects that it's decoder has the same rank as the
    /// encoder from which it is receiving packets. Note, that the rank of an
    /// encoder means how many packets that encoder has available for coding.
    /// So if the decoder has decoded as many packets as were available then
    /// we might be able to access the packets even before reaching full rank
    /// at the decoder. The reason we say "might" is that if the decoder is not
    /// a Gaussian Elimination decoder (which also does backward substitution)
    /// immediately then symbols might not yet be fully decoded. So you have to
    /// make sure the decoder used performs both forward and backward
    /// substitution on every symbols as it is being received to use this
    /// feature.
    template<class SuperCoder>
    class payload_rank_encoder :
        public payload_rank_writer<rank_writer<SuperCoder> >
    { };

}

