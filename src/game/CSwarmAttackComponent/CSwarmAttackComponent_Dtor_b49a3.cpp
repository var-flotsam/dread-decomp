// CSwarmAttackComponent unit (tools/tu_map.py unit 1162). fn_71007462fc: the complete-object
// destructor beside the deleting one (fn_7100746368): both vtables (+0x0 and +0x40), the counted
// reference at +0x140 dropped, then the base destructor fn_7101135ff0. Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a6da50[];

struct UnkSwarmAttackComponent_b49a3 {
    void** vtable;
    u8 pad_0x8[0x38];
    void** vtable_0x40;
    u8 pad_0x48[0xf8];
    UnkRefObject* field_0x140;
};

void fn_7101135ff0(UnkSwarmAttackComponent_b49a3* self);

void fn_71007462fc(UnkSwarmAttackComponent_b49a3* self) {
    self->vtable = g_unk_7101a6da50 + 2;
    self->vtable_0x40 = g_unk_7101a6da50 + 0x3e;
    if (UnkRefObject* p = self->field_0x140) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
    fn_7101135ff0(self);
}
