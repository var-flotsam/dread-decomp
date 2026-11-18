// CSPBGun's unit (tools/tu_map.py unit 1317): firing. Keeps the shot (+0x18a0); once the gun's slot 0x48
// allows it and the shot is live, fires a projectile (fn_710085ddb4) at each entry of the spread (+0x18a8)
// around the owner's position along the animation's bone direction, steps the spread's cooldown and row,
// then calls the script's "bb" hook. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkTU437.h"
#include "types.h"

using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkGrappleAnswer_b50;


// As src/game/CPowerBeamGun/CPowerBeamGun_Fire_ovb0138.cpp defines them.
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

// As src/game/CPowerBeamGun/CPowerBeamGun_Fire_ovb0138.cpp declares it.

// The shot being fired: copied whole into the gun, which makes it 6 bytes of data.
struct UnkSPBShot_l85db1c {
    u32 field_0x0;
    bool live;  // 0x4
    u8 field_0x5;

    UnkSPBShot_l85db1c();
};

struct UnkSPBOwner_l85db1c {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1e0 - 0x68];
    UnkScriptCaller* script;  // 0x1e0
    u8 pad_0x1e8[0x220 - 0x1e8];
    UnkReflObject* animation;  // 0x220
    void* field_0x228;
    u8 pad_0x230[0x2b0 - 0x230];
    void* field_0x2b0;
};

struct UnkSPBScript_l85db1c {
    u8 pad_0x0[0x10];
    const char* name;  // 0x10
    u8 pad_0x18[0x44 - 0x18];
    s32 calls;  // 0x44
};

// As src/game/UnkTU1303/fn_71008372ac.cpp defines it.
struct UnkSpreadEntry_ovb0262g {
    bool ok;  // 0x0
    CVector3D offset;  // 0x4
    f32 scale;  // 0x10
    f32 extra;  // 0x14
    UnkSpreadEntry_ovb0262g() : ok(false), offset{0.0f, 0.0f, 0.0f}, scale(0.0f), extra(0.0f) {}
};

struct UnkSpread_ovb0262g;
// As src/game/UnkTU1303/fn_71008372ac.cpp defines it.
UnkSpreadEntry_ovb0262g fn_71008372ac(UnkSpread_ovb0262g* self, u32 index, const CVector3D* direction,
                                      const CVector3D* spread);

// The gun's spread of shots, as fn_71008372ac reads it (UnkSpread_ovb0262g), with the members this
// function steps: the cooldown (fn_71008372ac gives nothing while it is set) and the table row.
struct UnkSPBSpread_l85db1c {
    u8 pad_0x0[0x20];
    u32 count;  // 0x20
    u32 rowCount;  // 0x24
    u8 pad_0x28[0x34 - 0x28];
    u32 reload;  // 0x34
    u32 cooldown;  // 0x38
    u32 row;  // 0x3c
};

struct UnkSPBGun_l85db1c {
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
    UnkSPBOwner_l85db1c* owner;  // 0x18
    u8 pad_0x20[0x70 - 0x20];
    UnkSPBScript_l85db1c* script;  // 0x70
    bool field_0x78;
    u8 pad_0x79[0x18a0 - 0x79];
    UnkSPBShot_l85db1c shot;  // 0x18a0
    UnkSPBSpread_l85db1c spread;  // 0x18a8
};
bool fn_710085ddb4(UnkSPBGun_l85db1c* self, UnkSPBShot_l85db1c* shot, const UnkBeamVec_ovb0138* pos,
                   const UnkBeamVec_ovb0138* dir, const UnkBeamVec_ovb0138* side, u32 id);

bool fn_710085db1c(UnkSPBGun_l85db1c* self, UnkSPBShot_l85db1c* shot) {
    self->shot = *shot;
    if (!self->owner || !self->vf_0x48())
        return false;

    if (shot->live) {
        UnkSPBOwner_l85db1c* owner = self->owner;
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

        for (u32 i = 0; i < self->spread.count; i++) {
            UnkSpreadEntry_ovb0262g point =
                fn_71008372ac(reinterpret_cast<UnkSpread_ovb0262g*>(&self->spread), i,
                              reinterpret_cast<const CVector3D*>(&dir), reinterpret_cast<const CVector3D*>(&side));
            if (!point.ok)
                continue;
            UnkBeamVec_ovb0138 at;
            at.x = pos.x + point.offset.x;
            at.y = pos.y + point.offset.y;
            at.z = pos.z + point.offset.z;
            if (!fn_710085ddb4(self, shot, &at, &dir, &side, id))
                break;
        }

        UnkSPBSpread_l85db1c& spread = self->spread;
        if (spread.cooldown != 0) {
            spread.cooldown--;
        } else {
            spread.cooldown = spread.reload;
            spread.row = (spread.row + 1) % spread.rowCount;
        }
    }

    UnkScriptCaller* script = self->owner->script;
    self->script->calls++;
    fn_7100305088(script, self->script->name, "bb", shot->live ? self->field_0x78 : false, false);
    return true;
}
