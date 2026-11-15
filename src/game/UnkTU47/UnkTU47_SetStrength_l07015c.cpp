// Unit 47 (tools/tu_map.py): the rumble manager's table of players. Sets the strength at +0x184 of
// the player registered under `key` and marks it dirty (+0x1a3). Names and layouts are placeholders.
#include "types.h"

struct UnkRumblePlayer_l07015c {
    u8 pad_0x0[0x184];
    f32 strength;  // 0x184
    u8 pad_0x188[0x1b];
    u8 dirty;  // 0x1a3
};

struct UnkRumbleTable_l07015c {
    u8 pad_0x0[0x20a0];
    UnkRumblePlayer_l07015c** values;  // 0x20a0
    u64* keys;                         // 0x20a8
    u32 count;                         // 0x20b0

    UnkRumblePlayer_l07015c** End() { return &values[count]; }
    UnkRumblePlayer_l07015c** Find(u64 key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
    bool Has(u64 key) { return Find(key) != End(); }
    UnkRumblePlayer_l07015c*& At(u64 key) {
        u64 i = 0;
        while (i < count && keys[i] != key)
            ++i;
        return values[i];
    }
};

void fn_710007015c(UnkRumbleTable_l07015c* self, u64 key, f32 strength) {
    if (!self->Has(key))
        return;
    if (UnkRumblePlayer_l07015c* player = self->At(key)) {
        player->strength = strength;
        player->dirty = 1;
    }
}
