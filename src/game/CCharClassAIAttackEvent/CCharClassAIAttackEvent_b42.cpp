// CCharClassAIAttackEvent's unit (tools/tu_map.py): its constructor, on the event base
// (fn_7100067ea4): the vtable, a string id built from "", a float of 0.5, two set flags and a cleared
// one, cleared words, a pair (0, -1), a word -1 and a cleared flag. Names and the layout are
// placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101aa5500[];

struct UnkAIAttackEvent_b42a3 {
    void* vtable;
    u8 unk_0x8[0x8];
    CStrId id_0x10;
    f32 field_0x18;
    bool field_0x1c;
    bool field_0x1d;
    bool field_0x1e;
    u64 field_0x20;
    u64 field_0x28;
    u64 field_0x30;
    s64 field_0x38;
    s32 field_0x40;
    bool field_0x44;
};

void fn_7100cac8b0(UnkAIAttackEvent_b42a3* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa5500 + 0x10;
    new (&self->id_0x10) CStrId("", true);
    self->field_0x18 = 0.5f;
    self->field_0x1c = true;
    self->field_0x1d = true;
    self->field_0x1e = false;
    self->field_0x20 = 0;
    self->field_0x28 = 0;
    self->field_0x30 = 0;
    self->field_0x38 = -1;
    self->field_0x40 = -1;
    self->field_0x44 = false;
}
