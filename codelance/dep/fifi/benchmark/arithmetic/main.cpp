// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cmath>
#include <stdexcept>

#include <gauge/gauge.hpp>
#include <gauge/console_printer.hpp>
#include <gauge/csv_printer.hpp>
#include <gauge/python_printer.hpp>

#include <fifi/arithmetics.hpp>
#include <fifi/fifi_utils.hpp>
#include <fifi/simple_online.hpp>
#include <fifi/full_table.hpp>
#include <fifi/log_table.hpp>
#include <fifi/extended_log_table.hpp>
#include <fifi/optimal_prime.hpp>
#include <fifi/field_types.hpp>

/// Benchmark fixture for the arithmetic benchmark
template<class FieldImpl>
class arithmetic_setup : public gauge::time_benchmark
{
public:

    /// The field implementation used
    typedef FieldImpl field_impl;

    /// The field type e.g. binary, binary8 etc
    typedef typename field_impl::field_type field_type;

    /// The value type of a field element
    typedef typename field_type::value_type value_type;

public:

    void get_options(gauge::po::variables_map& options)
    {
        auto sizes = options["size"].as<std::vector<uint32_t> >();
        auto vectors = options["vectors"].as<std::vector<uint32_t> >();
        auto operations =options["operations"].as<std::vector<std::string> >();
        auto access = options["access"].as<std::vector<std::string> >();

        // We make one pool with random data as source for the
        // computations
        uint32_t max_size =
            *std::max_element(sizes.begin(), sizes.end());

        assert((max_size % sizeof(value_type)) == 0);
        uint32_t max_length = max_size / sizeof(value_type);

        uint32_t max_vectors =
            *std::max_element(vectors.begin(), vectors.end());

        m_random_symbols_one.resize(max_vectors);
        m_random_symbols_two.resize(max_vectors);

        m_temp.resize(max_length);

        for(uint32_t j = 0; j < max_vectors; ++j)
        {
            m_random_symbols_one[j].resize(max_length);
            m_random_symbols_two[j].resize(max_length);

            for(uint32_t i = 0; i < max_length; ++i)
            {
                m_random_symbols_one[j][i] = rand();
                m_random_symbols_two[j][i] = rand();
            }
        }

        assert(sizes.size() > 0);
        assert(vectors.size() > 0);
        assert(operations.size() > 0);
        assert(access.size() > 0);

        for(const auto& s : sizes)
        {
            for(const auto& v : vectors)
            {
                for(const auto& o : operations)
                {
                    for(const auto& a : access)
                    {
                        gauge::config_set cs;
                        cs.set_value<uint32_t>("vector_size", s);

                        // Based on the desired vector size (in bytes)
                        // we calculate the length of the vector in
                        // field elements
                        assert((s % sizeof(value_type)) == 0);
                        uint32_t length = s / sizeof(value_type);
                        assert(length > 0);

                        cs.set_value<uint32_t>("vector_length", length);
                        cs.set_value<uint32_t>("vectors", v);
                        cs.set_value<std::string>("operation", o);
                        cs.set_value<std::string>("data_access", a);

                        add_configuration(cs);
                    }
                }
            }
        }
    }

    /// Prepares the data structures between each run
    void setup()
    {
        gauge::config_set cs = get_current_configuration();

        uint32_t length = cs.get_value<uint32_t>("vector_length");
        uint32_t vectors = cs.get_value<uint32_t>("vectors");

        if(vectors != m_symbols_one.size())
        {
            m_symbols_one.resize(vectors);
            m_symbols_two.resize(vectors);
        }

        for(uint32_t j = 0; j < vectors; ++j)
        {
            if(length != m_symbols_one[j].size())
            {
                m_symbols_one[j].resize(length);
                m_symbols_two[j].resize(length);
            }

            std::copy_n(&m_random_symbols_one[j][0],
                        length,
                        &m_symbols_one[j][0]);

            std::copy_n(&m_random_symbols_two[j][0],
                        length,
                        &m_symbols_two[j][0]);
        }

    }

