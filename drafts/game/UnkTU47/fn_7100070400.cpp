// Unit 47 (tools/tu_map.py): the rumble manager's table of players. Calls fn_7100070f64 on the player
// registered under `key`, if there is one. Names and layouts are placeholders.
#include "types.h"

struct UnkRumblePlayer_l070400;
void fn_7100070f64(UnkRumblePlayer_l070400* player);

struct UnkRumbleTable_l070400 {
    u8 pad_0x0[0x20a0];
    UnkRumblePlayer_l070400** values;  // 0x20a0
    u64* keys;                         // 0x20a8
    u32 count;                         // 0x20b0

    UnkRumblePlayer_l070400** End() { return &values[count]; }
    u32 IndexOf(u64 key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return i;
        return count;
    }
    UnkRumblePlayer_l070400** Find(u64 key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
    UnkRumblePlayer_l070400* operator[](u64 key) { return values[IndexOf(key)]; }
};

void fn_7100070400(UnkRumbleTable_l070400* self, u64 key) {
    if (self->Find(key) == self->End())
        return;
    if (UnkRumblePlayer_l070400* player = (*self)[key])
        fn_7100070f64(player);
}
