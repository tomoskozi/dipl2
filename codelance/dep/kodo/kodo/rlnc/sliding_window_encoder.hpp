// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "full_vector_codes.hpp"
#include "sliding_window_generator.hpp"
#include "sliding_window_systematic_encoder.hpp"

#include "../partial_decoding_tracker.hpp"
#include "../rank_info.hpp"
#include "../payload_rank_encoder.hpp"
#include "../payload_rank_decoder.hpp"
#include "../payload_rank_recoder.hpp"
#include "../proxy_remote_rank.hpp"
#include "../coefficient_value_access.hpp"
#include "../rank_symbol_decoding_status_updater.hpp"

#include "../pivot_status_reader.hpp"
#include "../feedback_pivot_status_reader.hpp"
#include "../final_feedback_reader.hpp"

namespace kodo
{

    /// @ingroup fec_stacks
    /// @brief Implementation of a complete RLNC sliding window encoder
    ///
    /// This configuration extends the on_the_fly_encoder with the
    /// possibility to read feedback from the sliding_window_decoder
    /// which will remove packets which have already been (partially)
    /// decoded from the encoding. To see an example of how to use
    /// this feature there is an example in the
    /// kodo/examples/sliding_window which demonstrates the use fo the
    /// feedback API.
    template<class Field>
    class sliding_window_encoder :
        public // Feedback API
               feedback_pivot_status_reader<
               final_feedback_reader<
               // Payload Codec API
               payload_rank_encoder<
               payload_encoder<
               // Codec Header API
               sliding_window_systematic_encoder<
               symbol_id_encoder<
               // Symbol ID API
               plain_symbol_id_writer<
               // Coefficient Generator API
               sliding_window_generator<
               // Encoder API
               encode_symbol_tracker<
               zero_symbol_encoder<
               linear_block_encoder<
               storage_aware_encoder<
               pivot_status_reader<
               rank_info<
               // Coefficient Storage API
               coefficient_value_access<
               coefficient_info<
               // Symbol Storage API
               deep_symbol_storage<
               storage_bytes_used<
               storage_block_info<
               // Finite Field API
               finite_field_math<typename fifi::default_field<Field>::type,
               finite_field_info<Field,
               // Factory API
               final_coder_factory_pool<
               // Final type
               sliding_window_encoder<Field>
                   > > > > > > > > > > > > > > > > > > > > > >
    { };

}



