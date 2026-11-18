// CAccessPointComponent's unit (tools/tu_map.py unit 1581): the access point's start step with its user.
// Places the user at the owner's anchor (as the teleporter does), keeps a weak handle to it (+0xe8), asks
// the user's movement for the access-point movement state, starts the use (fn_7100aa0b04) and requests
// state 6 from the state machine at +0x100 ("none" 8). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "../UnkSmRequest_A5_b73.h"
#include "game/CAbilityGhostAura.h"
#include "game/CGrabComponent.h"
#include "game/CUsableComponent.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "game/UnkUsableKeys.h"
#include "game/CSamusInteractMovementState/CParams.h"
#include "types.h"

struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend) == 0xc, "UnkTrackBlend layout");

struct UnkDesc16_ovb0004 {
    u64 a, b;
};

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
struct UnkLeaveParams_l9968ec {
    void* vtable;
    UnkMoveTransition_ovb0004 transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_ovb0004 field_0x3c;
    u32 field_0x44;

    explicit UnkLeaveParams_l9968ec(void* state_vtable)
        : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = state_vtable;
    }
};
static_assert(sizeof(UnkLeaveParams_l9968ec) == 0x48, "layout");

struct UnkMatrix4_b0233 {
    f32 m[16];
};

struct UnkTeleporterActor_b0233;
struct UnkTeleporterObject_b0233;

struct UnkApMovement_la6e8a4;
// The access point's movement state (its vtable).

struct UnkApUser_la6e8a4 {
    u8 pad_0x0[0x208];
    UnkApMovement_la6e8a4* movement;  // 0x208
    u8 pad_0x210[0x3b0 - 0x210];
    bool field_0x3b0;
    u8 pad_0x3b1[0xf];
    bool field_0x3c0;
};

struct UnkApOwner_la6e8a4 {
    u8 pad_0x0[0x260];
    void* field_0x260;
};

struct UnkAp_la6e8a4 {
    u8 pad_0x0[0x30];
    UnkApOwner_la6e8a4* owner;  // 0x30
    u8 pad_0x38[0xe8 - 0x38];
    UnkWeakHandle* user;  // 0xe8
    u8 pad_0xf0[0x100 - 0xf0];
    UnkSmMachine_A5_b73 machine;  // 0x100
};
static_assert(__builtin_offsetof(UnkAp_la6e8a4, machine) == 0x100, "layout");

bool fn_7100a6e8a4(UnkAp_la6e8a4* self, UnkApUser_la6e8a4* user, const char* const*) {
    if (!user)
        return false;
    UnkApOwner_la6e8a4* owner = self->owner;
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
    if (UnkApMovement_la6e8a4* movement = user->movement) {
        UnkLeaveParams_l9968ec params(reinterpret_cast<char*>(&g_unk_7101a874c0[2]));  // the vtable, past its two header words
        fn_710031433c(reinterpret_cast<UnkAccessTarget_la70e48*>(movement), &params);
    }
    u64 a = fn_7100aa1f50(reinterpret_cast<UnkShipRecharge_A5_b73*>(self), 6);
    u64 b = fn_7100aa1888(reinterpret_cast<UnkShipRecharge_A5_b73*>(self), 6);
    fn_7100aa0b04(reinterpret_cast<UnkUsableVirt_A6_b62*>(self), a, b, g_unk_710164b540);
    RequestState_A5_b73<15, 8>(&self->machine, 6);
    return true;
}
