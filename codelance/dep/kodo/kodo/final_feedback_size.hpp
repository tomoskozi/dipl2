// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{

    /// @ingroup feedback_layers
    ///
    /// @brief This layer terminates a series of
    ///        layer::factory::max_feedback_size() const and
    ///        layer::feedback_size() const calls.
    template<class SuperCoder>
    class final_feedback_size : public SuperCoder
    {
    public:

        /// The factory layer
        class factory : public SuperCoder::factory
        {
        public:

            /// @copydoc layer::factory::factory(uint32_t,uint32_t)
            factory(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory(max_symbols, max_symbol_size)
            { }

            /// @copydoc layer::factory::max_feedback_size() const
            uint32_t max_feedback_size() const
            {
                return 0;
            }
        };

    public:

        /// @copydoc layer::feedback_size() const
        uint32_t feedback_size() const
        {
            return 0;
        }

    };

}
