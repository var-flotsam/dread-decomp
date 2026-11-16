// CSamusHangMovementState::CParams unit: when the owner's gun (+0x2a8) is busy, holds a ready hyper beam or
// the game manager says so, plays the hang-to-shooting-pose transition on the owner's animation component
// (+0x220) unless an ignore track plays; the tweak track fills the request in. Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1730.h"
#include "types.h"

struct UnkGunComponent;
using base::global::CStrId;
const CStrId* fn_71008407fc(UnkGunComponent* self);
base::reflection::UnkReflObject* fn_71008407d8(UnkGunComponent* self);
bool fn_7100845fac(void* beam);
bool fn_7100df59a8(UnkGameManager*);

struct UnkHangGun_l9a0c80 {
    u8 pad_0x0[0x178];
    bool busy;  // 0x178
};

struct UnkHangOwner_l9a0c80 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2a8 - 0x228];
    UnkHangGun_l9a0c80* gun;  // 0x2a8
};

struct UnkHangParams_l9a0c80 {
    u8 pad_0x0[0x30];
    UnkHangOwner_l9a0c80* owner;  // 0x30
};

struct UnkHangBlend_l9a0c80 {
    u64 kind;
    u32 value;
};

struct UnkHangRequest_l9a0c80 {
    u64 hash;
    s64 id;            // 0x8
    f32 blend_time;    // 0x10
    u32 mode;          // 0x14
    u64 blend_kind;    // 0x18
    u32 blend_value;   // 0x20
    u32 flags;         // 0x24
    u32 mask;          // 0x28
};

// The track that fills the request in.
struct UnkHangTrack_l9a0c80 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkHangRequest_l9a0c80* request);
};

inline const UnkFlags_SpitSend& UnkNone_l9a0c80() {
    static UnkFlags_SpitSend s_value;
    return s_value;
}

static bool HyperBeamReady(UnkHangGun_l9a0c80* gun) {
    UnkGunComponent* component = reinterpret_cast<UnkGunComponent*>(gun);
    if (!(*fn_71008407fc(component) == GameManagerIdTable().field_0x1848))
        return false;
    base::reflection::UnkReflObject* beam = fn_71008407d8(component);
    if (beam && !fn_710009d50c(beam->vf_0x0(), fn_71008424e8()))
        beam = nullptr;
    return fn_7100845fac(beam);
}

void fn_71009a0c80(UnkHangParams_l9a0c80* self) {
    UnkHangOwner_l9a0c80* owner = self->owner;
    UnkHangGun_l9a0c80* gun = owner->gun;
    if (!gun)
        return;
    UnkAnimationComponent* animation = owner->animation;
    if (!gun->busy && !HyperBeamReady(gun) && !fn_7100df59a8(g_unk_7101d47310))
        return;
    if (fn_710117b348(animation, fn_7100c2f4bc()))
        return;

    const UnkHangBlend_l9a0c80* blend = reinterpret_cast<const UnkHangBlend_l9a0c80*>(&g_unk_7172769740);
    u32 flags = UnkNone_l9a0c80().value;
    u64 kind = blend->kind;
    u32 value = blend->value;
    u32 mask = UnkAll_SpitSend().value;
    UnkHangRequest_l9a0c80 request;
    request.hash = 0x0addd0f24aa5f054;
    request.id = -1;
    request.blend_time = 0.12f;
    request.mode = 1;
    request.blend_kind = kind;
    request.blend_value = value;
    request.flags = flags;
    request.mask = mask;

    UnkTrackQuery query = {fn_7100c5bb9c(), nullptr, -1.0f};
    if (fn_710117b08c(animation, &query))
        reinterpret_cast<UnkHangTrack_l9a0c80*>(query.result)->vf_0x18(&request);
    fn_7101176ea8(reinterpret_cast<UnkStruct_7101176ea8*>(animation),
                  reinterpret_cast<const UnkRequest_7101176ea8*>(&request), 0xf4061ff25a2163d6);
}
