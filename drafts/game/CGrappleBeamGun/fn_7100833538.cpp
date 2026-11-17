// CGrappleBeamGun's unit (tools/tu_map.py unit 1301): drops the entity held at +0x1880 (running
// fn_7100232c68 on it first) and the movement held at +0x1888, loads the actor definition named at
// +0x1240 of the id table and, for an entity, holds it and its movement component, which is pointed
// back at `self`. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU341.h"
#include "game/UnkTU344.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkActorDefFactory;
struct UnkPowerUpLifeEntry_b52;
struct UnkPowerUpLifeType_b52;

extern u32 g_unk_7172a0ba4c;

struct UnkGrappleName_l833538 {
    u8 unk_0x0[0x10];
    char text[0x34];
    union {
        u32 use_count;  // 0x44
    };
};

struct alignas(8) UnkGrappleDefType_l833538 {
    u32 id;
};

struct UnkGrappleMovement_l833538 : UnkWeakTarget {
    u8 pad_0x8[0x528];
    void* owner;  // 0x530

    void vf_0x290(void* arg) {
        reinterpret_cast<void (*)(UnkGrappleMovement_l833538*, void*)>((*reinterpret_cast<void***>(this))[0x290 / 8])(
            this, arg);
    }
};

struct UnkGrappleBeamGun_l833538 {
    u8 pad_0x0[0x18];
    void* field_0x18;
    u8 pad_0x20[0x1880 - 0x20];
    UnkWeakHandle* entity;    // 0x1880
    UnkWeakHandle* movement;  // 0x1888
};

struct UnkRefl_fn_7100833f4c;
template <>
base::reflection::UnkReflObject* base::reflection::refl_FindTyped<UnkRefl_fn_7100833f4c>(void*);

static inline char* IdTable() {
    return reinterpret_cast<char*>(&base::reflection::refl_GetSingleton());
}

void fn_7100833538(UnkGrappleBeamGun_l833538* self) {
    if (UnkWeakHandle* old = self->entity) {
        if (old->target) {
            fn_7100232c68(old->target);
            ResetRef(&self->entity);
        }
    }
    ResetRef(&self->movement);

    UnkReflObject* entry;
    {
        UnkGrappleDefType_l833538 type;
        UnkActorDefFactory* factory = g_unk_71726a0220;
        CStrId* id = reinterpret_cast<CStrId*>(IdTable() + 0x1240);
        u32 type_id = g_unk_7172a0ba4c;
        UnkGrappleName_l833538* name = reinterpret_cast<UnkGrappleName_l833538*>(id->data);
        ++name->use_count;
        const char* text = reinterpret_cast<UnkGrappleName_l833538*>(id->data)->text;
        type.id = type_id;
        entry = reinterpret_cast<UnkReflObject*>(
            fn_710119438c(factory, text, id, reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
    }
    if (!entry)
        return;
    if (!fn_710009d50c(entry->vf_0x0(), fn_7100d70c20()))
        return;

    UnkWeakHandle* handle = fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(entry));
    AssignRef(&self->entity, handle);
    if (!handle || !handle->target)
        return;
    auto* movement = reinterpret_cast<UnkGrappleMovement_l833538*>(
        base::reflection::refl_FindTyped<UnkRefl_fn_7100833f4c>(handle->target));
    if (!movement)
        return;
    movement->vf_0x290(self->field_0x18);
    movement->owner = self;
    AssignRef(&self->movement, GetWeakHandle(movement));
}
