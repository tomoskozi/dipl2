// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cmath>

#include <gauge/gauge.hpp>
#include <gauge/console_printer.hpp>
#include <gauge/python_printer.hpp>

#include <fifi/prime2325_binary_search.hpp>
#include <fifi/prime2325_bitmap.hpp>

std::vector<uint32_t> block_lengths()
{
    std::vector<uint32_t> l;
    for(uint32_t b = 1000; b < 3750000; b += 500000)
    {
        l.push_back(b);
    }

    return l;
}

std::vector<uint32_t> k_values()
{
    std::vector<uint32_t> k;
    k.push_back(1);
    k.push_back(2);
    k.push_back(3);
    k.push_back(4);
    return k;
}

/// This test shows how you can create configurations
/// for the benchmarks. This could be different string
/// lengths or similar
class run_prime2325_bitmap : public gauge::time_benchmark
{
public:

    run_prime2325_bitmap()
        {
            std::vector<uint32_t> lengths = block_lengths();

            for(uint32_t i = 0; i < lengths.size(); ++i)
            {
                gauge::config_set cs;
                cs.set_value<uint32_t>("block_length", lengths[i]);
                add_configuration(cs);
            }
        }

    void setup()
        {
            gauge::config_set cs = get_current_configuration();

            uint32_t length = cs.get_value<uint32_t>("block_length");

            m_block.resize(length);
            for(uint32_t i = 0; i < length; ++i)
                m_block[i] = rand();
        }

    std::vector<uint32_t> m_block;
};

BENCHMARK_F(run_prime2325_bitmap, Prime2325Bitmap, FindPrefix, 5)
{

    gauge::config_set cs = get_current_configuration();

    uint32_t length = cs.get_value<uint32_t>("block_length");

    fifi::prime2325_bitmap bitmap_search(length);

    volatile uint32_t prefix = 0;
    // This is where the clock runs
    RUN{
        prefix = bitmap_search.find_prefix(sak::storage(m_block));
    }

    (void)prefix;
}

/// This test shows how you can create configurations
/// for the benchmarks. This could be different string
/// lengths or similar
class run_prime2325_binary_search : public gauge::time_benchmark
{
public:

    run_prime2325_binary_search()
        {
            std::vector<uint32_t> lengths = block_lengths();
            std::vector<uint32_t> ks = k_values();

            for(uint32_t i = 0; i < lengths.size(); ++i)
            {
                for(uint32_t j = 0; j < ks.size(); ++j)
                {
                    gauge::config_set cs;
                    cs.set_value<uint32_t>("block_length", lengths[i]);
                    cs.set_value<uint32_t>("k", ks[j]);
                    add_configuration(cs);
                }
            }
        }

    void setup()
        {
            gauge::config_set cs = get_current_configuration();

            uint32_t length = cs.get_value<uint32_t>("block_length");

            m_block.resize(length);
            for(uint32_t i = 0; i < length; ++i)
                m_block[i] = rand();
        }

    std::vector<uint32_t> m_block;
};

BENCHMARK_F(run_prime2325_binary_search, Prime2325BinarySearch, FindPrefix, 5)
{

    gauge::config_set cs = get_current_configuration();

    uint32_t length = cs.get_value<uint32_t>("block_length");
    uint32_t k = cs.get_value<uint32_t>("k");

    fifi::prime2325_binary_search search(length, k);

    volatile uint32_t prefix = 0;
    // This is where the clock runs
    RUN{
        prefix = search.find_prefix(sak::storage(m_block));
    }

    (void)prefix;
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


