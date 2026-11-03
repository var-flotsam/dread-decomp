// CCharClassDropItemOverwriteProbabilitiesEvent's unit (tools/tu_map.py): its constructor, on the
// event base (fn_7100067ea4): the vtable, a cleared block, a copy of the empty string id
// (g_unk_7101d470e0), another cleared block, a second copy, the pair (0, 1), a third copy and an
// empty base::UnkVector (allocator flags 0xf). Names and the layout are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101aa5860[];
void fn_7100067ea4(void* self);  // the event base constructor

struct UnkDropItemOverwriteEntry_b42a3;

struct UnkDropItemOverwriteProbabilitiesEvent_b42a3 {
    void* vtable;
    u8 unk_0x8[0x4];
    u32 field_0xc[18];
    CStrId id_0x58;
    u64 field_0x60[3];
    CStrId id_0x78;
    u32 field_0x80;
    u32 field_0x84;
    CStrId id_0x88;
    base::UnkVector<UnkDropItemOverwriteEntry_b42a3*> items;
};

void fn_7100cb81b4(UnkDropItemOverwriteProbabilitiesEvent_b42a3* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa5860 + 0x10;
    __builtin_memset(self->field_0xc, 0, sizeof(self->field_0xc));
    new (&self->id_0x58) CStrId(g_unk_7101d470e0);
    __builtin_memset(self->field_0x60, 0, sizeof(self->field_0x60));
    new (&self->id_0x78) CStrId(g_unk_7101d470e0);
    self->field_0x80 = 0;
    self->field_0x84 = 1;
    new (&self->id_0x88) CStrId(g_unk_7101d470e0);
    new (&self->items) base::UnkVector<UnkDropItemOverwriteEntry_b42a3*>(0xf);
}
