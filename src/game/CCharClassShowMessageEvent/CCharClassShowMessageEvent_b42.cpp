// CCharClassShowMessageEvent's unit (tools/tu_map.py): its constructor, on the event base
// (fn_7100067ea4): the vtable, two copies of the empty string id (g_unk_7101d470e0) and a set flag.
// Names and the layout are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101aa1d58[];

struct UnkShowMessageEvent_b42a3 {
    void* vtable;
    u8 unk_0x8[0x8];
    CStrId id_0x10;
    CStrId id_0x18;
    bool field_0x20;
};

void fn_7100be0368(UnkShowMessageEvent_b42a3* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa1d58 + 0x10;
    new (&self->id_0x10) CStrId(g_unk_7101d470e0);
    new (&self->id_0x18) CStrId(g_unk_7101d470e0);
    self->field_0x20 = true;
}
