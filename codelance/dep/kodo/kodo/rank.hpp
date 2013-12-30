// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <boost/shared_ptr.hpp>

#include "has_rank.hpp"

namespace kodo
{

    /// @ingroup generic_api
    /// @copydoc rank(const T&)
    template<bool what, class T>
    inline auto rank(T& t, char (*)[what] = 0) -> decltype(t.rank())
    {
        // Explanation for the char (*)[what] here:
        // http://stackoverflow.com/a/6917354/1717320
        return t.rank();
    }

    /// @ingroup generic_api
    /// @copydoc rank(const T&)
    template<bool what, class T>
    inline uint32_t rank(T& t, char (*)[!what] = 0)
    {
        (void)t;

        // We do the assert here - to make sure that this call is not
        // silently ignored in cases where the stack does not have the
        // rank() function. However, this assert can
        // be avoided by using the has_systematic_encoder
        assert(0);

        return 0;
    }

    /// @ingroup generic_api
    /// Generic function overload for cases where rank is part
    /// of a stack. @see systematic_encoder::rank() const
    /// @param t The stack to query
    template<class T>
    inline auto rank(T& t) -> decltype(rank<has_rank<T>::value, T>(t))
    {
        return rank<has_rank<T>::value, T>(t);
    }

    /// @ingroup generic_api
    /// @copydoc rank(const T&)
    template<class T>
    inline auto rank(boost::shared_ptr<T>& t) -> decltype(rank(*t))
    {
        return rank(*t);
    }

}


