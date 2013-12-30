// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "../check_partial_generator.hpp"
#include "../uniform_generator.hpp"
#include "../pivot_aware_generator.hpp"

namespace kodo
{

    /// @ingroup coefficient_generator_layers
    ///
    /// @brief Generates an uniform random coefficient (from the
    ///        chosen Finite Field) for every symbol. In addition
    ///        using the pivot_aware_generate means that we will only
    ///        generate non-zero coefficients for symbols which are
    ///        available locally.
    template<class SuperCoder>
    class on_the_fly_generator :
        public check_partial_generator<
               uniform_generator<
               pivot_aware_generator<
               SuperCoder> > >
    { };

}


