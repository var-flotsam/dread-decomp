// CMissileBaseGun's unit (tools/tu_map.py unit 1309): firing a missile. Once the gun may fire, takes the
// ammunition, spawns the missile actor from the factory by the gun's name, places it at the owner's
// position along the aim direction, sets up its movement (kind flags, suffix track name, aim components
// kept against the aim, sub-area), and plays the launch sound. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CGrabComponent.h"
#include "game/CMissileBaseGun.h"
#include "game/CModelUpdaterComponent.h"
#include "game/CShotManager/CTunableShotManager.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkBeamIdHolder_ovb0138.h"
#include "game/UnkBeamVec_ovb0138.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/UnkTU437.h"
#include "game/UnkTU54.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkHyperBeamSoundHandle {
    void* ref;
    s64 serial;
};

struct UnkGrappleAnswer_b50;
struct UnkPowerUpLifeEntry_b52;
struct UnkPowerUpLifeType_b52;
struct UnkMissileShotArg_l84ec4c;

struct alignas(8) UnkMissileDefType_l84ec4c {
    u32 id;
};

struct UnkMissileName_l84ec4c {
    u8 unk_0x0[0x10];
    char text[0x34];
    u32 use_count;  // 0x44
};

// The animation component's tunable part at +0x1f0 of the owner.
struct UnkMissileTune_l84ec4c {
    u8 pad_0x0[0x10c];
    f32 field_0x10c;
    f32 field_0x110;
    f32 field_0x114;
};

struct UnkMissileOwner_l84ec4c {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1f0 - 0x68];
    UnkMissileTune_l84ec4c* tune;  // 0x1f0
    u8 pad_0x1f8[0x220 - 0x1f8];
    UnkReflObject* animation;  // 0x220
    void* field_0x228;
    u8 pad_0x230[0x260 - 0x230];
    void* grab;  // 0x260
    u8 pad_0x268[0x2b0 - 0x268];
    void* field_0x2b0;
};

struct UnkMissileShotArg_l84ec4c {
    u32 kind;
    bool flag;
};

struct UnkMissileSound_l84ec4c {
    u8 pad_0x0[0x2e8];
    struct Timeline {
        virtual void vf_0x0();
    }* timeline;
};

struct UnkMissileAnim_l84ec4c : UnkReflObject {
    u8 pad[0x530 - sizeof(UnkReflObject)];
    u32 field_0x530;
};

// The spawned actor.
struct UnkMissileEntity_l84ec4c {
    void** vtable;
    u8 pad_0x8[0x2e0];
    struct Effect {
        virtual void vf_0x0();
    }* field_0x2e8;

    void SetPosition(const CVector3D* position, bool flag) {
        reinterpret_cast<void (*)(UnkMissileEntity_l84ec4c*, const CVector3D*, bool)>(vtable[0xd0 / 8])(this, position, flag);
    }
    void SetRotation(const CVector3D* angles, bool flag) {
        reinterpret_cast<void (*)(UnkMissileEntity_l84ec4c*, const CVector3D*, bool)>(vtable[0xd8 / 8])(this, angles, flag);
    }
};

struct UnkMissile_l84ec4c;

struct UnkMissileGun_l84ec4c {
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
    u8 pad_0x10[0x0];

    u64 field_0x8;
    CStrId name;  // 0x10
    UnkMissileOwner_l84ec4c* owner;  // 0x18
    u8 pad_0x20[0x1874 - 0x20];
    u32 field_0x1874;

    u32 Flags() { return reinterpret_cast<u32 (*)(UnkMissileGun_l84ec4c*)>((*reinterpret_cast<void***>(this))[0x198 / 8])(this); }
    f32 Float(u32 slot) { return reinterpret_cast<f32 (*)(UnkMissileGun_l84ec4c*)>((*reinterpret_cast<void***>(this))[slot / 8])(this); }
};

