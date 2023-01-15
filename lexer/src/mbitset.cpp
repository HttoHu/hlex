#include "../includes/mbitset.h"

namespace DS
{
    void BitSet::insert(int x)
    {
        int cur_len = data.size() * int_bits;

        if (cur_len <= x)
            data.resize(x / int_bits + 1);
        data[x / int_bits] |= (1ull << (x % int_bits));
    }
    void BitSet::erase(int x)
    {
        int cur_len = data.size() * int_bits;

        if (cur_len > x && (data[x / int_bits] >> (x % int_bits) & 1))
            data[x / int_bits] ^= (1ull << (x % int_bits));
    }
    BitSet &BitSet::operator|=(const BitSet &bs)
    {
        if (data.size() < bs.data.size())
            data.resize(bs.data.size());
        for (int i = 0; i < bs.data.size(); i++)
            data[i] |= bs.data[i];
        return *this;
    }

    BitSet &BitSet::operator&=(const BitSet &bs)
    {
        if (data.size() > bs.data.size())
            data.resize(bs.data.size());
        for (int i = 0; i < data.size(); i++)
            data[i] &= bs.data[i];
        return *this;
    }
    bool BitSet::count(int x) const
    {
        return (data.size() * int_bits > x && (data[x / int_bits] >> (x % int_bits) & 1));
    }
    std::vector<int> BitSet::to_vector() const
    {
        std::vector<int> ret;
        int base = 0;
        for (int i = 0; i < data.size(); i++, base += int_bits)
        {
            if (data[i] == 0)
                continue;
            int cur = base;
            for (int_type j = 0; j < int_bits; j++, cur++)
            {
                if ((data[i] >> j) & 1)
                    ret.push_back(cur);
            }
        }
        return ret;
    }

}