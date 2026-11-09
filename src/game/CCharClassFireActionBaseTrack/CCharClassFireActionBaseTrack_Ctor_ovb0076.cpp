// CCharClassFireActionBaseTrack's unit (tools/tu_map.py): a constructor on the middle class
// (fn_7100d2c4e0) whose class vtable is 0x7101aa6cd8, with four string ids from 0x38 copied from
// the global empty id and a cleared byte at 0x58. Names and layouts are placeholders.
#include <new>
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101aa6cd8[];

struct UnkFireActionBaseTrack_ovb0076 {
    void** vtable;
    u8 unk_0x8[0x30];
    CStrId ids[4];
    bool field_0x58;
};

struct UnkClass;
void fn_7100d2c4e0(UnkFireActionBaseTrack_ovb0076* self, UnkClass* cls);

void fn_7100cf68f4(UnkFireActionBaseTrack_ovb0076* self, UnkClass* cls) {
    fn_7100d2c4e0(self, cls);
    self->vtable = g_unk_7101aa6cd8 + 2;
    new (&self->ids[0]) CStrId(g_unk_7101d470e0);
    new (&self->ids[1]) CStrId(g_unk_7101d470e0);
    new (&self->ids[2]) CStrId(g_unk_7101d470e0);
    new (&self->ids[3]) CStrId(g_unk_7101d470e0);
    self->field_0x58 = false;
}
