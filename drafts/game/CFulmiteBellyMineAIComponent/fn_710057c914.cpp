// CFulmiteBellyMineAIComponent unit (tools/tu_map.py unit 826): fn_710057c914 is the mine's update after the base
// one: it arms once the owner has stood still for a moment, runs the pending requests, paces the beeps by the
// fuse's progress, warns the timeline near the end, tints the model and checks whether to blow up. Names and
// layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "game/CMovementComponent.h"
#include "game/UnkActor.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::math::CVector4D;

struct UnkBellyMineKey_l57c914;
extern UnkBellyMineKey_l57c914 g_unk_7101649f70;  // reached through the GOT

struct UnkBellyMineAI_l57c914;
struct UnkMineAI3_ovb0132;
void fn_710057cd48(UnkBellyMineAI_l57c914* self);
void fn_710057dccc(UnkBellyMineAI_l57c914* self);
void fn_710057dadc(UnkMineAI3_ovb0132* self, f32 volume, f32 pitch);
void fn_710057d040(UnkBellyMineAI_l57c914* self, bool flag);
bool fn_710057d6f0(UnkBellyMineAI_l57c914* self);
void fn_710057d858(UnkBellyMineAI_l57c914* self);
void fn_710057d518(UnkBellyMineAI_l57c914* self, f32 dt);

// The mine's tunables.
struct UnkBellyMineDef_l57c914 {
    u8 pad_0x0[0x558];
    f32 warn_time;         // 0x558
    f32 beep_interval_end; // 0x55c
    f32 beep_interval;     // 0x560
    f32 volume_start;      // 0x564
    f32 volume_end;        // 0x568
    f32 pitch_start;       // 0x56c
    f32 pitch_end;         // 0x570
};

struct UnkBellyMineModel_l57c914 {
    u8 pad_0x0[0x30];
    CVector4D* tint;  // 0x30
};

struct UnkBellyMineMesh_l57c914 {
    u8 pad_0x0[0x14c];
    CVector4D tint;  // 0x14c
    u8 pad_0x15c[0x94];
    UnkBellyMineModel_l57c914* model;  // 0x1f0
};

struct UnkBellyMineNode3_l57c914 {
    u8 pad_0x0[0x48];
    UnkBellyMineMesh_l57c914** mesh;  // 0x48
};

struct UnkBellyMineNode2_l57c914 {
    u8 pad_0x0[0x78];
    UnkBellyMineNode3_l57c914* node;  // 0x78
};

struct UnkBellyMineNode1_l57c914 {
    u8 pad_0x0[0x138];
    UnkBellyMineNode2_l57c914* node;  // 0x138
};

struct UnkBellyMineRender_l57c914 {
    u8 pad_0x0[0x50];
    UnkBellyMineNode1_l57c914* node;  // 0x50
};

struct UnkBellyMineLife_l57c914 {
    u8 pad_0x0[0x70];
    u32 state;  // 0x70: 2 when dead
};

struct UnkBellyMineState_l57c914 {
    UnkMovement_71010d08d4* movement;  // 0x146c8
    UnkBellyMineDef_l57c914* def;      // 0x146d0
    u8 sound[0x10];                    // 0x146d8
    f32 fuse;                          // 0x146e8
    f32 elapsed;                       // 0x146ec
    UnkActor* carrier;                 // 0x146f0
    bool done;                         // 0x146f8
    bool field_0x146f9;
    bool armed;                        // 0x146fa
    bool pending_b;                    // 0x146fb
    bool pending_c;                    // 0x146fc
    u8 pad_0x146fd[3];
    u8 move[0xc];                      // 0x14700
    bool pending_move;                 // 0x1470c
    CVector2D dir;                     // 0x14710
    u8 pad_0x14718[0x1c];
    f32 still_time;                    // 0x14734
    CVector3D home;                    // 0x14738
    u8 pad_0x14744[0xc];
    bool warned;                       // 0x14750
    bool in_shape;                     // 0x14751
    f32 beep_time;                     // 0x14754
};
static_assert(sizeof(UnkBellyMineState_l57c914) == 0x90, "layout");

