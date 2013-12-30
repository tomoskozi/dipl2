// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#ifndef FIFI_BINARY_EXTENSION_FIELD_HPP
#define FIFI_BINARY_EXTENSION_FIELD_HPP

#include <algorithm>
#include <cmath>
#include <cassert>
#include <stdint.h>

#include <boost/noncopyable.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include "simple_online.hpp"

namespace fifi
{

    /// Produces an extended log table for multiplication
    /// and division.
    template<class Field>
    class binary_extension_field
    {
    public:

        /// Typedef of the data type used for each field element
        typedef typename Field::value_type value_type;

        /// Typedef of the field type used
        typedef Field field_type;

        /// Disable this for the binary field. The binary field is not an
        /// extension field.
        BOOST_STATIC_ASSERT((boost::is_same<field_type, binary>::value == false));

        /// Disable the prime fields
        BOOST_STATIC_ASSERT((boost::is_same<field_type, prime2325>::value == false));

    public:

        /// @copydoc finite_field::add()
        value_type add(value_type element_one, value_type element_two) const;

        /// @copydoc finite_field::subtract()
        value_type subtract(value_type element_one, value_type element_two) const;

    public:

        /// Access to the simple online algorithms. These are use for the
        /// initialization of the lookup table and for the addition and
        /// subtraction operations since those are the same in both the binary
        /// and binary extension fields
        simple_online<field_type> m_simple_online;

    };

    template<class Field>
    inline typename binary_extension_field<Field>::value_type
    binary_extension_field<Field>::add(value_type element_one, value_type element_two) const
    {
        return m_simple_online.add(element_one, element_two);
    }

    template<class Field>
    inline typename binary_extension_field<Field>::value_type
    binary_extension_field<Field>::subtract(value_type element_one, value_type element_two) const
    {
        return m_simple_online.subtract(element_one, element_two);
    }
}

#endif

