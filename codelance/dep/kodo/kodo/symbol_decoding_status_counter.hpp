// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <vector>

namespace kodo
{

    /// @ingroup decoder_api
    /// @brief The symbol decoding status counter maintains
    /// information about how many symbols are in which state.
    ///
    /// On the decoder side there are three states a symbol can be in
    /// information about these states can be found in the
    /// symbol_decoding_status_tracker layer. This layer maintains
    /// counters showing how many symbols are in each state.
    template<class SuperCoder>
    class symbol_decoding_status_counter : public SuperCoder
    {
    public:

        /// Constructor
        symbol_decoding_status_counter()
            : m_symbols_missing(0),
              m_symbols_seen(0),
              m_symbols_decoded(0)
        { }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_symbols_missing = the_factory.symbols();
            m_symbols_seen = 0;
            m_symbols_decoded = 0;
        }

        /// @copydoc layer::set_symbol_missing(uint32_t)
        void set_symbol_missing(uint32_t index)
        {
            assert(index < SuperCoder::symbols());

            if(SuperCoder::is_symbol_seen(index))
            {
                ++m_symbols_missing;

                assert(m_symbols_missing <= SuperCoder::symbols());
                assert(m_symbols_seen > 0);

                --m_symbols_seen;
            }
            else if(SuperCoder::is_symbol_decoded(index))
            {
                ++m_symbols_missing;

                assert(m_symbols_missing <= SuperCoder::symbols());
                assert(m_symbols_decoded > 0);

                --m_symbols_decoded;
            }

            SuperCoder::set_symbol_missing(index);
        }

        /// @copydoc layer::set_symbol_seen(uint32_t)
        void set_symbol_seen(uint32_t index)
        {
            assert(index < SuperCoder::symbols());

            if(SuperCoder::is_symbol_missing(index))
            {
                ++m_symbols_seen;

                assert(m_symbols_seen <= SuperCoder::symbols());
                assert(m_symbols_missing > 0);

                --m_symbols_missing;
            }
            else if(SuperCoder::is_symbol_decoded(index))
            {
                ++m_symbols_seen;

                assert(m_symbols_seen <= SuperCoder::symbols());
                assert(m_symbols_decoded > 0);

                --m_symbols_decoded;
            }

            SuperCoder::set_symbol_seen(index);
        }

        /// @copydoc layer::set_symbol_decoded(uint32_t)
        void set_symbol_decoded(uint32_t index)
        {
            assert(index < SuperCoder::symbols());

            if(SuperCoder::is_symbol_missing(index))
            {
                ++m_symbols_decoded;

                assert(m_symbols_decoded <= SuperCoder::symbols());
                assert(m_symbols_missing > 0);

                --m_symbols_missing;
            }
            else if(SuperCoder::is_symbol_seen(index))
            {
                ++m_symbols_decoded;

                assert(m_symbols_decoded <= SuperCoder::symbols());
                assert(m_symbols_seen > 0);

                --m_symbols_seen;
            }

            SuperCoder::set_symbol_decoded(index);
        }

        /// @copydoc layer::symbols_missing() const
        uint32_t symbols_missing() const
        {
            return m_symbols_missing;
        }

        /// @copydoc layer::symbols_seen() const
        uint32_t symbols_seen() const
        {
            return m_symbols_seen;
        }

        /// @copydoc layer::symbols_decoded() const
        uint32_t symbols_decoded() const
        {
            return m_symbols_decoded;
        }

    public:

        /// Counter for keeping track of the currently missing symbols.
        uint32_t m_symbols_missing;

        /// Counter for the symbols which have been "seen", see the
        /// symbol_decoding_status_tracker layer for a description of
        /// what seen means.
        uint32_t m_symbols_seen;

        /// Counter for symbols which have been fully decoded.
        uint32_t m_symbols_decoded;

    };

}

