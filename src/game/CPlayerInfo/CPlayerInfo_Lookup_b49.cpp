// CPlayerInfo unit (tools/tu_map.py unit 2592): fn_7100f0cc48 picks the slot (0x38 bytes each) that the
// Input tunable's word at +0x40 names, resetting that word to 0 when it is past the slot count at +0x10,
// then returns the value paired with `key` in the slot's list (pointer at +0x24, count at +0x2c; 16-byte
// entries, key at +0x8, value at +0xc), or 0. Names and layouts are placeholders.
#include "game/UnkTUProperty.h"
#include "types.h"

extern UnkTunable g_unk_71727298b0;

struct UnkKeyedValue_b49 {
    u8 pad_0x0[0x8];
    s32 key;
    u32 value;
};

struct UnkInputTunableView_b49 {
    u8 pad_0x0[0x40];
    s32 slot;
};

struct __attribute__((packed)) UnkKeyedSlot_b49 {
    u8 pad_0x0[0x10];
    s32 slot_count;
    u8 pad_0x14[0x10];
    UnkKeyedValue_b49* entries;
    u32 count;
    u8 pad_0x30[0x8];
};

u32 fn_7100f0cc48(UnkKeyedSlot_b49* self, s32 key) {
    UnkInputTunableView_b49* tunable = reinterpret_cast<UnkInputTunableView_b49*>(&g_unk_71727298b0);
    s32 slot = tunable->slot;
    if (slot >= self->slot_count) {
        slot = 0;
        tunable->slot = 0;
    }
    UnkKeyedSlot_b49* s = reinterpret_cast<UnkKeyedSlot_b49*>(reinterpret_cast<char*>(self) + (u64)(u32)slot * 0x38);
    if (u32 n = s->count) {
        UnkKeyedValue_b49* it = s->entries;
        UnkKeyedValue_b49* end = it + n;
        do {
            if (it->key == key)
                return it->value;
            ++it;
        } while (it != end);
    }
    return 0;
}
