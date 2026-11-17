// CSamusFloorSlideMovementState (unit 1459): leaves the slide. When the movement is still moving
// (+0xf9) it requests one movement state and pushes the movement on at its speed in the direction it
// faces; otherwise it requests another. Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/math/CVector.h"
#include "game/CAbilityGhostAura.h"
#include "game/CMovementComponent.h"
#include "game/UnkTU1455.h"
#include "game/UnkTU1730.h"
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

struct UnkLeaveMovement_l9968ec {
    u8 pad_0x0[0xf9];
    bool moving;  // 0xf9
    u8 pad_0xfa[0x12];
    f32 facing;  // 0x10c
    u8 pad_0x110[0xcc];
    f32 field_0x1dc;
    f32 field_0x1e0;
};
static_assert(__builtin_offsetof(UnkLeaveMovement_l9968ec, field_0x1dc) == 0x1dc, "layout");

struct UnkLeaveState_l9968ec {
    void* vtable;
    UnkAirTarget_B71a6* target;  // 0x8
    u8 pad_0x10[0x28];
    UnkLeaveMovement_l9968ec* movement;  // 0x38
};

struct UnkMove_b152;
f32 fn_71009e754c(UnkMove_b152* self);

static inline s32 Sign(f32 v) {
    return (v > 0.0f) - (v < 0.0f);
}

void fn_71009968ec(UnkLeaveState_l9968ec* self) {
    if (self->movement->moving) {
        UnkLeaveParams_l9968ec params(g_unk_7101a22ce8 + 0x10);
        fn_710098ae04(self->target, reinterpret_cast<UnkAirRequest_B71a6*>(&params));
        base::math::CVector3D push;
        push.y = self->movement->field_0x1dc;
        push.z = self->movement->field_0x1e0;
        f32 speed = fn_71009e754c(reinterpret_cast<UnkMove_b152*>(self->movement));
        push.x = speed * Sign(self->movement->facing);
        fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(self->movement),
                      reinterpret_cast<const UnkMovementVec_71010d08d4*>(&push), true);
    } else {
        UnkLeaveParams_l9968ec params(reinterpret_cast<char*>(g_unk_7101a22d18) + 0x10);
        fn_710098ae04(self->target, reinterpret_cast<UnkAirRequest_B71a6*>(&params));
    }
}