// The missile's movement object as the gun sets it up.
struct UnkMissile_l84ec4c {
    void** vtable;
    u8 pad_0x8[0x38];
    f32 dir_x;  // 0x40
    f32 dir_y;
    f32 dir_z;
    u8 pad_0x4c[0x1ec];
    u32 field_0x238;
    u32 field_0x23c;
    u32 field_0x240;
    u8 pad_0x244[0x104];
    u8 request[0xb4];  // 0x348
    u32 field_0x3fc;
    u8 pad_0x400[8];
    f32 field_0x408;
    f32 field_0x40c;
    u8 pad_0x410[0x188];
    CStrId suffix;  // 0x598
    u8 pad_0x5a0[0x58];
    bool field_0x5f8;
    u8 pad_0x5f9[0x1f];
    void* field_0x618;
};
static_assert(__builtin_offsetof(UnkMissile_l84ec4c, field_0x3fc) == 0x3fc, "layout");
static_assert(__builtin_offsetof(UnkMissile_l84ec4c, suffix) == 0x598, "layout");
static_assert(__builtin_offsetof(UnkMissile_l84ec4c, field_0x618) == 0x618, "layout");

void CMissileBaseGun_ConsumeOneAmmo(UnkMissileGun_l84ec4c* self);
bool fn_710084f30c(UnkMissileGun_l84ec4c* self, const CVector3D* position, const CVector3D* dir,
                   UnkMissileEntity_l84ec4c* entity, UnkMissile_l84ec4c* missile);
UnkMissile_l84ec4c* fn_7100554660(UnkMissileEntity_l84ec4c* entity);
void fn_7100ac7db4(UnkMissile_l84ec4c* missile, const char* name, bool flag);
void* fn_7100eeb730(UnkManager* manager, const base::math::CVector2D* point);

static inline void VCall(void* object, u32 slot, void* a) {
    reinterpret_cast<void (*)(void*, void*)>((*reinterpret_cast<void***>(object))[slot / 8])(object, a);
}

// An aim component is dropped when it points against the aim.
static inline f32 Keep(f32 value, f32 dir) {
    if (value > 0.0f && !(dir > 0.0f))
        return 0.0f;
    return ((dir >= 0.0f) & (value < 0.0f)) ? 0.0f : value;
}