struct UnkBellyMineAI_l57c914 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x146c8 - 0x38];
    UnkBellyMineState_l57c914 state;  // 0x146c8
};

static inline bool IsNear(f32 a, f32 b) {
    return a - 0.001f <= b && a + 0.001f >= b;
}

static inline bool IsNear(const CVector3D& a, const CVector3D& b) {
    return IsNear(a.x, b.x) && IsNear(a.y, b.y) && IsNear(a.z, b.z);
}

// From `from` to `to` by `t`, clamped to the ends.
static inline f32 Lerp(f32 from, f32 to, f32 t) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    return (1.0f - t) * from + t * to;
}

void fn_710057c914(UnkBellyMineAI_l57c914* self, f32 dt) {
    fn_7100369c44(reinterpret_cast<UnkSelf_7100369c44*>(self), dt);
    UnkActor* owner = self->owner;
    if (static_cast<UnkBellyMineLife_l57c914*>(owner->field_0x1b8)->state == 2)
        return;
    UnkBellyMineState_l57c914& state = self->state;

    CVector3D pos;
    pos.x = owner->field_0x44;
    pos.y = owner->field_0x54;
    pos.z = owner->field_0x64;
    if (IsNear(state.home, pos)) {
        state.still_time += dt;
        if (state.still_time >= 0.2f && !state.armed)
            state.armed = true;
    } else {
        state.still_time = 0.0f;
        state.home = pos;
    }

    if (state.pending_c)
        fn_710057cd48(self);
    if (state.pending_b) {
        state.pending_b = false;
        fn_710057dccc(self);
    }
    if (state.pending_move) {
        state.pending_move = false;
        fn_71010d08d4(state.movement, reinterpret_cast<const UnkMovementVec_71010d08d4*>(state.move), true);
    }

    if (state.armed) {
        if (!state.done) {
            f32 progress = state.elapsed / state.fuse;
            UnkBellyMineDef_l57c914* def = state.def;
            f32 interval = Lerp(def->beep_interval, def->beep_interval_end, progress);
            if (!(state.beep_time >= interval)) {
                state.beep_time += dt;
            } else {
                state.beep_time = 0.0f;
                f32 volume = Lerp(def->volume_start, def->volume_end, progress);
                f32 pitch = Lerp(def->pitch_start, def->pitch_end, progress);
                fn_710057dadc(reinterpret_cast<UnkMineAI3_ovb0132*>(self), volume, pitch);
            }
        }
        state.elapsed += dt;
        if (state.elapsed >= state.fuse) {
            state.elapsed = 0.0f;
            fn_710057d040(self, false);
        }
    }

    f32 t = state.elapsed / state.fuse;
    f32 glow;
    if (t >= 1.0f)
        glow = 1.0f;
    else if (t < 0.0f)
        glow = 0.0f;
    else
        glow = t + (1.0f - t) * 0.0f;

    if (state.fuse - state.elapsed < state.def->warn_time && !state.warned) {
        state.warned = true;
        if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101649f70), &mask.id, false, -1.0f);
        }
    }

    if (UnkBellyMineNode1_l57c914* node = static_cast<UnkBellyMineRender_l57c914*>(self->owner->field_0x228)->node) {
        f32 c = glow * glow;
        CVector4D tint;
        tint.x = c;
        tint.y = c;
        tint.z = c;
        tint.w = c;
        UnkBellyMineMesh_l57c914* mesh = *node->node->node->mesh;
        fn_710007b518(&mesh->tint, &tint);
        if (UnkBellyMineModel_l57c914* model = mesh->model)
            *model->tint = tint;
    }

    if (!state.armed)
        fn_710057d518(self, dt);
    if (fn_710057d6f0(self))
        fn_710057d858(self);
}
