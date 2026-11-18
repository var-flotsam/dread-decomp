// CSamusGunComponent's unit (tools/tu_map.py unit 1315): reacts to touching another actor. When the contact
// is blocked (kind 2, or the actor has the part fn_71005012bc finds, as its AI may decide) any held grab is
// released; otherwise an actor that may be pushed gets a knock-back away from the owner and its AI is told.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CSamusGunComponent.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU794.h"
#include "types.h"

struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;

struct UnkDamageRequest_l85b3d0 {
    u8 pad_0x0[0x100];

    UnkDamageRequest_l85b3d0() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkDamageRequest_l85b3d0() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};

struct UnkVec3_l85b3d0 {
    f32 x;
    f32 y;
    f32 z;

    UnkVec3_l85b3d0(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
};

struct UnkMovement_l85b3d0;
struct UnkMovementVtable_l85b3d0 {
    u8 pad_0x0[0x208];
    void (*vf_0x208)(UnkMovement_l85b3d0* self, const UnkVec3_l85b3d0* direction);
    u8 pad_0x210[0x390 - 0x210];
    void (*vf_0x390)(UnkMovement_l85b3d0* self, const UnkVec3_l85b3d0* impulse, UnkDamageRequest_l85b3d0* request);
};

struct UnkMovement_l85b3d0 {
    UnkMovementVtable_l85b3d0* vtable;
    u8 pad_0x8[0x7c0];
    f32 field_0x7c8;
};
static_assert(__builtin_offsetof(UnkMovement_l85b3d0, field_0x7c8) == 0x7c8, "layout");

struct UnkReflected_l85b3d0;
struct UnkCharClass_l85b3d0;
struct UnkActor_l85b3d0;
struct UnkAIVtable_l85b3d0 {
    const base::reflection::CType* (*vf_0x0)(UnkReflected_l85b3d0* self);
    u8 pad_0x8[0x690 - 0x8];
    void (*vf_0x690)(UnkReflected_l85b3d0* self, void* contact, bool* blocked);
    u8 pad_0x698[0x7b8 - 0x698];
    void (*vf_0x7b8)(UnkReflected_l85b3d0* self, UnkActor_l85b3d0* by, UnkDamageRequest_l85b3d0* request);
};

// An AI component and its character class share the reflected header.
struct UnkReflected_l85b3d0 {
    UnkAIVtable_l85b3d0* vtable;
    u8 pad_0x8[0x8];
    UnkCharClass_l85b3d0* char_class;  // 0x10, on an AI component
};

struct UnkCharClass_l85b3d0 : UnkReflected_l85b3d0 {
    u8 pad_0x18[0x4d5 - 0x18];
    bool field_0x4d5;
    u8 pad_0x4d6[0x50c - 0x4d6];
    bool field_0x50c;
};
static_assert(__builtin_offsetof(UnkCharClass_l85b3d0, field_0x50c) == 0x50c, "layout");

template <const base::reflection::CClass* (*GetClass)()>
static inline bool IsA(UnkReflected_l85b3d0* object) {
    const base::reflection::CType* type = object->vtable->vf_0x0(object);
    return fn_710009d50c(type, GetClass());
}

struct UnkLife_l85b3d0 {
    u8 pad_0x0[0x70];
    s32 state;  // 0x70
};

struct UnkReaction_l85b3d0;
struct UnkReactionVtable_l85b3d0 {
    u8 pad_0x0[0x1a0];
    void (*vf_0x1a0)(UnkReaction_l85b3d0* self);
};
struct UnkReaction_l85b3d0 {
    UnkReactionVtable_l85b3d0* vtable;
};

struct UnkActor_l85b3d0 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0x1b8 - 0x48];
    UnkLife_l85b3d0* life;  // 0x1b8
    u8 pad_0x1c0[0x208 - 0x1c0];
    UnkMovement_l85b3d0* movement;  // 0x208
    u8 pad_0x210[0x230 - 0x210];
    void* field_0x230;
    u8 pad_0x238[0x8];
    UnkReflected_l85b3d0* ai;  // 0x240
    u8 pad_0x248[0x258 - 0x248];
    UnkReaction_l85b3d0* reaction;  // 0x258
};
static_assert(__builtin_offsetof(UnkActor_l85b3d0, reaction) == 0x258, "layout");

