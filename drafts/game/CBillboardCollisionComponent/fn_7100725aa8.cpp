// CBillboardCollisionComponent's unit (tools/tu_map.py): taking every occurrence of an entry out of the
// flat map at +0x140. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkTU67.h"
#include "types.h"

struct UnkBillboardInfo_l725aa8 {
    u8 pad_0x0[0x20];
    u32 id;  // 0x20
};

struct UnkBillboardKey_l725aa8 {
    u8 pad_0x0[0x10];
    UnkBillboardInfo_l725aa8* info;  // 0x10
};

// A flat map: values, their keys and the shared count.
struct UnkBillboardMap_l725aa8 {
    UnkEntry** values;
    UnkBillboardKey_l725aa8** keys;
    u32 count;

    UnkEntry** Find(const UnkEntry* value) {
        for (u64 i = 0; i != count; ++i) {
            if (values[i] == value)
                return &values[i];
        }
        return values + count;
    }
};

struct UnkBillboardCollision_l725aa8 {
    u8 pad_0x0[0x140];
    UnkBillboardMap_l725aa8 map;  // 0x140
    u8 pad_0x158[0x10];
    base::global::CRntVector<UnkEntry*> removed;  // 0x168
    base::global::CRntVector<u32> counters;       // 0x190
};

// Takes `entry` out of the map as often as it is there: each time it goes on the removed list, its
// key's counter drops, it is switched off, and the last pair takes its place.
void fn_7100725aa8(UnkBillboardCollision_l725aa8* self, UnkEntry* entry) {
    UnkBillboardMap_l725aa8& map = self->map;
    UnkEntry** p = map.Find(entry);
    if (p == map.values + map.count)
        return;
    do {
        self->removed.PushBack(entry);
        --*fn_710008c504(&self->counters, map.keys[(u32)(p - map.values)]->info->id);
        fn_7100d35a98(entry, false);
        u32 index = (u32)(p - map.values);
        if (map.count - 1 != index) {
            map.values[index] = map.values[map.count - 1];
            map.keys[index] = map.keys[map.count - 1];
            index = map.count - 1;
        }
        map.count = index;
        p = map.Find(entry);
    } while (p != map.values + map.count);
}
