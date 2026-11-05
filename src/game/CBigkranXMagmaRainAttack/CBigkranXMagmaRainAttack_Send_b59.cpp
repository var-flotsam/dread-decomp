// CBigkranXMagmaRainAttack's unit (tools/tu_map.py): fn_71003ea6d8 sets the stage at +0x34: stages 0 and 1
// take the count at +0xf0 of the tunables (+0x28) into +0x5c; stage 2 takes +0xec into +0x58 and tells the
// owner's (+0x8) CBigkranXAIComponent (+0x240) state 1 (fn_71003f4f0c); stage 3 sends the owner's
// character class (+0x220) a request (descriptors as UnkSpitSend) and tells the AI component state 0.
// fn_71003eab78 handles an event of kind 1 whose string id is a fixed one (g_unk_71726c1820): by the
// squared distance to the actor the owner acts against, against the tunables' near (+0xfc) and far
// (+0x100) radii, it sends one of three hashes. Names are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

using base::reflection::CClass;

struct UnkMagmaRainAI_b49;
void fn_71003f4f0c(UnkMagmaRainAI_b49* ai, u32 state);

struct UnkMagmaRainTunables_b59 {
    u8 pad_0x0[0xec];
    u32 field_0xec;
    u32 field_0xf0;
    u8 pad_0xf4[8];
    f32 field_0xfc;
    f32 field_0x100;
};

struct UnkMagmaRainOwner_b59 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x18];
    base::reflection::UnkReflObject* field_0x240;
};

struct UnkMagmaRain_b59 {
    void** vtable;
    UnkMagmaRainOwner_b59* owner;
    u8 pad_0x10[0x18];
    UnkMagmaRainTunables_b59* field_0x28;
    u8 pad_0x30[4];
    u32 stage;
    u8 pad_0x38[0x20];
    u32 field_0x58;
    u32 field_0x5c;
};

static inline void SetAIState_b59(UnkMagmaRainOwner_b59* owner, u32 state) {
    if (base::reflection::UnkReflObject* ai = owner->field_0x240) {
        if (fn_710009d50c(ai->vf_0x0(), fn_71003f4454()))
            fn_71003f4f0c(reinterpret_cast<UnkMagmaRainAI_b49*>(ai), state);
    }
}

void fn_71003ea6d8(UnkMagmaRain_b59* self, u32 stage) {
    if (self->stage == stage)
        return;
    self->stage = stage;
    switch (stage) {
    case 0:
    case 1:
        self->field_0x5c = self->field_0x28->field_0xf0;
        break;
    case 2:
        if (self->field_0x28)
            self->field_0x58 = self->field_0x28->field_0xec;
        if (self->owner)
            SetAIState_b59(self->owner, 1);
        break;
    case 3:
        if (!self->owner)
            break;
        if (void* anim = self->owner->field_0x220) {
            UnkSpitSend(anim, 0xcd8fc2f707c5dbb3, 0xf4061ff25a2163d6);
            if (!self->owner)
                break;
        }
        SetAIState_b59(self->owner, 0);
        break;
    }
}

// The fixed string id the event is compared with (reached directly).
extern base::global::CStrId g_unk_71726c1820 __attribute__((visibility("hidden")));

struct UnkMagmaRainEventInner_b59 {
    u8 pad_0x0[0x38];
    base::global::CStrId id;
};

struct UnkMagmaRainEventData_b59 {
    u8 pad_0x0[0x10];
    UnkMagmaRainEventInner_b59* field_0x10;
};

struct UnkMagmaRainEvent_b59 {
    u32 kind;
    u8 pad_0x4[4];
    UnkMagmaRainEventData_b59* field_0x8;
};

struct UnkVec3_b59 {
    f32 x, y, z;
};

static inline UnkVec3_b59 operator-(const UnkVec3_b59& a, const UnkVec3_b59& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

bool fn_71003eab78(UnkMagmaRain_b59* self, UnkMagmaRainEvent_b59* event) {
    const base::global::CStrId& id = event->field_0x8 && event->field_0x8->field_0x10
                                         ? event->field_0x8->field_0x10->id
                                         : g_unk_7101d470e0;
    if (id.data == &g_unk_7101d47108->empty)
        return true;
    if (event->kind != 1 || id.data != g_unk_71726c1820.data)
        return true;
    UnkMagmaRainOwner_b59* owner = self->owner;
    if (!owner)
        return true;
    void* anim = owner->field_0x220;
    UnkActor* actor = fn_7100d7c990(reinterpret_cast<UnkActor*>(owner));
    if (!anim || !actor)
        return true;
    UnkMagmaRainTunables_b59* tunables = self->field_0x28;
    if (!tunables)
        return true;
    UnkActor* me = reinterpret_cast<UnkActor*>(self->owner);
    UnkVec3_b59 a = {actor->field_0x44, actor->field_0x54, actor->field_0x64};
#if defined(__aarch64__)
    asm volatile("" : "+w"(a.z));  // keeps the original's load order; a no-op on other targets
#endif
    UnkVec3_b59 b = {me->field_0x44, me->field_0x54, me->field_0x64};
    UnkVec3_b59 d = a - b;
    f32 dx = d.x, dy = d.y, dz = d.z;
    f32 d2 = dx * dx + dy * dy + dz * dz;
    u64 hash;
    if (d2 < tunables->field_0xfc * tunables->field_0xfc)
        hash = 0x457b169dc49ad046;
    else
        hash = d2 < tunables->field_0x100 * tunables->field_0x100 ? 0xc1f5864e8b1edf2a : 0x8afbd3b7e0a6c4f0;
    UnkSpitSend(anim, hash, 0xf4061ff25a2163d6);
    return true;
}
