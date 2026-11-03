#include "types.h"
#include "base/UnkMatrix44.h"

struct UnkUnit443Vec3 {
    f32 x;
    f32 y;
    f32 z;
    UnkUnit443Vec3(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkUnit443Vec3(const UnkUnit443Vec3& o) : x(o.x), y(o.y), z(o.z) {}
    UnkUnit443Vec3 operator*(f32 s) const { return UnkUnit443Vec3(x * s, y * s, z * s); }
    UnkUnit443Vec3 operator+(const UnkUnit443Vec3& o) const { return UnkUnit443Vec3(x + o.x, y + o.y, z + o.z); }
};

struct UnkUnit443Owner {
    u8 unk_0x0[0x38];
    UnkMatrix44 xform;
};

struct UnkUnit443Params {
    u8 unk_0x0[0x58];
    f32 height;
};

struct UnkUnit443Self {
    u8 unk_0x0[0x30];
    UnkUnit443Owner* owner;
    u8 unk_0x38[0x8];
    UnkUnit443Params* params;
};

UnkUnit443Vec3 fn_7100360130(const UnkUnit443Self* self) {
    UnkUnit443Owner* owner = self->owner;
    UnkUnit443Vec3 pos(owner->xform.m[3], owner->xform.m[7], owner->xform.m[11]);
    const UnkMatrix44& m = fn_710002614c(&owner->xform);
    UnkUnit443Vec3 up(m.m[1], m.m[5], m.m[9]);
    return pos + up * self->params->height;
}
