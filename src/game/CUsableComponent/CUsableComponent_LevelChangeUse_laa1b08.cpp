// CUsableComponent unit (tools/tu_map.py, unit 1607): the level-change use. Runs the
// "Scenario.OnLevelChangeUse" script with the level name, keeps a weak handle to the user, hands the user's
// movement (+0x208) an access transition, then either finishes the linked cutscene's use or starts the use
// through fn_7100aa0b04 (a fade on failure), requests state 6 and tells the unit's listeners.
// Names and layouts are placeholders.
#include "game/UnkUsableKeys.h"
#include "../UnkSmRequest_A5_b73.h"
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkCloneHelpers.h"
#include "base/global/CStrId.h"
#include "game/CAbilityGhostAura.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CCutsceneComponent.h"
#include "game/CSamusInteractMovementState/CParams.h"
#include "game/CUsableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2656.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"


struct UnkAccessTarget_la70e48;

struct UnkCutsceneComponent_b53;
bool fn_71007a6d6c(UnkCutsceneComponent_b53* self);

struct UnkClone_fn_710082d0ac;
using UnkUsableListeners_laa1b08 = base::UnkDeferredSet<base::UnkDeferredCallback<u64>>;
extern template void base::refl_DeferredInvoke<UnkClone_fn_710082d0ac, u64>(UnkUsableListeners_laa1b08*,
                                                                           const u64*);

struct __attribute__((packed, aligned(4))) UnkTrackBlend_laa1b08 {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend_laa1b08) == 0xc, "layout");

struct UnkDesc16_laa1b08 {
    u64 a, b;
};

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h and a mask.
struct UnkMoveTransition_laa1b08 {
    s64 id;
    UnkDesc16_laa1b08 desc;
    UnkTrackBlend_laa1b08 blend;
    u32 field_0x24;
    u32 mask;

    UnkMoveTransition_laa1b08(UnkDesc16_laa1b08 d, UnkTrackBlend_laa1b08 b, u32 m)
        : id(-1), desc(d), blend(b), field_0x24(0), mask(m) {}
    UnkMoveTransition_laa1b08()
        : UnkMoveTransition_laa1b08(*reinterpret_cast<const UnkDesc16_laa1b08*>(&g_unk_710164b958),
                                    *reinterpret_cast<const UnkTrackBlend_laa1b08*>(&g_unk_7172769740),
                                    UnkAll_SpitSend().value) {}
};
static_assert(sizeof(UnkMoveTransition_laa1b08) == 0x30, "layout");

typedef u64 UnkU64Align4_laa1b08 __attribute__((aligned(4)));

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8) with the access vtable g_unk_7101a874c0.
struct UnkUseParams_laa1b08 {
    void* vtable;
    UnkMoveTransition_laa1b08 transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_laa1b08 field_0x3c;
    u32 field_0x44;

    UnkUseParams_laa1b08() : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = reinterpret_cast<char*>(g_unk_7101a874c0) + 0x10;
    }
};
static_assert(sizeof(UnkUseParams_laa1b08) == 0x48, "layout");

struct UnkUser_laa1b08 {
    u8 pad_0x0[0x208];
    UnkAccessTarget_la70e48* movement;  // 0x208
};

struct UnkUsable_laa1b08;
struct UnkUsableVtable_laa1b08 {
    void* pad_0x0[0x1a8 / 8];
    u64 (*vf_0x1a8)(UnkUsable_laa1b08* self);
    void* pad_0x1b0[(0x200 - 0x1b0) / 8];
    u64 (*vf_0x200)(UnkUsable_laa1b08* self);
    void* pad_0x208[(0x2b8 - 0x208) / 8];
    void (*vf_0x2b8)(UnkUsable_laa1b08* self, bool flag);
};

struct UnkUsable_laa1b08 {
    UnkUsableVtable_laa1b08* vtable;
    u8 pad_0x8[0x10];
    void* field_0x18;  // what the linked cutscene is looked up from
    u8 pad_0x20[0xe8 - 0x20];
    UnkWeakHandle* user;  // 0xe8
    u8 pad_0xf0[0x100 - 0xf0];
    UnkSmMachine_A5_b73 machine;  // 0x100
};
static_assert(__builtin_offsetof(UnkUsable_laa1b08, machine) == 0x100, "layout");

// The unit's link groups (g_unk_7172753260) as this function sees them: its listeners at +0x80.
struct UnkUsableEvents_laa1b08 {
    u8 pad_0x0[0x80];
    UnkUsableListeners_laa1b08 listeners;  // 0x80
};

// Tells the unit's listeners about the usable.
static inline void NotifyListeners_laa1b08(UnkUsable_laa1b08* self) {
    u64 arg = reinterpret_cast<u64>(self);
    base::refl_DeferredInvoke<UnkClone_fn_710082d0ac, u64>(
        &reinterpret_cast<UnkUsableEvents_laa1b08*>(&g_unk_7172753260)->listeners, &arg);
}

bool fn_7100aa1b08(UnkUsable_laa1b08* self, UnkUser_laa1b08* user, const char* const* level) {
    if (!user)
        return false;
    fn_7100000790("Scenario.OnLevelChangeUse", "s", *level ? *level : g_unk_710162e2ed);
    AssignRef(&self->user, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(user)));
    void* cutscene = fn_71004067a4(self->field_0x18);
    bool loaded = cutscene ? fn_71007a6d6c(static_cast<UnkCutsceneComponent_b53*>(cutscene)) : true;
    if (UnkAccessTarget_la70e48* movement = user->movement) {
        UnkUseParams_laa1b08 params;
        fn_710031433c(movement, &params);
    }
    if (!loaded) {
        fn_71007a8280(cutscene);
        RequestState_A5_b73<15, 8>(&self->machine, 6);
    } else {
        u64 a = self->vtable->vf_0x1a8(self);
        u64 b = self->vtable->vf_0x200(self);
        if (fn_7100aa0b04(reinterpret_cast<UnkUsableVirt_A6_b62*>(self), a, b, g_unk_710164b668)) {
            RequestState_A5_b73<15, 8>(&self->machine, 6);
        } else {
            self->vtable->vf_0x2b8(self, false);
            void* fx = g_unk_7101d47310->field_0x1040;
            base::math::CVector3D black;
            black.x = 0.0f;
            black.y = 0.0f;
            black.z = 0.0f;
            fn_7101053c14(fx, &black, 0.033f, 1.0f, 1.0f);
        }
    }
    NotifyListeners_laa1b08(self);
    return true;
}
