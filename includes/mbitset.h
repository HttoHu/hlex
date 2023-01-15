#pragma once

#include <vector>

namespace DS
{
    using int_type = unsigned long long;
    constexpr int int_bits = sizeof(int_type) * 8;

    struct BitSet
    {
    public:
        void insert(int x);
        void erase(int x);
        // |=
        BitSet &operator|=(const BitSet &bs);
        // &=
        BitSet &operator&=(const BitSet &bs);
        bool operator==(const BitSet &bs) const { return data == bs.data; }
        bool count(int x) const;
        std::vector<int> to_vector() const;

    public:
        std::vector<int_type> data;
    };

    struct BitSetHash
    {
        std::size_t operator()(const BitSet &bs) const noexcept
        {
            std::size_t ret = 1;
            for (auto num : bs.data)
                ret = ((ret * 2) + num);
            return ret;
        }
    };
}