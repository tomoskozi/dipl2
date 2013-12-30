// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <stdexcept>

#include <gauge/gauge.hpp>
#include <gauge/console_printer.hpp>
#include <gauge/python_printer.hpp>

#include <fifi/simple_online.hpp>
#include <fifi/full_table.hpp>
#include <fifi/log_table.hpp>
#include <fifi/extended_log_table.hpp>
#include <fifi/optimal_prime.hpp>
#include <fifi/field_types.hpp>

std::vector<uint32_t> setup_lengths()
{
    std::vector<uint32_t> lengths;
    lengths.push_back(100);
    lengths.push_back(1000);
    lengths.push_back(2000);
    return lengths;
}



template<class Field>
void multiply_loop(const Field &field,
                   typename Field::value_type *pr,
                   const typename Field::value_type *pa,
                   const typename Field::value_type *pb,
                   uint64_t elements)
{
    RUN{
        for(uint64_t i = 0; i < elements; ++i)
        {
            pr[i] = field.multiply(pa[i], pb[i]);
        }
    }
}

template<class Field>
void divide_loop(const Field &field,
                 typename Field::value_type *pr,
                 const typename Field::value_type *pa,
                 const typename Field::value_type *pb,
                 uint64_t elements)
{
    RUN{
        for(uint64_t i = 0; i < elements; ++i)
        {
            pr[i] = field.divide(pa[i], pb[i]);
        }
    }
}

template<class Field>
void add_loop(const Field &field,
              typename Field::value_type *pr,
              const typename Field::value_type *pa,
              const typename Field::value_type *pb,
              uint64_t elements)
{
    RUN{
        for(uint64_t i = 0; i < elements; ++i)
        {
            pr[i] = field.add(pa[i], pb[i]);
        }
    }
}

template<class Field>
void subtract_loop(const Field &field,
                   typename Field::value_type *pr,
                   const typename Field::value_type *pa,
                   const typename Field::value_type *pb,
                   uint64_t elements)
{
    RUN{
        for(uint64_t i = 0; i < elements; ++i)
        {
            pr[i] = field.subtract(pa[i], pb[i]);
        }
    }
}



template<class Field>
class basic_operations_setup : public gauge::time_benchmark
{
public:

    typedef Field field_type;
    typedef typename Field::value_type value_type;

public:

    basic_operations_setup()
        {
            std::vector<uint32_t> lengths = setup_lengths();
            //std::vector<std::string> operations = setup_operations();

            for(uint32_t i = 0; i < lengths.size(); ++i)
            {
//                for(uint32_t k = 0; k < operations.size(); ++k)
//                {
                    gauge::config_set cs;
                    cs.set_value<uint32_t>("vector_length", lengths[i]);
                    //                  cs.set_value<std::string>("operation", operations[k]);
                    cs.set_value<uint32_t>("element_size", sizeof(value_type));
                    add_configuration(cs);
                    //    }
            }
        }

    void setup()
        {
            gauge::config_set cs = get_current_configuration();

            uint32_t length = cs.get_value<uint32_t>("vector_length");

            m_results.resize(length);
            m_symbols_one.resize(length);
            m_symbols_two.resize(length);
            m_symbols_two_nonzero.resize(length);

            for(uint32_t i = 0; i < length; ++i)
            {
                value_type s1 = rand() %
                    std::numeric_limits<value_type>::max();

                value_type s2 = rand() %
                    std::numeric_limits<value_type>::max();

                value_type s3 = rand() %
                    (std::numeric_limits<value_type>::max() - 1);


                m_symbols_one[i] = s1;
                m_symbols_two[i] = s2;
                m_symbols_two_nonzero[i] = s3 + 1;
            }

        }

    void run_multiply()
        {
            gauge::config_set cs = get_current_configuration();
            uint32_t length = cs.get_value<uint32_t>("vector_length");

            multiply_loop(m_field,
                          &m_results[0],
                          &m_symbols_one[0],
                          &m_symbols_two[0],
                          length);
        }

    void run_divide()
        {
            gauge::config_set cs = get_current_configuration();
            uint32_t length = cs.get_value<uint32_t>("vector_length");

            divide_loop(m_field,
                        &m_results[0],
                        &m_symbols_one[0],
                        &m_symbols_two_nonzero[0],
                        length);
        }

    void run_add()
        {
            gauge::config_set cs = get_current_configuration();
            uint32_t length = cs.get_value<uint32_t>("vector_length");

            add_loop(m_field,
                     &m_results[0],
                     &m_symbols_one[0],
                     &m_symbols_two[0],
                     length);
        }

    void run_subtract()
        {
            gauge::config_set cs = get_current_configuration();
            uint32_t length = cs.get_value<uint32_t>("vector_length");

            subtract_loop(m_field,
                          &m_results[0],
                          &m_symbols_one[0],
                          &m_symbols_two[0],
                          length);
        }

protected:

    field_type m_field;

    std::vector< value_type > m_results;
    std::vector< value_type > m_symbols_one;
    std::vector< value_type > m_symbols_two;
    std::vector< value_type > m_symbols_two_nonzero;

};


typedef basic_operations_setup< fifi::simple_online<fifi::binary8> >
    setup_simple_online_binary8;

