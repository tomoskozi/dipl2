// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file fifi_utils.hpp
/// \brief utility functions
/// 
/// Utility functions

#pragma once

#include <sak/ceil_division.hpp>

#include "field_types.hpp"

namespace fifi
{

    /// Returns the number of value_type elements needed to store a certain
    /// number of field elements
    /// @param elements the number of field elements
    /// @return the number of value_type elements needed
    template<class Field>
    inline uint32_t elements_to_length(uint32_t elements)
    {
        assert(elements > 0);

        return elements;
    }

    /// elements_to_length specilization for the binary field
    /// @copydoc elements_to_length(uint32_t)
    template<>
    inline uint32_t elements_to_length<binary>(uint32_t elements)
    {
        assert(elements > 0);

        return sak::ceil_division(elements, binary::bits_per_value);
    }

    /// Returns the minimum size in bytes required to accommodate a certain
    /// number of field elements
    /// @param elements the number of field elements 
    /// @return the size in bytes needed to store the field elements
    template<class Field>
    inline uint32_t elements_to_size(uint32_t elements)
    {
        assert(elements > 0);

        return elements_to_length<Field>(elements)*sizeof(typename Field::value_type);
    }

    /// Returns the number of value_type elements needed to store
    /// a certain number of bytes
    /// @param bytes the number of bytes to store
    /// @return the number of value_type elements that need to be stored
    template<class Field>
    inline uint32_t size_to_length(uint32_t bytes)
    {
        assert(bytes > 0);

        uint32_t bytes_per_value = sizeof(typename Field::value_type);

        // Make sure that the number of bytes is a multiple of element size
        assert( (bytes % bytes_per_value) == 0);
        return bytes / bytes_per_value;
    }

    /// Returns the size in bytes needed to store a certain
    /// number of value_type elements
    /// @param length the number of value_type elements to store
    /// @return the size in bytes needed to store the value_type elements
    template<class Field>
    inline uint32_t length_to_size(uint32_t length)
    {
        assert(length > 0);

        return length * sizeof(typename Field::value_type);
    }

    /// Returns the number of field elements needed to store a certain
    /// number of value_type elements
    /// @param length the number of value_type elements
    /// @return the number of field elements needed
    template<class Field>
    inline uint32_t length_to_elements(uint32_t length)
    {
        assert(length > 0);

        return length;
    }

    /// length_to_elements specilization for the binary field
    /// @copydoc length_to_elements(uint32_t)
    template<>
    inline uint32_t length_to_elements<binary>(uint32_t length)
    {
        assert(length > 0);

        return length*binary::bits_per_value;
    }

    /// Returns the number of field elements that can fit within a certain
    /// number of bytes
    /// @param bytes the number of bytes to store the field elements
    /// @return the number of field elements stored within the bytes 
    template<class Field>
    inline uint32_t size_to_elements(uint32_t bytes)
    {
        assert(bytes > 0);

        //return size_to_length<Field>(bytes);
        return length_to_elements<Field>((size_to_length<Field>(bytes)));
    }

    /// Usefull abstraction functions for accessing field elements if
    /// a mixed binary & other fields implementation is created.
    /// Returns the value of an element at the specific position in the
    /// symbol.
    /// @param elements elements to get value from 
    /// @param index index of element to access
    /// @return the value of the element at specified index
    template<class Field>
    inline typename Field::value_type
    get_value(const typename Field::value_type *elements, uint32_t index)
    {
        assert(elements != 0);
        return elements[index];
    }

    /// get_value specilization for the binary field
    template<>
    inline binary::value_type
    get_value<binary>(const binary::value_type *elements, uint32_t index)
    {
        assert(elements != 0);

        uint32_t array_index =
            index / binary::bits_per_value;

        uint32_t offset =
            index % binary::bits_per_value;

        return (elements[array_index] >> offset) & 0x1;
    }

    /// Usefull abstraction function for assigning field elements a specific
    /// value.
    /// @param elements elements to manipulate
    /// @param index index of element
    /// @param value value to assign element
    template<class Field>
    inline void set_value(typename Field::value_ptr elements, uint32_t index,
                          typename Field::value_type value)
    {
        assert(elements != 0);
        elements[index] = value;
    }

    /// set_value specilization for the binary field
    template<>
    inline void set_value<binary>(binary::value_ptr elements, uint32_t index,
                                  binary::value_type value)
    {
        /// @todo: find a better/faster way to do this

        assert(elements != 0);
        assert(value < 2); // only {0,1} allowed

        uint32_t array_index = index / binary::bits_per_value;
        uint32_t offset = index % binary::bits_per_value;

        binary::value_type mask = 1 << offset;

        if(value)
        {
            elements[array_index] |= mask;
        }
        else
        {
            elements[array_index] &= ~mask;
        }
    }

    /// Usefull abstraction function for swapping two field elements
    /// @param elements, elements to manipulate
    /// @param index1,index2 indexes of elements to swap
    template<class Field>
    inline void swap_values(typename Field::value_ptr elements, uint32_t index1,
                            uint32_t index2)
    {
        assert(elements != 0);

        typename Field::value_type value1 = get_value<Field>(elements, index1);
        typename Field::value_type value2 = get_value<Field>(elements, index2);

        set_value<Field>(elements, index1, value2);
        set_value<Field>(elements, index2, value1);
    }

}

