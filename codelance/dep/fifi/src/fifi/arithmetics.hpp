// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#ifndef FIFI_ARITHMETICS_HPP
#define FIFI_ARITHMETICS_HPP

#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>

#include "field_types.hpp"
#include "log_table.hpp"
#include "extended_log_table.hpp"
#include "full_table.hpp"
#include "is_binary.hpp"
#include "optimal_prime.hpp"
#include "disable_if_binary.hpp"

// Certain applications e.g. Error Correcting Codes (ECC) perform specific
// compound operations. These operations can for certain field implementations
// be implemented faster or smarter. This file provides commonly used operations.

namespace fifi
{

    /// Generic version of adding two buffers
    ///
    /// Provides: dest[i] = dest[i] + src[i]
    /// @param field the field implementation
    /// @param dest the destination element buffer
    /// @param src the source element buffer
    /// @param length the size of destination and source buffers
    ///        in value_type elements
    template<class FieldImpl>
    inline void
    add(const FieldImpl &field,
        typename FieldImpl::value_type * __restrict dest,
        const typename FieldImpl::value_type * __restrict src,
        uint32_t length)
    {
        assert(dest != 0);
        assert(src != 0);
        // assert((((uintptr_t)dest) & 15) == 0); // Check alignment
        // assert((((uintptr_t)src) & 15) == 0); // Check alignment
        assert(length > 0);

        for(uint32_t i = 0; i < length; ++i)
        {
            typename FieldImpl::value_type a = dest[i];
            typename FieldImpl::value_type b = src[i];
            dest[i] = field.add(a, b);
        }
    }

    template<>
    inline void
    add(const optimal_prime<prime2325> &/*field*/,
        optimal_prime<prime2325>::value_type * __restrict dest,
        const optimal_prime<prime2325>::value_type * __restrict src,
        uint32_t length)
    {
        assert(dest != 0);
        assert(src != 0);
        // assert((((uintptr_t)dest) & 15) == 0); // Check alignment
        // assert((((uintptr_t)src) & 15) == 0); // Check alignment
        assert(length > 0);

        for(uint32_t i = 0; i < length; ++i)
        {
            dest[i] = dest[i] + src[i];

            // If dest[i] >= src[i] the we did not have a 32 bit overflow
            //
            //     1 if no overflow
            //     0 if overflow
            //
            // This means that (dest[i] >= src[i]) - 1 becomes:
            //
            //     0 or 0x00000000 if no overflow
            //     -1 or 0xffffffff if overflow
            //
            // An overflow is equivalent to a mod 2^32 since
            // we are working 2^32 - 5 we simply add 5 to
            // compensate
            dest[i] = dest[i] + (5 & ((dest[i] >= src[i]) - 1));

            // Conditional move version
            //dest[i] = dest[i] >= prime2325::prime ?
            //    dest[i] - prime2325::prime : dest[i];

            // The line below does the following:
            // prime2325::prime > dest[i] becomes:
            //
            //     1 if prime is larger
            //     0 if prime is smaller or equal
            //
            // If that means the (prime2325::prime > dest[i]) - 1 becomes:
            //
            //     0 or 0x00000000 if prime is larger
            //     -1 or 0xffffffff if the prime is smaller or equal
            //
            // This mask is then used to determine whether the subtraction
            // with the prime statement is in effect of not
            //
            // Our measurements show that this branch-less version
            // yields a significant performance gain over both the
            // branched and conditional move versions
            dest[i] = dest[i] -
                (prime2325::prime & ((prime2325::prime > dest[i]) - 1));

        }
    }


    /// Generic version of subtract two buffers
    ///
    /// Provides: dest[i] = dest[i] - src[i]
    /// @param field the field implementation
    /// @param dest the destination element buffer
    /// @param src the source element buffer
    /// @param length the size of destination and source buffers
    ///        in value_type elements
    template<class FieldImpl>
    inline void
    subtract(const FieldImpl &field,
             typename FieldImpl::value_type * __restrict dest,
             const typename FieldImpl::value_type * __restrict src,
             uint32_t length)
    {
        assert(dest != 0);
        assert(src != 0);
        // assert((((uintptr_t)dest) & 15) == 0); // Check alignment
        // assert((((uintptr_t)src) & 15) == 0); // Check alignment
        assert(length > 0);

        for(uint32_t i = 0; i < length; ++i)
        {
            dest[i] = field.subtract(dest[i], src[i]);
        }
    }

