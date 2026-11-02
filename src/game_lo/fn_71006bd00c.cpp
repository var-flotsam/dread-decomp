// The deleting destructor of a class in an unnamed unit (0x71006bcf84-0x71006bd63c): restore the
// vtable, destroy the member at +0x10 (fn_710059c7b8), drop the counted reference at +0x8 and free
// the object. Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"

void fn_710059c7b8(void* self);
extern void* g_unk_7101a64388[];

struct UnkObject_71006bd00c {
    void** vtable;
    UnkRefObject* field_0x8;
    u8 field_0x10[0x10];
};

void fn_71006bd00c(UnkObject_71006bd00c* self) {
    self->vtable = g_unk_7101a64388 + 2;
    fn_710059c7b8(self->field_0x10);
    if (UnkRefObject* p = self->field_0x8) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
    operator delete(self);
}
