// CChozoZombieXSpawnPointComponent (unit 1528): the reset after the base step. Its three render items
// go back to the render manager, and the values it changed in the linked table (+0x3b8) are put back
// from the saved (index, value) pairs, unless the table is idle. Names and layouts are placeholders.
// build-flags: no-strict-aliasing
#include "game/UnkRenderManager.h"
#include "types.h"

struct UnkSpawnRenderObject_la18e98 {
    u8 pad_0x0[0x8];
    u8 item[0x8];  // 0x8
};

struct UnkSpawnTableEntry_la18e98 {
    u8 pad_0x0[0x8];
    u32 value;  // 0x8
};

struct UnkSpawnTable_la18e98 {
    UnkSpawnTableEntry_la18e98* entries;
    u32 state;  // 0x8
    u8 pad_0xc[0x20];
    bool paused;  // 0x2c

    bool IsRunning() const { return !paused && state != 0; }
};

struct UnkSpawnSavedValue_la18e98 {
    u32 index;
    u32 value;
};

struct UnkSpawnPoint_la18e98 {
    u8 pad_0x0[0x3b8];
    UnkSpawnTable_la18e98* table;  // 0x3b8
    UnkSpawnRenderObject_la18e98* objects[3];  // 0x3c0
    UnkSpawnSavedValue_la18e98* saved;  // 0x3d8
    u32 saved_count;  // 0x3e0
    u8 pad_0x3e4[0x1c];
    bool field_0x400;
};

void fn_710112b968(UnkSpawnPoint_la18e98* self);

static inline void Release(UnkSpawnRenderObject_la18e98* object) {
    if (object)
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(object->item), 0);
}

void fn_7100a18e98(UnkSpawnPoint_la18e98* self) {
    fn_710112b968(self);
    Release(self->objects[0]);
    Release(self->objects[1]);
    Release(self->objects[2]);
    self->objects[0] = nullptr;
    self->objects[1] = nullptr;
    self->objects[2] = nullptr;
    UnkSpawnTable_la18e98* table = self->table;
    if (table->state != table->IsRunning()) {
        UnkSpawnSavedValue_la18e98* end = self->saved + self->saved_count;
        for (UnkSpawnSavedValue_la18e98* it = self->saved; it != end; ++it)
            self->table->entries[it->index].value = it->value;
    }
    self->saved_count = 0;
    self->table = nullptr;
    self->field_0x400 = false;
}
