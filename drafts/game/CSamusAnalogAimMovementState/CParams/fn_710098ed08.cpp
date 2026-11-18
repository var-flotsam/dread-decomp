// CSamusAnalogAimMovementState::CParams's unit (tools/tu_map.py unit 1456): leaves analog aiming. When the
// movement allows it (and is not moving faster than the run information's floor), it either asks for
// another movement state, when the owner's object at +0x2b0 holds a link of kind 3 and is not running, or
// sends the owner's animation a mode-0 request and pushes the movement on at the aim speed, scaled by a
// tunable in one game mode; then it settles the movement. Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityGhostAura.h"
#include "game/CMovementComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGroundMovement_ovb0138.h"
#include "game/UnkTU1455.h"
#include "game/UnkTU1501.h"
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
struct UnkAimParams_l98ed08 {
    void* vtable;
    UnkMoveTransition_ovb0004 transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_ovb0004 field_0x3c;
    u32 field_0x44;

    explicit UnkAimParams_l98ed08(void* state_vtable)
        : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = state_vtable;
    }
};
static_assert(sizeof(UnkAimParams_l98ed08) == 0x48, "layout");

struct UnkAimLink_l98ed08 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};

struct UnkAimRunState_l98ed08 {
    u8 pad_0x0[0x50];
    UnkAimLink_l98ed08* link;  // 0x50
    u8 pad_0x58[0x360 - 0x58];
    bool running;  // 0x360
};

struct UnkAimOwner_l98ed08 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
    u8 pad_0x228[0x2b0 - 0x228];
    UnkAimRunState_l98ed08* run_state;  // 0x2b0
};

struct UnkAimInfo_l98ed08 {
    u8 pad_0x0[0xac];
    f32 speed;  // 0xac
    u8 pad_0xb0[0x10];
    f32 field_0xc0;  // 0xc0
};

// The game manager's mode bytes as this function reads them.
struct UnkAimGameModes_l98ed08 {
    u8 pad_0x0[0xc4];
    bool field_0xc4;
    u8 pad_0xc5;
    bool field_0xc6;
    u8 pad_0xc7;
    s32 field_0xc8;
    u8 pad_0xcc[0x10c - 0xcc];
    bool field_0x10c;
};

struct UnkAimState_l98ed08 {
    void* vtable;
    UnkAirTarget_B71a6* target;  // 0x8
    u8 pad_0x10[0x20];
    UnkAimOwner_l98ed08* owner;            // 0x30
    UnkGroundMovement_ovb0138* movement;  // 0x38
    UnkAimInfo_l98ed08* info;             // 0x40
};

struct UnkAimMoving_l98ed08 {
    u8 pad_0x0[0xf9];
    bool moving;  // 0xf9
    u8 pad_0xfa[0x1e0 - 0xfa];
    f32 field_0x1e0;  // 0x1e0
};

extern base::reflection::UnkTunable g_unk_7172a03d80;
bool fn_71009e9bb8(UnkGroundMovement_ovb0138* movement);
struct UnkGroundRequest_ovb0138;
bool fn_71009ed058(UnkGroundMovement_ovb0138* movement, UnkGroundRequest_ovb0138* req);

static inline UnkAimMoving_l98ed08* Moving(UnkGroundMovement_ovb0138* movement) {
    return reinterpret_cast<UnkAimMoving_l98ed08*>(movement);
}

// The aim speed's scale in that game mode (a tunable's +0x2c).
static inline f32 AimSpeedScale() {
    return *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172a03d80) + 0x2c);
}

// A mode-0 request to the animation under the two ids (as UnkSpitSend does).
static inline void Send(void* animation, u64 hash, base::reflection::UnkReflTypeIdArg* mode,
                        base::reflection::UnkReflTypeIdArg* mask) {
    mode->id = 0;
    mask->id = UnkAll_SpitSend().value;
    fn_7101176690(animation, hash, 0xf4061ff25a2163d6ull, mode, &g_unk_710164b958, &g_unk_7172769740, mask);
}

void fn_710098ed08(UnkAimState_l98ed08* self) {
    if (!fn_71009e9bb8(self->movement))
        return;
    if (Moving(self->movement)->moving && !(self->movement->field_0x200 <= self->info->field_0xc0))
        return;
    UnkAimRunState_l98ed08* run = self->owner->run_state;
    if (run && run->link && run->link->kind == 3 && !run->running) {
        UnkAimParams_l98ed08 params(g_unk_7101a22ce8 + 0x10);
        if (fn_71009ed058(self->movement, reinterpret_cast<UnkGroundRequest_ovb0138*>(&params)))
            fn_710098ae04(self->target, reinterpret_cast<UnkAirRequest_B71a6*>(&params));
    } else {
        f32 speed = self->info->speed;
        const UnkAimGameModes_l98ed08* modes = reinterpret_cast<const UnkAimGameModes_l98ed08*>(g_unk_7101d47310);
        if ((modes->field_0xc8 != 0 || modes->field_0x10c) && modes->field_0xc4) {
            bool unscaled = modes->field_0xc6;
            f32 scaled = speed * AimSpeedScale();
            speed = unscaled ? speed : scaled;
        }
        base::reflection::UnkReflTypeIdArg mode;
        base::math::CVector3D push;
        base::reflection::UnkReflTypeIdArg mask;
        Send(self->owner->animation, 0x3bd70b0741e799c2ull, &mode, &mask);
        push.z = Moving(self->movement)->field_0x1e0;
        push.x = 0.0f;
        push.y = speed;
        fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(self->movement),
                      reinterpret_cast<const UnkMovementVec_71010d08d4*>(&push), true);
    }
    fn_71009ea070(self->movement);
}
