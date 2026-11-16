// Unit 47 (tools/tu_map.py): the rumble manager's table of players. Stops the player given, if the
// table holds a non-null player under it, marks it (+0x9e), and removes its entry by moving the last
// entry into its place. Names and layouts are placeholders.
#include "types.h"

struct UnkRumblePlayer_l06f914 {
    u8 pad_0x0[0x9e];
    u8 field_0x9e;
};

struct UnkRumbleReset_b67a1;
void fn_7100070dd4(UnkRumbleReset_b67a1* self);

// A slot of the table's two parallel arrays (copied whole when an entry moves).
struct UnkRumbleSlot_l06f914 {
    UnkRumblePlayer_l06f914* player;
};
struct UnkRumbleKey_l06f914 {
    u64 key;
};

struct UnkRumbleTable_l06f914 {
    u8 pad_0x0[0x20a0];
    UnkRumbleSlot_l06f914* values;  // 0x20a0
    UnkRumbleKey_l06f914* keys;     // 0x20a8
    u32 count;                      // 0x20b0

    UnkRumbleSlot_l06f914* End() { return &values[count]; }
    UnkRumbleSlot_l06f914* Find(u64 key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i].key == key)
                return &values[i];
        return End();
    }
    bool Has(u64 key) { return Find(key) != End(); }
    UnkRumblePlayer_l06f914* At(u64 key) {
        u64 i = 0;
        while (i < count && keys[i].key != key)
            ++i;
        return values[i].player;
    }
    // Removes the entry at `it` by moving the last entry into its place; the table copies its
    // elements bitwise, as the engine's arrays do, so the count is read again after each copy.
    void Erase(UnkRumbleSlot_l06f914* it) {
        u32 index = static_cast<u32>(it - values);
        if (index != count - 1) {
            __builtin_memcpy(&values[index], &values[count - 1], sizeof(UnkRumbleSlot_l06f914));
            __builtin_memcpy(&keys[index], &keys[count - 1], sizeof(UnkRumbleKey_l06f914));
        }
        count = count - 1;
    }
};

void fn_710006f914(UnkRumbleTable_l06f914* self, UnkRumblePlayer_l06f914* player) {
    u64 key = reinterpret_cast<u64>(player);
    if (!self->Has(key))
        return;
    UnkRumblePlayer_l06f914* entry = self->At(key);
    if (!entry)
        return;
    fn_7100070dd4(reinterpret_cast<UnkRumbleReset_b67a1*>(entry));
    player->field_0x9e = 1;
    UnkRumbleSlot_l06f914* it = self->Find(key);
    if (it == self->End())
        return;
    self->Erase(it);
}
