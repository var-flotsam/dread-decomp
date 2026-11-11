// CRockDiverSpawnPointComponent: its constructor (fn_7100a1b6b4). The spawn point base constructor
// fn_71011258e4, the vtable and cleared fields from +0x3b8, then the class-entry pass of
// base::core::CBaseObject (fn_7100023854). Names and layouts are placeholders.
#include "base/core/CBaseObject.h"
#include "game/UnkTU2704.h"
#include "types.h"

struct UnkNode;
extern void* g_unk_7101a8ca70[];

struct UnkRockDiverSpawnPoint {
    void** vtable;
    u8 pad_0x8[0x3b0];
    bool field_0x3b8;
    u8 pad_0x3b9[3];
    u32 field_0x3bc;
    u8 pad_0x3c0[4];
    u32 field_0x3c4[2];
    u32 field_0x3cc;
};

void fn_7100a1b6b4(UnkRockDiverSpawnPoint* self) {
    fn_71011258e4(self);
    self->field_0x3b8 = false;
    self->vtable = g_unk_7101a8ca70 + 2;
    self->field_0x3bc = 0;
    self->field_0x3c4[0] = 0;
    self->field_0x3c4[1] = 0;
    self->field_0x3cc = 0;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
