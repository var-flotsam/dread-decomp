// CHydrogigaAIComponent's unit (tools/tu_map.py unit 883): a start step. Enables the colliders of every part
// and the owner's named collider, copies the four values of the target fn_710110c438 reports, resets both
// shoot activators, and, when the lazily resolved link's target is a cutscene that is playing, registers the
// callback fn_71005c4824 with the game manager. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "game/CBossAIComponent.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2730.h"
#include "game/UnkTU437.h"
#include "game/UnkTU883.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkAIJ_b0144;
struct UnkCoreXAI_A2_b73;
struct UnkTentacleCollider_ovb0132;
struct UnkTentacleOwner_ovb0132;
struct UnkHidrogigaActivator_ovb0036;
struct UnkHidrogigaActivator_b44a2;
struct UnkShootActivatorHidrogiga_b47;
struct UnkCutsceneComponent_ovb0077;

struct UnkCoreXQuery_A2_b73 {
    u8 pad_0x0[0x8];
    UnkWeakHandle** field_0x8;
    u32 field_0x10;
};

struct UnkCallback_kr {
    void* object;
    void (*fn)();
};

void fn_71010ff6f4(UnkAIJ_b0144* self, u32 mode);
void fn_71003aff64(UnkCoreXAI_A2_b73* self, u32 a, u32 b);
UnkCoreXQuery_A2_b73* fn_710110c438(UnkCoreXAI_A2_b73* self);
UnkCollisionObject* fn_7100709c8c(UnkTentacleCollider_ovb0132* self, UnkTentacleOwner_ovb0132* owner,
                                  const CStrId* group, u32 flags);
void fn_7100898434(UnkHidrogigaActivator_ovb0036* self, bool enabled);
void fn_7100897458(UnkHidrogigaActivator_b44a2* self);
void fn_7100898498(UnkShootActivatorHidrogiga_b47* self, bool value);
bool fn_71007aa610(UnkCutsceneComponent_ovb0077* self);
void fn_71005c4824();

// A part's collider definition: the group name its collider is made under.
struct UnkHydrogigaPartDef_l5be0ec {
    u8 pad_0x0[0x8];
    CStrId group;  // +0x8
};

struct UnkHydrogigaParts_l5be0ec {
    u8 pad_0x0[0x530];
    s32 count;  // +0x530
};
UnkHydrogigaPartDef_l5be0ec* fn_71005b9ab8(UnkHydrogigaParts_l5be0ec* self, s32 index);

// The target whose four values are copied (refreshed through vf_0x98 when its flag is set).
struct UnkHydrogigaSource_l5be0ec {
    void** vtable;
    u8 pad_0x8[0x38];
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    u8 pad_0x50[0x10];
    bool dirty;  // +0x60

    void vf_0x98() { reinterpret_cast<void (*)(UnkHydrogigaSource_l5be0ec*)>(vtable[0x98 / 8])(this); }
};

struct UnkHydrogigaOwner_l5be0ec {
    u8 pad_0x0[0x240];
    void* field_0x240;
    u8 pad_0x248[0x58];
    UnkCollisionContainer* field_0x2a0;
};

struct UnkHydrogigaState_l5be0ec {
    UnkHydrogigaParts_l5be0ec* parts;  // +0x0
    UnkWeakHandle* activator_a;        // +0x8
    UnkWeakHandle* activator_b;        // +0x10
    u8 pad_0x18[0x3c0 - 0x18];
    u8 source[0x28];  // +0x3c0
    u32 link_state;   // +0x3e8
    u8 pad_0x3ec[4];
    base::UnkRefHandle link;  // +0x3f0
    u8 pad_0x3f8[0x450 - 0x3f8];
    f32 values[4];  // +0x450
};

struct UnkHydrogigaAI_l5be0ec {
    u8 pad_0x0[0x30];
    UnkHydrogigaOwner_l5be0ec* owner;  // +0x30
    u8 pad_0x38[0x149a0 - 0x38];
    UnkHydrogigaState_l5be0ec state;  // +0x149a0
};
static_assert(__builtin_offsetof(UnkHydrogigaAI_l5be0ec, state) == 0x149a0, "layout");

