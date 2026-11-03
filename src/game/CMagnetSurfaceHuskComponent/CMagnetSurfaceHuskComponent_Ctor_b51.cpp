// Unit 1415 (tools/tu_map.py hint CCharClassMagnetSurfaceHuskComponent): a constructor. The base
// constructor (fn_71011acd10), the vtable, then two empty string ids at +0x58 and +0x60 (cleared,
// then pointed at the pool's empty string, creating the pool first when there is none; the shape
// of CNailuggerAcidBallMovementComponent_b40.cpp). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkStruct_71008bca30;
void fn_71011acd10(UnkStruct_71008bca30*);
extern void* g_unk_7101a83028[];

struct UnkMagnetSurfaceHusk {
    void** vtable;
    u8 pad_0x8[0x50];
    base::global::UnkStringData* field_0x58;
    base::global::UnkStringData* field_0x60;
};

static inline __attribute__((always_inline)) void InitEmpty_msh(base::global::UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_7100923d60(UnkMagnetSurfaceHusk* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = g_unk_7101a83028 + 2;
    InitEmpty_msh(&self->field_0x58);
    InitEmpty_msh(&self->field_0x60);
}