    template<>
    inline void
    subtract(const optimal_prime<prime2325> &/*field*/,
             optimal_prime<prime2325>::value_type * __restrict dest,
             const optimal_prime<prime2325>::value_type * __restrict src,
             uint32_t length)
    {
        assert(dest != 0);
        assert(src != 0);
        // assert((((uintptr_t)dest) & 15) == 0); // Check alignment
        // assert((((uintptr_t)src) & 15) == 0); // Check alignment
        assert(length > 0);

        for(uint32_t i = 0; i < length; ++i)
        {
            // If dest[i] >= src[i] then we will not have an underflow
            //
            //     1 if no underflow
            //     0 if underflow
            //
            // This means that (dest[i] >= src[i]) - 1 becomes:
            //
            //     0 or 0x00000000 if no underflow
            //     -1 or 0xffffffff if underflow
            //
            // An underflow is equivalent to a mod 2^32 since
            // we are working 2^32 - 5 we simply subtract 5 to
            // compensate
            dest[i] = (dest[i] - src[i]) - (5 & ((dest[i] >= src[i]) - 1));

            // Different from addition there is no way we can end up
            // above the prime so we don't have to do anything else
        }
    }

    /// Generic version of multiplying two buffers
    ///
    /// Provides: dest[i] = dest[i] * src[i]
    /// @param field the field implementation
    /// @param dest the destination element buffer
    /// @param src the source element buffer
    /// @param length the size of destination and source buffers
    ///        in value_type elements
    template<class FieldImpl>
    inline void
    multiply(const FieldImpl &field,
             typename FieldImpl::value_type * __restrict dest,
             const typename FieldImpl::value_type * __restrict src,
             uint32_t length)
    {
        assert(dest != 0);
        assert(src != 0);
        // assert((((uintptr_t)dest) & 15) == 0); // Check alignment
        // assert((((uintptr_t)src) & 15) == 0); // Check alignment
        assert(length > 0);

        for(uint32_t i = 0; i < length; ++i)
        {
            dest[i] = field.multiply(dest[i], src[i]);
        }
    }

    /// Generic version of multiplying a buffer with a constant
    ///
    /// Provides: dest[i] = dest[i] * constant
    /// @param field the field implementation
    /// @param dest the destination element buffer
    /// @param constant the constant multiplier
    /// @param length the size of destination and source buffers
    ///        in value_type elements
    template<class FieldImpl>
    inline void
    multiply_constant(const FieldImpl &field,
                      typename FieldImpl::value_type constant,
                      typename FieldImpl::value_type *dest,
                      uint32_t length)
    {
        assert(dest != 0);
        // assert((((uintptr_t)dest) & 15) == 0); // Check alignment
        assert(length > 0);

        typedef typename FieldImpl::field_type field_type;

        // This function does not work for the binary field
        // since multiplying with a constant does not behave as
        // expected (the binary field does a bit-wise AND operation).
        BOOST_STATIC_ASSERT((is_binary<field_type>::value == false));

        for(uint32_t i = 0; i < length; ++i)
        {
            dest[i] = field.multiply(dest[i], constant);
        }
    }


    /// The overload is taken for binary fields for the multiply_constant(...)
    /// functions. Since the binary field only consists of elements {0,1}
    /// therefore we either zero the buffer or leave it untouched.
    ///
    /// @see generic version of multiply_constant(...) for parameters
    template<template <class> class FieldImpl>
    inline void
    multiply_constant(const FieldImpl<binary> &/*field*/,
                      typename binary::value_type constant,
                      typename binary::value_type *dest,
                      uint32_t length)
    {
        assert(dest != 0);
        // assert((((uintptr_t)dest) & 15) == 0); // Check alignment
        assert(length > 0);

        // Only {0,1} allowed
        assert(constant == 0 || constant == 1);

        if(constant == 0)
        {
            std::fill_n(dest, length, 0);
        }
    }

