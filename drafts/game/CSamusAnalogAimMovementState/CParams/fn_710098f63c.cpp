// CSamusAnalogAimMovementState::CParams (unit 1456): after the base update, unless `skip`, the movement still
// moves (+0xf9) or the target is not in state 15, asks for the next movement state. The transition takes the
// id table's +0xff8 id and is filled in by a tweak-fall track; it is played on the owner's animation when the
// owner's +0x2b0 object is in state 2 or 3, and requested from the target otherwise. Names and layouts are
// placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManagerIdTable.h"
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
struct UnkAimParams_l98f63c {
    void* vtable;
    UnkMoveTransition_ovb0004 transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_ovb0004 field_0x3c;
    u32 field_0x44;

    explicit UnkAimParams_l98f63c(void* state_vtable)
        : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = state_vtable;
    }
};
static_assert(sizeof(UnkAimParams_l98f63c) == 0x48, "layout");

// The track that fills the transition in.
struct UnkAimTrack_l98f63c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkMoveTransition_ovb0004* transition);
};

struct UnkAimMode_l98f63c {
    u8 pad_0x0[0x10];
    u32 state;  // 0x10
};

struct UnkAimModeHolder_l98f63c {
    u8 pad_0x0[0x50];
    UnkAimMode_l98f63c* mode;  // 0x50
};

struct UnkAimOwner_l98f63c {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2b0 - 0x228];
    UnkAimModeHolder_l98f63c* field_0x2b0;  // 0x2b0
};

struct UnkAimTarget_l98f63c {
    u8 pad_0x0[0xa4];
    s32 state;  // 0xa4
};

struct UnkAimMovement_l98f63c {
    u8 pad_0x0[0xf9];
    bool moving;  // 0xf9
};

struct UnkAimState_l98f63c {
    void* vtable;
    UnkAimTarget_l98f63c* target;  // 0x8
    u8 pad_0x10[0x20];
    UnkAimOwner_l98f63c* owner;         // 0x30
    UnkAimMovement_l98f63c* movement;  // 0x38
};

void fn_71009b1e14(UnkAimState_l98f63c* self, bool skip);

// The id table's id at +0xff8 (not yet in UnkGameManagerIdTable.h).
static inline const base::global::CStrId& IdTableId_0xff8() {
    return *reinterpret_cast<const base::global::CStrId*>(reinterpret_cast<const char*>(&GameManagerIdTable()) +
                                                         0xff8);
}

void fn_710098f63c(UnkAimState_l98f63c* self, bool skip) {
    fn_71009b1e14(self, skip);
    if (skip || self->movement->moving || self->target->state != 15)
        return;

    UnkAimParams_l98f63c params(g_unk_7101a22ce8 + 0x10);
    UnkMoveTransition_ovb0004* transition = &params.transition;
    transition->id = IdTableId_0xff8().data->Key();

    UnkTrackQuery query = {fn_7100d2fa08(), nullptr, -1.0f};
    fn_710117b08c(self->owner->animation, &query);
    if (query.result)
        reinterpret_cast<UnkAimTrack_l98f63c*>(query.result)->vf_0x18(transition);

    UnkAimMode_l98f63c* mode = self->owner->field_0x2b0->mode;
    if (mode && (mode->state & ~1u) == 2)
        fn_7101176ea8(reinterpret_cast<UnkStruct_7101176ea8*>(self->owner->animation),
                      reinterpret_cast<const UnkRequest_7101176ea8*>(transition), 0xf4061ff25a2163d6);
    else
        fn_710098ae04(reinterpret_cast<UnkAirTarget_B71a6*>(self->target),
                      reinterpret_cast<UnkAirRequest_B71a6*>(&params));
}
