// CTrainUsableComponent's unit (tools/tu_map.py): the train's use step. After the usable component's own
// step (fn_7100aa23d4), when the owner's animation (+0x220) plays the train animation, the music manager
// (g_unk_71726bc560, created on first use) switches channels 0xc over 1.5 seconds and the game manager
// runs fn_7100e015b8; the player's input component (+0x1d8) is then re-enabled. The mask types are
// src/game/UnkLoadingScreen.cpp's. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CInputComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

struct UnkUsableComponent2_A6_b62;
void fn_7100aa23d4(UnkUsableComponent2_A6_b62* self);

extern void* g_unk_71726bc560;
// The channel mask as src/game/UnkLoadingScreen.cpp defines it (passed through a copy), and its two
// guarded constants.
struct UnkLoadingMask {
    u32 bits;
    UnkLoadingMask() : bits(0) {}
    UnkLoadingMask(u32 b) : bits(b) {}
    UnkLoadingMask(const UnkLoadingMask& o) : bits(o.bits) {}
};
template <class T>
struct UnkLoadingMaskOf {
    static const UnkLoadingMask& None() {
        static UnkLoadingMask s;
        return s;
    }
    static const UnkLoadingMask& All() {
        static UnkLoadingMask s(~None().bits & 0x1ff);
        return s;
    }
};
struct UnkLoadingMaskTag;

// A copy of `mask` with `bit` flipped.
static inline UnkLoadingMask Flipped(const UnkLoadingMask& mask, u32 bit) {
    return UnkLoadingMask((mask.bits & bit) ? mask.bits & ~bit : mask.bits | bit);
}

// The music manager, created on first use.
static inline void* MusicManager() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = manager;
    }
    return manager;
}

struct UnkTrainIds_ovb0135 {
    u8 pad_0x0[0x30];
    u64 animation;  // 0x30
};
extern __attribute__((visibility("hidden"))) UnkTrainIds_ovb0135* g_unk_7172752d38;
extern __attribute__((visibility("hidden"))) char g_unk_7172752d30[];

struct UnkTrainOwner_ovb0135 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkTrainUsable_ovb0135 {
    u8 pad_0x0[0x30];
    UnkTrainOwner_ovb0135* owner;  // 0x30
};

struct UnkTrainPlayer_ovb0135 {
    u8 pad_0x0[0x1d8];
    struct UnkInputComponent* input;  // 0x1d8
};

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
struct UnkInputComponent;
void fn_7100e015b8(UnkGameManager* manager);

void fn_7100a99ba4(UnkTrainUsable_ovb0135* self) {
    fn_7100aa23d4(reinterpret_cast<UnkUsableComponent2_A6_b62*>(self));
    UnkAnimationComponent* animation = self->owner->animation;
    if (animation && fn_710117ae38(animation, g_unk_7172752d38->animation, false)) {
        fn_7100fd7b44(MusicManager(), UnkLoadingMask(0xc),
                      Flipped(UnkLoadingMaskOf<UnkLoadingMaskTag>::All(), 0x100), 1.5f);
        fn_7100e015b8(g_unk_7101d47310);
    }
    UnkTrainPlayer_ovb0135* player = reinterpret_cast<UnkTrainPlayer_ovb0135*>(fn_7100dec788(g_unk_7101d47310));
    if (player && player->input)
        fn_7100872460(player->input, true, false, g_unk_7172752d30);
}
