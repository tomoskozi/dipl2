// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <algorithm>
#include <sak/convert_endian.hpp>

namespace kodo
{

    /// @ingroup state_layers
    ///
    /// @brief Read the rank information from the provided buffer and
    ///        makes it available in the stack.
    template<class SuperCoder>
    class rank_reader : public SuperCoder
    {
    public:

        /// @copydoc layer::rank_type
        typedef typename SuperCoder::rank_type rank_type;

    public:

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            // Reset the state
            m_remote_rank = 0;
        }

        /// Reads rank information from the provided buffer
        ///
        /// @note The buffer used must have at least the size reported
        ///       by the rank_info::rank_size() function.
        ///
        /// @param buffer The buffer containing the rank
        void read_rank(const uint8_t* buffer)
        {
            assert(buffer != 0);

            // Read the rank from the payload
            rank_type remote_rank =
                sak::big_endian::get<rank_type>(buffer);

            // We set the maximum rank of the previously seen and the
            // newly received. Re-ordering of packets might cause a
            // the remote rank to be lower than what has previously
            // been seen
            m_remote_rank = std::max(remote_rank, m_remote_rank);
        }

        /// @copydoc layer::remote_rank() const
        rank_type remote_rank() const
        {
            return m_remote_rank;
        }

    private:

        /// Stores the read remote rank
        rank_type m_remote_rank;

    };

}


