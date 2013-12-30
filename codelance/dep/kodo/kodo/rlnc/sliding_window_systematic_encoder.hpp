// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../systematic_encoder.hpp"
#include "../storage_aware_systematic_phase.hpp"
#include "../default_systematic_phase.hpp"
#include "../remote_pivot_aware_systematic_phase.hpp"

namespace kodo
{

    /// @ingroup encoder_layers
    ///
    /// @brief Implements a systematic encoder where systematic
    ///        symbols are generate if one of the following two
    ///        conditions apply: 1) The symbol has not been sent
    ///        systematically before 2) There is only one symbol which
    ///        has not yet been received at the remote receiver.
    template<class SuperCoder>
    class sliding_window_systematic_encoder :
        public systematic_encoder<
               default_systematic_phase<true,
               remote_pivot_aware_systematic_phase<
               storage_aware_systematic_phase<SuperCoder> > > >
    { };

}


