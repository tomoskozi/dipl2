// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cassert>
#include <cstdint>
#include <algorithm>

namespace kodo
{

    /// Policy object for determining the "direction" of the
    /// linear_block_decoder e.g. whether we will look for the
    /// first pivot from the beginning of the coefficients vector
    /// or in some other way.
    ///
    /// The backward_linear_block decoder will search for pivots from
    /// the right of the encoding vector.
    /// E.g. if you see an encoding vector like:
    ///
    ///   <-----------+ direction of search for pivots
    ///
    ///   0 1 0 1 1 0 0
    ///   ^       ^   ^
    ///   |       |   |
    ///   |       |   +----+  First coefficient searched
    ///   |       +--------+  First non zero coefficient
    ///   +----------------+  Last coefficient searched
    ///
    struct backward_linear_block_decoder_policy
    {
        /// Construct the policy with the desired start and stop interval
        /// but ends included so [start:stop]
        /// @param start The first element to look at
        /// @param stop The last element to look at
        backward_linear_block_decoder_policy(uint32_t start, uint32_t stop)
            : m_start(start+1),
              m_stop(stop)
        {
            assert(m_start > m_stop);
        }

        /// @return true if the policy is at the end
        bool at_end() const
        {
            return m_start <= m_stop;
        }

        /// Advance the policy to the next index
        void advance()
        {
            assert(!at_end());
            --m_start;
        }

        /// @return the current index
        uint32_t index() const
        {
            return m_start - 1;
        }

        /// @param a The first value
        /// @param b The second value
        /// @return the maximum value of the two input values
        static uint32_t max(uint32_t a, uint32_t b)
        {
            return std::min(a,b);
        }

        /// @param a The first value
        /// @param b The second value
        /// @return the minimum value of the two input values
        static uint32_t min(uint32_t a, uint32_t b)
        {
            return std::max(a,b);
        }

        /// The start value
        uint32_t m_start;

        /// The end value
        uint32_t m_stop;

    };

}

