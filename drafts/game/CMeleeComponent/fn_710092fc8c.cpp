// CMeleeComponent's unit: lands a melee hit on another actor: a damage request to its life component
// (flag 0x40000 while the owner plays a dash melee track), else its AI is told, else its hit receiver, else an
// item's life component takes 1000 damage. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU357.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkRequest;
struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;

struct UnkMeleeHitSource_l92fc8c {
    u8 pad_0x0[0x10];
    u64 field_0x10;
};

// What the hit is: its source, then a value and three points.
struct UnkMeleeHit_l92fc8c {
    UnkMeleeHitSource_l92fc8c* source;
    u8 pad_0x8[0x8];
    u32 field_0x10;
    UnkVec3f field_0x14;
    UnkVec3f field_0x20;
    UnkVec3f field_0x2c;
    u32 field_0x38;
    u32 field_0x3c;
};

struct UnkMeleeDamageRequest_l92fc8c {
    u8 pad_0x0[0x8];
    UnkWeakHandle* source;  // 0x8
    UnkWeakHandle* target;  // 0x10
    u8 pad_0x18[0x48];
    u64 field_0x60;
    u32 field_0x68;
    UnkVec3f field_0x6c;
    UnkVec3f field_0x78;
    UnkVec3f field_0x84;
    u32 field_0x90;
    u32 field_0x94;
    u64 field_0x98;
    u8 pad_0xa0[0x14];
    u32 flags;  // 0xb4
    u8 pad_0xb8[0x48];

    // Takes the hit's source and points.
    void SetHit(u64 arg, const UnkMeleeHit_l92fc8c* hit) {
        field_0x98 = arg;
        field_0x60 = hit->source ? hit->source->field_0x10 : 0;
        field_0x68 = hit->field_0x10;
        field_0x6c = hit->field_0x14;
        field_0x78 = hit->field_0x20;
        field_0x84 = hit->field_0x2c;
        field_0x90 = hit->field_0x38;
        field_0x94 = hit->field_0x3c;
    }

    UnkMeleeDamageRequest_l92fc8c() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkMeleeDamageRequest_l92fc8c() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};
static_assert(sizeof(UnkMeleeDamageRequest_l92fc8c) == 0x100, "request layout");

struct UnkMeleeLife_l92fc8c {
    void** vtable;
    u8 pad_0x8[0x4c];
    f32 field_0x54;

    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkMeleeLife_l92fc8c*)>(vtable[0])(this);
    }
    void vf_0x188(UnkMeleeDamageRequest_l92fc8c* request) {
        reinterpret_cast<void (*)(UnkMeleeLife_l92fc8c*, UnkMeleeDamageRequest_l92fc8c*)>(vtable[0x188 / 8])(
            this, request);
    }
};

struct UnkMeleeOwner_l92fc8c {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkMeleeAI_l92fc8c {
    void** vtable;

    void vf_0x380(UnkMeleeOwner_l92fc8c* attacker) {
        reinterpret_cast<void (*)(UnkMeleeAI_l92fc8c*, UnkMeleeOwner_l92fc8c*)>(vtable[0x380 / 8])(this, attacker);
    }
};

struct UnkMeleeReceiver_l92fc8c {
    void** vtable;

    void vf_0x190(UnkMeleeOwner_l92fc8c* attacker, u64 arg, UnkMeleeHit_l92fc8c* hit) {
        reinterpret_cast<void (*)(UnkMeleeReceiver_l92fc8c*, UnkMeleeOwner_l92fc8c*, u64, UnkMeleeHit_l92fc8c*)>(
            vtable[0x190 / 8])(this, attacker, arg, hit);
    }
};

struct UnkMeleeTarget_l92fc8c {
    u8 pad_0x0[0x1b8];
    UnkMeleeLife_l92fc8c* life;  // 0x1b8
    u8 pad_0x1c0[0x80];
    UnkMeleeAI_l92fc8c* ai;  // 0x240
};

struct UnkMeleeComponent_l92fc8c {
    u8 pad_0x0[0x30];
    UnkMeleeOwner_l92fc8c* owner;  // 0x30

    // The damage flags: 0x40000 is added while the owner plays a dash melee track.
    u32 DamageFlags() {
        u32 flags = 0x800;
        if (UnkAnimationComponent* animation = owner->animation) {
            if (fn_710117b348(animation, fn_7100c13dd8()))
                flags = 0x40800;
        }
        return flags;
    }
};

bool fn_710092fc8c(UnkMeleeComponent_l92fc8c* self, UnkMeleeTarget_l92fc8c* target, u64 arg,
                   UnkMeleeHit_l92fc8c* hit) {
    if (UnkMeleeLife_l92fc8c* life = target->life) {
        f32 amount = life->field_0x54;
        UnkMeleeDamageRequest_l92fc8c request;
        request.flags = self->DamageFlags();
        AssignRef(&request.source, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(self->owner)));
        AssignRef(&request.target, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(target)));
        fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), amount);
        request.SetHit(arg, hit);
        life->vf_0x188(&request);
        return true;
    }
    if (target->ai) {
        target->ai->vf_0x380(self->owner);
        return true;
    }
    if (UnkWeakTarget* found = fn_7100286ad4(target)) {
        if (!arg || !hit)
            return false;
        reinterpret_cast<UnkMeleeReceiver_l92fc8c*>(found)->vf_0x190(self->owner, arg, hit);
        return true;
    }
    // An item's life component takes a fixed amount.
    UnkMeleeLife_l92fc8c* life = target->life;
    if (!life || !fn_710009d50c(life->vf_0x0(), fn_71008dff9c()))
        return false;
    UnkMeleeDamageRequest_l92fc8c request;
    request.flags = self->DamageFlags();
    AssignRef(&request.source, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(self->owner)));
    AssignRef(&request.target, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(target)));
    fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), 1000.0f);
    u64 source = hit->source ? hit->source->field_0x10 : 0;
    request.field_0x98 = arg;
    request.field_0x60 = source;
    request.field_0x68 = hit->field_0x10;
    request.field_0x6c = hit->field_0x14;
    request.field_0x78 = hit->field_0x20;
    request.field_0x84 = hit->field_0x2c;
    request.field_0x90 = hit->field_0x38;
    request.field_0x94 = hit->field_0x3c;
    life->vf_0x188(&request);
    return true;
}