struct UnkRefCounted_l85b3d0;
struct UnkRefCountedVtable_l85b3d0 {
    u8 pad_0x0[0x10];
    void (*vf_0x10)(UnkRefCounted_l85b3d0* self);
};
struct UnkRefCounted_l85b3d0 {
    UnkRefCountedVtable_l85b3d0* vtable;
    s32 refs;  // 0x8
};

struct UnkHeld_l85b3d0 {
    UnkRefCounted_l85b3d0* object;
    u8 pad_0x8[0x10];
};

struct UnkSamusGun_l85b3d0 {
    u8 pad_0x0[0x30];
    UnkActor_l85b3d0* owner;  // 0x30
    u8 pad_0x38[0x340 - 0x38];
    bool holding;  // 0x340
    u32 field_0x344;
    UnkHeld_l85b3d0* held;  // 0x348
    u32 held_count;  // 0x350
};
static_assert(__builtin_offsetof(UnkSamusGun_l85b3d0, held_count) == 0x350, "layout");

// Bosses whose class allows it are pushed harder.
static inline f32 PushSpeed(UnkReflected_l85b3d0* ai) {
    if (ai && IsA<fn_71003ab524>(ai)) {
        UnkCharClass_l85b3d0* boss = ai->char_class;
        if (boss && IsA<fn_71004186ec>(boss) && !boss->field_0x50c)
            return 2000.0f;
    }
    return 1300.0f;
}

void fn_710085b3d0(UnkSamusGun_l85b3d0* self, UnkActor_l85b3d0* other, const s32* kind, void* contact) {
    bool blocked = *kind == 2 ? true : fn_71005012bc(reinterpret_cast<UnkDoorLifeOwner_b56*>(other)) != nullptr;
    if (UnkReflected_l85b3d0* ai = other->ai)
        ai->vtable->vf_0x690(ai, contact, &blocked);

    if (blocked) {
        if (!self->holding)
            return;
        self->holding = false;
        self->field_0x344 = 0;
        for (u64 i = 0; i < self->held_count; ++i) {
            UnkRefCounted_l85b3d0* object = self->held[i].object;
            if (object && --object->refs == 0)
                object->vtable->vf_0x10(object);
        }
        self->held_count = 0;
        if (UnkActor_l85b3d0* owner = self->owner) {
            fn_7100d7c94c(reinterpret_cast<UnkActor*>(owner), &g_unk_710164ad68);
            fn_7100d7c94c(reinterpret_cast<UnkActor*>(self->owner), &g_unk_710164ad60);
        }
        return;
    }

    bool alive = other->life && other->life->state != 2;
    bool pushable = false;
    if (UnkReflected_l85b3d0* ai = other->ai) {
        UnkCharClass_l85b3d0* charClass = ai->char_class;
        if (charClass && IsA<fn_7100408b2c>(charClass))
            pushable = charClass->field_0x4d5;
    }
    if (!(alive || pushable) || other->field_0x230)
        return;

    UnkDamageRequest_l85b3d0 request;
    UnkMovement_l85b3d0* movement;
    if (self->owner && (movement = self->owner->movement)) {
        UnkVec3_l85b3d0 impulse(PushSpeed(other->ai), 1500.0f, 0.0f);
        if (self->owner->x - other->x < 0.0f)
            impulse.x = -impulse.x;
        movement->vtable->vf_0x390(movement, &impulse, &request);
        movement->field_0x7c8 = 0.2f;
        UnkVec3_l85b3d0 direction(impulse.x > 0.0f ? 1.0f : -1.0f, 0.0f, 0.0f);
        movement->vtable->vf_0x208(movement, &direction);
    }

    UnkReflected_l85b3d0* ai = other->ai;
    if (ai && IsA<fn_710066167c>(ai))
        ai->vtable->vf_0x7b8(ai, self->owner, &request);
    else if (other->reaction)
        other->reaction->vtable->vf_0x1a0(other->reaction);
}