    /// Generic version of adding two buffers after multiplying
    /// with a constant.
    ///
    /// Provides: dest[i] = dest[i] + (src[i] * constant)
    /// @param field the field implementation
    /// @param constant the constant field value used for
    ///        the multiplication
    /// @param dest the destination element buffer
    /// @param src the source element buffer
    /// @param length the size of destination and source buffers
    ///        in value_type elements
    template<class FieldImpl>
    inline void
    multiply_add(const FieldImpl &field,
                 typename FieldImpl::value_type constant,
                 typename FieldImpl::value_type * __restrict dest,
                 const typename FieldImpl::value_type * __restrict src,
                 typename FieldImpl::value_type * __restrict temp,
                 uint32_t length)
    {
        assert(dest != 0);
        assert(src != 0);
        assert(temp != 0);
        // assert((((uintptr_t)dest) & 15) == 0); // Check alignment
        // assert((((uintptr_t)src) & 15) == 0); // Check alignment
        // assert((((uintptr_t)temp) & 15) == 0); // Check alignment
        assert(length > 0);

        typedef typename FieldImpl::field_type field_type;

        // This function does not work for the binary field
        // since the multiply function does not behave correctly
        // so make sure that we do not use it.
        BOOST_STATIC_ASSERT((is_binary<field_type>::value == false));

        if(constant == 0)
            return;

        typedef typename FieldImpl::value_type value_type;


        for(uint32_t i = 0; i < length; ++i)
        {
            value_type d = field.multiply(constant, src[i]);
            dest[i] = field.add(dest[i], d);
        }

    }

    template<>
    inline void
    multiply_add(const optimal_prime<prime2325> &field,
                 optimal_prime<prime2325>::value_type constant,
                 optimal_prime<prime2325>::value_type * __restrict dest,
                 const optimal_prime<prime2325>::value_type * __restrict src,
                 optimal_prime<prime2325>::value_type * __restrict temp,
                 uint32_t length)
    {
        assert(dest != 0);
        assert(src != 0);
        assert(temp != 0);
        // assert((((uintptr_t)dest) & 15) == 0); // Check alignment
        // assert((((uintptr_t)src) & 15) == 0); // Check alignment
        // assert((((uintptr_t)temp) & 15) == 0); // Check alignment
        assert(length > 0);

        if(constant == 0)
            return;

        for(uint32_t i = 0; i < length; ++i)
        {
            temp[i] = field.multiply(constant, src[i]);
        }

        add(field, dest, temp, length);

    }

    // This overload is for the full table implementation where
    // a small optimization is possible when adding two buffers
    // after multiplying with a constant.
    //
    // @see generic version of multiply_add(...) for parameters
    template<>
    inline void
    multiply_add(const full_table<binary8> &field,
                 full_table<binary8>::value_type constant,
                 full_table<binary8>::value_type * __restrict dest,
                 const full_table<binary8>::value_type * __restrict src,
                 full_table<binary8>::value_type * /*__restrict temp*/,
                 uint32_t length)
    {
        assert(dest != 0);
        assert(src  != 0);
        // assert((((uintptr_t)dest) & 15) == 0); // Check alignment
        // assert((((uintptr_t)src) & 15) == 0); // Check alignment
        assert(length > 0);

        if(constant == 0)
            return;

        typedef full_table<binary8>::value_type value_type;

        // In the multiplication table the constant is used to indentify
        // the row number. Therefore the constant is used as an offset,
        // and all possible results can then be accessed on the following
        // number of indices indices.
        const value_type *offset =
            &field.m_multtable[(constant << binary8::degree)];

        for(uint32_t i = 0; i < length; ++i)
        {
            value_type v = offset[src[i]];
            dest[i] = field.add(v, dest[i]);
        }
    }

    /// The overload is taken for binary fields for the multiply_add(..)
    /// functions. Since the binary field only consists of elements {0,1}
    /// therefore it only makes sense to do the addition.
    ///
    /// @see generic version of multiply_add(...) for parameters
    template<template<class> class FieldImpl>
    inline void multiply_add(const FieldImpl<binary> &field,
                             typename binary::value_type constant,
                             typename binary::value_type * __restrict dest,
                             const typename binary::value_type * __restrict src,
                             typename binary::value_type * /*__restrict temp*/,
                             uint32_t length)
    {
        assert(dest != 0);
        assert(src  != 0);
        // assert((((uintptr_t)dest) & 15) == 0); // Check alignment
        // assert((((uintptr_t)src) & 15) == 0); // Check alignment
        assert(length > 0);

        // Only {0,1} allowed
        assert(constant == 0 || constant == 1);

        if(constant == 0) // Do nothing
        {
            return;
        }
        else // Add the two buffers
        {
            add(field, dest, src, length);
        }
    }

