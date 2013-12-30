// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{

    /// @ingroup coefficient_generator_layers
    /// @brief The pivot aware generator will determine whether to use the
    ///        layer::generate(uint8_t*) or layer::generate_partial(uint8_t*)
    template<class SuperCoder>
    class pivot_aware_generator : public SuperCoder
    {
    public:

        /// @copydoc layer::can_generate() const
        bool can_generate() const
        {
            return SuperCoder::rank() == SuperCoder::symbols();
        }

        /// @copydoc layer::can_generate(uint32_t) const
        bool can_generate(uint32_t index) const
        {
            return SuperCoder::is_symbol_pivot(index);
        }

    };
}


