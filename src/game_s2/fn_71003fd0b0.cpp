// Turns the cannon's aim (angle at 0x146bc, current value at 0x146c0) towards the player:
// after fn_7100d7c990 gives the target object, the angle becomes atan2 of the position
// difference (values at 0x54 and 0x44), is normalised, and the current value moves towards it
// by at most the tunable at 0x24 times `scale`. fn_71003fd194 then gets the current value.
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkAIObject {
    virtual const CClass* vf_0x0();
};
struct UnkPositioned {
    u8 unk_0x0[0x44];
    f32 field_0x44;
    u8 unk_0x48[0xc];
    f32 field_0x54;
};
struct UnkAim {
    f32 angle;
    f32 current;
};
struct UnkSelf {
    u8 unk_0x0[0x10];
    UnkAIObject* object;
    u8 unk_0x18[0x18];
    UnkPositioned* field_0x30;
    u8 unk_0x38[0x14684];
    UnkAim aim;
};
struct UnkTunable {
    u8 unk_0x0[0x24];
    f32 field_0x24;
};

extern UnkTunable g_unk_71726c32a0;
extern "C" f32 atan2f(f32 y, f32 x);
const CClass* fn_71003feb44();  // CCharClassCentralUnitCannonAIComponent::refl_GetClass
UnkPositioned* fn_7100d7c990(UnkPositioned* object);
void fn_710007a170(UnkAim* aim, f32 current);
void fn_71003fd194(UnkSelf* self, f32 current);

// NON_MATCHING(minor): the final select is `fcsel ... le` in the original, `fcsel ... gt` with the operands swapped here.
void fn_71003fd0b0(UnkSelf* self, f32 scale) {
    UnkAim* aim = &self->aim;
    if (self->object)
        fn_710009d50c(self->object->vf_0x0(), fn_71003feb44());
    f32 rate = g_unk_71726c32a0.field_0x24;
    f32 current;
    if (UnkPositioned* target = fn_7100d7c990(self->field_0x30)) {
        UnkPositioned* me = self->field_0x30;
        f32 tx = target->field_0x44;
        f32 ty = target->field_0x54;
        f32 mx = me->field_0x44;
        f32 my = me->field_0x54;
        f32 dx = tx - mx;
        f32 dy = ty - my;
        aim->angle = atan2f(dy, dx);
        fn_710007a170(aim, aim->current);
        f32 step = rate * scale;
        f32 value = aim->angle;
        f32 from = aim->current;
        f32 lo = from - step;
        f32 hi = step + from;
        f32 up = value > lo ? value : lo;
        f32 down = value < hi ? value : hi;
        current = value > from ? down : up;
        aim->current = current;
    } else {
        current = aim->current;
    }
    fn_71003fd194(self, current);
}
