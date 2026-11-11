// Unit 98 (tools/tu_map.py, 0x71000f35d0-0x71000f43c0): a sound controller over two nn::atk players.
// fn_71000f3608 starts a group's members on the target (nn::atk's 3D engine first): each member whose
// flag bit is among `flags` raises the target's level at +0x20 to at least 1, bit 1 of `flags` keeps the
// count at +0x14 from going negative and bit 2 clears the value at +0x24 for a flagged member. fn_71000f413c
// fades a level (+0x14 to the target at +0x1c, over the time at +0x20) or, with no time, sets it on the
// sound's effect at once. Names and layouts are placeholders.
#include "game/UnkTU2772.h"
#include "types.h"

class UnkEffect_b0184 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual bool vf_0x28();
};

struct UnkEffectRef_b0184 {
    UnkEffect_b0184* effect;
};

struct UnkSoundControl_b0242 {
    u8 pad_0x0[0x14];
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    UnkEffectRef_b0184* ref;  // 0x28
    u8 pad_0x30[0x4b];
    bool field_0x7b;
};
static_assert(__builtin_offsetof(UnkSoundControl_b0242, field_0x7b) == 0x7b, "layout");

void fn_71000f413c(UnkSoundControl_b0242* self, bool enable, f32 value, f32 time) {
    if (self->field_0x7b == enable)
        return;
    f32 target = enable ? value : 0.0f;
    self->field_0x7b = enable;
    if (!(time <= 0.0f)) {
        self->field_0x18 = self->field_0x14;
        self->field_0x1c = target;
        self->field_0x20 = time;
        self->field_0x24 = 0.0f;
        return;
    }
    if (self->ref && self->ref->effect && self->ref->effect->vf_0x28()) {
        if (!self->ref->effect)
            return;
        fn_710120e660(self->ref->effect, target, 0);
        if (!self->ref->effect)
            return;
        fn_710120e660(self->ref->effect, 0.0f, 1);
        if (!self->ref->effect)
            return;
        fn_710120e660(self->ref->effect, 0.0f, 2);
        if (!self->ref->effect)
            return;
        fn_710120e658(self->ref->effect, -target);
        return;
    }
    self->field_0x18 = self->field_0x14;
    self->field_0x1c = target;
    self->field_0x20 = 0.001f;
    self->field_0x24 = 0.0f;
}

struct UnkLink_b0242 {
    UnkLink_b0242* prev;
    UnkLink_b0242* next;
};
struct UnkSoundTarget_b0242 {
    u8 pad_0x0[0x14];
    s32 field_0x14;
    u8 pad_0x18[8];
    f32 field_0x20;
    f32 field_0x24;
};
struct UnkSoundGroup_b0242 {
    u8 pad_0x0[0x28];
    UnkLink_b0242 members;  // 0x28: each member's flag word is 8 bytes before its link
};
void fn_71012054b8(void* engine, UnkSoundTarget_b0242* target, UnkSoundGroup_b0242* group, u32 a, u32 b);

void fn_71000f3608(void* engine, UnkSoundTarget_b0242* target, UnkSoundGroup_b0242* group, u32 a, u32 b, u32 flags) {
    fn_71012054b8(engine, target, group, a, b);
    for (UnkLink_b0242* link = group->members.next; link != &group->members; link = link->next) {
        u32 member = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(link) - 8) & 1;
        if (member & flags) {
            f32 v = target->field_0x20;
            target->field_0x20 = v > 1.0f ? v : 1.0f;
        }
        if (flags & 2) {
            s32 w = target->field_0x14;
            target->field_0x14 = w > 0 ? w : 0;
        }
        if (member && (flags & 4))
            target->field_0x24 = 0.0f;
    }
}
