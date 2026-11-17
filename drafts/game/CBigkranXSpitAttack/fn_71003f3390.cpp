// CBigkranXSpitAttack's unit: spawns one spit projectile. Loads the actor definition its owner's
// character class names; when the entity's movement is a magma ball with a projectile character
// class and the owner has a target, places it at `position`, aims the movement at `target` with the
// flight time `distance` over the class's speed, and keeps a weak reference to it in `spit`; an
// entity that was not launched is destroyed. Returns whether it was launched. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU341.h"
#include "game/UnkTU344.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkActorDefFactory;
struct UnkPowerUpLifeEntry_b52;
struct UnkPowerUpLifeType_b52;

// The projectile's actor definition, as CCharClassBigkranXAIComponent names it.
const CStrId* fn_71003f6358(UnkReflObject* char_class);

struct alignas(8) UnkSpitDefType_3f3390 {
    u32 id;
};

struct UnkSpitAI_3f3390 : UnkReflObject {
    u8 pad_0x8[0x8];
    UnkReflObject* char_class;  // 0x10
};

struct UnkSpitProjectileClass_3f3390 : UnkReflObject {
    u8 pad_0x8[0xcc];
    f32 speed;  // 0xd4
};

struct UnkSpitMovement_3f3390 : UnkReflObject {
    u8 pad_0x8[0x8];
    UnkReflObject* char_class;  // 0x10
    u8 pad_0x18[0x28];
    UnkVec3f target;  // 0x40
    u8 pad_0x4c[0x30];
    f32 time;  // 0x7c
    u8 pad_0x80[0x88];
    f32 field_0x108;
    u8 pad_0x10c[0x464];
    bool field_0x570;

    void vf_0x290(UnkActor* owner) {
        reinterpret_cast<void (*)(UnkSpitMovement_3f3390*, UnkActor*)>((*reinterpret_cast<void***>(this))[0x290 / 8])(this, owner);
    }
    void vf_0x2c0() {
        reinterpret_cast<void (*)(UnkSpitMovement_3f3390*)>((*reinterpret_cast<void***>(this))[0x2c0 / 8])(this);
    }
};
static_assert(__builtin_offsetof(UnkSpitMovement_3f3390, field_0x570) == 0x570, "layout");

struct UnkSpitEntity_3f3390 : UnkReflObject {
    u8 pad_0x8[0x1e8];
    UnkReflObject* movement;  // 0x1f0
};

struct UnkBigkranXSpitAttack_3f3390 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
};

template <const base::reflection::CClass* (*GetClass)()>
static inline bool IsA(UnkReflObject* object) {
    return fn_710009d50c(object->vf_0x0(), GetClass());
}

// The definition the owner's AI character class names, or the empty id.
static inline const CStrId* SpitName(UnkActor* owner) {
    if (owner) {
        UnkReflObject* ai = static_cast<UnkReflObject*>(owner->field_0x240);
        if (ai && IsA<fn_71003f4454>(ai)) {
            UnkReflObject* char_class = static_cast<UnkSpitAI_3f3390*>(ai)->char_class;
            if (char_class && IsA<fn_71003f5604>(char_class))
                return fn_71003f6358(char_class);
        }
    }
    return &g_unk_7101d470e0;
}

bool fn_71003f3390(UnkBigkranXSpitAttack_3f3390* self, const UnkVec3f* position, const UnkVec3f* target,
                   UnkWeakHandle** spit, f32 distance, f32 field_0x108) {
    CStrId name(*SpitName(self->owner));
    UnkSpitEntity_3f3390* entity = nullptr;
    UnkSpitMovement_3f3390* movement = nullptr;
    UnkSpitProjectileClass_3f3390* projectile = nullptr;
    if (name.data != &g_unk_7101d47108->empty) {
        UnkReflObject* spawned;
        {
            UnkSpitDefType_3f3390 type;
            UnkActorDefFactory* factory = g_unk_71726a0220;
            ++name.data->reads;
            u32 type_id = g_unk_7172a0ba48;
            const char* path = reinterpret_cast<const char*>(&name.data->String());
            type.id = type_id;
            spawned = reinterpret_cast<UnkReflObject*>(
                fn_710119438c(factory, path, &name, reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
        }
        if (spawned && IsA<fn_7100d70c20>(spawned)) {
            entity = static_cast<UnkSpitEntity_3f3390*>(spawned);
            UnkReflObject* m = entity->movement;
            if (m && IsA<fn_71006df3d4>(m)) {
                movement = static_cast<UnkSpitMovement_3f3390*>(m);
                UnkReflObject* c = movement->char_class;
                if (c && IsA<fn_7100ac0730>(c))
                    projectile = static_cast<UnkSpitProjectileClass_3f3390*>(c);
            }
        }
    }

    bool launched = false;
    UnkActor* aim = self->owner ? fn_7100d7c990(self->owner) : nullptr;
    if (projectile && aim) {
        movement->vf_0x290(self->owner);
        reinterpret_cast<UnkActor*>(entity)->vf_0xd0(position, true);
        launched = true;
        movement->target = *target;
        movement->field_0x570 = false;
        movement->field_0x108 = field_0x108;
        movement->time = distance / projectile->speed;
        movement->vf_0x2c0();
    }
    if (entity) {
        if (launched)
            AssignRef(spit, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(entity)));
        else
            fn_7100232c68(entity);
    }
    return launched;
}
