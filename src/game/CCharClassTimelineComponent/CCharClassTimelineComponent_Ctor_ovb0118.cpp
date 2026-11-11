// CCharClassTimelineComponent's unit (tools/tu_map.py): the constructor (fn_7100797234): the base
// constructor fn_71011acd10, the vtable, a cleared word at +0x58, an empty id at +0x60 and the word
// after it, then the enum default zero (a guarded static) at +0x70. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CCharClassComponent.h"
#include "types.h"


struct UnkStruct_71008bca30;
extern void* g_unk_7101a6f8a0[];  // the class's vtable

struct UnkCharClassTimeline_ovb0118 {
    void** vtable;
    u8 pad_0x8[0x50];
    u32 field_0x58;
    base::global::UnkStringData* id;  // 0x60: an empty string id
    u64 field_0x68;
    u32 field_0x70;
};

void fn_7100797234(UnkCharClassTimeline_ovb0118* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = g_unk_7101a6f8a0 + 2;
    self->field_0x58 = 0;
    self->id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->id = &g_unk_7101d47108->empty;
    self->field_0x68 = 0;
    self->field_0x70 = base::reflection::refl_GetEnumZero<0x7101d57cc8>();
}
