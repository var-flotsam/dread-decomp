// CCharClassAINavigationComponent's unit (tools/tu_map.py): the constructor (fn_71006f7a78). Names and
// layouts are placeholders.
#include "types.h"

struct UnkStruct_71008bca30;
void fn_71011acd10(UnkStruct_71008bca30*);
struct UnkNode;
void fn_7100023854(UnkNode*);
extern void* g_unk_7101a6b7a0[];

struct UnkAINavigationComponent_ovb0043 {
    void** vtable;
    u8 pad_0x8[0x10];
    u64 field_0x18;
    u8 pad_0x20[0x38];
    u32 field_0x58;
    f32 field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    f32 field_0x6c;
    f32 field_0x70;
    bool field_0x74;
    u8 pad_0x75[3];
    f32 field_0x78;
};

// The constructor: the base (fn_71011acd10), the vtable and the tunable defaults, then fn_7100023854.
void fn_71006f7a78(UnkAINavigationComponent_ovb0043* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = g_unk_7101a6b7a0 + 2;
    self->field_0x58 = 0;
    self->field_0x5c = 150.0f;
    self->field_0x60 = 1.0f;
    self->field_0x64 = 25.0f;
    self->field_0x68 = 120.0f;
    self->field_0x6c = 3.0f;
    self->field_0x70 = 0.5f;
    self->field_0x74 = false;
    self->field_0x78 = 10.0f;
    self->field_0x18 = 0x109a;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
