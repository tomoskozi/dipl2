// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace kodo
{

    /// @ingroup type_traits
    /// Type trait helper allows compile time detection of whether a
    /// codec contains a layer with the member function rank()
    ///
    /// Example:
    ///
    /// typedef kodo::full_rlnc8_encoder encoder_t;
    ///
    /// if(kodo::has_rank<encoder_t>::value)
    /// {
    ///     // Do something here
    /// }
    ///
    template<typename T>
    struct has_rank
    {
    private:
        typedef std::true_type yes;
        typedef std::false_type no;

        template<typename U>
        static auto test(int) ->
            decltype(std::declval<U>().rank(), yes());

        template<typename> static no test(...);

    public:

        static const bool value = std::is_same<decltype(test<T>(0)),yes>::value;
    };

}


