// game::logic::collision::COBoxShape2D: its clone (fn_7100d6875c) and its read from a file
// (fn_7100d68484). The clone is as the other 2D shapes'
// (src/game/logic/collision/UnkShape2DClone_b46.cpp): a new 0xd8-byte shape built by the shape base
// constructor from the source's transform (fn_7100d6c034, from +0x8), the class's vtable, a cleared
// flag (+0xd0) and the source's oriented box (+0xa8) copied word by word, its angle before its
// offset. Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "game/logic/collision/CShape.h"
#include "types.h"

#include <math.h>

using base::global::CRntFile;

struct UnkShape2D_b46a3;

extern char g_unk_7101aa8c30[];  // COBoxShape2D's vtable

struct UnkOBoxShape2DClone_ovb0062 {
    void* vtable;
    u8 transform[0xa0];  // 0x8
    u32 center[2];       // 0xa8
    u32 axis_x[2];       // 0xb0
    u32 axis_y[2];       // 0xb8
    u32 size[2];         // 0xc0
    u32 offset;          // 0xc8
    u32 angle;           // 0xcc
    bool field_0xd0;
};

UnkOBoxShape2DClone_ovb0062* fn_7100d6875c(const UnkOBoxShape2DClone_ovb0062* other) {
    auto* self = static_cast<UnkOBoxShape2DClone_ovb0062*>(__builtin_operator_new(0xd8));
    fn_7100d6c034(reinterpret_cast<UnkShape2D_b46a3*>(self), other->transform);
    self->vtable = g_unk_7101aa8c30 + 0x10;
    self->field_0xd0 = false;
    self->center[0] = other->center[0];
    self->center[1] = other->center[1];
    self->axis_x[0] = other->axis_x[0];
    self->axis_x[1] = other->axis_x[1];
    self->axis_y[0] = other->axis_y[0];
    self->axis_y[1] = other->axis_y[1];
    self->size[0] = other->size[0];
    self->size[1] = other->size[1];
    self->angle = other->angle;
    self->offset = other->offset;
    return self;
}

struct UnkPolygonCollectionShape;

struct UnkOBoxShape2DRead_ovb0062 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    u8 pad_0x8[0xa0];
    f32 center_x;  // 0xa8
    f32 center_y;  // 0xac
    f32 axis_x_x;  // 0xb0
    f32 axis_x_y;  // 0xb4
    f32 axis_y_x;  // 0xb8
    f32 axis_y_y;  // 0xbc
    f32 size_x;    // 0xc0
    f32 size_y;    // 0xc4
    f32 offset;    // 0xc8
    f32 angle;     // 0xcc
};

// Reads the shape's transform (fn_7100d6c288, declared as
// src/game/logic/collision/CCapsuleShape2D/CCapsuleShape2D_Read_ovb0015.cpp has it), then the box's
// angle in degrees and its size (all zero once the file is exhausted): the box is centred on the
// origin, its axes turned by the angle, which is kept as the offset with a zero angle, and the shape
// updates itself (slot 0x80). The size is stored as read, without a float type.
bool fn_7100d68484(UnkOBoxShape2DRead_ovb0062* self, CRntFile* file) {
    fn_7100d6c288(reinterpret_cast<UnkPolygonCollectionShape*>(self), file);
    f32 degrees = 0.0f;
    f32 width = 0.0f;
    f32 height = 0.0f;
    if (file->position < file->size) {
        fn_710007c714(file, &degrees, 4, 1);
        fn_710007c714(file, &width, 4, 1);
        fn_710007c714(file, &height, 4, 1);
    }
    f32 radians = (f32)((f64)degrees * 3.141592653589793 / 180.0);
    u32 width_bits, height_bits;
    __builtin_memcpy(&width_bits, &width, 4);
    __builtin_memcpy(&height_bits, &height, 4);
    f32 c = cosf(radians);
    f32 s = sinf(radians);
    f32 ns = -sinf(radians);
    f32 c2 = cosf(radians);
    self->center_x = 0.0f;
    self->center_y = 0.0f;
    self->axis_x_x = c;
    self->axis_x_y = s;
    self->axis_y_x = ns;
    self->axis_y_y = c2;
    __builtin_memcpy(&self->size_x, &width_bits, 4);
    __builtin_memcpy(&self->size_y, &height_bits, 4);
    self->angle = 0.0f;
    self->offset = radians;
    self->vf_0x80();
    return true;
}
