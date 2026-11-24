// CTrainUsableComponentCutScene's unit (tools/tu_map.py): the cutscene use of the train. Runs the
// "Scenario.OnLevelChangeUse" script, keeps a weak handle to the user, picks up the linked cutscene's
// owner, asks the user's movement machine for the state its params name, then starts the use through
// fn_7100aa0b04 and requests state 6, or calls its own slot 0x2b8.
// Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "../../../src/game/UnkSmRequest_A5_b73.h"
#include "game/CAbilityGhostAura.h"
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU437.h"
#include "game/CGrabComponent.h"
#include "game/CUsableComponent.h"
#include "game/UnkDesc16_ovb0004.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkMatrix4_b0233.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "game/UnkUsableKeys.h"
#include "game/CSamusInteractMovementState/CParams.h"
#include "types.h"

extern const u8 g_unk_710164b600[] __attribute__((visibility("hidden")));

struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend) == 0xc, "UnkTrackBlend layout");

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h and a mask.
struct UnkMoveTransition_ovb0004 {
    s64 id;
    UnkDesc16_ovb0004 desc;
    UnkTrackBlend blend;
    u32 field_0x24;
    u32 mask;

    UnkMoveTransition_ovb0004(UnkDesc16_ovb0004 d, UnkTrackBlend b, u32 m)
        : id(-1), desc(d), blend(b), field_0x24(0), mask(m) {}
    UnkMoveTransition_ovb0004()
        : UnkMoveTransition_ovb0004(*reinterpret_cast<const UnkDesc16_ovb0004*>(&g_unk_710164b958),
                                    *reinterpret_cast<const UnkTrackBlend*>(&g_unk_7172769740),
                                    UnkAll_SpitSend().value) {}
};
static_assert(sizeof(UnkMoveTransition_ovb0004) == 0x30, "layout");

// A 64-bit word at a 4-byte aligned offset.
typedef u64 UnkU64Align4_ovb0004 __attribute__((aligned(4)));

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8), given the vtable of the state it asks for.
struct UnkTrainParams_la9ad48 {
    void* vtable;
    UnkMoveTransition_ovb0004 transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_ovb0004 field_0x3c;
    u32 field_0x44;

    UnkTrainParams_la9ad48()
        : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = reinterpret_cast<char*>(g_unk_7101a874c0) + 0x10;
    }
};
static_assert(sizeof(UnkTrainParams_la9ad48) == 0x48, "layout");



struct UnkCutsceneComponent_b53;
bool fn_71007a6d6c(UnkCutsceneComponent_b53* self);
void fn_71007aa604(void* cutscene);

struct UnkMoveState_la9ad48 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);
    virtual void vf_0x18(s32 next);
    virtual void vf_0x20(f32 dt);
    virtual void vf_0x28();
    virtual void Configure(const UnkTrainParams_la9ad48* params);

    u64 field_0x8;
    u32 id;
};

struct UnkTrainParamsVtable_la9ad48 {
    void* pad_0x0[3];
    u32 (*vf_0x18)(const UnkTrainParams_la9ad48* self);
};

struct UnkTrainMovement_la9ad48 {
    u8 pad_0x0[0x2cf8];
    UnkSmMachine_A5_b73 machine;  // 0x2cf8
};
static_assert(__builtin_offsetof(UnkTrainMovement_la9ad48, machine) == 0x2cf8, "layout");

struct UnkTrainUser_la9ad48 {
    u8 pad_0x0[0x208];
    UnkTrainMovement_la9ad48* movement;  // 0x208
};

struct UnkTrainLink_la9ad48 {
    u8 pad_0x0[0x10];
    void* cutscene_source;  // 0x10
};

struct UnkTrainGame_la9ad48 {
    u8 pad_0x0[0x2442];
    bool field_0x2442;
};

