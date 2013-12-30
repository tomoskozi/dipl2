// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <boost/shared_ptr.hpp>

#include "has_read_feedback.hpp"

namespace kodo
{

    /// @ingroup generic_api
    /// @copydoc read_feedback(const T&)
    template<bool what, class T>
    inline void read_feedback(T& t, const uint8_t* buffer, char (*)[what] = 0)
    {
        // Explanation for the char (*)[what] here:
        // http://stackoverflow.com/a/6917354/1717320
        t.read_feedback(buffer);
    }

    /// @ingroup generic_api
    /// @copydoc read_feedback(const T&)
    template<bool what, class T>
    inline void read_feedback(T& t, const uint8_t* buffer, char (*)[!what] = 0)
    {
        (void) t;
        (void) buffer;

        // We do the assert here - to make sure that this call is not
        // silently ignored in cases where the stack does not have the
        // read_feedback() function. However, this assert can
        // be avoided by using the has_read_feedback.hpp functionality
        assert(0);
    }

    /// @ingroup generic_api
    /// Generic function overload for cases where read_feedback is part
    /// of a stack.
    /// @param t The stack to query
    template<class T>
    inline void read_feedback(T& t, const uint8_t* buffer)
    {
        read_feedback<has_read_feedback<T>::value, T>(t, buffer);
    }

    /// @ingroup generic_api
    /// @copydoc read_feedback(const T&, const uint8_t*)
    template<class T>
    inline void read_feedback(boost::shared_ptr<T>& t, const uint8_t* buffer)
    {
        read_feedback(*t, buffer);
    }

}


