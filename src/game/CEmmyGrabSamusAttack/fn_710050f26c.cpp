// Switches two attached effect objects off and, while the owner's EMMY AI component (with its
// EMMY character class) is in state 1, toggles each one's mode and restarts it with the given
// colours and timings, then runs a follow-up.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkSceneObject.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::UnkReflObject;

// Passed by value but in memory, so its copy constructor is user-provided.
struct UnkColor_710050f26c {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
    UnkColor_710050f26c(const UnkColor_710050f26c& o) : r(o.r), g(o.g), b(o.b), a(o.a) {}
};

struct UnkRGB_710050f26c {
    f32 r;
    f32 g;
    f32 b;
};

struct UnkColorArg_710050f26c {
    UnkRGB_710050f26c rgb;
    f32 a;
    UnkColorArg_710050f26c(const UnkRGB_710050f26c& rgb_, f32 a_) : rgb(rgb_), a(a_) {}
    UnkColorArg_710050f26c(const UnkColorArg_710050f26c& o) : rgb(o.rgb), a(o.a) {}
};

struct UnkColorIn_710050f26c {
    UnkRGB_710050f26c rgb;
    f32 a;
};

// The effects are scene objects (game/UnkSceneObject.h).
bool fn_710014826c(UnkSceneObject* effect, u32 mode, u32 flags, UnkColorArg_710050f26c color, bool restart,
                   f32 time);
void fn_7100148600(UnkSceneObject* effect, u32 mode, u32 flags, UnkColor_710050f26c from, UnkColor_710050f26c to,
                   f32 delay, f32 time);

struct UnkAIState_710050f26c {
    u8 unk_0x0[0x18c];
    s32 state;
};

struct UnkCharClass_710050f26c : UnkReflObject {
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
    virtual UnkAIState_710050f26c* vf_0xd8();
};

struct UnkAIComponent_710050f26c : UnkReflObject {
    u8 unk_0x8[8];
    UnkCharClass_710050f26c* char_class;  // 0x10
};

struct UnkActor_710050f26c {
    u8 unk_0x0[0x240];
    UnkAIComponent_710050f26c* ai;
};

struct UnkStruct_710050f26c {
    u8 unk_0x0[0x8];
    UnkActor_710050f26c* actor;
    u8 unk_0x10[0x6e0];
    UnkSceneObject* field_0x6f0;
    UnkSceneObject* field_0x6f8;
    u8 unk_0x700[0xb4];
    u32 field_0x7b4;
    u32 field_0x7b8;
};

void fn_710050a1b0(UnkStruct_710050f26c* self);

// NON_MATCHING(minor): the two stores copying the colour's rgb into the first call's argument are
// swapped.
void fn_710050f26c(UnkStruct_710050f26c* self, f32 time, const UnkColorIn_710050f26c* color, f32 time2,
                   const UnkColor_710050f26c* color2, f32 offset) {
    fn_71001be430(self->field_0x6f8->field_0x130, 0, false);
    fn_71001be430(self->field_0x6f8->field_0x130, 1, false);
    fn_71001be430(self->field_0x6f0->field_0x130, 0, false);
    fn_71001be430(self->field_0x6f0->field_0x130, 1, false);
    fn_7100149490(self->field_0x6f8, false);
    fn_7100149490(self->field_0x6f0, false);

    UnkAIComponent_710050f26c* ai = self->actor->ai;
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_710053b934()))
        return;
    UnkCharClass_710050f26c* cc = ai->char_class;
    if (!cc || !fn_710009d50c(cc->vf_0x0(), fn_710051e010()))
        return;
    if (cc->vf_0xd8()->state != 1)
        return;

    if (UnkSceneObject* effect = self->field_0x6f8) {
        u32 mode = self->field_0x7b8 == 0;
        self->field_0x7b8 = mode;
        fn_710014826c(effect, mode, 0x28, UnkColorArg_710050f26c(color->rgb, 0.5f < color->a ? 0.5f : color->a), true,
                      time);
        fn_71001be430(self->field_0x6f8->field_0x130, self->field_0x7b8, true);
        fn_7100149490(self->field_0x6f8, true);
    }
    if (UnkSceneObject* effect = self->field_0x6f0) {
        u32 mode = self->field_0x7b4 == 0;
        f32 delay = time2 - offset < 0.0f ? 0.0f : time2 - offset;
        self->field_0x7b4 = mode;
        fn_7100148600(effect, mode, 0x28, *color2, *color2, delay, time2);
        fn_71001be430(self->field_0x6f0->field_0x130, self->field_0x7b4, true);
        fn_7100149490(self->field_0x6f0, true);
    }
    fn_710050a1b0(self);
}
