// Unit 47 (tools/tu_map.py): the rumble manager's table of players. Passes a flag to the player
// registered under `key` (fn_7100070e64), if there is one. Names and layouts are placeholders.
#include "types.h"

struct UnkRumblePlayer_l070204;
void fn_7100070e64(UnkRumblePlayer_l070204* player, bool flag);

struct UnkRumbleTable_l070204 {
    u8 pad_0x0[0x20a0];
    UnkRumblePlayer_l070204** values;  // 0x20a0
    u64* keys;                         // 0x20a8
    u32 count;                         // 0x20b0

    UnkRumblePlayer_l070204** End() { return &values[count]; }
    UnkRumblePlayer_l070204** Find(u64 key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
    u64 IndexOf(u64 key) {
        u64 i = 0;
        for (; i != count; ++i)
            if (keys[i] == key)
                break;
        return i;
    }
};

void fn_7100070204(UnkRumbleTable_l070204* self, u64 key, bool flag) {
    if (self->Find(key) == self->End())
        return;
    UnkRumblePlayer_l070204* player = self->values[self->IndexOf(key)];
    if (player)
        fn_7100070e64(player, flag);
}
