// base::spatial::CAABox2D's unit (tools/tu_map.py unit 106): reflected setters of the box's minimum
// and maximum corners, the same shape as COBox2D's size setter (src/base/spatial/COBox2D_Setters.cpp,
// fn_7100110a54): each reads a 2D vector out of a value holder (in place, or behind a pointer when the
// holder is in mode 1 or its type is wider than 8 bytes) and answers a 0x18-byte status {0, 0, 2, 1}.
// Names of the holder's parts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

struct UnkAABox2D106 {
    CVector2D min;  // 0x0
    CVector2D max;  // 0x8
};

struct UnkValueType106 {
    u8 pad_0x0[0x18];
    u64 size;  // 0x18
};

struct UnkValue106 {
    union {
        u8 inline_data[8];
        u8* data;
    };
    UnkValueType106* type;  // 0x8
    u8 pad_0x10[0x4];
    u32 mode;  // 0x14
};

struct UnkOwner106 {
    u8 pad_0x0[0x8];
    UnkAABox2D106* box;  // 0x8
};

struct UnkStatus106 {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;
};

static inline const u8* ValueData106(const UnkValue106* v) {
    return (v->mode == 1 || v->type->size >= 9) ? v->data : v->inline_data;
}

static inline UnkStatus106 Done106() {
    UnkStatus106 status;
    status.field_0x0 = 0;
    status.field_0x8 = 0;
    status.field_0x10 = 2;
    status.field_0x14 = 1;
    return status;
}

UnkStatus106 fn_710010d464(void* context, UnkOwner106* owner, UnkValue106* const* value) {
    UnkValue106* v = *value;
    UnkAABox2D106* self = owner->box;
    const CVector2D* corner = reinterpret_cast<const CVector2D*>(ValueData106(v));
    f32 x = corner->x;
    f32 y = corner->y;
    self->min.x = x;
    self->min.y = y;
    return Done106();
}

UnkStatus106 fn_710010d4f4(void* context, UnkOwner106* owner, UnkValue106* const* value) {
    UnkValue106* v = *value;
    UnkAABox2D106* self = owner->box;
    const CVector2D* corner = reinterpret_cast<const CVector2D*>(ValueData106(v));
    f32 x = corner->x;
    f32 y = corner->y;
    self->max.x = x;
    self->max.y = y;
    return Done106();
}