    /// Tests the dest[i] = dest[i] OP src[i] functions
    template<class Function>
    void run_binary(Function f)
    {
        gauge::config_set cs = get_current_configuration();
        uint32_t length = cs.get_value<uint32_t>("vector_length");
        uint32_t vectors = cs.get_value<uint32_t>("vectors");
        std::string data_access = cs.get_value<std::string>("data_access");

        if(data_access == "linear")
        {
            RUN{

                for(uint32_t i = 0; i < vectors; ++i)
                {
                    f(m_field, &(m_symbols_one[i][0]),
                      &(m_symbols_two[i][0]), length);
                }

            }
        }
        else if(data_access == "random")
        {
            RUN{

                for(uint32_t i = 0; i < vectors; ++i)
                {
                    uint32_t index_one = rand() % vectors;
                    uint32_t index_two = rand() % vectors;

                    f(m_field, &(m_symbols_one[index_one][0]),
                      &(m_symbols_two[index_two][0]), length);
                }

            }
        }
        else
        {
            assert(0);
        }
    }

    /// Tests the dest[i] = dest[i] OP (src[i] * constant) functions
    template<class Function>
    void run_binary_constant(Function f)
    {
        gauge::config_set cs = get_current_configuration();
        uint32_t length = cs.get_value<uint32_t>("vector_length");
        uint32_t vectors = cs.get_value<uint32_t>("vectors");
        std::string data_access = cs.get_value<std::string>("data_access");

        if(data_access == "linear")
        {
            // Clock is ticking
            RUN{

                value_type constant = rand() % field_type::max_value;

                for(uint32_t i = 0; i < vectors; ++i)
                {
                    f(m_field, constant, &(m_symbols_one[i][0]),
                      &(m_symbols_two[i][0]), &m_temp[0], length);
                }
            }

        }
        else if(data_access == "random")
        {
            // Clock is ticking
            RUN{

                value_type constant = rand() % field_type::max_value;

                for(uint32_t i = 0; i < vectors; ++i)
                {
                    uint32_t index_one = rand() % vectors;
                    uint32_t index_two = rand() % vectors;

                    f(m_field, constant, &(m_symbols_one[index_one][0]),
                      &(m_symbols_two[index_two][0]), &m_temp[0], length);
                }
            }

        }
        else
        {
            assert(0);
        }
    }

    /// Tests the dest[i] = dest[i] * constant functions
    template<class Function>
    void run_unary_constant(Function f)
    {
        gauge::config_set cs = get_current_configuration();
        uint32_t length = cs.get_value<uint32_t>("vector_length");
        uint32_t vectors = cs.get_value<uint32_t>("vectors");
        std::string data_access = cs.get_value<std::string>("data_access");

        if(data_access == "linear")
        {

            RUN{

                value_type constant = rand() % field_type::max_value;

                for(uint32_t i = 0; i < vectors; ++i)
                {
                    f(m_field, constant, &(m_symbols_one[i][0]), length);
                }
            }
        }
        else if(data_access == "random")
        {
            RUN{

                value_type constant = rand() % field_type::max_value;

                for(uint32_t i = 0; i < vectors; ++i)
                {
                    uint32_t index = rand() % vectors;

                    f(m_field, constant, &(m_symbols_one[index][0]), length);
                }
            }

        }
        else
        {
            assert(0);
        }
    }

    /// Starts a new benchmark according to the current configuration
    void benchmark()
    {
        gauge::config_set cs = get_current_configuration();
        std::string operation = cs.get_value<std::string>("operation");

        if(operation == "dest[i] = dest[i] + src[i]")
        {
            run_binary(&fifi::add<field_impl>);
        }
        else if(operation == "dest[i] = dest[i] - src[i]")
        {
            run_binary(&fifi::subtract<field_impl>);
        }
        else if(operation == "dest[i] = dest[i] * src[i]")
        {
            run_binary(&fifi::multiply<field_impl>);
        }
        else if(operation == "dest[i] = dest[i] + (constant * src[i])")
        {
            run_binary_constant(&fifi::multiply_add<field_impl>);
        }
        else if(operation == "dest[i] = dest[i] - (constant * src[i])")
        {
            run_binary_constant(&fifi::multiply_subtract<field_impl>);
        }
        else if(operation == "dest[i] = dest[i] * constant")
        {
            run_unary_constant(&fifi::multiply_constant<field_impl>);
        }
        else
        {
            throw std::runtime_error("Unknown operation type");
        }
    }

protected:

    /// The field implementation
    field_impl m_field;

    /// The first buffer of vectors
    std::vector< std::vector<value_type> > m_symbols_one;

    /// The second buffer of vectors
    std::vector< std::vector<value_type> > m_symbols_two;

    /// Random data for the first buffer of symbols
    std::vector< std::vector<value_type> > m_random_symbols_one;

    /// Random data for the second buffer of symbols
    std::vector< std::vector<value_type> > m_random_symbols_two;

    /// Temp buffer required for composite algorithms
    std::vector<value_type> m_temp;

};


