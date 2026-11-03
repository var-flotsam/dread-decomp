// CArenaManager::CTunableArenaManager's unit (tools/tu_map.py): passes each 0x80-byte entry of the
// array at +0x8 (count at +0x10) to fn_7100e14908 with the object at +0x0 and the game manager's
// member at +0x968. Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkArenaEntry2_b46a3 {
    u8 data[0x80];
};

void fn_7100e14908(UnkArenaEntry2_b46a3* entry, void* owner, void* context);

struct UnkArenaEntries2_b46a3 {
    void* owner;
    UnkArenaEntry2_b46a3* data;  // 0x8
    u32 count;                   // 0x10
};

void fn_7100e148a0(UnkArenaEntries2_b46a3* self) {
    if (!self->count)
        return;
    void* context = reinterpret_cast<char*>(g_unk_7101d47310) + 0x968;
    for (UnkArenaEntry2_b46a3* e = self->data; e != self->data + self->count; ++e)
        fn_7100e14908(e, self->owner, context);
}
