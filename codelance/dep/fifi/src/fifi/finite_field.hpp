// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#ifndef FIFI_FINITE_FIELD_HPP
#define FIFI_FINITE_FIELD_HPP

#include <boost/shared_ptr.hpp>

namespace fifi
{

    /// A finite field object provides a wrapper around a concrete field
    /// implementation.
    template<class FieldImpl>
    class finite_field
    {
    public:

        /// field implementation typedef
        typedef FieldImpl field_impl;

        /// field implementation shared pointer typedef
        typedef boost::shared_ptr<field_impl> impl_type;

        /// field type typedef
        typedef typename field_impl::field_type field_type;

        /// value type typedef
        typedef typename field_type::value_type value_type;

    public:

        /// Constructor to create a new finite field
        finite_field();

        ///  Operator performing the field multiplication
        ///  @param element_one the first field element
        ///  @param element_two the second field element
        ///  @return the result after multiplication
        value_type multiply(value_type element_one, value_type element_two) const;

        /// Perform division of two field elements
        /// @param numerator the element in the numerator
        /// @param denominator the element in the denominator
        /// @return the result after division
        value_type divide(value_type numerator, value_type denominator) const;

        ///  Operator performing the field inversion
        ///  @param element the field element to be inverted
        ///  @return the result after inversion
        value_type invert(value_type element) const;

        ///  Performs the field addition
        ///  @param element_one the first field element
        ///  @param element_two the second field element
        ///  @return the result after addition
        value_type add(value_type element_one, value_type element_two) const;

        ///  Performs the field subtraction
        ///  @param element_one the first field element
        ///  @param element_two the second field element
        ///  @return the result after subtraction
        value_type subtract(value_type element_one, value_type element_two) const;

        ///  Returns a reference to the field implementation
        FieldImpl& impl();

    private:

        ///  The private implementation
        impl_type m_impl;
    };

    template<class FieldImpl>
    finite_field<FieldImpl>::finite_field() : m_impl(new FieldImpl)
    {}

    template<class FieldImpl>
    typename finite_field<FieldImpl>::value_type finite_field<FieldImpl>::multiply(
        value_type element_one, value_type element_two) const
    {
        return m_impl->multiply(element_one, element_two);
    }

    template<class FieldImpl>
    typename finite_field<FieldImpl>::value_type finite_field<FieldImpl>::divide(
        value_type numerator, value_type denominator) const
    {
        return m_impl->divide(numerator, denominator);
    }

    template<class FieldImpl>
    typename finite_field<FieldImpl>::value_type finite_field<FieldImpl>::invert(
        value_type element) const
    {
       return m_impl->invert(element);
    }

    template<class FieldImpl>
    typename finite_field<FieldImpl>::value_type finite_field<FieldImpl>::add(
        value_type element_one, value_type element_two) const
    {
        return m_impl->add(element_one, element_two);
    }

    template<class FieldImpl>
    typename finite_field<FieldImpl>::value_type finite_field<FieldImpl>::subtract(
        value_type element_one, value_type element_two) const
    {
        return m_impl->subtract(element_one, element_two);
    }

    template<class FieldImpl>
    FieldImpl& finite_field<FieldImpl>::impl()
        {
            return *m_impl;
        }
}

#endif

