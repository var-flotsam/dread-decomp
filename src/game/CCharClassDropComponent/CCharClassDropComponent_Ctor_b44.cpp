// CCharClassDropComponent (tools/tu_map.py; the name from reflection): the constructor. The base
// constructor fn_71011acd10, the vtable, flags and values (0.1 at +0x74), the string id "ALL" at
// +0x80, two drop tables at +0x88 and +0x118 (fn_71007c7ddc), -1 at +0x1c, then the vtable's slot
// at +0x18 on itself. Names and layout are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/CCharClassComponent.h"
#include "game/SDropProbabilities.h"
#include "types.h"

struct UnkStruct_71008bca30;
extern void* g_unk_7101a71518[];  // the class's vtable

typedef u64 UnkU64Align4_b44a2 __attribute__((aligned(4)));

struct UnkDropDef_b44a2 {
    void** vtable;
    u8 pad_0x8[0x10];
    u32 field_0x18;
    s32 field_0x1c;
    u8 pad_0x20[0x38];
    u64 field_0x58;
    u64 field_0x60;
    bool field_0x68;
    u8 pad_0x69[3];
    UnkU64Align4_b44a2 field_0x6c;
    f32 field_0x74;
    f32 field_0x78;
    u32 pad_0x7c;
    base::global::CStrId field_0x80;
    u8 field_0x88[0x90];
    u8 field_0x118[0x90];
    bool field_0x1a8;
};

void fn_71007bf01c(UnkDropDef_b44a2* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = g_unk_7101a71518 + 2;
    self->field_0x58 = 0x1000000;
    self->field_0x60 = 0;
    self->field_0x68 = true;
    self->field_0x6c = 0;
    self->field_0x74 = 0.1f;
    self->field_0x78 = 0.0f;
    new (&self->field_0x80) base::global::CStrId("ALL", true);
    fn_71007c7ddc(self->field_0x88);
    fn_71007c7ddc(self->field_0x118);
    self->field_0x18 = 0;
    self->field_0x1c = -1;
    self->field_0x1a8 = false;
    reinterpret_cast<void (*)(UnkDropDef_b44a2*)>(self->vtable[3])(self);
}
