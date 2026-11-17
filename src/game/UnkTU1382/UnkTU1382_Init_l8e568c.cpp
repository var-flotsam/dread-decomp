// Unit 1382: binds an animated set of values to its owner and definition, and sets each value's three
// copies to the definition's starting value, telling the object after each (virtual slots 0x78 to 0xa8),
// then passes on its two flags. Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector4D;

// CVector4D with its member-wise copy and its out-of-line assignment, which returns the destination
// (base/math/CVector4D.h declares fn_710007b518 as returning void).
struct UnkTweenVec4_l8e568c : CVector4D {
    UnkTweenVec4_l8e568c(const CVector4D& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        w = o.w;
    }
    UnkTweenVec4_l8e568c(const UnkTweenVec4_l8e568c& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        w = o.w;
    }
    UnkTweenVec4_l8e568c& operator=(const UnkTweenVec4_l8e568c& o) {
        using Assign = UnkTweenVec4_l8e568c* (*)(UnkTweenVec4_l8e568c*, const UnkTweenVec4_l8e568c*);
        return *reinterpret_cast<Assign>(fn_710007b518)(this, &o);
    }
};

// A value with two more copies of it (which is which is not known yet) and two words after them.
template <class T>
struct UnkTween_l8e568c {
    T value[3];
    u32 field_a;
    u32 field_b;

    void Set(T v) { value[0] = value[1] = value[2] = v; }
};

struct UnkTweenDef_l8e568c {
    u8 pad_0x0[0x18];
    UnkVec3f field_0x18;
    UnkVec3f field_0x24;
    CVector4D field_0x30;
    CVector4D field_0x40;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    bool field_0x5c;
    bool field_0x5d;
};

struct UnkOwner_l8e568c;

class UnkTweenSet_l8e568c {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(bool flag);
    virtual void vf_0x50();
    virtual void vf_0x58(bool flag);
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();

    UnkTweenDef_l8e568c* def;  // 0x8
    UnkOwner_l8e568c* owner;   // 0x10
    u32 pad_0x18;
    UnkTween_l8e568c<UnkVec3f> field_0x1c;
    UnkTween_l8e568c<UnkVec3f> field_0x48;
    UnkTween_l8e568c<UnkTweenVec4_l8e568c> field_0x74;
    UnkTween_l8e568c<UnkTweenVec4_l8e568c> field_0xac;
    UnkTween_l8e568c<f32> field_0xe4;
    UnkTween_l8e568c<f32> field_0xf8;
    UnkTween_l8e568c<f32> field_0x10c;
};
static_assert(__builtin_offsetof(UnkTweenSet_l8e568c, field_0x74) == 0x74, "layout");
static_assert(__builtin_offsetof(UnkTweenSet_l8e568c, field_0x10c) == 0x10c, "layout");

bool fn_71008e568c(UnkTweenSet_l8e568c* self, UnkOwner_l8e568c* owner, UnkTweenDef_l8e568c* def) {
    self->def = def;
    self->owner = owner;
    if (!owner || !def)
        return true;

    self->field_0x1c.Set(def->field_0x18);
    self->vf_0x78();
    self->field_0x48.Set(self->def->field_0x24);
    self->vf_0x80();
    self->field_0x74.Set(self->def->field_0x30);
    self->vf_0x88();
    self->field_0xac.Set(self->def->field_0x40);
    self->vf_0x90();
    self->field_0xe4.Set(self->def->field_0x50);
    self->vf_0x98();
    self->field_0xf8.Set(self->def->field_0x54);
    self->vf_0xa0();
    self->field_0x10c.Set(self->def->field_0x58);
    self->vf_0xa8();
    self->vf_0x48(self->def->field_0x5c);
    self->vf_0x58(self->def->field_0x5d);
    return true;
}
