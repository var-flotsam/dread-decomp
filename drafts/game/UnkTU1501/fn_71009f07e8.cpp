// UnkTU1501 (CSamusMovement's unit, tools/tu_map.py unit 1501): builds a track request for the id table's
// name at +0x1d78; plays it directly while the state at +0x2d08 is in mode 2, else hands it to the
// movement state machine at +0x2cf8 as CParams (fn_710098ae04). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1455.h"
#include "game/UnkTU1730.h"
#include "types.h"

extern UnkDesc_SpitSend g_unk_710164b968;  // reached through the GOT

struct __attribute__((packed, aligned(4))) UnkTrackBlend_l9f07e8 {
    u64 kind;
    f32 value;
};

struct UnkDesc16_l9f07e8 {
    u64 a, b;
};

// A track request: an id (-1 for none), the two descriptors of UnkSpitSend.h, flags and a mask.
struct UnkTrackRequest_l9f07e8 {
    s64 id;
    UnkDesc16_l9f07e8 desc;
    UnkTrackBlend_l9f07e8 blend;
    u32 flags;
    u32 mask;

    UnkTrackRequest_l9f07e8(UnkDesc16_l9f07e8 d, UnkTrackBlend_l9f07e8 b, u32 m)
        : id(-1), desc(d), blend(b), flags(0), mask(m) {}
    UnkTrackRequest_l9f07e8()
        : UnkTrackRequest_l9f07e8(*reinterpret_cast<const UnkDesc16_l9f07e8*>(&g_unk_710164b958),
                                  *reinterpret_cast<const UnkTrackBlend_l9f07e8*>(&g_unk_7172769740),
                                  UnkAll_SpitSend().value) {}
};
static_assert(sizeof(UnkTrackRequest_l9f07e8) == 0x30, "layout");

// A 64-bit word at a 4-byte aligned offset.
typedef u64 UnkU64Align4_l9f07e8 __attribute__((aligned(4)));

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8) with the derived vtable g_unk_7101a22d18.
struct UnkMoveParams_l9f07e8 {
    void* vtable;
    UnkTrackRequest_l9f07e8 request;  // 0x8
    u32 field_0x38;
    UnkU64Align4_l9f07e8 field_0x3c;
    u32 field_0x44;

    explicit UnkMoveParams_l9f07e8(const UnkTrackRequest_l9f07e8& r)
        : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10) {
        request = r;
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = reinterpret_cast<char*>(g_unk_7101a22d18) + 0x10;
    }
};
static_assert(sizeof(UnkMoveParams_l9f07e8) == 0x48, "layout");

struct UnkMoveOwner_l9f07e8 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* animation;  // 0x220
};

struct UnkMoveMode_l9f07e8 {
    u8 pad_0x0[0x10];
    s32 mode;  // 0x10
};

struct UnkSamusMovement_l9f07e8 {
    u8 pad_0x0[0x30];
    UnkMoveOwner_l9f07e8* owner;  // 0x30
    u8 pad_0x38[0x2cf8 - 0x38];
    u8 field_0x2cf8[0x10];  // the movement state machine
    UnkMoveMode_l9f07e8* field_0x2d08;
};
static_assert(__builtin_offsetof(UnkSamusMovement_l9f07e8, field_0x2d08) == 0x2d08, "layout");

void fn_71009f07e8(UnkSamusMovement_l9f07e8* self) {
    UnkTrackRequest_l9f07e8 request;
    request.id = GameManagerIdTable().field_0x1d78.data->Key();
    request.desc = *reinterpret_cast<const UnkDesc16_l9f07e8*>(&g_unk_710164b968);
    request.flags |= 2;
    UnkMoveMode_l9f07e8* state = self->field_0x2d08;
    if (state && state->mode == 2) {
        fn_7101176ea8(self->owner->animation, reinterpret_cast<const UnkRequest_7101176ea8*>(&request),
                      0xf4061ff25a2163d6);
    } else {
        UnkAirTarget_B71a6* machine = reinterpret_cast<UnkAirTarget_B71a6*>(self->field_0x2cf8);
        UnkMoveParams_l9f07e8 params(request);
        fn_710098ae04(machine, reinterpret_cast<UnkAirRequest_B71a6*>(&params));
    }
}
