// CSamusHangMovementState::CParams unit: on a CCharClassCheckHangShotEndEvent, unless the owner's gun is busy
// or holds a ready hyper beam, or the game manager says so, sends the owner's animation component the id of
// the stealth tweak track (when the movement is in stealth and the track names one) or the id table's
// default with mode 0 (as UnkSpitSend, src/game_s2/UnkSpitSend.h). Names and layouts are placeholders.
#include "../../../../build/lean/scratch/game/UnkGameManagerIdTable.h"
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CGunComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkGunComponent;
using base::global::CStrId;
const CStrId* fn_71008407fc(UnkGunComponent* self);
bool fn_7100845fac(void* beam);
bool fn_7100df59a8(UnkGameManager*);

struct UnkHangShotGun_l9a2758 {
    u8 pad_0x0[0x178];
    bool busy;  // 0x178
};

struct UnkHangShotOwner_l9a2758 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2a8 - 0x228];
    UnkHangShotGun_l9a2758* gun;  // 0x2a8
};

struct UnkHangShotMovement_l9a2758 {
    u8 pad_0x0[0x7e4];
    bool stealth;  // 0x7e4
};

struct UnkHangShotParams_l9a2758 {
    u8 pad_0x0[0x30];
    UnkHangShotOwner_l9a2758* owner;        // 0x30
    UnkHangShotMovement_l9a2758* movement;  // 0x38
};

struct UnkHangShotEvent_l9a2758 : base::reflection::UnkReflObject {
    u8 pad_0x8[4];
    bool field_0xc;
};

static bool IsEmpty(const CStrId& id) {
    return id.data == &g_unk_7101d47108->empty;
}

// Whether the owner's gun keeps the shot going: a ready hyper beam or a busy gun.
static bool GunBusy(UnkHangShotOwner_l9a2758* owner) {
    if (!owner)
        return false;
    UnkHangShotGun_l9a2758* gun = owner->gun;
    if (!gun)
        return false;
    if (*fn_71008407fc(reinterpret_cast<UnkGunComponent*>(gun)) == GameManagerIdTable().field_0x1848) {
        auto* beam = static_cast<base::reflection::UnkReflObject*>(
            fn_710083cd30(reinterpret_cast<UnkComponent*>(gun)));
        if (beam && !fn_710009d50c(beam->vf_0x0(), fn_71008424e8()))
            beam = nullptr;
        if (fn_7100845fac(beam))
            return true;
    }
    return gun->busy;
}

void fn_71009a2758(UnkHangShotParams_l9a2758* self, UnkHangShotEvent_l9a2758* event) {
    if (!fn_710009d50c(event->vf_0x0(), fn_7100cafed8()))
        return;
    if (GunBusy(self->owner) || fn_7100df59a8(g_unk_7101d47310))
        return;
    if (event->field_0xc && !self->movement->stealth)
        return;
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return;
    base::reflection::UnkReflTypeIdArg mode;
    const CStrId* id = &GameManagerIdTable().field_0x1320;
    UnkTrackQuery query;
    if (self->movement->stealth) {
        query = {fn_7100d33d28(), nullptr, -1.0f};
        fn_710117b08c(animation, &query);
        if (query.result && !IsEmpty(query.result->sources))
            id = &query.result->sources;
    }
    u64 hash = id->data->Key();
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    CAnimationComponent_SendActionToTrack(animation, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}
