// Unit 47 (tools/tu_map.py): the rumble manager's table of players. Stores a three-float vector at
// +0x174 of the player registered under `key` and marks it changed (+0x1a4). Names and layouts are
// placeholders.
#include "types.h"

struct UnkVec3_l070344 {
    f32 x, y, z;
};

struct UnkRumblePlayer_l070344 {
    u8 pad_0x0[0x174];
    UnkVec3_l070344 vec;  // 0x174
    u8 pad_0x180[0x24];
    u8 changed;  // 0x1a4
};
static_assert(__builtin_offsetof(UnkRumblePlayer_l070344, changed) == 0x1a4, "UnkRumblePlayer placeholder layout");

struct UnkRumbleTable_l070344 {
    u8 pad_0x0[0x20a0];
    UnkRumblePlayer_l070344** values;  // 0x20a0
    u64* keys;                         // 0x20a8
    u32 count;                         // 0x20b0

    UnkRumblePlayer_l070344** End() { return &values[count]; }
    UnkRumblePlayer_l070344** Find(u64 key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
    bool Has(u64 key) { return Find(key) != End(); }
    UnkRumblePlayer_l070344*& At(u64 key) {
        u64 i = 0;
        while (i < count && keys[i] != key)
            ++i;
        return values[i];
    }
};

void fn_7100070344(UnkRumbleTable_l070344* self, u64 key, const UnkVec3_l070344* vec) {
    if (!self->Has(key))
        return;
    if (UnkRumblePlayer_l070344* player = self->At(key)) {
        player->vec.x = vec->x;
        player->vec.y = vec->y;
        player->vec.z = vec->z;
        player->changed = 1;
    }
}
