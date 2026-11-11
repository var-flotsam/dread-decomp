// The complete-object destructor beside fn_71006bd00c (same unnamed unit, 0x71006bcf84-0x71006bd63c):
// restore the vtable, destroy the member at +0x10 (fn_710059c7b8) and drop the counted reference at
// +0x8. Names and layouts are placeholders.
#include "game/UnkTU1055.h"
#include "game/UnkTU852.h"
#include "game_s2/UnkWeakHandle.h"

struct UnkObject_71006bcfb0 {
    void** vtable;
    UnkRefObject* field_0x8;
    u8 field_0x10[0x10];
};

void fn_71006bcfb0(UnkObject_71006bcfb0* self) {
    self->vtable = g_unk_7101a64388 + 2;
    fn_710059c7b8(self->field_0x10);
    if (UnkRefObject* p = self->field_0x8) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
}
