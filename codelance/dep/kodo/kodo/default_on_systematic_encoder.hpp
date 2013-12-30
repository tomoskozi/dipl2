// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "systematic_encoder.hpp"
#include "storage_aware_systematic_phase.hpp"
#include "default_systematic_phase.hpp"

namespace kodo
{

    /// @brief Implements a systematic encoder where the default
    /// behavior is to produce systematic symbols if possible.
    template<class SuperCoder>
    class default_on_systematic_encoder :
        public systematic_encoder<
               default_systematic_phase<true,
               storage_aware_systematic_phase<SuperCoder> > >
    { };

}


