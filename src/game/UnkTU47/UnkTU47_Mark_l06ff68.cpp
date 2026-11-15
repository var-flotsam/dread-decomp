// Unit 47 (tools/tu_map.py): the rumble manager's table of players. Sets the flag at +0x9e of the
// player given, if the table holds a player under it. Names and layouts are placeholders.
#include "types.h"

struct UnkRumblePlayer_l06ff68 {
    u8 pad_0x0[0x9e];
    u8 field_0x9e;
};

struct UnkRumbleTable_l06ff68 {
    u8 pad_0x0[0x20a0];
    void** values;  // 0x20a0
    u64* keys;      // 0x20a8
    u32 count;      // 0x20b0

    void** End() { return &values[count]; }
    void** Find(u64 key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
    bool Has(u64 key) { return Find(key) != End(); }
    void*& At(u64 key) {
        u64 i = 0;
        while (i < count && keys[i] != key)
            ++i;
        return values[i];
    }
};

void fn_710006ff68(UnkRumbleTable_l06ff68* self, UnkRumblePlayer_l06ff68* player) {
    u64 key = reinterpret_cast<u64>(player);
    if (!self->Has(key))
        return;
    if (self->At(key))
        player->field_0x9e = 1;
}
