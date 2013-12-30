// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <sak/convert_endian.hpp>

#include "remote_rank_writer.hpp"
#include "payload_rank_writer.hpp"

namespace kodo
{

    /// @ingroup payload_codec_layers state_layers
    /// @brief The payload rank recoder writes the rank of the encoder into
    ///        the payload buffer.
    ///
    /// The payload rank recoder will forward rank returned by the
    /// seen_encoder_rank() function. This is done to avoid problems where
    /// a recoder has seen a rank of e.g. 3 but itself has rank 1. If the
    /// recoder would forward its own rank another decoder might falsely
    /// detect is_partially_complete as it also sees rank 1.
    template<class SuperCoder>
    class payload_rank_recoder :
        public payload_rank_writer<remote_rank_writer<SuperCoder> >
    { };

}

