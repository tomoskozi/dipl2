// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <boost/dynamic_bitset.hpp>

#include <sak/ceil_division.hpp>

namespace kodo
{

    /// @ingroup state_layers
    ///
    /// @brief The pivot status bitset provides a building block for
    ///        layers that wish to use a bitset to keep track of
    ///        (partially) decoded symbols.
    template<class SuperCoder>
    class pivot_status_bitset : public SuperCoder
    {
    public:

        /// The data used for the status vector
        typedef uint8_t status_block_type;

    public:

        /// The factory layer
        class factory : public SuperCoder::factory
        {
        public:

            /// @copydoc layer::factory::factory(uint32_t,uint32_t)
            factory(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory(max_symbols, max_symbol_size)
            { }

            /// @copydoc layer::factory::max_pivot_status_size() const
            uint32_t max_pivot_status_size() const
            {
                // Assuming we will use one bit per pivot element here and that
                // a byte is 8 bit :)
                return sak::ceil_division(
                    SuperCoder::factory::max_symbols(), 8);
            }
        };

    public:

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory& the_factory)
        {
            SuperCoder::construct(the_factory);

            m_pivot_status.resize(the_factory.max_symbols(), false);

            assert(pivot_status_size() <= the_factory.max_pivot_status_size());
        }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_pivot_status.resize(the_factory.symbols());
            m_pivot_status.reset();

            assert(pivot_status_size() <= the_factory.max_pivot_status_size());
        }

        /// @return The size in bytes of decoder status vector
        uint32_t pivot_status_size() const
        {
            static_assert(sizeof(status_block_type) == 1,
                          "We assume that the block type is 1 byte in the "
                          "calculation here");

            assert(std::numeric_limits<uint32_t>::max() >=
                   m_pivot_status.num_blocks());

            return (uint32_t) m_pivot_status.num_blocks();
        }

    protected:

        /// Tracks the symbols which have been marked as pivot
        boost::dynamic_bitset<status_block_type> m_pivot_status;

    };

}

