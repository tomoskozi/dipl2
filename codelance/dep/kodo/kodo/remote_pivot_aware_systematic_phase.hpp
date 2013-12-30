// Copyright Steinwurf Apes 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <boost/dynamic_bitset.hpp>

namespace kodo
{

    /// @ingroup encoder_layers
    ///
    /// @brief This class is used to detect whether the encoder should
    ///        produce a systematic symbol based on the remote pivot
    ///        status (i.e. the symbols marked as pivot at a remote)
    template<class SuperCoder>
    class remote_pivot_aware_systematic_phase : public SuperCoder
    {
    public:

        /// @return true if we are in the systematic phase (i.e. there
        ///         are systematic packet to send) otherwise false
        bool in_systematic_phase() const
        {
            if(single_symbol_available())
            {
                return true;
            }
            else
            {
                return SuperCoder::in_systematic_phase();
            }
        }

        /// @return The index of the next symbol to be sent in a
        uint32_t next_systematic_symbol() const
        {
            if(single_symbol_available())
            {
                uint32_t next_symbol = 0;
                bool next_symbol_found = false;

                for(uint32_t i = 0; i < SuperCoder::symbols(); ++i)
                {
                    bool is_remote_pivot =
                        SuperCoder::remote_is_symbol_pivot(i);

                    bool is_pivot = SuperCoder::is_symbol_pivot(i);

                    if(!is_remote_pivot && is_pivot)
                    {
                        next_symbol_found = true;
                        next_symbol = i;
                        break;
                    }
                }

                assert(next_symbol_found);
                return next_symbol;

            }
            else
            {
                return SuperCoder::next_systematic_symbol();
            }

        }

        /// This function returns whether the encoder only has a single symbol
        /// available not already at the decoder. Initially this function was
        /// implemented as:
        ///
        ///    return SuperCoder::remote_rank() + 1 == SuperCoder::rank();
        ///
        /// However this is not sufficient if the decoder also
        /// receives packets from another source. This function
        /// therefore loops over all the local pivots and checks
        /// whether it only has one pivot not seen by the deocder if
        /// this is the case it returns that a single symbol is
        /// available.
        ///
        /// @return true if a single symbol is available.
        bool single_symbol_available() const
        {
            uint32_t difference = 0;
            for(uint32_t i = 0; i < SuperCoder::symbols(); ++i)
            {
                bool is_remote_pivot =
                    SuperCoder::remote_is_symbol_pivot(i);

                bool is_pivot = SuperCoder::is_symbol_pivot(i);

                if(!is_remote_pivot && is_pivot)
                {
                    ++difference;
                }

                if(difference > 1)
                {
                    break;
                }
            }

            return difference == 1;

        }

    };

}

