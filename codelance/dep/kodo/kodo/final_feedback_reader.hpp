// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "final_feedback_size.hpp"

namespace kodo
{

    /// @ingroup feedback_layers
    ///
    /// @brief This layer terminates a series of
    ///        layer::read_feedback(const uint8_t*) function calls.
    template<class SuperCoder>
    class final_feedback_reader : public final_feedback_size<SuperCoder>
    {
    public:

        /// @copydoc layer::read_feedback(const uint8_t*)
        void read_feedback(const uint8_t* feedback)
        {
            assert(feedback);
        }

    };

}
