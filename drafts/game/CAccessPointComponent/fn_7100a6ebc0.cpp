// CAccessPointComponent's unit (tools/tu_map.py unit 1581): the access point's leave step. Fades the game
// manager's effect player with the char class's two times; outside the main menu the char class's music is
// requested again in the main menu (kind 4), the sound system's first group is switched on, the user's
// timeline id is played and the two counters are cleared; elsewhere the component's own slot 0x2b8 runs. Names and
// layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/snd/CSoundSystem.h"
#include "game/CBossAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkMask.h"
#include "game/UnkTU23.h"
#include "game/UnkTU2656.h"
#include "game/UnkTimelineComponent.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;

struct UnkMgr_0xf18;

// The music manager, created on first use.
static inline void* MusicManager() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(manager);
    }
    return manager;
}

// The music request's empty mode, built on first use.
inline const UnkMask& MusicModeNone_la6ebc0() {
    static UnkMask s_none asm("g_unk_717274f508");
    return s_none;
}

// The game manager as this function reads it: the current mode's name at +0x438.
struct UnkGameManagerMode_la6ebc0 {
    u8 pad_0x0[0x438];
    CRntString mode;  // 0x438
};

struct UnkApMusic_la6ebc0 {
    u8 pad_0x0[0x10];
    CRntString name;  // 0x10
    u32 field_0x40;
    u32 use_count;  // 0x44
};

struct UnkApCharClass_la6ebc0 {
    u8 pad_0x0[0x190];
    UnkApMusic_la6ebc0* music;  // 0x190
    u8 pad_0x198[0x1ac - 0x198];
    f32 fade_in;   // 0x1ac
    f32 fade_out;  // 0x1b0
};

struct UnkAp_la6ebc0 {
    void** vtable;
    u8 pad_0x8[0xe0];
    UnkWeakHandle* user;  // 0xe8
    u8 pad_0xf0[0x698 - 0xf0];
    UnkApCharClass_la6ebc0* char_class;  // 0x698
    u8 pad_0x6a0[0x908 - 0x6a0];
    u32 field_0x908;
    u8 pad_0x90c[0x914 - 0x90c];
    u32 field_0x914;

    void vf_0x2b8(bool flag) {
        reinterpret_cast<void (*)(UnkAp_la6ebc0*, bool)>(vtable[0x2b8 / 8])(this, flag);
    }
};
static_assert(__builtin_offsetof(UnkAp_la6ebc0, field_0x914) == 0x914, "layout");

struct UnkMusicMode;
void CSoundSystem_SetGroupsMuted(UnkSoundSystemMasks_ovb0074* self, bool on, const u32* mask);
extern const u64 g_unk_710164b548;

void fn_7100a6ebc0(UnkAp_la6ebc0* self) {
    void* fx = g_unk_7101d47310->field_0x1040;
    f32 fade_in = self->char_class->fade_in;
    f32 fade_out = self->char_class->fade_out;
    base::math::CVector3D color;
    color.x = 0.0f;
    color.y = 0.0f;
    color.z = 0.0f;
    fn_7101053c14(fx, &color, fade_in, fade_out, 1.0f);
    if (CRntString_EqualsCString(&reinterpret_cast<UnkGameManagerMode_la6ebc0*>(g_unk_7101d47310)->mode, "MAINMENU")) {
        UnkApCharClass_la6ebc0* char_class = self->char_class;
        void* music = MusicManager();
        ++char_class->music->use_count;
        const CRntString* name = &char_class->music->name;
        UnkMask mode(MusicModeNone_la6ebc0());
        fn_7100fd90d8(-1.0f, -1.0f, music, name, 4, reinterpret_cast<const UnkMusicMode*>(&mode), 0, 0, nullptr);
        alignas(8) u32 mask = 1;
        CSoundSystem_SetGroupsMuted(reinterpret_cast<UnkSoundSystemMasks_ovb0074*>(g_unk_7101d4d820), true, &mask);
        self->field_0x914 = 0;
        if (self->user && self->user->target)
            fn_7100d7c828(reinterpret_cast<UnkActor*>(self->user->target), &g_unk_710164b548, false);
        self->field_0x908 = 0;
    } else {
        self->vf_0x2b8(false);
    }
}