/// Using this macro we may specify options. For specifying options
/// we use the boost program options library. So you may additional
/// details on how to do it in the manual for that library.
BENCHMARK_OPTION(arithmetic_options)
{
    gauge::po::options_description options;

    std::vector<uint32_t> size;
    size.push_back(100);
    size.push_back(2000);

    auto default_size =
        gauge::po::value<std::vector<uint32_t> >()->default_value(
            size, "")->multitoken();

    std::vector<uint32_t> vectors;
    vectors.push_back(16);
    vectors.push_back(64);
    vectors.push_back(128);
    vectors.push_back(512);

    auto default_vectors =
        gauge::po::value<std::vector<uint32_t> >()->default_value(
            vectors, "")->multitoken();

    std::vector<std::string> operations;
    operations.push_back("dest[i] = dest[i] + src[i]");
    operations.push_back("dest[i] = dest[i] - src[i]");
    operations.push_back("dest[i] = dest[i] * src[i]");
    operations.push_back("dest[i] = dest[i] + (constant * src[i])");
    operations.push_back("dest[i] = dest[i] - (constant * src[i])");
    operations.push_back("dest[i] = dest[i] * constant");

    auto default_operations =
        gauge::po::value<std::vector<std::string> >()->default_value(
            operations, "")->multitoken();

    std::vector<std::string> access;
    access.push_back("random");
    access.push_back("linear");

    auto default_access =
        gauge::po::value<std::vector<std::string> >()->default_value(
            access, "")->multitoken();

    options.add_options()
        ("size", default_size, "Set the size of a vector in bytes");

    options.add_options()
        ("vectors", default_vectors, "Set the number of vectors to "
         "perform the operations on");

    options.add_options()
        ("operations", default_operations, "Set operations type");

    options.add_options()
        ("access", default_access, "Set the data access pattern");


    gauge::runner::instance().register_options(options);
}

// typedef arithmetic_setup< fifi::simple_online<fifi::binary> >
//     setup_simple_online_binary;

// BENCHMARK_F(setup_simple_online_binary, Arithmetic, SimpleOnline, 5)
// {
//     benchmark();
// }

typedef arithmetic_setup< fifi::simple_online<fifi::binary8> >
setup_simple_online_binary8;

BENCHMARK_F(setup_simple_online_binary8, Arithmetic, SimpleOnline8, 5)
{
    benchmark();
}

typedef arithmetic_setup< fifi::simple_online<fifi::binary16> >
setup_simple_online_binary16;

BENCHMARK_F(setup_simple_online_binary16, Arithmetic, SimpleOnline16, 5)
{
    benchmark();
}

typedef arithmetic_setup< fifi::full_table<fifi::binary8> >
setup_full_table_binary8;

BENCHMARK_F(setup_full_table_binary8, Arithmetic, FullTable8, 5)
{
    benchmark();
}

typedef arithmetic_setup< fifi::log_table<fifi::binary8> >
setup_log_table_binary8;

BENCHMARK_F(setup_log_table_binary8, Arithmetic, LogTable8, 5)
{
    benchmark();
}

typedef arithmetic_setup< fifi::log_table<fifi::binary16> >
setup_log_table_binary16;

BENCHMARK_F(setup_log_table_binary16, Arithmetic, LogTable16, 5)
{
    benchmark();
}

typedef arithmetic_setup< fifi::extended_log_table<fifi::binary8> >
setup_extended_log_table_binary8;

BENCHMARK_F(setup_extended_log_table_binary8, Arithmetic, ExtendedLogTable8, 5)
{
    benchmark();
}


typedef arithmetic_setup< fifi::extended_log_table<fifi::binary16> >
setup_extended_log_table_binary16;

BENCHMARK_F(setup_extended_log_table_binary16, Arithmetic, ExtendedLogTable16, 5)
{
    benchmark();
}


typedef arithmetic_setup< fifi::optimal_prime<fifi::prime2325> >
setup_optimal_prime2325;

BENCHMARK_F(setup_optimal_prime2325, Arithmetic, OptimalPrime2325, 5)
{
    benchmark();
}


int main(int argc, const char* argv[])
{

    srand(static_cast<uint32_t>(time(0)));

    gauge::runner::instance().printers().push_back(
        std::make_shared<gauge::console_printer>());

    gauge::runner::instance().printers().push_back(
        std::make_shared<gauge::csv_printer>());

    gauge::runner::instance().printers().push_back(
        std::make_shared<gauge::python_printer>());


    gauge::runner::run_benchmarks(argc, argv);

    return 0;
}

