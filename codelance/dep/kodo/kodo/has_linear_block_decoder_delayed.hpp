// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "linear_block_decoder_delayed.hpp"

namespace kodo
{

    /// @ingroup type_traits
    /// Type trait helper allows compile time detection of whether an
    /// encoder / decoder contains the linear_block_decoder_delayed
    ///
    /// Example:
    ///
    /// typedef kodo::full_rlnc8_decoder decoder_t;
    ///
    /// if(kodo::has_linear_block_decoder_delayed<decoder_t>::value)
    /// {
    ///     // Do something here
    /// }
    ///
    template<class T>
    struct has_linear_block_decoder_delayed
    {
        template<class U>
        static uint8_t test(const kodo::linear_block_decoder_delayed<U> *);

        static uint32_t test(...);

        static const bool value = sizeof(test(static_cast<T*>(0))) == 1;
    };

}


