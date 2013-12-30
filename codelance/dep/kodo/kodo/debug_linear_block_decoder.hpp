// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <cassert>
#include <iostream>
#include <vector>
#include <iomanip>


#include <fifi/fifi_utils.hpp>

namespace kodo
{

    /// @ingroup debug
    /// @brief Debug layer which allows inspecting the state of a
    ///        linear block decoder.
    ///
    /// The debug layer will print out the coding coefficients used by
    /// the decoder, when the print_decoder_state() function is called.
    ///
    /// This will print a matrix like the following (in the following
    /// case we have 3 symbols):
    ///
    /// 0 U:  1 0 0
    /// 1 C:  0 1 1
    /// 2 ?:  0 0 0
    ///
    /// The first line should be interpret as follows:
    ///
    ///   0 U:  1 0 0
    ///   ^ ^   ^ ^ ^
    ///   | |   | | +-----+  3rd coding coefficient
    ///   | |   | +-------+  2nd coding coefficient
    ///   | |   +---------+  1st coding coefficient
    ///   | +-------------+  The symbol state
    ///   +---------------+  The symbol's pivot position
    ///
    /// A symbol can be in 3 states:
    ///
    /// U: The symbol is uncoded (i.e. only one non zero coefficient should
    ///    be in the coding coefficients.
    /// C: The symbol is coded
    /// ?: No symbol for this position in the decoder has been see yet.
    ///
    /// The coding coefficient values will depend on the chosen finite
    /// field
    template<class SuperCoder>
    class debug_linear_block_decoder : public SuperCoder
    {
    public:

        /// @copydoc layer::field_type
        typedef typename SuperCoder::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename field_type::value_type value_type;

    public:

        /// Prints the decoder's state to the output stream
        /// @param out, the output stream
        void print_decoder_state(std::ostream &out) const
        {
            for(uint32_t i = 0; i < SuperCoder::symbols(); ++i)
            {
                if (SuperCoder::is_symbol_missing(i))
                {
                    out << std::setfill(' ') << std::setw(3) << i << " ?:  ";
                }
                else if (SuperCoder::is_symbol_seen(i))
                {
                    out << std::setfill(' ') << std::setw(3) << i << " C:  ";
                }
                else
                {
                    assert(SuperCoder::is_symbol_decoded(i));
                    out << std::setfill(' ') << std::setw(3) << i << " U:  ";
                }

                const value_type* c = SuperCoder::coefficient_vector_values(i);

                for(uint32_t j = 0; j < SuperCoder::coefficient_vectors(); ++j)
                {

                    static_assert(sizeof(uint32_t) >= sizeof(value_type),
                                  "value_type will overflow in this print");

                    value_type value = SuperCoder::coefficient_value(c, j);
                    out << (uint32_t)value << " ";
                }

                out << std::endl;
            }

        }

    };

}

