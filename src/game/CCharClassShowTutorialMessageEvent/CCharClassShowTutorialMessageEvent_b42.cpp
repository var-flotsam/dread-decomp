// CCharClassShowTutorialMessageEvent's unit (tools/tu_map.py): its value-initialising constructor
// (the object cleared first, as `new (p) T()` does for a class without a user-provided constructor),
// on the event base (fn_7100067ea4): the vtable, a copy of the empty string id (g_unk_7101d470e0)
// and an id set to the pool's empty entry (the pool created first when there is none). Names and the layout are
// placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101aa1d88[];
void fn_7100067ea4(void* self);  // the event base constructor

struct UnkShowTutorialMessageEvent_b42a3 {
    void* vtable;
    u8 unk_0x8[0x8];
    CStrId id_0x10;
    CStrId id_0x18;
};

void fn_7100be1054(UnkShowTutorialMessageEvent_b42a3* self) {
    __builtin_memset(self, 0, sizeof(*self));
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa1d88 + 0x10;
    new (&self->id_0x10) CStrId(g_unk_7101d470e0);
    self->id_0x18.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->id_0x18.data = &g_unk_7101d47108->empty;
}
