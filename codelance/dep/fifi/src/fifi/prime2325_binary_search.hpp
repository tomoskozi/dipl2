// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#ifndef FIFI_PRIME_2325_BINARY_SEARCH_HPP
#define FIFI_PRIME_2325_BINARY_SEARCH_HPP

#include <sak/storage.hpp>

#include "field_types.hpp"

namespace fifi
{
    /// Binary search algorithms for finding an unused bit prefix in arbitrary
    /// binary data. The algorithms can search for the prefix using k passes.
    /// When k = 2 the algorithms will have to pass over the entire block of
    /// data twice, when k = 3 the algorithms will pass over the data three
    /// times etc. This requires additional time, however the larger k is the
    /// smaller the memory consumption becomes.
    /// Using the found prefix we may map arbitrary data to the 2^32 - 5 prime
    /// field an approach which was suggested by Crowley et al.
    struct prime2325_binary_search
    {
        /// The data type used for the bitmap
        typedef uint32_t counter_type;

        /// Create a new binary search object
        ///
        /// @param max_block_length denotes the largest block size in bytes
        ///        that can be searched using this algorithm.
        /// @param k_pass denotes the number times to iterate over the data. The
        ///        larger k_pass the less memory but more computations are needed.
        prime2325_binary_search(uint32_t max_block_length, uint32_t k_pass = 2)
            : m_max_block_length(max_block_length),
              m_max_block_size(m_max_block_length * 4),
              m_k_pass(k_pass)
            {
                assert(m_max_block_length <= prime2325::max_block_length);
                assert(m_max_block_length > 0);
                assert(m_k_pass > 0);

                // How many bits are needed to find an unused prefix
                uint32_t prefix_bits =
                    prime2325::prefix_length(m_max_block_length);

                // Calculate the shifts required for each pass
                // Example m_k_pass_bits = 4
                // For k = 0, shift_prefix = 28
                // For k = 1, shift_prefix = 24
                // For k = 2, shift_prefix = 20
                // Etc.
                m_shift_prefix.resize(m_k_pass, 0);

                // Since we only look at m_k_pass_bits at-a-time we need to zero
                // everything else after shifting the prefix down
                // I.e. so we get 00000....0001111 if m_k_pass_bits = 4
                m_shift_mask.resize(m_k_pass, 0);

                // We only look at values that match our already found prefix,
                // this mask
                // For k = 0, mask = 00000000000...0000000000
                // For k = 1, mask = 11110000000...0000000000
                // For k = 2, maks = 11111111000...0000000000
                // Etc.
                m_prefix_mask.resize(m_k_pass, 0);

                // The number of bits inspected in the k'th step
                m_k_pass_bits.resize(m_k_pass, 0);

                // If we do two passes and the prefix length is odd we do both
                // small and large shifts e.g. for 2 passes with a prefix length
                // of 5 we have a shift of 3 and a shift of 2
                uint32_t large_shifts = ((prefix_bits - 1) / m_k_pass) + 1;
                uint32_t small_shifts = prefix_bits / m_k_pass;

                // Since large shift is one larger than small shifts we can see
                // how many large we need by checking how many bits are covered
                // by small_shifts
                uint32_t number_large_shifts =
                    prefix_bits - m_k_pass * small_shifts;

                uint32_t shift_sum = 0;

                for(uint32_t i = 0; i < m_k_pass; ++i)
                {

                    m_prefix_mask[i] = ~(~0U >> shift_sum);

                    if(i < number_large_shifts)
                    {
                        m_shift_mask[i] = ~(~0U << large_shifts);
                        shift_sum += large_shifts;
                        m_k_pass_bits[i] = large_shifts;
                    }
                    else
                    {
                        m_shift_mask[i] = ~(~0U << small_shifts);
                        shift_sum += small_shifts;
                        m_k_pass_bits[i] = small_shifts;
                    }

                    assert(shift_sum < 32);
                    m_shift_prefix[i] = 32 - shift_sum;
                }

                // We need one bucket with a counter for each of the
                // prefixes inspected in each k pass
                uint32_t number_of_buckets = 1 << large_shifts;
                assert(number_of_buckets);

                m_buckets.resize(number_of_buckets);
            }

        /// @param block_length the block length in uint32_t elements
        /// @param k_pass the number of passes to take
        /// @return the size in bytes needed for the binary search
        static uint32_t size_needed(uint32_t block_length, uint32_t k_pass)
            {
                assert(block_length > 0);
                assert(block_length <= prime2325::max_block_length);

                // How many bits are needed to find an unused prefix
                uint32_t prefix_bits =
                    prime2325::prefix_length(block_length);

                uint32_t large_shifts = ((prefix_bits - 1) / k_pass) + 1;
                uint32_t number_of_buckets = 1 << large_shifts;

                return number_of_buckets * sizeof(counter_type);
            }

