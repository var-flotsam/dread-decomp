// CSluggerSpitAttack's unit (unit 1060): spawns the spit projectile the slugger's character class names,
// places it, aims its projectile movement (direction, speed, height) and keeps a weak handle to it; when it
// cannot be aimed the spawned entity is dropped again. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU341.h"
#include "game/UnkTU344.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

// The slugger AI's character class: vtable slot 0xd8 names the projectile to spawn.
struct UnkSluggerAIClass_l6c32a0 {
    void** vtable;

    const CStrId* ProjectileId() {
        return reinterpret_cast<const CStrId* (*)(UnkSluggerAIClass_l6c32a0*)>(vtable[0xd8 / 8])(this);
    }
};

// The slugger AI component: its character class at +0x10.
struct UnkSluggerAI_l6c32a0 {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkReflObject* charclass;  // +0x10
};

struct UnkSpitAttack_l6c32a0 {
    void** vtable;
    UnkActor* actor;  // +0x8
};

// A spawned entity: its movement component at +0x1f0.
struct UnkSpitEntity_l6c32a0 {
    void** vtable;
    u8 pad_0x8[0x1e8];
    UnkReflObject* movement;  // +0x1f0

    // vtable slot 0xd0: places the entity at `pos`.
    void SetPosition(const UnkVec3f* pos, bool flag) {
        reinterpret_cast<void (*)(UnkSpitEntity_l6c32a0*, const UnkVec3f*, bool)>(vtable[0xd0 / 8])(this, pos, flag);
    }
};

struct UnkSpitDirection_l6c32a0 {
    f32 x;
    f32 y;
    f32 z;
};

// The projectile movement's character class: the speed scale at +0xd4.
struct UnkProjectileMovementClass_l6c32a0 {
    u8 pad_0x0[0xd4];
    f32 field_0xd4;
};

struct UnkProjectileMovement_l6c32a0 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkSpitDirection_l6c32a0 direction;  // +0x40
    u8 pad_0x4c[0x30];
    f32 field_0x7c;
    u8 pad_0x80[0x88];
    f32 field_0x108;
    u8 pad_0x10c[0x464];
    bool field_0x570;

    // vtable slot 0x290: the actor that shot it.
    void SetOwner(UnkActor* owner) {
        reinterpret_cast<void (*)(UnkProjectileMovement_l6c32a0*, UnkActor*)>(vtable[0x290 / 8])(this, owner);
    }
    // vtable slot 0x2c0: starts it.
    void Start() { reinterpret_cast<void (*)(UnkProjectileMovement_l6c32a0*)>(vtable[0x2c0 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkProjectileMovement_l6c32a0, field_0x570) == 0x570, "layout");

// Whether `obj` is of the class `Get` returns (the object's class is read first).
template <const base::reflection::CClass* (*Get)()>
inline bool IsA(UnkReflObject* obj) {
    const base::reflection::CClass* cls = obj->vf_0x0();
    return fn_710009d50c(cls, Get());
}

// The id of the projectile the slugger's character class names, or the empty id.
static inline const CStrId* ProjectileId(UnkSpitAttack_l6c32a0* self) {
    if (UnkActor* actor = self->actor) {
        UnkReflObject* ai = static_cast<UnkReflObject*>(actor->field_0x240);
        if (ai && IsA<fn_71006c577c>(ai)) {
            UnkReflObject* charclass = reinterpret_cast<UnkSluggerAI_l6c32a0*>(ai)->charclass;
            if (charclass && IsA<fn_71006c4de4>(charclass))
                return reinterpret_cast<UnkSluggerAIClass_l6c32a0*>(charclass)->ProjectileId();
        }
    }
    return &g_unk_7101d470e0;
}

bool fn_71006c32a0(UnkSpitAttack_l6c32a0* self, const UnkVec3f* position, const UnkSpitDirection_l6c32a0* direction,
                   UnkWeakHandle** out, f32 speed, f32 height) {
    CStrId id(*ProjectileId(self));
    UnkReflObject* spawned = nullptr;
    if (id.data != &g_unk_7101d47108->empty) {
        UnkActorDefFactory* factory = g_unk_71726a0220;
        ++id.data->reads;
        u32 type = g_unk_7172a0ba48;
        spawned = reinterpret_cast<UnkReflObject*>(
            fn_710119438c(factory, reinterpret_cast<const char*>(&id.data->String()), &id,
                          reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
    }
    UnkSpitEntity_l6c32a0* entity =
        spawned && IsA<fn_7100d70c20>(spawned) ? reinterpret_cast<UnkSpitEntity_l6c32a0*>(spawned) : nullptr;

    UnkProjectileMovement_l6c32a0* movement = nullptr;
    UnkProjectileMovementClass_l6c32a0* movement_class = nullptr;
    if (entity) {
        UnkReflObject* component = entity->movement;
        if (component && IsA<fn_7100ac2434>(component)) {
            movement = reinterpret_cast<UnkProjectileMovement_l6c32a0*>(component);
            UnkReflObject* charclass = reinterpret_cast<UnkSluggerAI_l6c32a0*>(component)->charclass;
            if (charclass && IsA<fn_7100ac0730>(charclass))
                movement_class = reinterpret_cast<UnkProjectileMovementClass_l6c32a0*>(charclass);
        }
    }

    bool aimed = false;
    UnkActor* target = self->actor ? fn_7100d7c990(self->actor) : nullptr;
    if (movement_class && target) {
        movement->SetOwner(self->actor);
        reinterpret_cast<UnkSpitEntity_l6c32a0*>(entity)->SetPosition(position, true);
        aimed = true;
        movement->direction.x = direction->x;
        movement->direction.y = direction->y;
        movement->direction.z = direction->z;
        movement->field_0x570 = false;
        movement->field_0x108 = height;
        movement->field_0x7c = speed / movement_class->field_0xd4;
        movement->Start();
    }

    if (entity) {
        if (aimed)
            AssignRef(out, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(entity)));
        else
            fn_7100232c68(entity);
    }
    return aimed;
}
