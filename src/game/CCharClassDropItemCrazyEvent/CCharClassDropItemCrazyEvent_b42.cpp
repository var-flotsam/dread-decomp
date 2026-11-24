// CCharClassDropItemCrazyEvent's unit (tools/tu_map.py): its constructor, on the event base
// (fn_7100067ea4): the vtable, a cleared flag, a cleared block, a copy of the empty string id
// (base::global::CStrId::Empty()), another cleared block, a second copy of the empty id and a cleared word. Names
// and the layout are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101aa5800[];

struct UnkDropItemCrazyEvent_b42a3 {
    void* vtable;
    u8 unk_0x8[0x4];
    bool bForce;
    u64 field_0x10[9];
    CStrId id_0x58;
    u64 field_0x60[3];
    CStrId id_0x78;
    u32 field_0x80;
};

void fn_7100cb5358(UnkDropItemCrazyEvent_b42a3* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa5800 + 0x10;
    self->bForce = false;
    __builtin_memset(self->field_0x10, 0, sizeof(self->field_0x10));
    new (&self->id_0x58) CStrId(base::global::CStrId::Empty());
    __builtin_memset(self->field_0x60, 0, sizeof(self->field_0x60));
    new (&self->id_0x78) CStrId(base::global::CStrId::Empty());
    self->field_0x80 = 0;
}
