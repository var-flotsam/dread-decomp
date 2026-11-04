// CCutsceneComponent's unit: reflected collection erase for a base::UnkVector of 0x88-byte polymorphic entries
// (destructor in vtable slot 1): erase the entry an index iterator names, inlined: destroy it, then move each later
// entry down one (fn_71007abba4) and destroy the moved-from one. The index goes through a byte offset from the
// vector itself (build/b67/b67a6.md). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkCutsceneEntry_A5_b68 {
    virtual void vf_0x0();
    virtual ~UnkCutsceneEntry_A5_b68();
    u8 pad_0x8[0x80];
};

void fn_71007abba4(UnkCutsceneEntry_A5_b68* dst, UnkCutsceneEntry_A5_b68* src);

static inline __attribute__((always_inline)) void EraseAt_A5_b68(base::UnkVector<UnkCutsceneEntry_A5_b68>* self,
                                                                 u32 index) {
    if (index >= self->count)
        return;
    self->data[index].~UnkCutsceneEntry_A5_b68();
    for (u64 i = index; i < self->count - 1; ++i) {
        UnkCutsceneEntry_A5_b68* src = &self->data[i] + 1;
        fn_71007abba4(src - 1, src);
        (&self->data[i] + 1)->~UnkCutsceneEntry_A5_b68();
    }
    self->count = self->count - 1;
}

void fn_71007ac148(base::UnkVector<UnkCutsceneEntry_A5_b68>* self, const u32* it) {
    UnkCutsceneEntry_A5_b68* base = reinterpret_cast<UnkCutsceneEntry_A5_b68*>(self);
    u64 offset = (u64)*it * sizeof(UnkCutsceneEntry_A5_b68);
    UnkCutsceneEntry_A5_b68* p = reinterpret_cast<UnkCutsceneEntry_A5_b68*>(reinterpret_cast<char*>(base) + offset);
    EraseAt_A5_b68(self, (u32)(p - base));
}
