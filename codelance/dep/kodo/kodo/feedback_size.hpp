// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <boost/shared_ptr.hpp>

#include "has_feedback_size.hpp"

namespace kodo
{

    /// @ingroup generic_api
    /// @copydoc feedback_size(const T&)
    template<bool what, class T>
    inline uint32_t feedback_size(const T& t, char (*)[what] = 0)
    {
        // Explanation for the char (*)[what] here:
        // http://stackoverflow.com/a/6917354/1717320
        return t.feedback_size();
    }

    /// @ingroup generic_api
    /// @copydoc feedback_size(const T&)
    template<bool what, class T>
    inline uint32_t feedback_size(const T& t, char (*)[!what] = 0)
    {
        (void) t;

        // We do the assert here - to make sure that this call is not
        // silently ignored in cases where the stack does not have the
        // feedback_size() function. However, this assert can
        // be avoided by using the has_feedback_size.hpp functionality
        assert(0);

        return 0;
    }

    /// @ingroup generic_api
    /// Generic function overload for cases where feedback_size is part
    /// of a stack.
    /// @param t The stack to query
    template<class T>
    inline uint32_t feedback_size(const T& t)
    {
        return feedback_size<has_feedback_size<T>::value, T>(t);
    }

    /// @ingroup generic_api
    /// @copydoc feedback_size(const T&, uint8_t*)
    template<class T>
    inline uint32_t feedback_size(const boost::shared_ptr<T>& t)
    {
        return feedback_size(*t);
    }

}


