// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#ifndef FIFI_PRIME_2325_BITMAP_HPP
#define FIFI_PRIME_2325_BITMAP_HPP

#include <sak/storage.hpp>

#include "field_types.hpp"

namespace fifi
{
    /// Bitmap algorithm for finding the an unused bit prefix within a block of
    /// data, using the prefix we may map arbitrary data to the 2^32 - 5 prime
    /// field a approach which was suggested by Crowley et al.
    struct prime2325_bitmap
    {
        /// The data type used for the bitmap
        typedef uint32_t mapping_type;

        /// Create a new prefix bitmap
        ///
        /// @param max_block_length denotes the largest block size in bytes that
        ///        can be searched using this algorithm.
        prime2325_bitmap(uint32_t max_block_length)
            : m_max_block_length(max_block_length),
              m_max_block_size(m_max_block_length * 4)
            {
                assert(m_max_block_length <= prime2325::max_block_length);
                assert(m_max_block_length > 0);

                // log2 of the maximum block size gives us the
                // maximum bit prefix needed
                uint32_t max_prefix_bits =
                    prime2325::prefix_length(m_max_block_length);

                // The amount to shift the prefix to get the prefix value:
                //
                // | b31 b30 b29 b28 b27  ... b1 b0 |
                //    ^               ^
                //    '-   prefix    -'
                //          value
                //
                // In the above example we have 5 prefix bits so we need to
                // shift it down 32-5 = 27 bits to get the numeric prefix value

                m_shift_prefix = 32 - max_prefix_bits;

                // The number of bits in the mapping type
                m_mapping_bits = std::numeric_limits<mapping_type>::digits;

                // We need one bit per possible prefix value we have
                // 2^m_max_prefix_bits possible values
                uint32_t bytes = size_needed(m_max_block_length);

                // ceil(x/y) = ((x - 1) / y) + 1
                uint32_t bitmap_elements = ((bytes - 1) / sizeof(mapping_type)) + 1;
                m_bitmap.resize(bitmap_elements);
            }

        /// @param block_length the length of a block
        /// @return the size in bytes needed for the bitmap depending on the
        /// block length i.e. the number of 32-bit integers in the block.
        static uint32_t size_needed(uint32_t block_length)
            {
                assert(block_length > 0);
                assert(block_length <= prime2325::max_block_length);

                uint32_t prefix_bits = prime2325::prefix_length(block_length);

                // ceil(x/y) = ((x - 1) / y) + 1
                uint32_t size_bytes = (((1 << prefix_bits) - 1) / 8) + 1;
                assert(size_bytes > 0);

                return size_bytes;
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
                // @see crowley2325::max_block_size()
                uint32_t size = sak::storage_size(first, last);

                assert(size <= m_max_block_size);
                assert(size > 0);

                std::fill(m_bitmap.begin(), m_bitmap.end(), 0);

                // Update the bitmap
                while(first != last)
                {
                    // Size must be multiple of 4 bytes due to the field 2^32-5
                    assert((first->m_size % 4) == 0);

                    uint32_t block_size = first->m_size / 4;
                    const uint32_t *block_data =
                        sak::cast_storage<uint32_t>(*first);

                    for(uint32_t i = 0; i < block_size; ++i)
                    {
                        update_bitmap(block_data[i]);
                    }

                    ++first;
                }

                uint32_t prefix = 0;
                bool found = find_in_bitmap(&prefix);

                assert(found);

                return prefix;

            }

        /// Iterates through the bitmap looking for the unused prefix
        /// @param prefix will contain the unused prefix
        /// @return returns true if the prefix was found otherwise false
        bool find_in_bitmap(uint32_t *prefix) const
            {
                // Find the missing prefix in the bitmap
                for(uint32_t i = 0; i < m_bitmap.size(); ++i)
                {
                    for(uint32_t j = 0; j < m_mapping_bits; ++j)
                    {
                        if((m_bitmap[i] & (1 << j)) == 0)
                        {
                            *prefix = (i*m_mapping_bits + j) << m_shift_prefix;
                            return true;
                        }
                    }
                }

                return false;
            }

        /// Updates the bitmap so that it reflects that the value has been found
        void update_bitmap(uint32_t value)
            {
                // We use a 32 bit data type for the base value
                uint32_t prefix_value = value >> m_shift_prefix;

                uint32_t index = prefix_value / m_mapping_bits;
                uint32_t offset = prefix_value % m_mapping_bits;

                m_bitmap[index] = m_bitmap[index] | 1 << offset;
            }

        /// Prints the identification of this algorithm
        void print_info(std::ostream &out)
            {
                out << "bitmap, max_block_length=" << m_max_block_length;
            }

        /// The maximum block length in elements of uint32_t
        uint32_t m_max_block_length;

        /// The maximum block size in bytes
        uint32_t m_max_block_size;

        // The number of bits in the mapping type
        uint32_t m_mapping_bits;

        /// The amount we need to shift the prefix
        uint32_t m_shift_prefix;

        /// The bitmap storing info about whether a specific prefix was found
        std::vector<mapping_type> m_bitmap;
    };
}

#endif


