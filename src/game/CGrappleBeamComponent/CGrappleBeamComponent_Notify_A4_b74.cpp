// CGrappleBeamComponent unit (tools/tu_map.py, unit 1277): classifies the current state (fn_7100ac82bc) and tells
// the listener at +0x530 about it through one of three vtable slots (0x160 for 1 and 7, 0x168 for 2, none for 11,
// 0x170 otherwise), passing the id at +0x3fc, the owner's (+0x30) position (the translation column of its
// transform at +0x38) and the owner. Returns the classification. Names and layouts are placeholders.
#include "types.h"

struct UnkGrappleBeam_A4_b74;
s32 fn_7100ac82bc(UnkGrappleBeam_A4_b74* self, void* arg);

struct UnkGrappleVec3_A4_b74 {
    f32 x, y, z;
};

struct UnkGrappleOwner_A4_b74 {
    u8 pad_0x0[0x38];
    f32 transform[3][4];  // 0x38

    UnkGrappleVec3_A4_b74 Position() const { return {transform[0][3], transform[1][3], transform[2][3]}; }
};

struct UnkGrappleListener_A4_b74 {
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
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160(u32 id, const UnkGrappleVec3_A4_b74& pos, UnkGrappleOwner_A4_b74* owner, void* extra);
    virtual void vf_0x168(u32 id, const UnkGrappleVec3_A4_b74& pos, UnkGrappleOwner_A4_b74* owner, void* extra);
    virtual void vf_0x170(u32 id, const UnkGrappleVec3_A4_b74& pos, UnkGrappleOwner_A4_b74* owner, void* extra);
};

struct UnkGrappleBeam_A4_b74 {
    u8 pad_0x0[0x30];
    UnkGrappleOwner_A4_b74* owner;
    u8 pad_0x38[0x3fc - 0x38];
    u32 id;  // 0x3fc
    u8 pad_0x400[0x530 - 0x400];
    UnkGrappleListener_A4_b74* listener;  // 0x530
};

// `arg` (x1) reaches fn_7100ac82bc untouched in the original (tools/forwarded_args.py).
s32 fn_7100819798(UnkGrappleBeam_A4_b74* self, void* arg) {
    s32 state = fn_7100ac82bc(self, arg);
    switch (state) {
    case 1:
    case 7: {
        UnkGrappleOwner_A4_b74* owner = self->owner;
        self->listener->vf_0x160(self->id, owner->Position(), owner, nullptr);
        break;
    }
    case 2: {
        UnkGrappleOwner_A4_b74* owner = self->owner;
        self->listener->vf_0x168(self->id, owner->Position(), owner, nullptr);
        break;
    }
    case 11:
        break;
    default: {
        UnkGrappleOwner_A4_b74* owner = self->owner;
        self->listener->vf_0x170(self->id, owner->Position(), owner, nullptr);
        break;
    }
    }
    return state;
}