static inline UnkWeakTarget* Target(UnkWeakHandle* handle) {
    return handle ? handle->target : nullptr;
}

// The link's weak handle, resolved from the source on first use (state 1 -> 2); none in state 0.
static inline UnkWeakHandle* Link(UnkHydrogigaState_l5be0ec* state) {
    switch (state->link_state) {
    case 1:
        {
            base::UnkRefPtrMember reference = fn_7100353be8(state->source);
            fn_710031d208(&state->link, reinterpret_cast<const base::UnkRefHandle*>(&reference));
        }
        state->link_state = 2;
        break;
    case 2:
        break;
    case 0:
        return nullptr;
    }
    return reinterpret_cast<UnkWeakHandle*>(state->link.ptr);
}

void fn_71005be0ec(UnkHydrogigaAI_l5be0ec* self) {
    UnkHydrogigaState_l5be0ec* state = &self->state;
    fn_71003acb5c(reinterpret_cast<UnkCoreXAI_A2_b73*>(self));
    fn_71010ff6f4(reinterpret_cast<UnkAIJ_b0144*>(self), 0);
    for (s32 i = 0; i < state->parts->count; ++i) {
        UnkHydrogigaPartDef_l5be0ec* part = fn_71005b9ab8(state->parts, i);
        if (!part)
            continue;
        UnkCollisionObject* collider =
            fn_7100709c8c(reinterpret_cast<UnkTentacleCollider_ovb0132*>(part),
                          reinterpret_cast<UnkTentacleOwner_ovb0132*>(self->owner), &part->group, 0x180);
        if (collider)
            fn_7100d35a98(reinterpret_cast<UnkEntry*>(collider), false);
    }
    if (UnkCollisionContainer* colliders = self->owner->field_0x2a0) {
        if (UnkCollisionObject* collider = fn_71008640a4(colliders, &g_unk_71726eb440))
            fn_7100d35a98(reinterpret_cast<UnkEntry*>(collider), false);
    }
    if (UnkCoreXQuery_A2_b73* query = fn_710110c438(static_cast<UnkCoreXAI_A2_b73*>(self->owner->field_0x240))) {
        if (query->field_0x10 == 1) {
            if (UnkWeakHandle* handle = *query->field_0x8) {
                if (auto* source = reinterpret_cast<UnkHydrogigaSource_l5be0ec*>(handle->target)) {
                    if (source->dirty)
                        source->vf_0x98();
                    state->values[0] = source->field_0x40;
                    state->values[1] = source->field_0x44;
                    state->values[2] = source->field_0x48;
                    state->values[3] = source->field_0x4c;
                }
            }
        }
    }
    fn_7100898434(reinterpret_cast<UnkHidrogigaActivator_ovb0036*>(Target(state->activator_a)), false);
    fn_7100898434(reinterpret_cast<UnkHidrogigaActivator_ovb0036*>(Target(state->activator_b)), false);
    fn_7100897458(reinterpret_cast<UnkHidrogigaActivator_b44a2*>(Target(state->activator_a)));
    fn_7100897458(reinterpret_cast<UnkHidrogigaActivator_b44a2*>(Target(state->activator_b)));
    fn_7100898498(reinterpret_cast<UnkShootActivatorHidrogiga_b47*>(Target(state->activator_a)), true);
    fn_7100898498(reinterpret_cast<UnkShootActivatorHidrogiga_b47*>(Target(state->activator_b)), true);
    fn_71003aff64(reinterpret_cast<UnkCoreXAI_A2_b73*>(self), 0, 1);
    UnkWeakHandle* link = Link(state);
    if (!link || !link->target)
        return;
    void* component = fn_71004067a4(link->target);
    if (component && fn_71007aa610(static_cast<UnkCutsceneComponent_ovb0077*>(component)))
        fn_71011995c0(g_unk_7101d47330, UnkCallback_kr{self, fn_71005c4824});
}
