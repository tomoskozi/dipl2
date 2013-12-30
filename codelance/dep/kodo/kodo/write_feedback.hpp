// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <boost/shared_ptr.hpp>

#include "has_write_feedback.hpp"

namespace kodo
{

    /// @ingroup generic_api
    /// @copydoc write_feedback(const T&)
    template<bool what, class T>
    inline uint32_t
    write_feedback(const T& t, uint8_t* buffer, char (*)[what] = 0)
    {
        // Explanation for the char (*)[what] here:
        // http://stackoverflow.com/a/6917354/1717320
        return t.write_feedback(buffer);
    }

    /// @ingroup generic_api
    /// @copydoc write_feedback(const T&)
    template<bool what, class T>
    inline uint32_t
    write_feedback(const T& t, uint8_t* buffer, char (*)[!what] = 0)
    {
        (void) t;
        (void) buffer;

        // We do the assert here - to make sure that this call is not
        // silently ignored in cases where the stack does not have the
        // write_feedback() function. However, this assert can
        // be avoided by using the has_write_feedback.hpp functionality
        assert(0);

        return 0;
    }

    /// @ingroup generic_api
    /// Generic function overload for cases where write_feedback is part
    /// of a stack.
    /// @param t The stack to query
    template<class T>
    inline uint32_t
    write_feedback(const T& t, uint8_t* buffer)
    {
        return write_feedback<has_write_feedback<T>::value, T>(t, buffer);
    }

    /// @ingroup generic_api
    /// @copydoc write_feedback(const T&, uint8_t*)
    template<class T>
    inline uint32_t
    write_feedback(const boost::shared_ptr<T>& t, uint8_t* buffer)
    {
        return write_feedback(*t, buffer);
    }

}


