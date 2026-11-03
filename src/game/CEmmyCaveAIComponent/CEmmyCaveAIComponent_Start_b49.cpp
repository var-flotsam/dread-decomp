// CEmmyCaveAIComponent (tools/tu_map.py): marks +0x29f24, runs the base routine (fn_710055b1e8), turns
// the owner (+0x30) to the angles (0, -1.57, 0) through its transform (+0x38, game/UnkXformNode.h) and
// its slot 0x118 (with mode 2), sets its +0x3c0 and zeroes three speeds (fn_7101103aa8 on the
// component, slot 0x98 of the owner's +0x298 and +0x218). Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkEmmyCaveSpeed_b49 {
    void** vtable;
    void vf_0x98(f32 v) { reinterpret_cast<void (*)(UnkEmmyCaveSpeed_b49*, f32)>(vtable[0x98 / 8])(this, v); }
};

struct UnkEmmyCaveOwner_b49 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform transform;
    u8 pad_0x94[0x184];
    UnkEmmyCaveSpeed_b49* field_0x218;
    u8 pad_0x220[0x78];
    UnkEmmyCaveSpeed_b49* field_0x298;
    u8 pad_0x2a0[0x120];
    bool field_0x3c0;

    void vf_0x118(UnkXform* xform, s32* mode) {
        reinterpret_cast<void (*)(UnkEmmyCaveOwner_b49*, UnkXform*, s32*)>(vtable[0x118 / 8])(this, xform, mode);
    }
};

struct UnkEmmyCaveAIStart_b49 {
    u8 pad_0x0[0x30];
    UnkEmmyCaveOwner_b49* owner;
    u8 pad_0x38[0x29f24 - 0x38];
    bool field_0x29f24;
};

void fn_710055b1e8(UnkEmmyCaveAIStart_b49* self);
void fn_7101103aa8(UnkEmmyCaveAIStart_b49* self, f32 speed);

void fn_71004f0c90(UnkEmmyCaveAIStart_b49* self) {
    self->field_0x29f24 = true;
    fn_710055b1e8(self);
    UnkEmmyCaveOwner_b49* owner = self->owner;
    owner->transform.SetAngle(UnkVec3f(0.0f, -1.57f, 0.0f));
    s32 mode = 2;
    owner->vf_0x118(&owner->transform, &mode);
    owner->field_0x3c0 = true;
    fn_7101103aa8(self, 0.0f);
    self->owner->field_0x298->vf_0x98(0.0f);
    self->owner->field_0x218->vf_0x98(0.0f);
}