        /// Finds a returns an unused prefix in the storage sequence
        /// @param sequence a storage sequence @see sak::storage_list
        /// @return an unused prefix
        template<class StorageSequence>
        uint32_t find_prefix(const StorageSequence &sequence)
            {
                return find_prefix(sequence.begin(), sequence.end());
            }

        /// Finds a returns an unused prefix in the storage objects
        /// which conform to the API of sak::mutable_storage or
        /// sak::const_storage
        /// @param first iterator to a storage object
        /// @param last iterator to the last storage object
        /// @return an unused prefix
        template<class StorageIterator>
        uint32_t find_prefix(StorageIterator first, StorageIterator last)
            {
                // @see prime2325::max_block_size()
                uint32_t size = sak::storage_size(first, last);

                assert(size <= m_max_block_size);
                assert(size > 0);

                uint32_t prefix = 0;

                for(uint32_t k = 0; k < m_k_pass; ++k)
                {
                    update_buckets(first, last, k, prefix);
                    update_prefix(k, &prefix);
                }

                return prefix;
            }

        /// Iterates through the bitmap looking for the unused prefix
        /// @param k denotes the current pass this controls where in the
        ///        prefix the bit pattern found should be placed
        /// @param prefix contains the so far found prefix and will contain an
        ///        updated prefix after inspecting the buckets
        /// @return returns true if the final prefix was found otherwise false
        bool update_prefix(uint32_t k, uint32_t *prefix) const
            {
                uint32_t min_index = 0;

                uint32_t valid_buckets = 1 << m_k_pass_bits[k];
                assert(valid_buckets <= m_buckets.size());

                for(uint32_t i = 1; i < valid_buckets; ++i)
                {
                    if(m_buckets[i] < m_buckets[min_index])
                    {
                        min_index = i;
                    }
                }

                // Update the prefix value
                // Example if the top 4 bits have been determined and we now
                // look for the next 4 bits in an 8 bit prefix:
                //
                // b31 b30 b29 b28 ? ? ? ? 0 0 0 0 0 0 .... 0 0 0 0
                uint32_t shift_prefix = m_shift_prefix[k];
                *prefix = *prefix | (min_index << shift_prefix);

                // If a counter has reached zero we are done
                return m_buckets[min_index] == 0;
            }

        /// Updates the bucket counters using the values in the storage.
        /// @param first iterator to a storage object
        /// @param last iterator to the last storage object
        /// @param k the current pass
        /// @param prefix the current prefix
        template<class StorageIterator>
        void update_buckets(StorageIterator first, StorageIterator last,
                            uint32_t k, uint32_t prefix)
            {
                // Empty buckets
                std::fill(m_buckets.begin(), m_buckets.end(), 0);

                uint32_t shift_prefix = m_shift_prefix[k];
                assert(shift_prefix < 32);

                uint32_t shift_mask = m_shift_mask[k];
                uint32_t prefix_mask = m_prefix_mask[k];

                while(first != last)
                {
                    // Size must be multiple of 4 bytes due to the field 2^32-5
                    assert((first->m_size % 4) == 0);

                    uint32_t block_size = first->m_size / 4;
                    const uint32_t *block_data =
                        sak::cast_storage<uint32_t>(*first);

                    for(uint32_t i = 0; i < block_size; ++i)
                    {
                        // We only count for values which match the already
                        // found prefix
                        if((block_data[i] & prefix_mask) == prefix)
                        {
                            uint32_t bucket_index =
                                (block_data[i] >> shift_prefix) & shift_mask;

                            assert(bucket_index < m_buckets.size());

                            ++m_buckets[bucket_index];
                        }
                    }

                    ++first;
                }
            }

        /// Prints the identification of this algorithm
        void print_info(std::ostream &out)
            {
                out << "binary_search, max_block_length=" << m_max_block_length
                    << " k_pass=" << m_k_pass;
            }

        /// The maximum block length in elements of uint32_t
        uint32_t m_max_block_length;

        /// The maximum block size in bytes
        uint32_t m_max_block_size;

        /// The number of passes over the data
        uint32_t m_k_pass;

        /// The number of bits inspected in step k
        std::vector<uint32_t> m_k_pass_bits;

        /// Then number of bit shifts needed in step k
        std::vector<uint32_t> m_shift_prefix;

        /// Mask used to only inspect the wanted bits in step k
        std::vector<uint32_t> m_shift_mask;

        /// Mask used to cut-away bits not needed when filtering
        /// prefix values
        std::vector<uint32_t> m_prefix_mask;

        /// The bitmap storing info about whether a specific prefix was found
        std::vector<counter_type> m_buckets;

    };
}

#endif

