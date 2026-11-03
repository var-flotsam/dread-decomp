// Unit 1476 (tools/tu_map.py hint CCharClassChangeStageNavMeshItemComponent): a constructor, a copy
// of fn_7100923d60 (CMagnetSurfaceHuskComponent_Ctor_b51.cpp) with its own vtable. The base
// constructor (fn_71011acd10), the vtable, then two empty string ids at +0x58 and +0x60. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkStruct_71008bca30;
void fn_71011acd10(UnkStruct_71008bca30*);
extern char g_unk_7101a884a0[];

struct UnkChangeStageNavMeshItem {
    void** vtable;
    u8 pad_0x8[0x50];
    base::global::UnkStringData* field_0x58;
    base::global::UnkStringData* field_0x60;
};

static inline __attribute__((always_inline)) void InitEmpty_csnm(base::global::UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_71009bafdc(UnkChangeStageNavMeshItem* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = reinterpret_cast<void**>(g_unk_7101a884a0 + 0x10);
    InitEmpty_csnm(&self->field_0x58);
    InitEmpty_csnm(&self->field_0x60);
}
