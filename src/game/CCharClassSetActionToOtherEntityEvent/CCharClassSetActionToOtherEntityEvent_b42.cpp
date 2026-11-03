// CCharClassSetActionToOtherEntityEvent's unit (tools/tu_map.py): its constructor, on the event base
// (fn_7100067ea4): the vtable, a copy of the empty string id (g_unk_7101d470e0), a set flag, a cleared
// word, another copy of the empty id, a cleared flag and a float of 1. Names and the layout are
// placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101aa1918[];
void fn_7100067ea4(void* self);  // the event base constructor

struct UnkSetActionToOtherEntityEvent_b42a3 {
    void* vtable;
    u8 unk_0x8[0x8];
    CStrId id_0x10;
    bool field_0x18;
    u32 field_0x1c;
    CStrId id_0x20;
    bool field_0x28;
    f32 field_0x2c;
};

void fn_7100bd009c(UnkSetActionToOtherEntityEvent_b42a3* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa1918 + 0x10;
    new (&self->id_0x10) CStrId(g_unk_7101d470e0);
    self->field_0x18 = true;
    self->field_0x1c = 0;
    new (&self->id_0x20) CStrId(g_unk_7101d470e0);
    self->field_0x28 = false;
    self->field_0x2c = 1.0f;
}
