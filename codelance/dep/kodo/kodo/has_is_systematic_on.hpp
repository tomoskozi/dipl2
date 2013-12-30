// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace kodo
{

    /// @ingroup type_traits
    /// Type trait helper allows compile time detection of whether an
    /// encoder contains a layer with the member function is_systematic_on()
    ///
    /// Example:
    ///
    /// typedef kodo::full_rlnc8_encoder encoder_t;
    ///
    /// if(kodo::has_is_systematic_on<encoder_t>::value)
    /// {
    ///     // Do something here
    /// }
    ///
    template<typename T>
    struct has_is_systematic_on
    {
    private:

        template<typename U>
        static auto test(int) ->
            decltype(std::declval<U>().is_systematic_on(), uint32_t());

        template<typename> static uint8_t test(...);

    public:

        static const bool value = sizeof(decltype(test<T>(0))) == 4;
    };

}


