// CAttackComponent (unit 2706): the base collision handler. A hit on another actor's CCollisionComponent
// goes to the owner's CProjectileMovement (fn_7100ac62c8) or else to the owner's attack listener; it is
// taken when that answers 1 or 4. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::UnkReflObject;

// The declaration's types (src/game/CEmmyAttackComponent/CEmmyAttackComponent_TileHit_A3_b64.cpp).
struct UnkTileHitLink1_A3_b64 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* object;
};
struct UnkTileHitLink0_A3_b64 {
    u8 pad_0x0[0x10];
    UnkTileHitLink1_A3_b64* link;
};

struct UnkTileHit_A3_b64 {
    u8 pad_0x0[0x10];
    UnkTileHitLink0_A3_b64* link;
    u8 pad_0x18[0xc];
    u8 at[1];  // 0x24
};

struct UnkTileHitOwner_A3_b64 {
    u8 pad_0x0[0x240];
    base::reflection::UnkReflObject* ai;
    u8 pad_0x248[0x18];
    void* field_0x260;
};

struct UnkEmmyAttackComponent_A3_b64 {
    u8 pad_0x0[0x30];
    UnkTileHitOwner_A3_b64* owner;
};

// The hit as this handler reads it: the collision and the source reference after it.
struct UnkAttackSource_l137c8c {
    u8 pad_0x0[0x10];
    void* object;  // 0x10
};

struct UnkAttackHit_l137c8c {
    u8 pad_0x0[0x10];
    UnkTileHitLink0_A3_b64* link;      // 0x10
    UnkAttackSource_l137c8c* source;  // 0x18
};

struct UnkAttackGate_l137c8c {
    void** vtable;
    u8 pad_0x8[0x68];
    s32 state;  // 0x70

    bool Allows() { return reinterpret_cast<bool (*)(UnkAttackGate_l137c8c*)>(vtable[0x198 / 8])(this); }
};

struct UnkAttackOwner_l137c8c;

struct UnkAttackListener_l137c8c {
    void** vtable;

    s32 OnHit(UnkAttackOwner_l137c8c* target, UnkTileHitLink0_A3_b64** hit, void* source, bool flag) {
        using Fn = s32 (*)(UnkAttackListener_l137c8c*, UnkAttackOwner_l137c8c*, UnkTileHitLink0_A3_b64**, void*, bool);
        return reinterpret_cast<Fn>(vtable[0x198 / 8])(this, target, hit, source, flag);
    }
};

// The owner actor as this handler reads it.
struct UnkAttackOwner_l137c8c {
    u8 pad_0x0[0x1b5];
    bool field_0x1b5;
    u8 pad_0x1b6[2];
    UnkAttackGate_l137c8c* gate;  // 0x1b8
    u8 pad_0x1c0[0x1f0 - 0x1c0];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x258 - 0x1f8];
    UnkAttackListener_l137c8c* listener;  // 0x258
};

struct UnkAttackCollision_l137c8c : UnkReflObject {
    u8 pad_0x8[0x28];
    UnkAttackOwner_l137c8c* owner;  // 0x30
};

s32 fn_7100ac62c8(UnkReflObject* movement, UnkAttackOwner_l137c8c* target, UnkTileHitLink0_A3_b64** hit);

static inline UnkAttackOwner_l137c8c* Owner(UnkEmmyAttackComponent_A3_b64* self) {
    return reinterpret_cast<UnkAttackOwner_l137c8c*>(self->owner);
}

bool fn_7101137c8c(UnkEmmyAttackComponent_A3_b64* self, UnkTileHit_A3_b64* hit) {
    UnkReflObject* component = hit->link->link->object;
    if (!component || !fn_710009d50c(component->vf_0x0(), fn_710085fc44()))
        return false;
    UnkAttackOwner_l137c8c* target = static_cast<UnkAttackCollision_l137c8c*>(component)->owner;
    if (!target || target == Owner(self))
        return false;
    if (Owner(self)->field_0x1b5)
        return false;
    UnkAttackGate_l137c8c* gate = Owner(self)->gate;
    if (gate && gate->state == 2) {
        if (!target->gate || !target->gate->Allows())
            return false;
    }

    s32 result;
    UnkReflObject* movement = Owner(self)->movement;
    if (movement && fn_710009d50c(movement->vf_0x0(), fn_7100ac2434())) {
        result = fn_7100ac62c8(movement, target, &hit->link);
    } else {
        UnkAttackListener_l137c8c* listener = Owner(self)->listener;
        if (!listener)
            return false;
        UnkAttackSource_l137c8c* source = reinterpret_cast<UnkAttackHit_l137c8c*>(hit)->source;
        result = listener->OnHit(target, &hit->link, source ? source->object : nullptr, false);
    }
    // Taken (1) or taken and finished (4).
    switch (result) {
    case 1:
    case 4:
        return true;
    default:
        return false;
    }
}
