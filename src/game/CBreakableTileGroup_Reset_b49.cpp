// CBreakableTileGroup's unit (tools/tu_map.py, the name from reflection): releases the group's data:
// the entry at +0xd0 is handed back to the collision manager (created on first use; fn_7100d46c60), the
// tile array at +0x48 (0xa8-byte tiles, destroyed by fn_7100c9dd20) and the arrays at +0x58 and +0x50
// are deleted, and the counters are cleared. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU365.h"
#include "types.h"

// The collision manager (as src/game/CCharacterMovement_2.cpp declares it).
struct UnkManager_7100863a8c;
void fn_7100d46804(UnkManager_7100863a8c* self);
struct UnkCollisionObject;
void fn_7100d46c60(UnkManager_7100863a8c* self, UnkCollisionObject* entry);

struct UnkBreakableTile_b49 {
    u8 data[0xa8];
    ~UnkBreakableTile_b49() asm("fn_7100c9dd20");
};

struct UnkBreakableTileGroupData {
    u8 pad_0x0[0x14];
    bool field_0x14;
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u8 pad_0x2c[0x1c];
    UnkBreakableTile_b49* tiles;  // 0x48
    u8* field_0x50;
    u8* field_0x58;
    u8 pad_0x60[0x70];
    void* entry;  // 0xd0
};
static_assert(__builtin_offsetof(UnkBreakableTileGroupData, entry) == 0xd0, "layout");

static inline UnkManager_7100863a8c* Manager() {
    UnkManager_7100863a8c* m = g_unk_71726a4788;
    if (!m) {
        m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return m;
}

void fn_7100ca07a8(UnkBreakableTileGroupData* self) {
    if (self->entry) {
        fn_7100d46c60(Manager(), static_cast<UnkCollisionObject*>(self->entry));
        self->entry = nullptr;
    }
    if (self->tiles) {
        delete[] self->tiles;
        self->tiles = nullptr;
    }
    if (self->field_0x58) {
        delete[] self->field_0x58;
        self->field_0x58 = nullptr;
    }
    if (self->field_0x50) {
        delete[] self->field_0x50;
        self->field_0x50 = nullptr;
    }
    self->field_0x14 = false;
    self->field_0x28 = 0;
    self->field_0x18 = 0;
    self->field_0x20 = 0;
}
