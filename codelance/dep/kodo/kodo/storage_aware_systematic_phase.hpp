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
    /// @brief This class is used to detect whether the encoder is in
    /// the systematic phase i.e. whether the next symbol to encode
    /// should be a uncoded systematic symbol. This is done by
    /// tracking which symbols has already been sent systematically
    /// and which symbols are currently available in the storage
    /// layers
    template<class SuperCoder>
    class storage_aware_systematic_phase : public SuperCoder
    {
    public:

        /// Make the encode_symbol overloads available
        using SuperCoder::encode_symbol;

    public:

        /// @copdydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory& the_factory)
        {
            SuperCoder::construct(the_factory);

            m_systematic_symbols_sent.resize(the_factory.max_symbols(), false);
        }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_offset = 0;
            m_systematic_count = 0;
            m_systematic_symbols_sent.reset();
        }

        /// @copydoc layer::encode_symbol(uint8_t*,uint32_t)
        void encode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
        {
            assert(symbol_index < m_systematic_symbols_sent.size());

            SuperCoder::encode_symbol(symbol_data, symbol_index);

            update_systematic_state(symbol_index);
        }

        /// @return true if we are in the systematic phase (i.e. there
        /// are systematic packet to send) otherwise false
        bool in_systematic_phase() const
        {
            // We know that there must be some systematic symbols to
            // send if the rank (denotes the number of symbols
            // available in the encoder matrix is larger than the
            // number of systematic symbols we have sent
            return m_systematic_count < SuperCoder::rank();
        }

        /// @return The index of the next symbol to be sent in a
        uint32_t next_systematic_symbol() const
        {
            assert(in_systematic_phase());

            bool next_symbol_found = false;
            uint32_t next_symbol = 0;

            // Find which symbol should be the next to send systematically
            for(uint32_t i = m_offset; i < SuperCoder::symbols(); ++i)
            {
                bool is_not_sent = !m_systematic_symbols_sent.test(i);
                bool is_pivot = SuperCoder::is_symbol_pivot(i);

                if(is_not_sent && is_pivot)
                {
                    next_symbol_found = true;
                    next_symbol = i;
                    break;
                }
            }

            assert(next_symbol_found);
            return next_symbol;
        }

        /// @return The number of systematically encoded packets produced
        ///         by this encoder
        uint32_t systematic_count() const
        {
            return m_systematic_count;
        }

    protected:

        /// This function updates the state of the layer to take into
        /// account that one more symbol has been sent systematically.
        /// @param symbol_index The symbol which has just been sent
        /// systematically
        void update_systematic_state(uint32_t symbol_index)
        {
            assert(symbol_index < SuperCoder::symbols());
            assert(symbol_index < m_systematic_symbols_sent.size());

            // If we have an one bit the symbol has been sent as systematic
            // before so:
            // 1) If the symbol has not been sent before is_not_sent will be 1
            // 2) If the symbol has been sent before is_not_sent will be 0
            bool is_not_sent = !m_systematic_symbols_sent.test(symbol_index);

            // Increment the count if the symbol was not previously sent
            m_systematic_count += is_not_sent;

            m_systematic_symbols_sent.set(symbol_index);

            // Update the offset when we can confirm that we have
            // systematically sent all symbols with a lower or equal
            // index than the next_symbol variable
            for(uint32_t i = m_offset; i <= symbol_index; ++i)
            {

                bool is_sent = m_systematic_symbols_sent.test(i);
                bool is_pivot = SuperCoder::is_symbol_pivot(i);

                if(is_sent && is_pivot)
                {
                    ++m_offset;
                }
                else
                {
                    break;
                }
            }

        }

    protected:

        /// When searching for the next symbol to send systematically
        /// we do not have to start from the beginning every time
        /// i.e. if we know that we have already sent the symbols with
        /// index 0 and 2 the we can start the search for the next
        /// symbol from index 1. The offset variable keeps track of
        /// the progress.
        uint32_t m_offset;

        /// Counts the number of systematic packets produced
        uint32_t m_systematic_count;

        /// Bitset which keeps track of the symbols that have been sent
        /// systematic
        boost::dynamic_bitset<uint8_t> m_systematic_symbols_sent;

    };

}

