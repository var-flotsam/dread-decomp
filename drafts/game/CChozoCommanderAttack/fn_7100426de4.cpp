// CChozoCommanderAttack's unit (tools/tu_map.py): turns the held aim angle (radians, +0x28) towards the
// angle vf_0xf0 asks for, faster the further off it is (between `min_rate` and `max_rate`, scaled by
// `speed`), keeps it within a half turn either way and feeds it, in degrees, to the animation's "Angle".
// Names and layouts are placeholders.
#include <math.h>

#include "../../../src/game_s7/UnkPropBagS7.h"
#include "base/global/CStrId.h"
#include "game/UnkTU54.h"
#include "types.h"

using base::global::CStrId;

struct UnkCommanderAnimation_l426de4 {
    u8 pad_0x0[0x80];
    unk_prop_bag::UnkPropBag params;  // 0x80
};

struct UnkCommanderOwner_l426de4 {
    u8 pad_0x0[0x220];
    UnkCommanderAnimation_l426de4* animation;  // 0x220
};

class UnkCommanderAttack_l426de4 {
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
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
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
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual f32 vf_0xf0(bool flag);  // the angle to turn to

    UnkCommanderOwner_l426de4* owner;  // 0x8
    u8 pad_0x10[0x18];
    f32 angle;  // 0x28
};
static_assert(__builtin_offsetof(UnkCommanderAttack_l426de4, angle) == 0x28, "layout");

void fn_7100426de4(UnkCommanderAttack_l426de4* self, bool flag, f32 speed, f32 min_rate, f32 max_rate) {
    if (!self->owner->animation)
        return;
    f32 target = self->vf_0xf0(flag);
    // fn_710007a170 moves the angle it is given (the first member of its pair) by a full turn towards
    // the current one.
    fn_710007a170(reinterpret_cast<UnkAim*>(&target), self->angle);
    f32 angle = self->angle;
    f32 off = fabsf(angle - target) / 1.5707964f;
    f32 blend = !(off <= 0.0f) ? (!(off >= 1.0f) ? off : 1.0f) : 0.0f;
    f32 rate = (max_rate - min_rate) * blend + min_rate;
    if (angle > target) {
        angle -= rate * speed;
        self->angle = angle;
        if (angle < target) {
            angle = target;
            self->angle = target;
        }
    } else if (angle < target) {
        angle += rate * speed;
        self->angle = angle;
        if (angle > target) {
            angle = target;
            self->angle = target;
        }
    }
    if (angle > 3.1415927f)
        self->angle = angle + -6.2831855f;
    else if (angle < -3.1415927f)
        self->angle = angle + 6.2831855f;
    angle = self->angle;
    f32 degrees = (f32)((angle * 180.0f) / 3.141592653589793);
    unk_prop_bag::fn_710033187c(&self->owner->animation->params, CStrId("Angle", true), degrees);
}
