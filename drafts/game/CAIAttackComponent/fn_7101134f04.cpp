// CAIAttackComponent unit (tools/tu_map.py unit 2705): a hit on a target. While the target slides through a
// floor tunnel, a hit is refused (6) when the owner's collision is among what the target's shape touches; so is
// one the AttackFail track or the attack itself turns away. A landed hit (1) is passed on to the attack, the AI
// and the weapon movement, plays the hit timeline and lets a projectile react. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityShinespark.h"
#include "game/CCharacterMovement.h"
#include "game/CMeleeComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU341.h"
#include "game/UnkTimelineCalls.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkCollisionObject;
bool fn_7100d5bfbc(void* tracker, u32 mask, UnkCollisionObject* object, void* arg, u32);

// The hits of a collision query: an inline array of up to eight 0x48-byte records after the count,
// each starting with two reference-counted pointers (the hit object's handle first). The
// destructor (vtable g_unk_7101a23478) releases both, as fn_7100331700 does.
struct UnkHitComponent : base::reflection::UnkReflObject {
    u8 pad_0x8[0x28];
    void* field_0x30;
};
struct UnkHitEntity {
    u8 pad_0x0[0x10];
    UnkHitComponent* field_0x10;  // the hit component
};
struct UnkHitHandle : UnkRefObject {
    UnkHitEntity* field_0x10;
};
struct __attribute__((packed)) UnkHitRecord {
    UnkHitHandle* field_0x0;
    UnkRefObject* field_0x8;
    u8 pad_0x10[0x38];
};

static inline void ReleaseRef(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

struct __attribute__((packed)) UnkHitList {
    void** vtable;
    u32 count;
    u8 storage[8 * sizeof(UnkHitRecord)];

    UnkHitRecord* items() { return reinterpret_cast<UnkHitRecord*>(storage); }

    UnkHitList() {
        vtable = g_unk_7101a23478 + 2;
        count = 0;
    }
    ~UnkHitList() {
        vtable = g_unk_7101a23478 + 2;
        for (u64 i = 0; i < count; i++) {
            ReleaseRef(items()[i].field_0x8);
            ReleaseRef(items()[i].field_0x0);
        }
        count = 0;
    }
    UnkHitRecord* begin() { return items(); }
    UnkHitRecord* end() { return reinterpret_cast<UnkHitRecord*>(storage + count * sizeof(UnkHitRecord)); }
};

// The default timeline mask: a guarded static built from another (zero) one.
struct UnkHitStaticZero_l134f04 {
    u32 value;
    UnkHitStaticZero_l134f04() : value(0) {}
};

inline const UnkHitStaticZero_l134f04& UnkHitStaticZero() {
    static UnkHitStaticZero_l134f04 s_value asm("g_unk_7101d57cc8");
    return s_value;
}

inline u32 UnkHitDefaultMask() {
    static u32 s_mask asm("g_unk_7101d57cb8") = ~UnkHitStaticZero().value & 0xff;
    return s_mask;
}

__attribute__((visibility("hidden"))) extern u64 g_unk_710164cfe0;  // the hit timeline id

// The shape fn_7100976588 gives: queried only while it has a +0x158.
struct UnkHitShape_l134f04 {
    u8 pad_0x0[0x158];
    void* field_0x158;
};

struct UnkHitSlideState_l134f04 {
    u8 pad_0x0[0x10];
    s32 state;  // 3: sliding through a floor tunnel
};

struct UnkHitSlide_l134f04 {
    u8 pad_0x0[0x2d08];
    UnkHitSlideState_l134f04* field_0x2d08;
};

struct UnkHitTarget_l134f04 {
    u8 pad_0x0[0x200];
    UnkFlagged* field_0x200;
    UnkHitSlide_l134f04* field_0x208;
    void* field_0x210;
    u8 pad_0x218[0x220 - 0x218];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x2f8 - 0x228];
    UnkMeleeComponent* melee;
};

struct UnkHitOwner_l134f04;

struct UnkHitAttack_l134f04 {
    virtual void vf_0x0();
    bool vf_0x60(UnkHitTarget_l134f04* target, void* hit, void* extra) {
        return reinterpret_cast<bool (*)(UnkHitAttack_l134f04*, UnkHitTarget_l134f04*, void*, void*)>(
            (*reinterpret_cast<void***>(this))[0x60 / 8])(this, target, hit, extra);
    }
    void vf_0x88(UnkHitTarget_l134f04* target, void* hit) {
        reinterpret_cast<void (*)(UnkHitAttack_l134f04*, UnkHitTarget_l134f04*, void*)>(
            (*reinterpret_cast<void***>(this))[0x88 / 8])(this, target, hit);
    }
    void vf_0x90(UnkHitOwner_l134f04* owner, UnkHitTarget_l134f04* target) {
        reinterpret_cast<void (*)(UnkHitAttack_l134f04*, UnkHitOwner_l134f04*, UnkHitTarget_l134f04*)>(
            (*reinterpret_cast<void***>(this))[0x90 / 8])(this, owner, target);
    }
};

// What the owner's AI (+0x240) and weapon movement (+0x1f0) are told about a hit, by vtable slot.
struct UnkHitListener_l134f04 {
    virtual void vf_0x0();
    void Call(u32 slot, UnkHitTarget_l134f04* target, void* hit) {
        reinterpret_cast<void (*)(UnkHitListener_l134f04*, UnkHitTarget_l134f04*, void*)>(
            (*reinterpret_cast<void***>(this))[slot / 8])(this, target, hit);
    }
    void vf_0x280(void* hit) {
        reinterpret_cast<void (*)(UnkHitListener_l134f04*, void*)>((*reinterpret_cast<void***>(this))[0x280 / 8])(
            this, hit);
    }
};

