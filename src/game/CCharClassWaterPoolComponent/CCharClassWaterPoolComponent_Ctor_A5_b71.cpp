// CCharClassWaterPoolComponent (the name from reflection): the in-place constructor its reflection descriptor
// points to (fn_0x28). The object is value-initialised (cleared), then the char class base constructor
// (fn_71008fd5b0) runs; it sets the vtable, an empty file-path id at +0x80 and cleared fields after it, then makes
// the post-construction virtual call (slot 3). Defined under the `void()` symbol
// src/reflection/CCharClassWaterPoolComponent.cpp declares. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "types.h"

struct UnkCharClassBase_A5_b71;
void fn_71008fd5b0(UnkCharClassBase_A5_b71* self);
extern void* g_unk_7101a811a8[];

struct UnkCharClassWaterPoolCtor_A5_b71 {
    void** vtable;
    u8 pad_0x8[0x78];
    base::global::UnkStringData* path;  // 0x80
    u64 field_0x88;
    u64 field_0x90;
    u32 field_0x98;
};
static_assert(sizeof(UnkCharClassWaterPoolCtor_A5_b71) == 0xa0, "layout");

void fn_710090bba4_A5_b71(UnkCharClassWaterPoolCtor_A5_b71* self) asm("_Z13fn_710090bba4v");
void fn_710090bba4_A5_b71(UnkCharClassWaterPoolCtor_A5_b71* self) {
    __builtin_memset(self, 0, sizeof(*self));
    fn_71008fd5b0(reinterpret_cast<UnkCharClassBase_A5_b71*>(self));
    self->vtable = &g_unk_7101a811a8[2];
    self->path = nullptr;
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    self->path = &g_unk_7101d47200->empty;
    self->field_0x88 = 0;
    self->field_0x90 = 0;
    self->field_0x98 = 0;
    reinterpret_cast<void (*)(UnkCharClassWaterPoolCtor_A5_b71*)>(self->vtable[3])(self);
}
