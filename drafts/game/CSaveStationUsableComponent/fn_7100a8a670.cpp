// CSaveStationUsableComponent's unit (tools/tu_map.py): the save station's start step with its user. Places
// the user at the owner's anchor, keeps a weak handle to it (+0xe8), asks the user's movement machine for the
// state its params name, starts the use (fn_7100aa0b04) and requests state 6 from the machine at +0x100.
// Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "../../../src/game/UnkSmRequest_A5_b73.h"
#include "game/CAbilityGhostAura.h"
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

extern const u8 g_unk_710164b5d0[] __attribute__((visibility("hidden")));

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
struct UnkSaveParams_la8a670 {
    void* vtable;
    UnkMoveTransition_ovb0004 transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_ovb0004 field_0x3c;
    u32 field_0x44;

    UnkSaveParams_la8a670()
        : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = reinterpret_cast<char*>(g_unk_7101a874c0) + 0x10;
    }
};
static_assert(sizeof(UnkSaveParams_la8a670) == 0x48, "layout");



struct UnkTeleporterActor_b0233;
struct UnkTeleporterObject_b0233;

struct UnkMoveState_la8a670 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);
    virtual void vf_0x18(s32 next);
    virtual void vf_0x20(f32 dt);
    virtual void vf_0x28();
    virtual void Configure(const UnkSaveParams_la8a670* params);

    u64 field_0x8;
    u32 id;
};

struct UnkSaveParamsVtable_la8a670 {
    void* pad_0x0[3];
    u32 (*vf_0x18)(const UnkSaveParams_la8a670* self);
};

struct UnkSaveMovement_la8a670 {
    u8 pad_0x0[0x2cf8];
    UnkSmMachine_A5_b73 machine;  // 0x2cf8
};
static_assert(__builtin_offsetof(UnkSaveMovement_la8a670, machine) == 0x2cf8, "layout");

struct UnkSaveUser_la8a670 {
    u8 pad_0x0[0x208];
    UnkSaveMovement_la8a670* movement;  // 0x208
    u8 pad_0x210[0x3b0 - 0x210];
    bool field_0x3b0;
    u8 pad_0x3b1[0xf];
    bool field_0x3c0;
};

struct UnkSaveOwner_la8a670 {
    u8 pad_0x0[0x260];
    void* field_0x260;
};

struct UnkSave_la8a670 {
    u8 pad_0x0[0x30];
    UnkSaveOwner_la8a670* owner;  // 0x30
    u8 pad_0x38[0xe8 - 0x38];
    UnkWeakHandle* user;  // 0xe8
    u8 pad_0xf0[0x100 - 0xf0];
    UnkSmMachine_A5_b73 machine;  // 0x100
};
static_assert(__builtin_offsetof(UnkSave_la8a670, machine) == 0x100, "layout");

// Hands the movement machine the params' state, as fn_710031433c does for the usable components.
static inline void RequestMovement_la8a670(UnkSaveMovement_la8a670* movement, UnkSaveParams_la8a670* params) {
    if (!fn_710009d50c(fn_71009a3d24(), fn_71009b0304()))
        return;
    u32 id = reinterpret_cast<UnkSaveParamsVtable_la8a670*>(params->vtable)->vf_0x18(params);
    UnkSmMachine_A5_b73* machine = &movement->machine;
    if (id > 14) {
        RequestState_A5_b73<14, 15>(machine, id);
        return;
    }
    UnkSmState_A5_b73* found;
    if (FindState_A5_b73(machine, id, found) )
        reinterpret_cast<UnkMoveState_la8a670*>(found)->Configure(params);
    RequestState_A5_b73<14, 15>(machine, id);
}

bool fn_7100a8a670(UnkSave_la8a670* self, UnkSaveUser_la8a670* user) {
    if (!user)
        return false;
    UnkSaveOwner_la8a670* owner = self->owner;
    if (void* component = owner->field_0x260) {
        UnkMatrix4_b0233 matrix;
        __builtin_memset(&matrix, 0, sizeof(matrix));
        matrix.m[15] = 1.0f;
        matrix.m[10] = 1.0f;
        matrix.m[5] = 1.0f;
        matrix.m[0] = 1.0f;
        fn_710113cb74(component, reinterpret_cast<UnkTeleporterActor_b0233*>(owner), nullptr, &matrix);
        fn_7100233f84(reinterpret_cast<UnkTeleporterObject_b0233*>(user), &matrix, true);
        user->field_0x3c0 = true;
        user->field_0x3b0 = true;
    }
    AssignRef(&self->user, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(user)));
    if (UnkSaveMovement_la8a670* movement = user->movement) {
        UnkSaveParams_la8a670 params;
        RequestMovement_la8a670(movement, &params);
    }
    u64 a = fn_7100aa1f50(reinterpret_cast<UnkShipRecharge_A5_b73*>(self), 6);
    u64 b = fn_7100aa1888(reinterpret_cast<UnkShipRecharge_A5_b73*>(self), 6);
    fn_7100aa0b04(reinterpret_cast<UnkUsableVirt_A6_b62*>(self), a, b, g_unk_710164b5d0);
    RequestState_A5_b73<15, 8>(&self->machine, 6);
    return true;
}
