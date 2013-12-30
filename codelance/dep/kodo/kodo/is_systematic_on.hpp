// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <boost/shared_ptr.hpp>

#include "systematic_encoder.hpp"
#include "has_is_systematic_on.hpp"

namespace kodo
{

    /// @ingroup generic_api
    /// @copydoc is_systematic_on(const T&)
    template<bool what, class T>
    inline bool is_systematic_on(const T& t, char (*)[what] = 0)
    {
        // Explanation for the char (*)[what] here:
        // http://stackoverflow.com/a/6917354/1717320
        return t.is_systematic_on();
    }

    /// @ingroup generic_api
    /// @copydoc is_systematic_on(const T&)
    template<bool what, class T>
    inline bool is_systematic_on(const T& t, char (*)[!what] = 0)
    {
        (void)t;

        // We do the assert here - to make sure that this call is not
        // silently ignored in cases where the stack does not have the
        // is_systematic_on() function. However, this assert can
        // be avoided by using the has_systematic_encoder
        assert(0);
        return false;
    }

    /// @ingroup generic_api
    /// Generic function overload for cases where is_systematic_on is part
    /// of a stack. @see systematic_encoder::is_systematic_on() const
    /// @param t The stack to query
    template<class T>
    inline bool is_systematic_on(const T& t)
    {
        return is_systematic_on<has_is_systematic_on<T>::value>(t);
    }

    /// @ingroup generic_api
    /// @copydoc is_systematic_on(const T&)
    template<class T>
    inline bool is_systematic_on(const boost::shared_ptr<T>& t)
    {
        return is_systematic_on(*t);
    }

}


