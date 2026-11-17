// UnkTU47 (tools/tu_map.py unit 47, next to the rumble players): the player of a key: the one the
// dictionary already holds, or else the first of the sixteen players not in use, recorded under the
// key and put back to its defaults (fn_71000709bc); null when all are in use. Names and layouts are
// placeholders.
#include "base/global/CRntSmallDictionary.h"
#include "types.h"

using base::global::CRntSmallDictionary;

struct UnkRumbleDefaults_A1_b73;
void fn_71000709bc(UnkRumbleDefaults_A1_b73* self);

struct UnkRumbleSlot_l06fbb0 {
    u8 pad_0x0[0x1a5];
    u8 in_use;  // 0x1a5
    u8 pad_0x1a6[0x2a];
};
static_assert(sizeof(UnkRumbleSlot_l06fbb0) == 0x1d0, "UnkRumbleSlot_l06fbb0 layout");

struct UnkRumbleOwner_l06fbb0 {
    u8 pad_0x0[0x3a0];
    UnkRumbleSlot_l06fbb0 slots[16];                                     // 0x3a0
    CRntSmallDictionary<const void*, UnkRumbleSlot_l06fbb0*> by_key;  // 0x20a0
};
static_assert(__builtin_offsetof(UnkRumbleOwner_l06fbb0, by_key) == 0x20a0, "layout");

typedef CRntSmallDictionary<const void*, UnkRumbleSlot_l06fbb0*> UnkRumbleSlotMap_l06fbb0;

// The index of the key, or the count when it is missing.
static inline u64 IndexOf(const UnkRumbleSlotMap_l06fbb0& map, const void* key) {
    u64 i = 0;
    for (; i < map.count; ++i) {
        if (map.keys[i] == key)
            break;
    }
    return i;
}

// The first player not in use, or null.
static inline UnkRumbleSlot_l06fbb0* FreeSlot(UnkRumbleOwner_l06fbb0* self) {
    for (UnkRumbleSlot_l06fbb0& slot : self->slots) {
        if (!slot.in_use)
            return &slot;
    }
    return nullptr;
}

UnkRumbleSlot_l06fbb0* fn_710006fbb0(UnkRumbleOwner_l06fbb0* self, const void* key) {
    UnkRumbleSlotMap_l06fbb0& by_key = self->by_key;
    if (by_key.Find(key) != by_key.End())
        return by_key.values[IndexOf(by_key, key)];

    UnkRumbleSlot_l06fbb0* slot = FreeSlot(self);
    if (slot) {
        if (by_key.count == by_key.capacity)
            by_key.Grow(by_key.count + by_key.step);
        by_key.keys[by_key.count] = key;
        by_key.values[by_key.count] = slot;
        ++by_key.count;
        fn_71000709bc(reinterpret_cast<UnkRumbleDefaults_A1_b73*>(slot));
    }
    return slot;
}
