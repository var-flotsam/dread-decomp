// CSamusMagnetGloveMovementState::CParams unit: on a CCharClassCheckMagnetShotEndEvent, unless the owner's gun
// (+0x2a8) holds a ready hyper beam, is busy or the game manager says so, sends the owner's animation component
// (+0x220) the shot-end request for the params' pose kind. Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CGunComponent.h"
#include "game/CHyperBeamGun.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkGunComponent;
struct UnkComponent;

struct UnkMagnetGun_l9a8708 {
    u8 pad_0x0[0x178];
    bool busy;  // 0x178
};

struct UnkMagnetOwner_l9a8708 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
    u8 pad_0x228[0x2a8 - 0x228];
    UnkMagnetGun_l9a8708* gun;  // 0x2a8
};

struct UnkMagnetParams_l9a8708 {
    u8 pad_0x0[0x30];
    UnkMagnetOwner_l9a8708* owner;  // 0x30
    u8 pad_0x38[0x1c0 - 0x38];
    u32 pose_kind;  // 0x1c0
};

static bool HyperBeamReady(UnkMagnetGun_l9a8708* gun) {
    if (!(*fn_71008407fc(reinterpret_cast<UnkGunComponent*>(gun)) == GameManagerIdTable().field_0x1848))
        return false;
    auto* beam = static_cast<base::reflection::UnkReflObject*>(fn_710083cd30(reinterpret_cast<UnkComponent*>(gun)));
    if (beam && !fn_710009d50c(beam->vf_0x0(), fn_71008424e8()))
        beam = nullptr;
    return fn_7100845fac(beam);
}

static inline void Send(void* animation, u64 hash, base::reflection::UnkReflTypeIdArg* mode,
                        base::reflection::UnkReflTypeIdArg* mask) {
    mode->id = 0;
    mask->id = UnkAll_SpitSend().value;
    fn_7101176690(animation, hash, 0xf4061ff25a2163d6ull, mode, &g_unk_710164b958, &g_unk_7172769740, mask);
}

void fn_71009a8708(UnkMagnetParams_l9a8708* self, base::reflection::UnkReflObject* event) {
    if (!fn_710009d50c(event->vf_0x0(), fn_7100cb0528()))
        return;
    UnkMagnetGun_l9a8708* gun = self->owner->gun;
    if (!gun)
        return;
    if (HyperBeamReady(gun) || gun->busy || fn_7100df59a8(g_unk_7101d47310))
        return;

    void* animation = self->owner->animation;
    switch (self->pose_kind) {
    case 0:
        base::reflection::UnkReflTypeIdArg mode_a;
        base::reflection::UnkReflTypeIdArg mask_a;
        Send(animation, 0xe24a858662f80d34ull, &mode_a, &mask_a);
        break;
    case 1:
    case 2:
        base::reflection::UnkReflTypeIdArg mode_b;
        base::reflection::UnkReflTypeIdArg mask_b;
        Send(animation, 0x09c4bee1802c2eefull, &mode_b, &mask_b);
        break;
    }
}
