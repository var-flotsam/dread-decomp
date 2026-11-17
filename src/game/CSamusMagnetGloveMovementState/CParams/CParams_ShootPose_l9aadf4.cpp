// CSamusMagnetGloveMovementState::CParams unit: when the owner's gun (+0x2a8) is busy, holds a ready hyper beam
// or the game manager says so, plays the transition to the shooting pose on the owner's animation component
// (+0x220) unless an ignore track plays; the tweak track or the params' pose kind picks the animation.
// Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CGunComponent.h"
#include "game/CHyperBeamGun.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkGunComponent;
using base::global::CStrId;
base::reflection::UnkReflObject* fn_71008407d8(UnkGunComponent* self);

struct UnkMagnetGun_l9aadf4 {
    u8 pad_0x0[0x178];
    bool busy;  // 0x178
};

struct UnkMagnetOwner_l9aadf4 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2a8 - 0x228];
    UnkMagnetGun_l9aadf4* gun;  // 0x2a8
};

struct UnkMagnetParams_l9aadf4 {
    u8 pad_0x0[0x30];
    UnkMagnetOwner_l9aadf4* owner;  // 0x30
    u8 pad_0x38[0x1c0 - 0x38];
    s32 pose_kind;  // 0x1c0
};

struct UnkMagnetBlend_l9aadf4 {
    u64 kind;
    u32 value;
};

struct UnkMagnetRequest_l9aadf4 {
    s64 id;
    s64 sub_id;        // 0x8
    f32 blend_time;    // 0x10
    u32 mode;          // 0x14
    u64 blend_kind;    // 0x18
    u32 blend_value;   // 0x20
    u32 flags;         // 0x24
    u32 mask;          // 0x28
};

// The track that fills the request in.
struct UnkMagnetTrack_l9aadf4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkMagnetRequest_l9aadf4* request);
};

inline const UnkFlags_SpitSend& UnkNone_l9aadf4() {
    static UnkFlags_SpitSend s_value;
    return s_value;
}

static bool HyperBeamReady(UnkMagnetGun_l9aadf4* gun) {
    UnkGunComponent* component = reinterpret_cast<UnkGunComponent*>(gun);
    if (!(*fn_71008407fc(component) == GameManagerIdTable().field_0x1848))
        return false;
    base::reflection::UnkReflObject* beam = fn_71008407d8(component);
    if (beam && !fn_710009d50c(beam->vf_0x0(), fn_71008424e8()))
        beam = nullptr;
    return fn_7100845fac(beam);
}

void fn_71009aadf4(UnkMagnetParams_l9aadf4* self) {
    UnkMagnetOwner_l9aadf4* owner = self->owner;
    UnkMagnetGun_l9aadf4* gun = owner->gun;
    if (!gun)
        return;
    UnkAnimationComponent* animation = owner->animation;
    if (!gun->busy && !HyperBeamReady(gun) && !fn_7100df59a8(g_unk_7101d47310))
        return;
    if (fn_710117b348(animation, fn_7100c2f4bc()))
        return;

    const UnkMagnetBlend_l9aadf4* blend = reinterpret_cast<const UnkMagnetBlend_l9aadf4*>(&g_unk_7172769740);
    u32 flags = UnkNone_l9aadf4().value;
    u64 kind = blend->kind;
    u32 value = blend->value;
    u32 mask = UnkAll_SpitSend().value;
    UnkMagnetRequest_l9aadf4 request;
    request.id = -1;
    request.sub_id = -1;
    request.blend_time = 0.12f;
    request.mode = 1;
    request.blend_kind = kind;
    request.blend_value = value;
    request.flags = flags;
    request.mask = mask;

    UnkTrackQuery query = {fn_7100c5bb9c(), nullptr, -1.0f};
    if (fn_710117b08c(animation, &query)) {
        reinterpret_cast<UnkMagnetTrack_l9aadf4*>(query.result)->vf_0x18(&request);
    } else {
        switch (self->pose_kind) {
        case 0:
            request.id = 0x4b74211df45a05d6;
            break;
        case 1:
        case 2:
            request.id = 0x61a7ccfa79dc6f0c;
            break;
        }
    }
    if (request.id == -1)
        return;
    fn_7101176ea8(reinterpret_cast<UnkStruct_7101176ea8*>(animation),
                  reinterpret_cast<const UnkRequest_7101176ea8*>(&request), 0xf4061ff25a2163d6);
}
