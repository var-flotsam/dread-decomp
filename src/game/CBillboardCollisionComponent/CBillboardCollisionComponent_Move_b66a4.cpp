// CBillboardCollisionComponent's unit (tools/tu_map.py): when `key` is in the flat map at +0x140
// (values, keys, count), moves it from the bucket its object's id names (key +0x10, its +0x20) to
// `id`: the counters of the vector at +0x190 (grown on demand, fn_710008c504) drop and rise by one.
// Names and layouts are placeholders.
#include "UnkClone.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkClone_fn_710008c504;
using UnkCounter_b66a4 = UnkCloneElem<UnkClone_fn_710008c504, 4>;
extern template UnkCounter_b66a4& base::UnkVector<UnkCounter_b66a4>::GrowAt(u32);

struct UnkBillboardInfo_b66a4 {
    u8 pad_0x0[0x20];
    u32 id;  // 0x20
};

struct UnkBillboardKey_b66a4 {
    u8 pad_0x0[0x10];
    UnkBillboardInfo_b66a4* info;  // 0x10
};

struct UnkBillboardMap_b66a4 {
    void** values;
    UnkBillboardKey_b66a4** keys;
    u32 count;

    void** Find(const UnkBillboardKey_b66a4* key) {
        for (u64 i = 0; i != count; ++i) {
            if (keys[i] == key)
                return &values[i];
        }
        return values + count;
    }
};

struct UnkBillboardCollision_b66a4 {
    u8 pad_0x0[0x140];
    UnkBillboardMap_b66a4 map;  // 0x140
    u8 pad_0x158[0x190 - 0x158];
    base::UnkVector<UnkCounter_b66a4> counters;  // 0x190
};

void fn_7100725cc0(UnkBillboardCollision_b66a4* self, UnkBillboardKey_b66a4* key, u32 id) {
    if (self->map.Find(key) != self->map.values + self->map.count) {
        --*reinterpret_cast<u32*>(&self->counters.GrowAt(key->info->id));
        ++*reinterpret_cast<u32*>(&self->counters.GrowAt(id));
    }
}