BENCHMARK_F(setup_simple_online_binary8, SimpleOnline8, Multiply, 5)
{
    run_multiply();
}

BENCHMARK_F(setup_simple_online_binary8, SimpleOnline8, Divide, 5)
{
    run_divide();
}

BENCHMARK_F(setup_simple_online_binary8, SimpleOnline8, Add, 5)
{
    run_add();
}

BENCHMARK_F(setup_simple_online_binary8, SimpleOnline8, Subtract, 5)
{
    run_subtract();
}


typedef basic_operations_setup< fifi::simple_online<fifi::binary16> >
    setup_simple_online_binary16;

BENCHMARK_F(setup_simple_online_binary16, SimpleOnline16, Multiply, 5)
{
    run_multiply();
}

BENCHMARK_F(setup_simple_online_binary16, SimpleOnline16, Divide, 5)
{
    run_divide();
}

BENCHMARK_F(setup_simple_online_binary16, SimpleOnline16, Add, 5)
{
    run_add();
}

BENCHMARK_F(setup_simple_online_binary16, SimpleOnline16, Subtract, 5)
{
    run_subtract();
}


typedef basic_operations_setup< fifi::full_table<fifi::binary8> >
    setup_full_table_binary8;

BENCHMARK_F(setup_full_table_binary8, FullTable8, Multiply, 5)
{
    run_multiply();
}

BENCHMARK_F(setup_full_table_binary8, FullTable8, Divide, 5)
{
    run_divide();
}

BENCHMARK_F(setup_full_table_binary8, FullTable8, Add, 5)
{
    run_add();
}

BENCHMARK_F(setup_full_table_binary8, FullTable8, Subtract, 5)
{
    run_subtract();
}


typedef basic_operations_setup< fifi::log_table<fifi::binary8> >
    setup_log_table_binary8;

BENCHMARK_F(setup_log_table_binary8, LogTable8, Multiply, 5)
{
    run_multiply();
}

BENCHMARK_F(setup_log_table_binary8, LogTable8, Divide, 5)
{
    run_divide();
}

BENCHMARK_F(setup_log_table_binary8, LogTable8, Add, 5)
{
    run_add();
}

BENCHMARK_F(setup_log_table_binary8, LogTable8, Subtract, 5)
{
    run_subtract();
}


typedef basic_operations_setup< fifi::log_table<fifi::binary16> >
    setup_log_table_binary16;

BENCHMARK_F(setup_log_table_binary16, LogTable16, Multiply, 5)
{
    run_multiply();
}

BENCHMARK_F(setup_log_table_binary16, LogTable16, Divide, 5)
{
    run_divide();
}

BENCHMARK_F(setup_log_table_binary16, LogTable16, Add, 5)
{
    run_add();
}

BENCHMARK_F(setup_log_table_binary16, LogTable16, Subtract, 5)
{
    run_subtract();
}


typedef basic_operations_setup< fifi::extended_log_table<fifi::binary8> >
    setup_extended_log_table_binary8;

BENCHMARK_F(setup_extended_log_table_binary8, ExtendedLogTable8, Multiply, 5)
{
    run_multiply();
}

BENCHMARK_F(setup_extended_log_table_binary8, ExtendedLogTable8, Divide, 5)
{
    run_divide();
}

BENCHMARK_F(setup_extended_log_table_binary8, ExtendedLogTable8, Add, 5)
{
    run_add();
}

BENCHMARK_F(setup_extended_log_table_binary8, ExtendedLogTable8, Subtract, 5)
{
    run_subtract();
}


typedef basic_operations_setup< fifi::extended_log_table<fifi::binary16> >
    setup_extended_log_table_binary16;

BENCHMARK_F(setup_extended_log_table_binary16, ExtendedLogTable16, Multiply, 5)
{
    run_multiply();
}

BENCHMARK_F(setup_extended_log_table_binary16, ExtendedLogTable16, Divide, 5)
{
    run_divide();
}

BENCHMARK_F(setup_extended_log_table_binary16, ExtendedLogTable16, Add, 5)
{
    run_add();
}

BENCHMARK_F(setup_extended_log_table_binary16, ExtendedLogTable16, Subtract, 5)
{
    run_subtract();
}


typedef basic_operations_setup< fifi::optimal_prime<fifi::prime2325> >
    setup_optimal_prime2325;

BENCHMARK_F(setup_optimal_prime2325, OptimalPrime2325, Multiply, 5)
{
    run_multiply();
}

BENCHMARK_F(setup_optimal_prime2325, OptimalPrime2325, Divide, 5)
{
    run_divide();
}

BENCHMARK_F(setup_optimal_prime2325, OptimalPrime2325, Add, 5)
{
    run_add();
}

BENCHMARK_F(setup_optimal_prime2325, OptimalPrime2325, Subtract, 5)
{
    run_subtract();
}


int main(int argc, const char* argv[])
{
    srand(static_cast<uint32_t>(time(0)));

    gauge::runner::instance().printers().push_back(
        std::make_shared<gauge::console_printer>());

    gauge::runner::instance().printers().push_back(
        std::make_shared<gauge::python_printer>("out.py"));

    gauge::runner::run_benchmarks(argc, argv);

    return 0;
}

