// UnkTU812 (tools/tu_map.py unit 812, next to CEmmyAttackComponent): whether an Emmy's attack hits
// `other`. Refuses while the Emmy (the owner's CEmmyAIComponent) is stunned or busy; for the player,
// also while the game is paused in modes 1 and 2, for one attack kind while the Emmy cannot grab, and
// for two hit kinds while the player hangs in the state fn_71009a60fc names. Otherwise the base decides
// (fn_7101135434). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CSamusMagnetGloveMovementState/CParams.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU39.h"
#include "game/UnkTU811.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkTunable;
extern UnkTunable g_unk_71726affc0;

struct UnkHangMoveState_A6_b70 {
    void* vtable;
    u8 pad_0x8[0x8];
    s32 kind;
};

#pragma pack(push, 4)
struct UnkHangMove_A6_b70 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x2d08 - 0x38];
    UnkHangMoveState_A6_b70* current;
    u8 pad_0x2d10[0x2d18 - 0x2d10];
    u32 state_count;
    UnkHangMoveState_A6_b70* states[1];  // 0x2d1c, 4-byte aligned
};
#pragma pack(pop)

// The tunable's mode (+0x38).
struct UnkEmmyTunable_l56a6a8 {
    u8 pad_0x0[0x38];
    u32 mode;  // 0x38
};

struct UnkEmmyAI_l56a6a8 {
    u8 pad_0x0[0x29350];
    u8 stun_timer[0xc8];  // 0x29350
    u8 busy_timer[0xb0e];  // 0x29418
    bool busy;  // 0x29f26
};
static_assert(__builtin_offsetof(UnkEmmyAI_l56a6a8, busy) == 0x29f26, "layout");

bool fn_7100551470(UnkEmmyAIComponent_b46a1* self);

struct UnkHitKind_l56a6a8 {
    u8 pad_0x0[0x18];
    u64 hash;  // 0x18
};

struct UnkHitSource_l56a6a8 {
    u8 pad_0x0[0x10];
    UnkHitKind_l56a6a8* kind;  // 0x10
};

struct UnkEmmyAttack_l56a6a8 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

bool fn_7101135434(UnkEmmyAttack_l56a6a8* self, UnkActor* other, UnkHitSource_l56a6a8** source,
                   UnkHitKind_l56a6a8* attack, u32 flag);

static inline UnkHangMoveState_A6_b70* FindState(UnkHangMove_A6_b70* move, s32 kind) {
    UnkHangMoveState_A6_b70** it = move->states;
    UnkHangMoveState_A6_b70** end = it + move->state_count;
    for (; it != end; ++it) {
        if ((*it)->kind == kind)
            return *it;
    }
    return nullptr;
}

bool fn_710056a6a8(UnkEmmyAttack_l56a6a8* self, UnkActor* other, UnkHitSource_l56a6a8** source,
                   UnkHitKind_l56a6a8* attack, u32 flag) {
    // The owner's AI component, kept only when it is a CEmmyAIComponent.
    auto* ai = static_cast<UnkReflObject*>(self->owner->field_0x240);
    bool is_emmy = false;
    if (ai) {
        if (fn_710009d50c(ai->vf_0x0(), fn_710053b934())) {
            auto* emmy = reinterpret_cast<UnkEmmyAI_l56a6a8*>(ai);
            if (fn_710002fb70(emmy->stun_timer))
                return false;
            if ((fn_710002fb70(emmy->busy_timer) || emmy->busy) &&
                fn_7100551470(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(emmy)))
                return false;
            is_emmy = true;
        } else {
            ai = nullptr;
        }
    }
    auto* emmy = reinterpret_cast<UnkEmmyAI_l56a6a8*>(ai);
    if (reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310)) == other) {
        bool paused = IsOpticCamouflageAbilityInState6(g_unk_7101d47310);
        u32 mode = reinterpret_cast<UnkEmmyTunable_l56a6a8*>(&g_unk_71726affc0)->mode;
        if ((mode == 1 || mode == 2) && paused)
            return false;
        if ((is_emmy & paused) && !fn_71005436e8(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(emmy)) && attack &&
            attack->hash == 0x3ef2e860f334c5d1)
            return false;
        auto* move = static_cast<UnkHangMove_A6_b70*>(other->field_0x208);
        if (move && *source && (*source)->kind) {
            u32 kind = fn_71009a60fc();
            if (kind <= 14) {
                UnkHangMoveState_A6_b70* state = FindState(move, kind);
                if (state && move->current == state && fn_71009ad440(state)) {
                    u64 hash = (*source)->kind->hash;
                    if (hash == 0x88ab87ce686750f7 || hash == 0x3b85cb706bc00f98)
                        return false;
                }
            }
        }
    }
    return fn_7101135434(self, other, source, attack, flag);
}