    /// Generic version of subtracting two buffers after multiplying
    /// with a constant.
    ///
    /// Provides: dest[i] = dest[i] - (src[i] * constant)
    /// @param field the field implementation
    /// @param constant the constant field value used for
    ///        the multiplication
    /// @param dest the destination element buffer
    /// @param src the source element buffer
    /// @param length the size of destination and source buffers
    ///        in value_type elements
    template<class FieldImpl>
    inline void
    multiply_subtract(const FieldImpl &field,
                      typename FieldImpl::value_type constant,
                      typename FieldImpl::value_type * __restrict dest,
                      const typename FieldImpl::value_type * __restrict src,
                      typename FieldImpl::value_type * __restrict temp,
                      uint32_t length)
    {
        assert(dest != 0);
        assert(src != 0);
        assert(temp != 0);
        // assert((((uintptr_t)dest) & 15) == 0); // Check alignment
        // assert((((uintptr_t)src) & 15) == 0); // Check alignment
        // assert((((uintptr_t)temp) & 15) == 0); // Check alignment
        assert(length > 0);

        typedef typename FieldImpl::field_type field_type;

        // This function does not work for the binary field
        // since the multiply function does not behave correctly
        // so make sure that we do not use it.
        BOOST_STATIC_ASSERT((is_binary<field_type>::value == false));

        if(constant == 0)
            return;

        typedef typename FieldImpl::value_type value_type;

        for(uint32_t i = 0; i < length; ++i)
        {
            value_type m = field.multiply(constant, src[i]);
            dest[i] = field.subtract(dest[i], m);
        }
    }

    template<>
    inline void
    multiply_subtract(const optimal_prime<prime2325> &field,
                      optimal_prime<prime2325>::value_type constant,
                      optimal_prime<prime2325>::value_type * __restrict dest,
                      const optimal_prime<prime2325>::value_type * __restrict src,
                      optimal_prime<prime2325>::value_type * __restrict temp,
                      uint32_t length)
    {
        assert(dest != 0);
        assert(src != 0);
        assert(temp != 0);
        // assert((((uintptr_t)dest) & 15) == 0); // Check alignment
        // assert((((uintptr_t)src) & 15) == 0); // Check alignment
        // assert((((uintptr_t)temp) & 15) == 0); // Check alignment
        assert(length > 0);

        if(constant == 0)
            return;

        for(uint32_t i = 0; i < length; ++i)
        {
            temp[i] = field.multiply(constant, src[i]);
        }

        subtract(field, dest, temp, length);

    }

    /// This overload is for the full table implementation where
    /// a small optimization is possible when subtracting two buffers
    /// after multiplying with a constant.
    ///
    /// @see generic version of multiply_subtract(...) for parameters
    template<>
    inline void
    multiply_subtract(const full_table<binary8> &field,
                      full_table<binary8>::value_type constant,
                      full_table<binary8>::value_type * __restrict dest,
                      const full_table<binary8>::value_type * __restrict src,
                      full_table<binary8>::value_type * /*__restrict temp*/,
                      uint32_t length)
    {
        assert(dest != 0);
        assert(src != 0);
        // assert((((uintptr_t)dest) & 15) == 0); // Check alignment
        // assert((((uintptr_t)src) & 15) == 0); // Check alignment
        assert(length > 0);

        if(constant == 0)
            return;

        typedef full_table<binary8>::value_type value_type;

        // In the multiplication table the constant is used to indentify
        // the row number. Therefore the constant is used as an offset,
        // and all possible results can then be accessed on the following
        // number of indices indices.
        const value_type *offset =
            &field.m_multtable[(constant << binary8::degree)];

        for(uint32_t i = 0; i < length; ++i)
        {
            value_type v = offset[src[i]];
            dest[i] = field.subtract(v, dest[i]);
        }
    }

    /// The overload is taken for binary fields for the multiply_subtract(..)
    /// functions. Since the binary field only consists of elements {0,1}
    /// therefore it only makes sense to do the subtraction.
    ///
    /// @see generic version of multiply_subtract(...) for parameters
    template<template<class> class FieldImpl>
    inline void
    multiply_subtract(const FieldImpl<binary> &field,
                      typename binary::value_type constant,
                      typename binary::value_type * __restrict dest,
                      const typename binary::value_type * __restrict src,
                      typename binary::value_type * /*__restrict temp*/,
                      uint32_t length)
    {
        assert(dest != 0);
        assert(src != 0);
        // assert((((uintptr_t)dest) & 15) == 0); // Check alignment
        // assert((((uintptr_t)src) & 15) == 0); // Check alignment
        assert(length > 0);

        // Only {0,1} allowed
        assert(constant == 0 || constant == 1);

        if(constant == 0) // Do nothing
        {
            return;
        }
        else // Subtract the two buffers
        {
            subtract(field, dest, src, length);
        }
    }
}

#endif

