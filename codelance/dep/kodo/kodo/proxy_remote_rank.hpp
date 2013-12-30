// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{

    /// @ingroup proxy_layers status_api
    /// @brief The proxy_remote_rank will forward calls to the
    ///        remote_rank() function to a main stack
    template<class SuperCoder>
    class proxy_remote_rank : public SuperCoder
    {
    public:

        /// @copydoc proxy_layer::main_stack
        typedef typename SuperCoder::main_stack main_stack;


        /// @copydoc layer::rank_type
        typedef typename main_stack::rank_type rank_type;

    public:

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_proxy = the_factory.proxy_stack();
            assert(m_proxy);
        }

        /// @copydoc layer::seen_encoder_rank() const
        rank_type remote_rank() const
        {
            assert(m_proxy);
            return m_proxy->remote_rank();
        }

    private:

        /// Store a pointer to the main stack
        const main_stack* m_proxy;

    };

}


