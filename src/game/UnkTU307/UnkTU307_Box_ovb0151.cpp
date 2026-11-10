// Unit 307: an axis-aligned box (a minimum and a maximum for each of three axes, from +0x8; a box with
// the minimum above the maximum is empty on that axis). Its range of an axis (the virtual at 0x50, here
// fn_7100216020), whether a point lies inside it, and whether it holds or overlaps another box. Names and
// layouts are placeholders.
#include "base/math/UnkSegment2D.h"
#include "types.h"

struct UnkBoxAxis_b0151 {
    f32 min;
    f32 max;
};

struct UnkBox_b0151 {
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
    virtual UnkVec2Result vf_0x50(u32 axis);  // the range of an axis
    UnkBoxAxis_b0151 axes[3];  // 0x8
};

// The range of an axis (the axis number counts modulo 3).
UnkVec2Result fn_7100216020(UnkBox_b0151* self, u32 axis) {
    const u8* base = reinterpret_cast<const u8*>(self);
    u32 k = axis % 3;
    if (__builtin_expect(k == 2, 0))
        return *reinterpret_cast<const UnkVec2Result*>(base + 0x18);
    if (__builtin_expect(k == 1, 0))
        return *reinterpret_cast<const UnkVec2Result*>(base + 0x10);
    return *reinterpret_cast<const UnkVec2Result*>(base + 0x8);
}

// Whether the point lies inside the box.
bool fn_71002163d4(UnkBox_b0151* self, const f32* point) {
    f32 px = point[0];
    bool in_x = self->axes[0].min <= px && self->axes[0].max >= px;
    f32 py = point[1];
    bool in_y = self->axes[1].min <= py && self->axes[1].max >= py;
    f32 pz = point[2];
    bool in_z = self->axes[2].min <= pz && self->axes[2].max >= pz;
    return in_x && in_y && in_z;
}

// Whether the box holds the other (per axis, the other's range lies within).
bool fn_710021646c(UnkBox_b0151* self, UnkBox_b0151* other) {
    bool in_x, in_y, in_z;
    {
        UnkVec2Result r = other->vf_0x50(0);
        in_x = r.x >= self->axes[0].min && r.y <= self->axes[0].max;
    }
    {
        UnkVec2Result r = other->vf_0x50(1);
        in_y = r.x >= self->axes[1].min && r.y <= self->axes[1].max;
    }
    {
        UnkVec2Result r = other->vf_0x50(2);
        in_z = r.x >= self->axes[2].min && r.y <= self->axes[2].max;
    }
    return in_x && in_y && in_z;
}

// Whether the other box overlaps this one (an empty range of the other never does).
bool fn_71002162b8(UnkBox_b0151* self, UnkBox_b0151* other) {
    bool in_x = false, in_y = false, in_z = false;
    {
        UnkVec2Result r = other->vf_0x50(0);
        if (r.x <= r.y && !(r.y < self->axes[0].min) && !(r.x > self->axes[0].max))
            in_x = true;
    }
    {
        UnkVec2Result r = other->vf_0x50(1);
        if (r.x <= r.y && !(r.y < self->axes[1].min) && !(r.x > self->axes[1].max))
            in_y = true;
    }
    {
        UnkVec2Result r = other->vf_0x50(2);
        if (r.x <= r.y && !(r.y < self->axes[2].min) && !(r.x > self->axes[2].max))
            in_z = true;
    }
    return in_x && in_y && in_z;
}
