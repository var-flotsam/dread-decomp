// NON_MATCHING(major): branch B stores field_0x3c as one 64-bit store and schedules the transition stores
// differently.
// CSamusHangMovementState::CParams unit: reports the hang state's transition to its target (+0x8): while the
// owner's (+0x30) part at +0x2b0 is flagged, the params with mode 3, otherwise the params with mode 1 carrying
// two values of the movement (+0x38); the transition id depends on a movement flag. Names and layouts are
// placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkTU1455.h"
#include "game/UnkTU1730.h"
#include "types.h"

extern char g_unk_7101a23ca8[];

struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend) == 0xc, "UnkTrackBlend layout");

struct UnkDesc16_l9a0984 {
    u64 a, b;
};

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h and a mask.
struct UnkMoveTransition_l9a0984 {
    s64 id;
    UnkDesc16_l9a0984 desc;
    UnkTrackBlend blend;
    u32 field_0x24;
    u32 mask;

    UnkMoveTransition_l9a0984(UnkDesc16_l9a0984 d, UnkTrackBlend b, u32 m)
        : id(-1), desc(d), blend(b), field_0x24(0), mask(m) {}
    UnkMoveTransition_l9a0984()
        : UnkMoveTransition_l9a0984(*reinterpret_cast<const UnkDesc16_l9a0984*>(&g_unk_710164b958),
                                    *reinterpret_cast<const UnkTrackBlend*>(&g_unk_7172769740),
                                    UnkAll_SpitSend().value) {}
};
static_assert(sizeof(UnkMoveTransition_l9a0984) == 0x30, "layout");

// A 64-bit word at a 4-byte aligned offset.
typedef u64 UnkU64Align4_l9a0984 __attribute__((aligned(4)));

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8).
struct UnkHangParams_l9a0984 {
    void* vtable;
    UnkMoveTransition_l9a0984 transition;  // 0x8
    u32 mode;                              // 0x38
    UnkU64Align4_l9a0984 field_0x3c;
    u32 field_0x44;

    UnkHangParams_l9a0984() : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {}
};
static_assert(sizeof(UnkHangParams_l9a0984) == 0x48, "layout");

// The params with mode 3 (vtable g_unk_7101a23ca8).
struct UnkHangParamsA_l9a0984 : UnkHangParams_l9a0984 {
    UnkHangParamsA_l9a0984() {
        mode = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = g_unk_7101a23ca8 + 0x10;
    }
};

// The params with the second descriptor (vtable g_unk_7101a22ce8).
struct UnkHangParamsB_l9a0984 : UnkHangParams_l9a0984 {
    UnkHangParamsB_l9a0984() {
        vtable = g_unk_7101a22ce8 + 0x10;
        transition.desc = *reinterpret_cast<const UnkDesc16_l9a0984*>(&g_unk_710164b968);
    }
};

struct UnkHangPart_l9a0984 {
    u8 pad_0x0[0x360];
    bool field_0x360;
};

struct UnkHangOwner_l9a0984 {
    u8 pad_0x0[0x2b0];
    UnkHangPart_l9a0984* field_0x2b0;
};

struct UnkHangMovement_l9a0984 {
    u8 pad_0x0[0x1d8];
    u32 field_0x1d8;
    u8 pad_0x1dc[4];
    u32 field_0x1e0;
    u8 pad_0x1e4[0x3a8 - 0x1e4];
    bool field_0x3a8;
    u8 pad_0x3a9[0x7e4 - 0x3a9];
    bool field_0x7e4;
};

struct UnkHangState_l9a0984 {
    void* vtable;
    UnkAirTarget_B71a6* target;  // 0x8
    u8 pad_0x10[0x20];
    UnkHangOwner_l9a0984* owner;         // 0x30
    UnkHangMovement_l9a0984* movement;   // 0x38
};

void fn_71009a0984(UnkHangState_l9a0984* self) {
    if (self->owner->field_0x2b0->field_0x360) {
        UnkHangParamsA_l9a0984 params;
        params.transition.id = self->movement->field_0x3a8 ? 0x0460f1244a905997 : 0x443a2422329aff27;
        fn_710098ae04(self->target, reinterpret_cast<UnkAirRequest_B71a6*>(&params));
    } else {
        UnkHangParamsB_l9a0984 params;
        UnkHangMovement_l9a0984* movement = self->movement;
        params.transition.id = movement->field_0x7e4 ? 0x2fbb52ce88cd9c73 : 0x519ba7a1e35e9102;
        params.mode = 1;
        params.field_0x3c = movement->field_0x1d8;
        params.field_0x44 = movement->field_0x1e0;
        fn_710098ae04(self->target, reinterpret_cast<UnkAirRequest_B71a6*>(&params));
    }
}