struct UnkTrainVtable_la9ad48 {
    void* pad_0x0[0x2b8 / 8];
    void (*vf_0x2b8)(struct UnkTrain_la9ad48* self, bool flag);
};

struct UnkTrain_la9ad48 {
    UnkTrainVtable_la9ad48* vtable;
    u8 pad_0x8[0xe0];
    UnkWeakHandle* user;  // 0xe8
    u8 pad_0xf0[0x100 - 0xf0];
    UnkSmMachine_A5_b73 machine;  // 0x100
    u8 pad_0x1b0[0x578 - 0x1b0];
    u8 field_0x578[0x28];
    u32 phase;  // 0x5a0
    u8 pad_0x5a4[0x4];
    UnkTrainLink_la9ad48* link;  // 0x5a8
};
static_assert(__builtin_offsetof(UnkTrain_la9ad48, machine) == 0x100, "layout");
static_assert(__builtin_offsetof(UnkTrain_la9ad48, link) == 0x5a8, "layout");

// Gives the state `id` of the machine the params, when there is one.
static void ConfigureState_la9ad48(UnkSmMachine_A5_b73* machine, u32 id, UnkTrainParams_la9ad48* params) {
    if (id > 14)
        return;
    UnkSmState_A5_b73* found;
    if (FindState_A5_b73(machine, id, found))
        reinterpret_cast<UnkMoveState_la9ad48*>(found)->Configure(params);
}

// Hands the movement machine the params' state, as fn_710031433c does for the usable components.
static inline void RequestMovement_la9ad48(UnkTrainMovement_la9ad48* movement, UnkTrainParams_la9ad48* params) {
    if (!fn_710009d50c(fn_71009a3d24(), fn_71009b0304()))
        return;
    u32 id = reinterpret_cast<UnkTrainParamsVtable_la9ad48*>(params->vtable)->vf_0x18(params);
    UnkSmMachine_A5_b73* machine = &movement->machine;
    ConfigureState_la9ad48(machine, id, params);
    machine->pending = id;
    if (machine->busy || id > 14)
        return;
    SwitchState_A5_b73<15>(machine, id, machine->current);
}

bool fn_7100a9ad48(UnkTrain_la9ad48* self, UnkTrainUser_la9ad48* user, const char* const* level) {
    if (!user)
        return false;
    fn_7100000790("Scenario.OnLevelChangeUse", "s", *level ? *level : g_unk_710162e2ed);
    AssignRef(&self->user, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(user)));
    if (self->phase == 1) {
        {
            base::UnkRefPtrMember handle = fn_7100353be8(self->field_0x578);
            fn_710031d208(reinterpret_cast<base::UnkRefHandle*>(&self->link),
                          reinterpret_cast<const base::UnkRefHandle*>(&handle));
        }
        self->phase = 2;
    }
    if (UnkTrainLink_la9ad48* link = self->link) {
        if (void* source = link->cutscene_source) {
            if (void* cutscene = fn_71004067a4(source)) {
                if (!fn_71007a6d6c(static_cast<UnkCutsceneComponent_b53*>(cutscene)) && reinterpret_cast<UnkTrainGame_la9ad48*>(g_unk_7101d47310)->field_0x2442)
                    fn_71007aa604(cutscene);
            }
        }
    }
    if (UnkTrainMovement_la9ad48* movement = user->movement) {
        UnkTrainParams_la9ad48 params;
        RequestMovement_la9ad48(movement, &params);
    }
    u64 a = fn_7100aa1f50(reinterpret_cast<UnkShipRecharge_A5_b73*>(self), 6);
    u64 b = fn_7100aa1888(reinterpret_cast<UnkShipRecharge_A5_b73*>(self), 6);
    if (fn_7100aa0b04(reinterpret_cast<UnkUsableVirt_A6_b62*>(self), a, b, g_unk_710164b600))
        RequestState_A5_b73<15, 8>(&self->machine, 6);
    else
        self->vtable->vf_0x2b8(self, false);
    return true;
}
