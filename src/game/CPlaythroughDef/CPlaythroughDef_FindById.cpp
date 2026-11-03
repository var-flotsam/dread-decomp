// CPlaythroughDef's unit (tools/tu_map.py unit 425): the first entry pointer of the array at +0x68
// (count +0x70) whose id (+0x10) is `*id`, or null; the same shape as fn_7100255368
// (src/game/CActorDef/CActorDef_FindById.cpp). Names are placeholders.
#include "types.h"

struct UnkPlaythroughEntry425 {
    u8 pad_0x0[0x10];
    u64 id;  // 0x10
};

struct UnkPlaythroughDef425 {
    u8 pad_0x0[0x68];
    UnkPlaythroughEntry425** entries;  // 0x68
    u32 count;                         // 0x70
};

UnkPlaythroughEntry425* fn_71002f2d30(const UnkPlaythroughDef425* self, const u64* id) {
    UnkPlaythroughEntry425** it = self->entries;
    UnkPlaythroughEntry425** end = it + self->count;
    for (; it != end; ++it) {
        UnkPlaythroughEntry425* entry = *it;
        if (entry->id == *id) {
            return entry;
        }
    }
    return nullptr;
}