bool fn_710084ec4c(UnkMissileGun_l84ec4c* self, UnkMissileShotArg_l84ec4c* shot) {
    if (!self->vf_0x48() || !self->owner)
        return false;
    CMissileBaseGun_ConsumeOneAmmo(self);
    UnkActorDefFactory* factory = g_unk_71726a0220;
    UnkMissileName_l84ec4c* name = reinterpret_cast<UnkMissileName_l84ec4c*>(self->name.data);
    ++name->use_count;
    UnkMissileDefType_l84ec4c type;
    type.id = g_unk_7172a0ba48;
    auto* entity = reinterpret_cast<UnkMissileEntity_l84ec4c*>(
        fn_710119438c(factory, name->text, &self->name, reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
    if (!entity)
        return false;
    if (!fn_710009d50c(reinterpret_cast<UnkReflObject*>(entity)->vf_0x0(), fn_7100d70c20()))
        return false;
    UnkMissileOwner_l84ec4c* owner = self->owner;
    CVector3D position;
    position.x = owner->x;
    position.y = owner->y;
    position.z = owner->z;
    if (void* target = owner->field_0x228) {
        u64 id = (*reinterpret_cast<UnkBeamIdHolder_ovb0138**>(
                      reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + 0xd58))->id;
        fn_710117364c(target, &id, reinterpret_cast<UnkGrappleAnswer_b50*>(&position));
    }
    CVector3D dir;
    dir.x = 1.0f;
    dir.y = 0.0f;
    dir.z = 0.0f;
    u32 anim_id = 0;
    if (UnkReflObject* animation = self->owner->animation) {
        if (fn_710009d50c(animation->vf_0x0(), fn_7100713bb0()))
            anim_id = static_cast<UnkMissileAnim_l84ec4c*>(animation)->field_0x530;
        fn_710034f54c(self->owner->field_0x2b0, anim_id, reinterpret_cast<UnkBeamVec_ovb0138*>(&dir));
    }
    entity->SetPosition(&position, true);
    {
        UnkVec3f angles = fn_710007a7f0(*reinterpret_cast<UnkVec3f*>(&dir));
        entity->SetRotation(reinterpret_cast<CVector3D*>(&angles), true);
    }
    UnkMissile_l84ec4c* missile = fn_7100554660(entity);
    if (missile) {
        missile->field_0x5f8 = shot->kind == 2;
        u32 flags = self->Flags();
        if (shot->kind == 2)
            flags |= 0x80;
        if (shot->flag)
            flags |= 0x400;
        VCall(missile, 0x2b8, self);
        missile->dir_x = dir.x;
        missile->dir_y = dir.y;
        missile->dir_z = dir.z;
        fn_7100ac7db4(missile, "Missile", false);
        VCall(missile, 0x290, self->owner);
        fn_71008c1538(reinterpret_cast<UnkRequest*>(missile->request), self->Float(0x180));
        missile->field_0x3fc = flags;
        missile->field_0x408 = self->Float(0x1a0);
        missile->field_0x40c = self->Float(0x1a8);
        if (self->owner && self->owner->animation) {
            UnkTrackQuery query = {fn_7100beff48(), nullptr, -1.0f};
            if (fn_710117b08c(reinterpret_cast<UnkAnimationComponent*>(self->owner->animation), &query) && query.result)
                missile->suffix = query.result->sources;
        }
        f32 a = 0.0f;
        f32 b = 0.0f;
        u32 c = 0;
        if (!(self->owner->grab && fn_710113d83c(self->owner->grab))) {
            if (UnkMissileTune_l84ec4c* tune = self->owner->tune) {
                a = tune->field_0x10c;
                b = tune->field_0x110;
                c = *reinterpret_cast<u32*>(&tune->field_0x114);
            }
            switch (anim_id) {
            case 0:
                a = Keep(a, dir.x);
                b = 0.0f;
                break;
            case 1:
                a = 0.0f;
                if (b >= 0.0f)
                    b = Keep(b, dir.y);
                else
                    b = 0.0f;
                break;
            case 2:
                a = 0.0f;
                if (b <= 0.0f)
                    b = Keep(b, dir.y);
                else
                    b = 0.0f;
                break;
            default:
                a = Keep(a, dir.x);
                b = Keep(b, dir.y);
                break;
            }
        }
        missile->field_0x238 = *reinterpret_cast<u32*>(&a);
        missile->field_0x23c = *reinterpret_cast<u32*>(&b);
        missile->field_0x240 = c;
        if (fn_710084f30c(self, &position, &dir, entity, missile)) {
            if (auto* effect = entity->field_0x2e8)
                reinterpret_cast<void (*)(void*, f32)>((*reinterpret_cast<void***>(effect))[0xb0 / 8])(effect, 0.0f);
        }
        VCall(missile, 0x2c0, nullptr);
        UnkManager* manager = g_unk_7101d47360;
        if (!manager) {
            manager = static_cast<UnkManager*>(fn_71000839a0(0xae8, 0x3f));
            fn_7100ee1f34(manager);
            g_unk_7101d47360 = manager;
        }
        void* area;
        {
            base::math::CVector2D point;
            fn_710007b430(&point, &position);
            area = fn_7100eeb730(manager, &point);
        }
        if (!area) {
            if (void* cameras = fn_7100defcec(g_unk_7101d47310)) {
                void* holder = *reinterpret_cast<void**>(static_cast<char*>(cameras) + 0xc38);
                area = holder ? *reinterpret_cast<void**>(static_cast<char*>(holder) + 0x10) : nullptr;
            }
        }
        missile->field_0x618 = area;
    }
    if (shot->kind == 2) {
        fn_7100ed6910(g_unk_71726af0b0, &position, 0xb, shot->flag ? 4 : 3, 0);
        fn_7100ed6af0(g_unk_71726af0b0, 0xb, 2);
    } else {
        fn_7100ed6910(g_unk_71726af0b0, &position, self->field_0x1874, 3, 0);
    }
    return true;
}
