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
    /// @brief The pivot status reader provides the functionality
    ///        needed to read information about pivot elements at a
    ///        "remote" device. This information can then later be use
    ///        to improve the performance of the encode / decoding.
    template<class SuperCoder>
    class pivot_status_reader : public pivot_status_bitset<SuperCoder>
    {
    public:

        /// The actual super type
        typedef pivot_status_bitset<SuperCoder> Super;

        /// @copydoc layer::rank_type
        typedef typename Super::rank_type rank_type;

        /// Access the bitset
        using Super::m_pivot_status;

    public:

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            Super::initialize(the_factory);
            m_remote_rank = 0;
        }

        /// Reads the decoder rank status. This function expects as
        /// input a bit vector where the index zero bit denotes the
        /// status of the symbol with index zero. If the bit is 0 it
        /// means that the symbol is still missing at the
        /// decoder. Otherwise if the bit is 1 it means that the
        /// symbol has been either seen or fully decoded by the
        /// decoder.
        /// @input buffer The input buffer containing the bit vector
        void read_pivot_status(const uint8_t *buffer)
        {
            assert(buffer);

            const uint8_t* last = buffer + Super::pivot_status_size();
            boost::from_block_range(buffer, last, m_pivot_status);
            m_remote_rank = m_pivot_status.count();
        }

        /// @copydoc layer::remote_rank() const
        rank_type remote_rank() const
        {
            return m_remote_rank;
        }

        /// @copydoc layer::remote_is_symbol_pivot(uint32_t) const
        bool remote_is_symbol_pivot(uint32_t index) const
        {
            assert(index < m_pivot_status.size());
            return m_pivot_status.test(index);
        }

    protected:

        /// Keeps track of the remote rank which has been read
        rank_type m_remote_rank;

    };

}

