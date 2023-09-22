#ifndef _OCTOPUS_H_
#define _OCTOPUS_H_

#include "HeavyPart.h"
#include "coco.h"

template<int bucket_num, int tot_memory_in_bytes>
class OctopusSketch
{
public:
    static constexpr int heavy_mem = bucket_num * sizeof(Bucket);
    static constexpr int light_mem = tot_memory_in_bytes - heavy_mem;
    static constexpr bool all_light = false;

    HeavyPart<bucket_num> heavy_part;
    CocoSketch light_part;

    OctopusSketch(): light_part(light_mem) {}
    ~OctopusSketch(){}

    void clear() {
        light_part.clear();
        if (bucket_num != 0)
            heavy_part.clear();
    }

    void insert(TUPLES key, int f = 1) {
        if (bucket_num == 0) {
            light_part.insert(key, f);
            return;
        }

        TUPLES swap_key;
        uint32_t swap_val = 0;

        int result = heavy_part.insert(key, swap_key, swap_val, f);
        switch(result)
        {
            case 0: return;
            case 1:{
                if(HIGHEST_BIT_IS_1(swap_val))
                    light_part.insert(swap_key, GetCounterVal(swap_val));
                else
                    light_part.insert(swap_key, swap_val);
                return;
            }
            case 2: light_part.insert(key, f);  return;
            default:
                printf("error return value !\n");
                exit(1);
        }
    }

    int query(TUPLES key) {
        if (bucket_num == 0)
            return light_part.query(key);
        uint32_t heavy_result = heavy_part.query(key);
        if(heavy_result == 0 || HIGHEST_BIT_IS_1(heavy_result))
        {
            int light_result = light_part.query(key);
            return (int)GetCounterVal(heavy_result) + light_result;
        }
        return heavy_result;
    }

    HashMap query_all() {
        if (bucket_num == 0)
            return light_part.query_all();
        HashMap mp1 = heavy_part.query_all(), mp2 = light_part.query_all();
        for (auto it = mp1.begin(); it != mp1.end(); it++) {
            mp2[it->first] += GetCounterVal(it->second);
        }
        return mp2;
    }

    uint32_t query_partial_key(TUPLES key, KeyType type, uint32_t mask=0xffffffff) {
        if (type == five_tuples)
            return query(key);
        HashMap mp = query_all();
        uint32_t ans = 0;
        for (auto it = mp.begin(); it != mp.end(); it++) {
            if (
                type == srcIP_dstIP   && key.srcIP() == it->first.srcIP() && key.dstIP()   == it->first.dstIP()   ||
                type == srcIP_srcPort && key.srcIP() == it->first.srcIP() && key.srcPort() == it->first.srcPort() ||
                type == dstIP_dstPort && key.dstIP() == it->first.dstIP() && key.dstPort() == it->first.dstPort() ||
                type == srcIP         && (key.srcIP() & mask) == (it->first.srcIP() & mask) ||
                type == dstIP         && (key.dstIP() & mask) == (it->first.dstIP() & mask)
            ) {
                ans += it->second;
            }
        }
        return ans;
    }
};

#endif // _OCTOPUS_H_
