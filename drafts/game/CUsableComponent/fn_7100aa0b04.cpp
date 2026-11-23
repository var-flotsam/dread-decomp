// CUsableComponent's unit (tools/tu_map.py): starts the use with the player's animation `user_anim` and the
// usable's own `own_anim` (empty: the user's). Fails without a player or when the player's animation
// component has no entry for the user's animation. When the player has a grabber and that entry is a
// grabber track, fills the grab record (+0x40) and hands it to the grabber; otherwise plays the user's
// animation on the player (unless use kind 7 is blocked by the player's +0x208) and the own animation on
// the owner, each restarting (mode 2) unless already playing. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/CGrabComponent.h"
#include "game/CUsableComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2656.h"
#include "types.h"

using base::global::CStrId;

struct UnkScreenFadeState_b48a3;
bool fn_7101053a1c(UnkScreenFadeState_b48a3* self, f32 t);
bool fn_7101176e78(UnkStruct_7101176ea8* self, void* key, void** out);
bool fn_710006ba58(void* tracks, const base::reflection::CClass* cls);
bool fn_7101177258(UnkAnimationComponent* self, void* id, u64 variant, u64 key, const u32& flags, const UnkS16& a,
                   const UnkS12& b, const u32& c);

// An animation entry: its tracks at +0x170.
struct UnkUsableAnimEntry_laa0b04 {
    u8 pad_0x0[0x170];
    u8 tracks[1];

    bool HasTrack(const base::reflection::CClass* cls) { return fn_710006ba58(tracks, cls); }
};

struct UnkUsablePlayerState_laa0b04 {
    u8 pad_0x0[0x10];
    u32 kind;
};

struct UnkUsablePlayerMode_laa0b04 {
    u8 pad_0x0[0x2d08];
    UnkUsablePlayerState_laa0b04* state;
};

struct UnkUsablePlayer_laa0b04 {
    u8 pad_0x0[0x208];
    UnkUsablePlayerMode_laa0b04* mode;  // 0x208
    u8 pad_0x210[0x10];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x38];
    void* grabber;  // 0x260
};

struct UnkUsableOwner_laa0b04 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

// The record handed to the grabber.
struct UnkUsableGrab_laa0b04 {
    u8 pad_0x0[0x10];
    CStrId user_anim;  // 0x10
    CStrId own_anim;   // 0x18
    u8 pad_0x20[0x4];
    u32 field_0x24;
    u8 pad_0x28[0x2];
    bool field_0x2a;
    bool field_0x2b;
    u8 pad_0x2c[0x4];
    u32 field_0x30;
};

struct UnkUsableUse_laa0b04 {
    u8 pad_0x0[0x10];
    u32 kind;
};

struct UnkUsableStart_laa0b04 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkUsableOwner_laa0b04* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkUsableGrab_laa0b04 grab;  // 0x40
    u8 pad_0x78[0x98];
    UnkUsableUse_laa0b04* use;  // 0x110
};
static_assert(__builtin_offsetof(UnkUsableStart_laa0b04, use) == 0x110, "UnkUsableStart_laa0b04 layout");

bool fn_7100aa0b04(UnkUsableVirt_A6_b62* component, u64 a, u64 b, const u8* c) {
    auto* self = reinterpret_cast<UnkUsableStart_laa0b04*>(component);
    const CStrId& user_anim = *reinterpret_cast<const CStrId*>(a);
    const CStrId& own_anim = *reinterpret_cast<const CStrId*>(b);
    auto* player = reinterpret_cast<UnkUsablePlayer_laa0b04*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return false;
    if (user_anim.data == &g_unk_7101d47108->empty)
        return false;
    UnkUsableAnimEntry_laa0b04* entry = nullptr;
    if (!fn_7101176e78(reinterpret_cast<UnkStruct_7101176ea8*>(player->animation),
                       reinterpret_cast<void*>(user_anim.data->Key()), reinterpret_cast<void**>(&entry)))
        return false;
    if (player->grabber && entry->HasTrack(fn_7100cff148())) {
        UnkUsableGrab_laa0b04& grab = self->grab;
        grab.field_0x24 = 0;
        grab.user_anim = user_anim;
        grab.own_anim = own_anim;
        grab.field_0x2a = false;
        grab.field_0x2b = false;
        if (user_anim.data != own_anim.data)
            grab.field_0x30 = 2;
        fn_710113d940(player->grabber, reinterpret_cast<UnkKraidGrab_b57a4*>(&grab), self->owner);
        return true;
    }

    // The requests' modes and masks, for the player's animation and for the owner's.
    base::reflection::UnkReflTypeIdArg user_mode, user_mask, own_mode, own_mask;
    // Use kind 7 plays nothing on the player unless its mode state is 0xc.
    bool blocked = false;
    if (self->use && self->use->kind == 7) {
        if (UnkUsablePlayerMode_laa0b04* mode = player->mode) {
            UnkUsablePlayerState_laa0b04* state = mode->state;
            blocked = !state || state->kind != 0xc;
        }
    }
    if (!blocked) {
        if (UnkAnimationComponent* animation = player->animation) {
            bool playing = fn_710117ae38(animation, user_anim.data->Key(), false);
            const UnkDesc_SpitSend* desc =
                fn_7101053a0c(g_unk_7101d47310->field_0x1040) ||
                        fn_7101053a1c(reinterpret_cast<UnkScreenFadeState_b48a3*>(g_unk_7101d47310->field_0x1040), 0.0f)
                    ? &g_unk_710164b968
                    : &g_unk_710164b958;
            user_mode.id = playing ? 0 : 2;
            user_mask.id = UnkAll_SpitSend().value;
            fn_7101177258(animation, reinterpret_cast<void*>(user_anim.data->Key()), *reinterpret_cast<const u64*>(c),
                          0xf4061ff25a2163d6, user_mode.id, *reinterpret_cast<const UnkS16*>(desc),
                          *reinterpret_cast<const UnkS12*>(&g_unk_7172769740), user_mask.id);
        }
    }

    if (UnkAnimationComponent* animation = self->owner->animation) {
        const CStrId& anim = own_anim.data == &g_unk_7101d47108->empty ? user_anim : own_anim;
        if (anim.data != &g_unk_7101d47108->empty) {
            own_mode.id = fn_710117ae38(animation, anim.data->Key(), false) ? 0 : 2;
            own_mask.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, anim.data->Key(), 0xf4061ff25a2163d6, &own_mode, &g_unk_710164b958,
                          &g_unk_7172769740, &own_mask);
        }
    }
    return true;
}
