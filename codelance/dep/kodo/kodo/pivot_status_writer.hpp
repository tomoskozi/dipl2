// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>

#include "pivot_status_bitset.hpp"

namespace kodo
{

    /// @ingroup state_layers
    ///
    /// @brief The pivot status writer keeps track of which symbols
    ///        currently have been marked as pivot in the decoding
    ///        matrix. This information can then be written using the
    ///        write function.
    template<class SuperCoder>
    class pivot_status_writer : public pivot_status_bitset<SuperCoder>
    {
    public:

        /// The actual super type
        typedef pivot_status_bitset<SuperCoder> Super;

        /// Access the bitset
        using Super::m_pivot_status;

    public:

        /// @copydoc layer::set_symbol_missing(uint32_t)
        void set_symbol_missing(uint32_t index)
        {
            assert(index < m_pivot_status.size());
            Super::set_symbol_missing(index);
            m_pivot_status.reset(index);
        }

        /// @copydoc layer::set_symbol_seen(uint32_t)
        void set_symbol_seen(uint32_t index)
        {
            assert(index < m_pivot_status.size());
            Super::set_symbol_seen(index);
            m_pivot_status.set(index);
        }

        /// @copydoc layer::set_symbol_decoded(uint32_t)
        void set_symbol_decoded(uint32_t index)
        {
            assert(index < m_pivot_status.size());
            Super::set_symbol_decoded(index);
            m_pivot_status.set(index);
        }

        /// Writes the pivot status to the provided buffer. The pivot status
        /// is a bit vector where each symbol represented by a 0 if the symbol
        /// is missing or a 1 if the symbol is pivot (seen or decoded).
        ///
        /// @note The buffer used must have at least the size reported by the
        ///       pivot_status_bitset<SuperCoder>::pivot_status_size()
        ///       function.
        ///
        ///
        void write_pivot_status(uint8_t *buffer) const
        {
            assert(buffer);
            boost::to_block_range(m_pivot_status, buffer);
        }

    };

}
