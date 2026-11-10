// Unit 227 of tools/tu_map.py (a sphere-shaped source, see src/game/UnkSphereOwner.cpp): the
// source's centre and radius. Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

extern "C" f32 cosf(f32);
extern "C" f32 sqrtf(f32);

struct UnkSphereSource;

struct UnkSphereSource_ovb0168 {
    u8 pad_0x0[0x20];
    s32 kind;      // 0x20
    u32 center_x;  // 0x24
    u32 center_y;  // 0x28
    u32 center_z;  // 0x2c
    f32 dir_x;     // 0x30
    f32 dir_y;     // 0x34
    f32 dir_z;     // 0x38
    u8 pad_0x3c[0x44];
    f32 field_0x80;
    f32 angle_a;   // 0x84
    f32 angle_b;   // 0x88
};
static_assert(__builtin_offsetof(UnkSphereSource_ovb0168, angle_b) == 0x88, "layout");

// The source's centre and radius: kind 5 is a capsule-like shape (the radius grows by twice the
// length of the vector at +0x30), kinds 2 and 3 take it as it is, the others have none (0).
void fn_710019fc40(UnkSphereSource* source, UnkVec3f* center, f32* radius) {
    UnkSphereSource_ovb0168* self = reinterpret_cast<UnkSphereSource_ovb0168*>(source);
    s32 kind = self->kind;
    u32* out = reinterpret_cast<u32*>(center);
    out[0] = self->center_x;
    out[1] = self->center_y;
    out[2] = self->center_z;
    if (kind == 5) {
        f32 base = self->field_0x80;
        f32 length = sqrtf(self->dir_x * self->dir_x + self->dir_y * self->dir_y + self->dir_z * self->dir_z);
        *radius = base + (length + length);
        return;
    }
    *radius = 0.0f;
    if ((self->kind & ~1) == 2)
        *radius = self->field_0x80;
}
