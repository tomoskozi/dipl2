// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "on_the_fly_codes.hpp"

#include "../pivot_status_writer.hpp"
#include "../feedback_pivot_status_writer.hpp"
#include "../final_feedback_writer.hpp"

namespace kodo
{

    /// @ingroup fec_stacks
    /// @brief Implementation of a complete RLNC sliding window decoder
    ///
    /// This configuration extends the on_the_fly_decoder with the
    /// possibility to generate feedback to the sliding_window_encoder
    /// which will remove packets which have already been (partially)
    /// decoded from the encoding. To see an example of how to use
    /// this feature there is an example in the
    /// kodo/examples/sliding_window which demonstrates the use fo the
    /// feedback API.
    template<class Field>
    class sliding_window_decoder :
        public // Feedback API
               feedback_pivot_status_writer<
               final_feedback_writer<
               // Payload API
               partial_decoding_tracker<
               rank_symbol_decoding_status_updater<
               payload_recoder<on_the_fly_recoding_stack,
               payload_rank_decoder<
               payload_decoder<
               // Codec Header API
               systematic_decoder<
               symbol_id_decoder<
               // Symbol ID API
               plain_symbol_id_reader<
               // Decoder API
               aligned_coefficients_decoder<
               forward_linear_block_decoder<
               pivot_status_writer<
               rank_info<
               symbol_decoding_status_counter<
               symbol_decoding_status_tracker<
               // Coefficient Storage API
               coefficient_value_access<
               coefficient_storage<
               coefficient_info<
               // Storage API
               deep_symbol_storage<
               storage_bytes_used<
               storage_block_info<
               // Finite Field API
               finite_field_math<typename fifi::default_field<Field>::type,
               finite_field_info<Field,
               // Factory API
               final_coder_factory_pool<
               // Final type
               sliding_window_decoder<Field>
                   > > > > > > > > > > > > > > > > > > > > > > > > >
    { };

}



