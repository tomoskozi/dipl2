// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#ifndef FIFI_ELEMENT_HPP
#define FIFI_ELEMENT_HPP

namespace fifi
{
    /// Represents an element in our field - any calculations
    /// preformed on this type of elements will be done over
    /// the specified field. This is not the most efficient way
    /// to do arithmetics over the field. Typically faster code
    /// can be derived by directly operating on e.g. the used
    /// field look-up table. However, in certain cases quick testing
    /// the field type can be quite useful.
    template<class FieldImpl>
    class element
    {
    public:

        /// Typedef of the field implementation type
        typedef FieldImpl field_impl_type;

        /// Typedef of the field type
        typedef typename field_impl_type::field_type field_type;

        /// Typedef of the data type used for each field element
        typedef typename field_impl_type::value_type value_type;

    public:
        /// Construct a zero initialized field element.
        /// @param field_impl the field to which the element belongs
        element(const field_impl_type &field_impl);

        /// Construct a value initialized field element
        /// @param value the initial value of the finite field
        /// @param field_impl the field to which the element belongs
        element(value_type value, const field_impl_type &field_impl);

        /// Addition assignment
        element& operator+=(const element &e);

        /// Substraction assignment
        element& operator-=(const element &e);

        /// Division assignment
        element& operator/=(const element &e);

        /// Multiplication assignment
        element& operator*=(const element &e);

        /// Access to the field element
        /// @return the ValueType constaining the decimal value of the
        ///         field element
        value_type value() const;

    private:
        /// The value to the specific field element
        value_type m_value;

        /// Reference to the field providing the arithmetic operations
        field_impl_type m_field_impl;
    };

    template<class FieldImpl>
    inline element<FieldImpl>::element(const field_impl_type &field_impl)
        : m_value(0),
          m_field_impl(field_impl)
    {
    }

    template<class FieldImpl>
    inline element<FieldImpl>::element(value_type value, const field_impl_type &field_impl)
        : m_value(value),
          m_field_impl(field_impl)
    {
    }

    template<class FieldImpl>
    inline element<FieldImpl>& element<FieldImpl>::operator+=(const element &e)
    {
        m_value = m_field_impl.add(m_value, e.m_value);
        return *this;
    }

    template<class FieldImpl>
    inline element<FieldImpl>& element<FieldImpl>::operator-=(const element<FieldImpl> &e)
    {
        m_value = m_field_impl.subtract(m_value, e.m_value);
        return *this;
    }

    template<class FieldImpl>
    inline element<FieldImpl>& element<FieldImpl>::operator/=(const element<FieldImpl> &e)
    {
        m_value = m_field_impl.divide(m_value, e.m_value);
        return *this;
    }

    template<class FieldImpl>
    inline element<FieldImpl>& element<FieldImpl>::operator*=(const element<FieldImpl> &e)
    {
        m_value = m_field_impl.multiply(m_value, e.m_value);
        return *this;
    }


    template<class FieldImpl>
    inline typename element<FieldImpl>::value_type element<FieldImpl>::value() const
    {
        return m_value;
    }

    /// Addition operator for a field element
    template<class FieldImpl>
    inline element<FieldImpl> operator+(const element<FieldImpl> &elementA,
                                        const element<FieldImpl> &elementB)
    {
        return element<FieldImpl>(elementA) += elementB;
    }

    /// Substraction operator for a field element
    template<class FieldImpl>
    inline element<FieldImpl> operator-(const element<FieldImpl> &elementA,
                                        const element<FieldImpl> &elementB)
    {
        return element<FieldImpl>(elementA) -= elementB;
    }

    /// Multiply operator for a field element
    template<class FieldImpl>
    inline element<FieldImpl> operator*(const element<FieldImpl> &elementA,
                                        const element<FieldImpl> &elementB)
    {
        return element<FieldImpl>(elementA) *= elementB;
    }

    /// Division operator for a field element
    template<class FieldImpl>
    inline element<FieldImpl> operator/(const element<FieldImpl> &elementA,
                                        const element<FieldImpl> &elementB)
    {
        return element<FieldImpl>(elementA) /= elementB;
    }

    /// ostream operator for a field element
    template<class FieldImpl>
    inline std::ostream& operator<<(std::ostream &os,
                                    const element<FieldImpl> &element)
    {
        return os << (unsigned)element.Value();
    }
}

#endif

