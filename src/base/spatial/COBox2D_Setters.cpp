// base::spatial::COBox2D unit (tools/tu_map.py unit 107): reflected setters of the box's size and
// angle: each reads its value out of a value holder (in place, or behind a pointer when the holder
// is in mode 1 or its type is wider than 8 bytes); the angle is set as fn_7100110664 does. Each
// answers a 0x18-byte status {0, 0, 2, 1}. Names of the holder's parts are placeholders.
#include <math.h>

#include "base/spatial/COBox2D.h"
#include "types.h"

using base::spatial::COBox2D;

f32 fn_710007a1d0(f32 angle);  // wraps an angle into (-pi, pi]

struct UnkValueType107 {
    u8 pad_0x0[0x18];
    u64 size;  // 0x18
};

struct UnkValue107 {
    union {
        u8 inline_data[8];
        u8* data;
    };
    UnkValueType107* type;  // 0x8
    u8 pad_0x10[0x4];
    u32 mode;  // 0x14
};

struct UnkOwner107 {
    u8 pad_0x0[0x8];
    COBox2D* box;  // 0x8
};

struct UnkStatus107 {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;
};

static inline const u8* ValueData(const UnkValue107* v) {
    return (v->mode == 1 || v->type->size >= 9) ? v->data : v->inline_data;
}

static inline UnkStatus107 Done() {
    UnkStatus107 status;
    status.field_0x0 = 0;
    status.field_0x8 = 0;
    status.field_0x10 = 2;
    status.field_0x14 = 1;
    return status;
}

UnkStatus107 fn_7100110a54(void* context, UnkOwner107* owner, UnkValue107* const* value) {
    UnkValue107* v = *value;
    COBox2D* self = owner->box;
    const base::math::CVector2D* size = reinterpret_cast<const base::math::CVector2D*>(ValueData(v));
    f32 x = size->x;
    f32 y = size->y;
    self->size.x = x;
    self->size.y = y;
    return Done();
}

UnkStatus107 fn_7100110ae8(void* context, UnkOwner107* owner, UnkValue107* const* value) {
    UnkValue107* v = *value;
    COBox2D* self = owner->box;
    self->angle = fn_710007a1d0(*reinterpret_cast<const f32*>(ValueData(v)));
    self->axis_x.x = cosf(self->angle + self->offset);
    self->axis_x.y = sinf(self->angle + self->offset);
    self->axis_y.x = -sinf(self->angle + self->offset);
    self->axis_y.y = cosf(self->angle + self->offset);
    return Done();
}
