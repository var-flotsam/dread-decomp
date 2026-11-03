// CCharClassWeightActivablePropComponent: its constructor. The base constructor fn_71011acd10, the vtable,
// a set flag at +0x58 and 0xc1c at +0x18, then the class-entry pass of base::core::CBaseObject
// (fn_7100023854). Names and layouts are placeholders.
#include "types.h"
struct UnkNode;
struct UnkStruct_71008bca30;
void fn_71011acd10(UnkStruct_71008bca30* self);
void fn_7100023854(UnkNode* node);
extern void* g_unk_7101a97168[];
struct UnkWeightActivablePropComponent {
    void** vtable;
    u8 pad_0x8[0x10];
    u32 field_0x18;
    u8 pad_0x1c[0x3c];
    bool field_0x58;
};
void fn_7100ac8ca0(UnkWeightActivablePropComponent* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = g_unk_7101a97168 + 2;
    self->field_0x58 = true;
    self->field_0x18 = 0xc1c;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
