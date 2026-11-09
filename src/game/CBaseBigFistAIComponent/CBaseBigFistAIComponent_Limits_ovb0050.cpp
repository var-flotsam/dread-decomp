// CBaseBigFistAIComponent unit (tools/tu_map.py unit 591): after the base step (fn_71003acb5c), asks
// fn_71010fc864 for the limits of kind 1. An owner whose x lies outside them is put back inside, a
// radius (+0x37c) away from the nearer limit, its flag at +0x3b0 set, and turned to face away from that
// limit (a y angle of +/- pi/2); then the flag at +0x3c0 is set. Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkCoreXAI_A2_b73;
void fn_71003acb5c(UnkCoreXAI_A2_b73*);

// A point of the limits fn_71010fc864 reports (zero unless set).
struct UnkBigFistLimit_ovb0050 {
    f32 x, y, z;
    UnkBigFistLimit_ovb0050() : x(0.0f), y(0.0f), z(0.0f) {}
};

// The kind of limits asked for (an 8-byte slot).
struct UnkBigFistKind_ovb0050 {
    u32 value;
    u32 unk_0x4;
};

struct UnkBigFistActor_ovb0050 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x9c[0x37c - 0x38 - sizeof(UnkXform)];
    f32 radius;  // 0x37c
    u8 pad_0x380[0x30];
    u8 field_0x3b0;  // a flag
    u8 pad_0x3b1[0xf];
    bool field_0x3c0;
};

struct UnkBigFistAI3_ovb0050 {
    u8 pad_0x0[0x30];
    UnkBigFistActor_ovb0050* owner;  // 0x30
};

bool fn_71010fc864(UnkBigFistAI3_ovb0050* self, UnkBigFistKind_ovb0050* kind, UnkBigFistLimit_ovb0050* min, UnkBigFistLimit_ovb0050* max);

void fn_71003e5d80(UnkBigFistAI3_ovb0050* self) {
    fn_71003acb5c(reinterpret_cast<UnkCoreXAI_A2_b73*>(self));
    UnkBigFistLimit_ovb0050 min;
    UnkBigFistLimit_ovb0050 max;
    bool found;
    {
        UnkBigFistKind_ovb0050 kind;
        kind.value = 1;
        found = fn_71010fc864(self, &kind, &min, &max);
    }
    if (!found)
        return;
    UnkBigFistActor_ovb0050* owner = self->owner;
    f32 x = owner->xform.m[0][3];
    if (x >= min.x && x <= max.x)
        return;
    bool near_min = __builtin_fabsf(x - min.x) <= __builtin_fabsf(x - max.x);
    f32 r = owner->radius;
    *reinterpret_cast<u8*>(&owner->field_0x3b0) = 1;  // untyped: the owner pointer is reloaded after it
    owner->xform.m[0][3] = near_min ? min.x + r : max.x - r;
    f32 angle = near_min ? 1.5707964f : -1.5707964f;
    owner = self->owner;
    owner->xform.SetAngle(UnkVec3f(0.0f, angle, 0.0f));
    owner->field_0x3c0 = true;
}
