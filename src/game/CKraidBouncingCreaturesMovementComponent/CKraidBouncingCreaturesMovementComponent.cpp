// CKraidBouncingCreaturesMovementComponent: its constructor (fn_7100605fe8). The base constructor
// fn_7100ac2c5c, the vtable, a cleared flag at +0x571, the floats -1 and 1 at +0x5d8, cleared fields to
// +0x600, then the class-entry pass of base::core::CBaseObject (fn_7100023854). Names and layouts are
// placeholders.
#include "types.h"

struct UnkNode;
void fn_7100ac2c5c(void* self);
void fn_7100023854(UnkNode* node);
extern void* g_unk_7101a55168[];

struct UnkKraidBouncingCreaturesMovement {
    void** vtable;
    u8 pad_0x8[0x569];
    bool field_0x571;
    u8 pad_0x572[0x66];
    f32 field_0x5d8;
    f32 field_0x5dc;
    u32 field_0x5e0;
    bool field_0x5e4;
    u8 pad_0x5e5[3];
    u64 field_0x5e8;
    u64 field_0x5f0;
    u64 field_0x5f8;
};

void fn_7100605fe8(UnkKraidBouncingCreaturesMovement* self) {
    fn_7100ac2c5c(self);
    self->field_0x5e0 = 0;
    self->vtable = g_unk_7101a55168 + 2;
    self->field_0x5d8 = -1.0f;
    self->field_0x5dc = 1.0f;
    self->field_0x5e4 = false;
    self->field_0x571 = false;
    self->field_0x5e8 = 0;
    self->field_0x5f0 = 0;
    self->field_0x5f8 = 0;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
