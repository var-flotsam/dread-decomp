// Unit 1436 of tools/tu_map.py (no name hint): a constructor. The base constructor (fn_71009545b8,
// passed the second argument), a cleared word at +0x40, the vtable, then two empty string ids at
// +0x48 and +0x50 (the shape of CNailuggerAcidBallMovementComponent_b40.cpp). Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkUnit1436;
void fn_71009545b8(UnkUnit1436* self, void* arg);
extern void* g_unk_7101a84eb0[];

struct UnkUnit1436 {
    void** vtable;
    u8 pad_0x8[0x38];
    u64 field_0x40;
    base::global::UnkStringData* field_0x48;
    base::global::UnkStringData* field_0x50;
};

static inline __attribute__((always_inline)) void InitEmpty_1436(base::global::UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_710095520c(UnkUnit1436* self, void* arg) {
    fn_71009545b8(self, arg);
    self->field_0x40 = 0;
    self->vtable = g_unk_7101a84eb0 + 2;
    InitEmpty_1436(&self->field_0x48);
    InitEmpty_1436(&self->field_0x50);
}
