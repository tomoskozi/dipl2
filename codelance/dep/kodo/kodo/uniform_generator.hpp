// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include <fifi/fifi_utils.hpp>

namespace kodo
{

    /// @ingroup coefficient_generator_layers
    /// @brief Generates an uniform random coefficient (from the chosen
    /// Finite Field) for every symbol.
    template<class SuperCoder>
    class uniform_generator : public SuperCoder
    {
    public:

        /// @copydoc layer::value_type
        typedef typename SuperCoder::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename SuperCoder::value_type value_type;

        /// The random generator used
        typedef boost::random::mt19937 generator_type;

        /// @copydoc layer::seed_type
        typedef generator_type::result_type seed_type;

    public:

        /// Constructor
        uniform_generator()
            : m_distribution(),
              m_value_distribution(field_type::min_value,
                                   field_type::max_value)
        { }

        /// @copydoc layer::generate(uint8_t*)
        void generate(uint8_t *coefficients)
        {
            assert(coefficients != 0);

            uint32_t size = SuperCoder::coefficient_vector_size();
            for(uint32_t i = 0; i < size; ++i)
            {
                coefficients[i] = m_distribution(m_random_generator);
            }
        }

        /// @copydoc layer::generate(uint8_t*)
        void generate_partial(uint8_t *coefficients)
        {
            assert(coefficients != 0);

            // Since we will not set all coefficients we should ensure
            // that the non specified ones are zero
            std::fill_n(coefficients, SuperCoder::coefficient_vector_size(), 0);

            value_type *c = reinterpret_cast<value_type*>(coefficients);

            uint32_t symbols = SuperCoder::symbols();

            for(uint32_t i = 0; i < symbols; ++i)
            {
                if(!SuperCoder::can_generate(i))
                {
                    continue;
                }

                value_type coefficient =
                    m_value_distribution(m_random_generator);

                fifi::set_value<field_type>(c, i, coefficient);
            }
        }

        /// @copydoc layer::seed(seed_type)
        void seed(seed_type seed_value)
        {
            m_random_generator.seed(seed_value);
        }

    private:

        /// The type of the uint8_t distribution
        typedef boost::random::uniform_int_distribution<uint8_t>
        uint8_t_distribution;

        /// Distribution that generates random bytes
        uint8_t_distribution m_distribution;

        /// The type of the value_type distribution
        typedef boost::random::uniform_int_distribution<value_type>
        value_type_distribution;

        /// Distribution that generates random values from a finite field
        value_type_distribution m_value_distribution;

        /// The random generator
        boost::random::mt19937 m_random_generator;

    };
}


