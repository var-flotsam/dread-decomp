// UnkTU1501 (CSamusMovement's unit, tools/tu_map.py unit 1501): ends a relax shot. Does nothing while a
// charged hyper beam fires, the input is held, the gun is busy or the game manager says so; otherwise
// plays what the relax-shot-end track asks for, or sends the animation the stealth track's sources, the
// crouch or the default end animation. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CGunComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1730.h"
#include "types.h"

using base::global::CStrId;

struct UnkGunComponent;
const base::global::CStrId* fn_71008407fc(UnkGunComponent* self);
bool fn_7100845fac(void* beam);
bool fn_7100df59a8(UnkGameManager*);

// Needs UnkGameManagerIdTable (include/game/UnkGameManagerIdTable.h) to name its CStrId members at
// +0xcc8 and +0x1da8 (field_0xcc8, field_0x1da8).

struct UnkShotDesc_l9ec09c {
    u64 words[2];
};

struct __attribute__((packed, aligned(4))) UnkShotBlend_l9ec09c {
    u64 kind;
    f32 value;
};

// An animation request: unset while the id is -1.
struct UnkShotRequest_l9ec09c {
    s64 id;
    u64 desc[2];      // 0x8
    u64 blend_kind;   // 0x18
    u32 blend_value;  // 0x20
    u32 flags;        // 0x24
    u32 mask;         // 0x28
};

// The track a query resolves, as it fills in a request.
struct UnkShotTrack_l9ec09c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkShotRequest_l9ec09c* request);
};

struct UnkShotGun_l9ec09c {
    u8 pad_0x0[0x178];
    bool busy;  // 0x178
};

struct UnkShotOwner_l9ec09c {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;  // 0x1d8
    u8 pad_0x1e0[0x220 - 0x1e0];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2a8 - 0x228];
    UnkShotGun_l9ec09c* gun;  // 0x2a8
};

struct UnkShotMovement_l9ec09c {
    u8 pad_0x0[0x30];
    UnkShotOwner_l9ec09c* owner;  // 0x30
    u8 pad_0x38[0x7e4 - 0x38];
    bool stealth;  // 0x7e4
};

struct UnkShotArgs_l9ec09c {
    u8 pad_0x0[0xc];
    bool aiming;  // 0xc
};

// A mode or mask passed along with a request.
struct UnkSendArg_l9ec09c : base::reflection::UnkReflTypeIdArg {
    explicit UnkSendArg_l9ec09c(u32 value) { id = value; }
};

// Sends the animation an id with the given mode and mask.
static inline void Send_l9ec09c(void* target, u64 hash, UnkSendArg_l9ec09c&& mode, UnkSendArg_l9ec09c&& mask) {
    fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

static inline bool SameId_l9ec09c(const CStrId& a, const CStrId& b) {
    return a.data == b.data;
}

// Whether the gun fires a charged hyper beam.
static inline bool HyperBeamFiring_l9ec09c(UnkShotGun_l9ec09c* gun) {
    const CStrId* current = fn_71008407fc(reinterpret_cast<UnkGunComponent*>(gun));
    if (!SameId_l9ec09c(*current, GameManagerIdTable().field_0x1848))
        return false;
    base::reflection::UnkReflObject* beam = static_cast<base::reflection::UnkReflObject*>(fn_710083cd30(reinterpret_cast<UnkComponent*>(gun)));
    if (beam && !fn_710009d50c(beam->vf_0x0(), fn_71008424e8()))
        beam = nullptr;
    return fn_7100845fac(beam);
}

void fn_71009ec09c(UnkShotMovement_l9ec09c* self, UnkShotArgs_l9ec09c* args) {
    UnkShotGun_l9ec09c* gun = self->owner->gun;
    if (gun && HyperBeamFiring_l9ec09c(gun))
        return;
    if (UnkInputComponent* input = self->owner->input) {
        if (fn_710087238c(input))
            return;
    }
    if (gun && gun->busy)
        return;
    if (fn_7100df59a8(g_unk_7101d47310))
        return;
    if (args->aiming && !self->stealth)
        return;
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return;

    UnkTrackQuery end = {fn_7100d25908(), nullptr, -1.0f};
    fn_710117b08c(animation, &end);
    const UnkShotDesc_l9ec09c* desc = reinterpret_cast<const UnkShotDesc_l9ec09c*>(&g_unk_710164b958);
    u64 desc0 = desc->words[0];
    u64 desc1 = desc->words[1];
    const UnkShotBlend_l9ec09c* blend = reinterpret_cast<const UnkShotBlend_l9ec09c*>(&g_unk_7172769740);
    u64 kind = blend->kind;
    u32 value = *reinterpret_cast<const u32*>(&blend->value);
    u32 mask = UnkAll_SpitSend().value;
    UnkShotRequest_l9ec09c request;
    request.id = -1;
    request.desc[0] = desc0;
    request.desc[1] = desc1;
    request.blend_kind = kind;
    request.blend_value = value;
    request.flags = 0;
    request.mask = mask;
    if (end.result) {
        reinterpret_cast<UnkShotTrack_l9ec09c*>(end.result)->vf_0x18(&request);
        if (request.id != -1) {
            fn_7101176ea8(reinterpret_cast<UnkStruct_7101176ea8*>(animation),
                          reinterpret_cast<const UnkRequest_7101176ea8*>(&request), 0xf4061ff25a2163d6);
            return;
        }
    }

    if (self->stealth) {
        UnkTrackQuery stealth = {fn_7100d33d28(), nullptr, -1.0f};
        fn_710117b08c(animation, &stealth);
        if (stealth.result && stealth.result->sources.data != &g_unk_7101d47108->empty) {
            Send_l9ec09c(animation, stealth.result->sources.data->Key(), UnkSendArg_l9ec09c(0), UnkSendArg_l9ec09c(UnkAll_SpitSend().value));
            return;
        }
        Send_l9ec09c(animation, GameManagerIdTable().field_0x1da8.data->Key(), UnkSendArg_l9ec09c(0), UnkSendArg_l9ec09c(UnkAll_SpitSend().value));
        return;
    }
    UnkAnimationComponent* current = self->owner->animation;
    if (current && fn_710117b348(current, fn_7100cf3a18())) {
        Send_l9ec09c(animation, GameManagerIdTable().field_0xcc8.data->Key(), UnkSendArg_l9ec09c(0), UnkSendArg_l9ec09c(UnkAll_SpitSend().value));
        return;
    }
    Send_l9ec09c(animation, GameManagerIdTable().field_0x1da8.data->Key(), UnkSendArg_l9ec09c(0), UnkSendArg_l9ec09c(UnkAll_SpitSend().value));
}
