// UnkTU811 (CEmmyAIComponent's unit): works out whether the owner can reach its target over the
// navigation mesh, stores the answer and, when the shown state changes, runs the update
// fn_7100540dd0. Returns the stored answer. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/CCharacterMovement.h"
#include "game/UnkActor.h"
#include "game/UnkNavMeshManager.h"
#include "types.h"

// A position as three words (as src/game_s2/fn_71005416bc.cpp has it).
struct UnkVec3Bits {
    u32 x, y, z;
    UnkVec3Bits(const UnkVec3Bits& other) : x(other.x), y(other.y), z(other.z) {}
};
struct UnkPlace;

void fn_7100540dd0(char* self);

struct UnkReachContext_l544fb8 {
    u8 pad_0x0[0x601];
    bool alwaysReachable;  // 0x601
};
struct UnkSource_7100286798;
u32 fn_7100413fd8(UnkSource_7100286798* self);
bool fn_7100ec14e4(void* self, const base::math::CVector3D* a, const base::math::CVector3D* b, u32 c, u32 d);

struct UnkReachState_l544fb8 {
    bool shown;  // 0x0
    u8 pad_0x1[0x1f];
    bool reachable;  // 0x20
    bool check;      // 0x21
};

struct UnkEmmyAI_l544fb8 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x14c14 - 0x38];
    UnkReachState_l544fb8 reach;  // 0x14c14
    u8 pad_0x14c36[0x29ee0 - 0x14c36];
    UnkReachContext_l544fb8* context;  // 0x29ee0
};
static_assert(__builtin_offsetof(UnkEmmyAI_l544fb8, context) == 0x29ee0, "UnkEmmyAI_l544fb8 layout");

static inline UnkNavMeshManager* NavManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

static inline base::math::CVector3D Position(const UnkActor* actor) {
    base::math::CVector3D pos;
    pos.x = actor->field_0x44;
    pos.y = actor->field_0x54;
    pos.z = actor->field_0x64;
    return pos;
}

// Where the owner's target stands: its place when it has one, else its transform; zero without a target.
static inline void GetTargetPosition(UnkActor* owner, base::math::CVector3D* out) {
    UnkActor* target = fn_7100d7c990(owner);
    if (!target) {
        out->x = 0.0f;
        out->y = 0.0f;
        out->z = 0.0f;
        return;
    }
    *out = Position(target);
    if (target->field_0x1f8) {
        UnkVec3Bits place = fn_7100979a60(static_cast<UnkPlace*>(target->field_0x1f8));
        __builtin_memcpy(out, &place, sizeof(*out));
    }
}

static inline bool IsReachable(UnkEmmyAI_l544fb8* self) {
    if (self->context->alwaysReachable)
        return true;
    if (!fn_7100d7c990(self->owner))
        return false;
    if (!self->reach.check)
        return false;
    UnkNavMeshManager* manager = NavManager();
    base::math::CVector3D from = Position(self->owner);
    base::math::CVector3D to;
    GetTargetPosition(self->owner, &to);
    return fn_7100ec14e4(manager, &from, &to, fn_7100413fd8(reinterpret_cast<UnkSource_7100286798*>(self->context)), 0);
}

// game_s2/fn_71005416bc.cpp declares this void with a u32 argument; it returns the stored answer and
// tests only the low bit of `force`.
bool fn_7100544fb8(UnkEmmyAI_l544fb8* self, bool force) {
    UnkReachState_l544fb8& state = self->reach;
    bool reachable = IsReachable(self);
    if (force || state.reachable != reachable) {
        state.reachable = reachable;
        if (state.shown != reachable) {
            state.shown = reachable;
            fn_7100540dd0(reinterpret_cast<char*>(self));
        }
    }
    return state.reachable;
}
