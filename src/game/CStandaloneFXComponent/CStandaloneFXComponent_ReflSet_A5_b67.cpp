// CStandaloneFXComponent's unit (tools/tu_map.py): a reflected setter for the position at +0x10 (three words copied
// from the argument); when the effect at +0x38 exists it rebuilds the transform from the position, the angles at
// +0x1c and the unit scale (fn_71000791a8, g_unk_71016477a8), then passes it to the effect (vf_0x170, vf_0x168).
// fn_710081541c is the same for the angles. Answers the empty status {0, 0, 2, 1}. Names are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::UnkReflValue;

extern const base::math::CVector3D g_unk_71016477a8;

struct UnkFxOwner_A5_b67 {
    u8 pad_0x0[0x18];
    char* node;
};

struct UnkFx_A5_b67 {
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
    virtual void vf_0x160();
    virtual void vf_0x168(void* node, const UnkMatrix44* m, u32 a, s32 b);
    virtual void vf_0x170();
};

struct UnkStandaloneFX_A5_b67 {
    UnkFxOwner_A5_b67* owner;
    u8 pad_0x8[0x8];
    u32 pos[3];
    UnkVec3f angle;
    u8 pad_0x28[0x10];
    UnkFx_A5_b67* fx;
};

UnkReflValue fn_7100815304(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    UnkStandaloneFX_A5_b67* self = *reinterpret_cast<UnkStandaloneFX_A5_b67**>(obj + 8);
    const u32* v = base::reflection::refl_ArgValue<u32>(arg);
    self->pos[0] = v[0];
    self->pos[1] = v[1];
    self->pos[2] = v[2];
    if (self->fx) {
        UnkMatrix44 m = fn_71000791a8(*reinterpret_cast<const UnkVec3f*>(self->pos), self->angle,
                                      reinterpret_cast<const UnkVec3f&>(g_unk_71016477a8));
        self->fx->vf_0x170();
        self->fx->vf_0x168(self->owner->node ? self->owner->node + 0x10 : nullptr, &m, 0, -1);
    }
    return base::reflection::refl_EmptyValue();
}

UnkReflValue fn_710081541c(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    UnkStandaloneFX_A5_b67* self = *reinterpret_cast<UnkStandaloneFX_A5_b67**>(obj + 8);
    const u32* v = base::reflection::refl_ArgValue<u32>(arg);
    u32* angle = reinterpret_cast<u32*>(&self->angle);
    angle[0] = v[0];
    angle[1] = v[1];
    angle[2] = v[2];
    if (self->fx) {
        UnkMatrix44 m = fn_71000791a8(*reinterpret_cast<const UnkVec3f*>(self->pos), self->angle,
                                      reinterpret_cast<const UnkVec3f&>(g_unk_71016477a8));
        self->fx->vf_0x170();
        self->fx->vf_0x168(self->owner->node ? self->owner->node + 0x10 : nullptr, &m, 0, -1);
    }
    return base::reflection::refl_EmptyValue();
}