struct UnkHitProjectile_l134f04 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x118 - 0x8];
    bool field_0x118;
};

struct UnkHitMovement_l134f04 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x8];
    UnkHitProjectile_l134f04* field_0x10;
};

struct UnkHitOwner_l134f04 {
    u8 pad_0x0[0x1f0];
    UnkHitMovement_l134f04* movement;
    u8 pad_0x1f8[0x220 - 0x1f8];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x240 - 0x228];
    UnkHitListener_l134f04* ai;
    u8 pad_0x248[0x2e8 - 0x248];
    UnkTimelineComponent* timeline;
};

struct UnkHitSettings_l134f04 {
    u8 pad_0x0[0xf8];
    bool field_0xf8;
};

struct UnkAIAttackComponent_l134f04 {
    u8 pad_0x0[0x30];
    UnkHitOwner_l134f04* owner;
    u8 pad_0x38[0x1a0 - 0x38];
    UnkHitAttack_l134f04* attack;
    bool field_0x1a8;
    u8 pad_0x1a9[0x340 - 0x1a9];
    UnkHitSettings_l134f04* settings;
};

s32 fn_7101136bb4(UnkAIAttackComponent_l134f04* self, UnkHitTarget_l134f04* target, void* hit, void* info,
                  void* extra);

// Whether a hit may land in a floor tunnel: false only while the target slides through one (or counts as
// doing so) and its shape touches the owner's collision.
static bool MayHitInTunnel(UnkAIAttackComponent_l134f04* self, UnkHitTarget_l134f04* target) {
    UnkHitSlide_l134f04* slide = target->field_0x208;
    bool sliding = (slide && slide->field_0x2d08 && slide->field_0x2d08->state == 3) || target->field_0x210;
    if (!sliding) {
        UnkAnimationComponent* anim = target->field_0x220;
        if (!anim || !fn_710117b348(anim, fn_7100c2b7c8()))
            return true;
    }
    UnkHitShape_l134f04* shape = reinterpret_cast<UnkHitShape_l134f04*>(fn_7100976588(target->field_0x200));
    if (!shape || !shape->field_0x158)
        return true;
    UnkGameManager* gm = g_unk_7101d47310;
    if (!gm->field_0x608)
        return true;
    void* tracker = gm->field_0x608->field_0x820;
    if (!tracker)
        return true;
    bool clear = true;
    UnkHitList hits;
    UnkCollisionObject* object = reinterpret_cast<UnkCollisionObject*>(shape);
    if (fn_7100d5bfbc(tracker, 0x100000, object, &hits, 0) || fn_7100d5bfbc(tracker, 0x8000000, object, &hits, 0)) {
        UnkHitRecord* end = hits.end();
        for (UnkHitRecord* it = hits.begin(); it != end; ++it) {
            if (!it->field_0x0 || !it->field_0x0->field_0x10)
                continue;
            UnkHitComponent* c = it->field_0x0->field_0x10->field_0x10;
            if (!c || !fn_710009d50c(c->vf_0x0(), fn_710085fc44()))
                continue;
            if (c->field_0x30 == self->owner)
                clear = false;
        }
    }
    return clear;
}

s32 fn_7101134f04(UnkAIAttackComponent_l134f04* self, UnkHitTarget_l134f04* target, void* hit, void* info,
                  void* extra) {
    if (self->settings->field_0xf8 && !MayHitInTunnel(self, target))
        return 6;
    UnkAnimationComponent* anim = self->owner->field_0x220;
    if (anim && fn_710117b348(anim, fn_7100c0d1d8()) && target->melee && fn_710092b604(target->melee))
        return 6;
    if (self->attack) {
        if (!self->attack->vf_0x60(target, hit, extra))
            return 6;
        if (self->attack)
            self->attack->vf_0x88(target, hit);
    }
    UnkHitListener_l134f04* ai = self->owner->ai;
    if (ai)
        ai->Call(0x2b8, target, hit);
    UnkHitMovement_l134f04* movement = self->owner->movement;
    UnkHitListener_l134f04* weapon = nullptr;
    bool told_weapon = false;
    if (movement && fn_710009d50c(movement->vf_0x0(), fn_7100ac6c48())) {
        weapon = reinterpret_cast<UnkHitListener_l134f04*>(movement);
        weapon->vf_0x280(hit);
        told_weapon = true;
    }
    s32 result = fn_7101136bb4(self, target, hit, info, extra);
    if (result != 1) {
        if (ai)
            ai->Call(0x2c8, target, hit);
        return result;
    }
    self->field_0x1a8 = true;
    if (self->attack)
        self->attack->vf_0x90(self->owner, target);
    if (ai)
        ai->Call(0x2c0, target, hit);
    if (told_weapon)
        weapon->Call(0x288, target, hit);
    if (UnkTimelineComponent* timeline = self->owner->timeline) {
        alignas(8) u32 value = UnkHitDefaultMask();
        fn_7100a2cb7c(timeline, &g_unk_710164cfe0, &value, false, -1.0f);
    }
    if (UnkHitMovement_l134f04* projectile = self->owner->movement) {
        if (fn_710009d50c(projectile->vf_0x0(), fn_7100ac2434())) {
            UnkHitProjectile_l134f04* cls = projectile->field_0x10;
            if (cls && fn_710009d50c(cls->vf_0x0(), fn_7100ac0730()) && cls->field_0x118)
                fn_7100232c68(self->owner);
        }
    }
    return 1;
}
