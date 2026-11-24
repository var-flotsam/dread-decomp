// CAbilityShinespark's unit (tools/tu_map.py, unit 433): after the base update, while the ability's state
// (+0x6b0) is 2 and the owner's animation plays a fixed track, sends it a fixed request; in state 4 runs the
// two state-4 steps. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CAnimationComponent.h"
#include "game/CMovementComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1730.h"
#include "types.h"

struct UnkStruct_7101176ea8;
struct UnkRequest_7101176ea8;
struct UnkGroundPlayer_b52;

// As src/game/CCharClassToStateTrack/CCharClassToStateTrack_Fill.cpp declares it.
struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};

struct UnkDesc_SpitSend {
    u64 field_0x0;
    u64 field_0x8;
};

// The request fn_7101176ea8 reads.
struct UnkShinesparkRequest_l328f58 {
    u64 id;
    UnkDesc_SpitSend desc;
    UnkTrackBlend blend;
    u32 mode;
    u32 mask;

    UnkShinesparkRequest_l328f58(u64 id, UnkDesc_SpitSend desc, UnkTrackBlend blend, u32 mode, u32 mask)
        : id(id), desc(desc), blend(blend), mode(mode), mask(mask) {}
};

struct UnkShinesparkOwner_l328f58 {
    u8 pad_0x0[0x208];
    UnkGroundPlayer_b52* movement;  // 0x208
    u8 pad_0x210[0x10];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkShinesparkEntity_l328f58 {
    u8 pad_0x0[0x30];
    UnkShinesparkOwner_l328f58* owner;
};

struct UnkShinesparkState_l328f58 {
    u8 pad_0x0[0x10];
    s32 kind;
};

struct UnkAbilityShinespark_l328f58 {
    u8 pad_0x0[0xc0];
    UnkShinesparkEntity_l328f58* entity;  // 0xc0
    u8 pad_0xc8[0x6b0 - 0xc8];
    UnkShinesparkState_l328f58* state;  // 0x6b0
};

void fn_71002f9254(UnkAbilityShinespark_l328f58* self);
void fn_7100329114(UnkAbilityShinespark_l328f58* self);
void fn_7100329458(UnkAbilityShinespark_l328f58* self);

void fn_7100328f58(UnkAbilityShinespark_l328f58* self) {
    fn_71002f9254(self);
    if (self->state && self->state->kind == 2) {
        UnkShinesparkOwner_l328f58* owner = self->entity->owner;
        if (owner->movement) {
            UnkAnimationComponent* animation = owner->animation;
            if (!fn_71010d1598(owner->movement) && fn_710117ae38(animation, 0xccbcbb5050c1a714ull, false)) {
                UnkShinesparkRequest_l328f58 req(0x50fff64f73b7f4d6ull, g_unk_710164b958, g_unk_7172769770, 0,
                                                    UnkAll_SpitSend().value);
                CAnimationComponent_SendActionRequestRecordToTrack(reinterpret_cast<UnkStruct_7101176ea8*>(animation),
                              reinterpret_cast<const UnkRequest_7101176ea8*>(&req), 0xf4061ff25a2163d6ull);
            }
        }
    }
    if (self->state && self->state->kind == 4) {
        fn_7100329114(self);
        fn_7100329458(self);
    }
}
