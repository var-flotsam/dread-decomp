// CSamusAnalogAimMovementState::CParams (unit 1456): while the target is in state 15, unless the movement is on
// the ground and fn_71009e1fd4 agrees, plays a fall-end transition on the owner's animation. A tweak track may
// change it; when it does not, the movement's two probes (vf_0x2e8) pick a nearer variant: none when both are
// within 100, the second's when it is within 60, the first's when it is within 40. Names and layouts are
// placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/CMovementComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU363.h"
#include "types.h"

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
struct UnkAimLandRequest_l98f028 {
    u64 id;
    UnkDesc_SpitSend desc;
    UnkTrackBlend blend;
    u32 mode;
    u32 mask;

    UnkAimLandRequest_l98f028(u64 id, UnkDesc_SpitSend desc, UnkTrackBlend blend, u32 mode, u32 mask)
        : id(id), desc(desc), blend(blend), mode(mode), mask(mask) {}
};

// As src/game/UnkTU1501/UnkTU1501_Movement_ovb0152.cpp declares it.
struct UnkMsg_b152 {
    s16 id;
    u8 field_0x2;
    u8 pad_0x3[9];
    f32 field_0xc;
};

// A probe result: valid when it has an id and a kind; field_0xc is its distance.
static inline bool IsValid(const UnkMsg_b152& probe) {
    return probe.id != -1 && probe.field_0x2 != 0xff;
}

// The track that fills the request in.
struct UnkAimLandTrack_l98f028 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkAimLandRequest_l98f028* request);
};

struct UnkAimLandMovement_l98f028 {
    void** vtable;

    void Probe(UnkMsg_b152* first, bool* first_flag, UnkMsg_b152* second, bool* second_flag) {
        reinterpret_cast<void (*)(UnkAimLandMovement_l98f028*, UnkMsg_b152*, bool*, UnkMsg_b152*, bool*)>(
            vtable[0x2e8 / 8])(this, first, first_flag, second, second_flag);
    }
};

struct UnkAimLandOwner_l98f028 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkAimLandTarget_l98f028 {
    u8 pad_0x0[0xa4];
    s32 state;  // 0xa4
};

struct UnkAimLandState_l98f028 {
    void* vtable;
    UnkAimLandTarget_l98f028* target;  // 0x8
    u8 pad_0x10[0x20];
    UnkAimLandOwner_l98f028* owner;         // 0x30
    UnkAimLandMovement_l98f028* movement;  // 0x38
};

// As src/game/CSamusGroundMovementState/CSamusGroundMovementState_Face_b52.cpp declares it.
bool fn_71009e1fd4(UnkGroundPlayer_b52* player);

void fn_710098f028(UnkAimLandState_l98f028* self) {
    if (self->target->state != 15)
        return;
    if (fn_71010d1598(reinterpret_cast<UnkGroundPlayer_b52*>(self->movement)) &&
        fn_71009e1fd4(reinterpret_cast<UnkGroundPlayer_b52*>(self->movement)))
        return;

    UnkAimLandRequest_l98f028 request(0x182a3d12989fdcbfull,
                                      *reinterpret_cast<const UnkDesc_SpitSend*>(&g_unk_710164b958),
                                      g_unk_7172769770, 0, UnkAll_SpitSend().value);
    UnkTrackQuery query = {fn_7100d2ffa8(), nullptr, -1.0f};
    if (UnkAnimationComponent* animation = self->owner->animation) {
        fn_710117b08c(animation, &query);
        if (query.result)
            reinterpret_cast<UnkAimLandTrack_l98f028*>(query.result)->vf_0x18(&request);
    }

    if (request.id == 0x182a3d12989fdcbfull) {
        UnkMsg_b152 first;
        fn_71002878dc(&first);
        UnkMsg_b152 second;
        fn_71002878dc(&second);
        self->movement->Probe(&first, nullptr, &second, nullptr);
        if (IsValid(second) && IsValid(first) && second.field_0xc < 100.0f && first.field_0xc < 100.0f) {
            // Both probes are near: the transition stays as it is.
        } else if (IsValid(second) && second.field_0xc < 60.0f) {
            request.id = 0xb3ee8895256ad4a1ull;
        } else if (IsValid(first) && first.field_0xc < 40.0f) {
            request.id = 0x9771f0ad6931a22dull;
        }
        fn_7100287954(&second);
        fn_7100287954(&first);
    }

    CAnimationComponent_SendActionRequestRecordToTrack(reinterpret_cast<UnkStruct_7101176ea8*>(self->owner->animation),
                  reinterpret_cast<const UnkRequest_7101176ea8*>(&request), 0xf4061ff25a2163d6ull);
}
