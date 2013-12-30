// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#ifndef FIFI_IS_PRIME2325_HPP
#define FIFI_IS_PRIME2325_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#include "field_types.hpp"

namespace fifi
{
    /// Will inherit from true-type if Field equal fifi::prime2325
    /// Can be used e.g.:
    ///
    ///    if( is_prime2325<field_type>::value )
    ///    {
    ///        /// do something clever
    ///    }
    ///
    template<class Field>
    struct is_prime2325 : public boost::is_same<Field, fifi::prime2325>
    { };
}

#endif

