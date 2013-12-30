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
    /// @brief The symbol decoding status tracker stores information about
    ///        the status of the different symbols contained within a decoder.
    template<class SuperCoder>
    class symbol_decoding_status_tracker : public SuperCoder
    {
    public:

        /// Defines the status that a symbol in a decoder can have.
        enum class symbol_decoding_status
        {
            /// missing means that the symbol has not yet been
            /// received by the decoder.
            missing,

            /// seen means that the symbol has been received, but has
            /// not yet been fully decoded. The use of the term "seen"
            /// is common in network coding literature when referring
            /// to partially decoded packets.
            seen,

            /// decoded means that the symbol has been received and
            /// fully decoded.
            decoded
        };

    public:

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory &the_factory)
        {
            SuperCoder::construct(the_factory);

            m_status.resize(the_factory.max_symbols(),
                            symbol_decoding_status::missing);
        }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            std::fill_n(m_status.begin(), the_factory.symbols(),
                        symbol_decoding_status::missing);
        }

        /// @copydoc layer::set_symbol_missing(uint32_t)
        void set_symbol_missing(uint32_t index)
        {
            assert(index < m_status.size());
            m_status[index] = symbol_decoding_status::missing;
        }

        /// @copydoc layer::set_symbol_seen(uint32_t)
        void set_symbol_seen(uint32_t index)
        {
            assert(index < m_status.size());
            m_status[index] = symbol_decoding_status::seen;
        }

        /// @copydoc layer::set_symbol_decoded(uint32_t)
        void set_symbol_decoded(uint32_t index)
        {
            assert(index < m_status.size());
            m_status[index] = symbol_decoding_status::decoded;
        }

        /// @copydoc layer::is_symbol_missing(uint32_t) const
        bool is_symbol_missing(uint32_t index) const
        {
            assert(index < m_status.size());
            return m_status[index] == symbol_decoding_status::missing;
        }

        /// @copydoc layer::is_symbol_seen(uint32_t) const
        bool is_symbol_seen(uint32_t index) const
        {
            assert(index < m_status.size());
            return m_status[index] == symbol_decoding_status::seen;
        }

        /// @copydoc layer::is_symbol_decoded(uint32_t) const
        bool is_symbol_decoded(uint32_t index) const
        {
            assert(index < m_status.size());
            return m_status[index] == symbol_decoding_status::decoded;
        }

    public:

        /// Tracks whether a symbol is still partially decoded
        std::vector<symbol_decoding_status> m_status;

    };

}


