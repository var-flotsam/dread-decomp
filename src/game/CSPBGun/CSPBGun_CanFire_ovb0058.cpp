// CSPBGun's unit (tools/tu_map.py unit 1317): whether the gun may fire (fn_710085e6d4). Names and
// layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;

struct UnkSPBGun_A5_b63;
base::global::UnkStringData** fn_7101177ae8(UnkAnimationComponent* self, u64 kind);

// What the usual track kind names (fn_7101177ae8 gives a pointer to it): its name and a use count.
struct UnkSPBTrack_b0058 {
    u8 pad_0x0[0x10];
    CRntString name;  // 0x10
    u8 pad_0x40[4];
    union {  // no type-based alias info: the track pointer is reloaded after the increment
        s32 uses;  // 0x44
    };
};

struct UnkSPBGunOwner_b0058 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* anim;  // 0x220
};

struct UnkSPBGun_b0058 {
    u8 pad_0x0[0x18];
    UnkSPBGunOwner_b0058* owner;  // 0x18
};

// No while the character class has a CCharClassCantFireTrack; while the current track's name starts
// with "spr_", only with a CCharClassAllowSPBShotTrack.
bool fn_710085e6d4(UnkSPBGun_A5_b63* gun) {
    static CRntString s_prefix("spr_");
    UnkSPBGun_b0058* self = reinterpret_cast<UnkSPBGun_b0058*>(gun);
    if (fn_710117b348(self->owner->anim, fn_7100cf0d38()))
        return false;
    UnkSPBTrack_b0058** track =
        reinterpret_cast<UnkSPBTrack_b0058**>(fn_7101177ae8(self->owner->anim, 0xf4061ff25a2163d6));
    ++(*track)->uses;
    if (fn_710007fc80(&(*track)->name, &s_prefix) && !fn_710117b348(self->owner->anim, fn_7100bf36f8()))
        return false;
    return true;
}
