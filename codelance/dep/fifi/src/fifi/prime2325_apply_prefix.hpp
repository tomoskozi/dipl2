// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#ifndef FIFI_PRIME2325_APPLY_PREFIX_HPP
#define FIFI_PRIME2325_APPLY_PREFIX_HPP

#include "field_types.hpp"

namespace fifi
{

    /// Overload for applying a prefix to a storage object
    /// @param first iterator to the beginning of the storage
    /// @param last iterator to the end of the storage
    /// @param prefix to apply
    template<class StorageIterator>
    inline void apply_prefix(StorageIterator first,
                             StorageIterator last,
                             uint32_t prefix)
    {
        while(first != last)
        {
            // Size must be multiple of 4 bytes due to the field
            // 2^32 - 5
            assert((first->m_size % 4) == 0);

            uint32_t block_size = first->m_size / 4;

            uint32_t *block_data =
                sak::cast_storage<uint32_t>(*first);

            for(uint32_t i = 0; i < block_size; ++i)
            {
                block_data[i] ^= prefix;
            }

            ++first;
        }

    }

    /// Applies a prefix to a storage sequence
    /// @param sequence a storage sequence @see sak::storage_list
    /// @param prefix to apply
    template<class StorageSequence>
    inline void apply_prefix(StorageSequence sequence, uint32_t prefix)
    {
        apply_prefix(sequence.begin(), sequence.end(), prefix);
    }

}

#endif

