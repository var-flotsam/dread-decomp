// The constructors of CCharClassFXSetMaterialPropertyEvent's and
// CCharClassFXSetMaterialPropertyTransitionEvent's units (the same shape as fn_7100136ecc,
// src/base/tunable/CTunable/fn_7100138090.cpp): the base constructor (fn_7100067ea4), the vtable,
// two string ids built from "" and the value fields, with a float of 1 at +0x38 or +0x3c. Names
// and the layouts are placeholders.
#include "types.h"

void fn_71000003d4(void* self, const char* s, bool flag);

struct UnkFXMaterialEvent {
    void** vtable;
    u8 unk_0x8[0x8];
    u64 id_0x10;
    u64 id_0x18;
    u32 field_0x20;
    u32 field_0x24;
    u64 field_0x28;
    u64 field_0x30;
    f32 field_0x38;
    f32 field_0x3c;
};

void fn_7100067ea4(void* self);  // the base constructor
extern void* g_unk_7101aa5b60[];
extern void* g_unk_7101aa5b90[];

__attribute__((always_inline)) inline void InitFXMaterialEvent(UnkFXMaterialEvent* self, void** vtable) {
    fn_7100067ea4(self);
    self->vtable = vtable + 2;
    fn_71000003d4(&self->id_0x10, "", true);
    fn_71000003d4(&self->id_0x18, "", true);
    self->field_0x20 = 0;
    self->field_0x24 = 1;
    self->field_0x28 = 0;
    self->field_0x30 = 0;
}

void fn_7100cc06e0(UnkFXMaterialEvent* self) {
    InitFXMaterialEvent(self, g_unk_7101aa5b60);
    self->field_0x38 = 1.0f;
}

void fn_7100cc1c80(UnkFXMaterialEvent* self) {
    InitFXMaterialEvent(self, g_unk_7101aa5b90);
    self->field_0x3c = 1.0f;
}
