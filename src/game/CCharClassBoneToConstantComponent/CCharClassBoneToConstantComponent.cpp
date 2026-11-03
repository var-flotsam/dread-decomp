// CCharClassBoneToConstantComponent: its constructor (fn_71008053d8). The base constructor
// fn_71011acd10, the vtable, two invalid (all-ones) words at +0x58, a cleared word at +0x1c, then the
// class-entry pass of base::core::CBaseObject (fn_7100023854). Names and layouts are placeholders.
#include "types.h"

struct UnkNode;
struct UnkStruct_71008bca30;
void fn_71011acd10(UnkStruct_71008bca30* self);
void fn_7100023854(UnkNode* node);
extern void* g_unk_7101a74a48[];

struct UnkBoneToConstantComponent {
    void** vtable;
    u8 pad_0x8[0x14];
    u32 field_0x1c;
    u8 pad_0x20[0x38];
    u64 field_0x58;
    u64 field_0x60;
};

void fn_71008053d8(UnkBoneToConstantComponent* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = g_unk_7101a74a48 + 2;
    self->field_0x58 = ~0ull;
    self->field_0x60 = ~0ull;
    self->field_0x1c = 0;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
