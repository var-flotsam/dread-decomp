// CPowerBeamGun's unit (tools/tu_map.py): firing a shot. Once the gun's slot 0x48 allows it, the
// shot starts at the owner's position (or the point its component at +0x228 gives for the singleton's
// id), goes along the direction of the animation component's current bone (fn_710034f54c) and plays
// its sound (fn_7100ed6910, kind 4 for a charged shot). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CModelUpdaterComponent.h"
#include "types.h"

using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkHyperBeamSoundHandle {
    void* ref;
    s64 serial;
};

struct UnkGrappleAnswer_b50;
UnkHyperBeamSoundHandle fn_7100ed6910(void* player, const CVector3D* pos, u32 id, u32 kind, bool flag);

struct UnkBeamVec_ovb0138 {
    f32 x, y, z;
};

struct UnkBeamIdHolder_ovb0138 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkBeamAnimation_ovb0138 : UnkReflObject {
    u8 pad[0x530 - sizeof(UnkReflObject)];
    u32 field_0x530;
};

struct UnkBeamOwner_ovb0138 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x220 - 0x68];
    UnkReflObject* animation;  // 0x220
    void* field_0x228;
    u8 pad_0x230[0x2b0 - 0x230];
    void* field_0x2b0;
};

struct UnkBeamShot_ovb0138 {
    u32 field_0x0;
    bool field_0x4;
};

struct UnkPowerBeamGun_ovb0138 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual bool vf_0x48();

    u8 pad_0x8[0x10];
    UnkBeamOwner_ovb0138* owner;  // 0x18
    u8 pad_0x20[0x1874 - 0x20];
    u32 field_0x1874;
    u8 pad_0x1878[0x8];
    bool field_0x1880;
};

void fn_710034f54c(void* object, u32 id, UnkBeamVec_ovb0138* dir);
void fn_71008538e8(UnkPowerBeamGun_ovb0138* self, UnkBeamShot_ovb0138* shot, const UnkBeamVec_ovb0138* pos,
                   const UnkBeamVec_ovb0138* dir, const UnkBeamVec_ovb0138* side, u32 id, bool flag);

bool fn_7100853720(UnkPowerBeamGun_ovb0138* self, UnkBeamShot_ovb0138* shot) {
    if (!self->owner || !self->vf_0x48())
        return false;
    self->field_0x1880 = false;
    UnkBeamOwner_ovb0138* owner = self->owner;
    f32 x = owner->x;
    f32 y = owner->y;
    f32 z = owner->z;
    UnkBeamVec_ovb0138 pos;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    if (void* target = owner->field_0x228) {
        u64 id = (*reinterpret_cast<UnkBeamIdHolder_ovb0138**>(
                      reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + 0xd58))->id;
        fn_710117364c(target, &id, reinterpret_cast<UnkGrappleAnswer_b50*>(&pos));
    }
    UnkBeamVec_ovb0138 dir;
    dir.x = 1.0f;
    dir.y = 0.0f;
    dir.z = 0.0f;
    u32 id = 0;
    if (UnkReflObject* animation = self->owner->animation) {
        if (fn_710009d50c(animation->vf_0x0(), fn_7100713bb0()))
            id = static_cast<UnkBeamAnimation_ovb0138*>(animation)->field_0x530;
        fn_710034f54c(self->owner->field_0x2b0, id, &dir);
    }
    UnkBeamVec_ovb0138 side = {-dir.y, dir.x, dir.z};
    fn_71008538e8(self, shot, &pos, &dir, &side, id, false);
    u32 kind = shot->field_0x4 ? 4 : 3;
    fn_7100ed6910(g_unk_71726af0b0, reinterpret_cast<const CVector3D*>(&pos), self->field_0x1874, kind, false);
    return true;
}
