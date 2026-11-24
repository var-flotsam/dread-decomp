// CElectricGeneratorComponent unit (tools/tu_map.py unit 1587): the level-change use. Calls the owner's
// "Scenario.OnLevelChangeUse" script with the level name, hands the user's movement (+0x208) the interact
// parameters and puts its state machine (+0x2cf8) in state 12, fades the screen effect, starts the use
// (fn_7100aa0b04) and puts the component's own state machine (+0x100) in state 1. Names and layouts are
// placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CUsableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2656.h"
#include "types.h"

extern void* g_unk_7101a874c0[];
extern const u8 g_unk_710164b570[] __attribute__((visibility("hidden")));

struct __attribute__((packed, aligned(4))) UnkTrackBlend_la7d74c {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend_la7d74c) == 0xc, "layout");

struct UnkDesc16_la7d74c {
    u64 a, b;
};

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h and a mask.
struct UnkMoveTransition_la7d74c {
    s64 id;
    UnkDesc16_la7d74c desc;
    UnkTrackBlend_la7d74c blend;
    u32 field_0x24;
    u32 mask;

    UnkMoveTransition_la7d74c(UnkDesc16_la7d74c d, UnkTrackBlend_la7d74c b, u32 m)
        : id(-1), desc(d), blend(b), field_0x24(0), mask(m) {}
    UnkMoveTransition_la7d74c()
        : UnkMoveTransition_la7d74c(*reinterpret_cast<const UnkDesc16_la7d74c*>(&g_unk_710164b958),
                                    *reinterpret_cast<const UnkTrackBlend_la7d74c*>(&g_unk_7172769740),
                                    UnkAll_SpitSend().value) {}
};
static_assert(sizeof(UnkMoveTransition_la7d74c) == 0x30, "layout");

typedef u64 UnkU64Align4_la7d74c __attribute__((aligned(4)));

// CSamusInteractMovementState::CParams (vtable g_unk_7101a874c0) over CSamusMovementState::CParams.
struct UnkInteractParams_la7d74c {
    void* vtable;
    UnkMoveTransition_la7d74c transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_la7d74c field_0x3c;
    u32 field_0x44;

    UnkInteractParams_la7d74c() : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = reinterpret_cast<char*>(g_unk_7101a874c0) + 0x10;
    }
};
static_assert(sizeof(UnkInteractParams_la7d74c) == 0x48, "layout");

struct UnkSmState_la7d74c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Enter(s32 previous);  // 0x10
    virtual void Leave(s32 next);      // 0x18
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void SetParams(void* params);  // 0x30
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_la7d74c* __attribute__((aligned(4))) UnkSmStatePtr_la7d74c;

struct UnkSmMachine_la7d74c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void Entered(UnkSmState_la7d74c* previous, UnkSmState_la7d74c* next);  // 0x20
    virtual void Left(UnkSmState_la7d74c* current, UnkSmState_la7d74c* next);      // 0x28

    u64 field_0x8;
    UnkSmState_la7d74c* current;
    UnkSmState_la7d74c* previous;
    u32 count;
    UnkSmStatePtr_la7d74c states[16];
    u32 pending;
    bool busy;

    UnkSmState_la7d74c* Find(u32 state) {
        // Walks the array by its size in bytes.
        UnkSmStatePtr_la7d74c* p = states;
        for (u64 size = (u64)count * sizeof(UnkSmStatePtr_la7d74c); size != 0; ++p, size -= sizeof(UnkSmStatePtr_la7d74c)) {
            if ((*p)->id == state)
                return *p;
        }
        return nullptr;
    }

    // Asks for state `state` and switches to it unless busy; `none` marks no request.
    void Request(u32 state, u32 none) {
        pending = state;
        if (busy)
            return;
        UnkSmState_la7d74c* cur = current;
        if (!cur || cur->id != state) {
            if (UnkSmState_la7d74c* next = Find(state)) {
                if (cur) {
                    cur->Leave(state);
                    Left(current, next);
                }
                previous = current;
                current = next;
                next->Enter(previous ? previous->id : none);
                Entered(previous, next);
            }
        }
        pending = none;
    }
};
static_assert(__builtin_offsetof(UnkSmMachine_la7d74c, pending) == 0xa4, "layout");

struct UnkUserMovement_la7d74c {
    u8 pad_0x0[0x2cf8];
    UnkSmMachine_la7d74c machine;  // 0x2cf8
};

struct UnkUser_la7d74c {
    u8 pad_0x0[0x208];
    UnkUserMovement_la7d74c* movement;  // 0x208
};

struct UnkElectricGenerator_la7d74c {
    u8 pad_0x0[0x100];
    UnkSmMachine_la7d74c machine;  // 0x100
};

static inline const char* OrEmpty_la7d74c(const char* s) { return s ? s : ""; }

bool fn_7100a7d74c(UnkElectricGenerator_la7d74c* self, UnkUser_la7d74c* user, const char* const* level) {
    if (user) {
        fn_7100000790("Scenario.OnLevelChangeUse", "s", OrEmpty_la7d74c(*level));
        if (UnkUserMovement_la7d74c* movement = user->movement) {
            UnkInteractParams_la7d74c params;
            if (fn_710009d50c(fn_71009a3d24(), fn_71009b0304())) {
                if (UnkSmState_la7d74c* state = movement->machine.Find(12))
                    state->SetParams(&params);
                movement->machine.Request(12, 15);
            }
        }
        void* fade = g_unk_7101d47310->field_0x1040;
        base::math::CVector3D color = {0.0f, 0.0f, 0.0f};
        fn_7101053c14(fade, &color, 0.033f, 1.0f, 1.0f);
        UnkShipRecharge_A5_b73* usable = reinterpret_cast<UnkShipRecharge_A5_b73*>(self);
        u64 a = CUsableComponent_GetUserAnimationIdForKind(usable, 1);
        u64 b = fn_7100aa1888(usable, 1);
        fn_7100aa0b04(reinterpret_cast<UnkUsableVirt_A6_b62*>(self), a, b, g_unk_710164b570);
        self->machine.Request(1, 8);
        return true;
    }
    return false;
}
