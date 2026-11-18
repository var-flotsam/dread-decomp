// Unit 307: grows an axis-aligned box to hold another box (each non-empty range of the other is added
// to this box's range of the same axis). Names and layouts are placeholders.
#include "base/math/UnkSegment2D.h"
#include "types.h"

struct UnkBoxAxis_b0151 {
    f32 min;
    f32 max;

    // Widens the range to hold the value (an empty range becomes just the value).
    void Add(f32 v) {
        if (min <= max) {
            if (min > v)
                min = v;
            else if (max < v)
                max = v;
        } else {
            min = v;
            max = v;
        }
    }

    // Widens the range to hold another (an empty other adds nothing).
    void Add(const UnkVec2Result& r) {
        if (r.x <= r.y) {
            Add(r.x);
            Add(r.y);
        }
    }
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

// Grows the box to hold the other box.
void fn_7100216090(UnkBox_b0151* self, UnkBox_b0151* other) {
    self->axes[0].Add(other->vf_0x50(0));
    self->axes[1].Add(other->vf_0x50(1));
    self->axes[2].Add(other->vf_0x50(2));
}
