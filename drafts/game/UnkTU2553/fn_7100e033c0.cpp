// Unit 2553 (CGameManager): mutes or restores the music, a sound group and the rumble channels while the
// game is paused. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/input/UnkRumbleManager.h"
#include "game/CBossAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU23.h"
#include "game/sound/CMusicManager.h"
#include "types.h"

struct UnkMusicMgr_b0019;
void fn_7100fdba30(UnkMusicMgr_b0019* self);
void fn_7100fdbbd0(UnkMusicMgr_b0019* self, bool force);

struct UnkSoundSystemMasks_ovb0074;
void fn_7100108464(UnkSoundSystemMasks_ovb0074* self, bool on, const u32* mask);
void fn_7100108b34(UnkSoundSystemMasks_ovb0074* self, s32 group, bool on, f32 time);

using base::input::UnkRumbleManager;
extern UnkRumbleManager* g_unk_7101d47348;
struct UnkTunableRumbleManager_b0068;
void fn_7100075020(UnkTunableRumbleManager_b0068* self, bool flag, const u32* channels);

// The game's mode holder at +0x498.
struct UnkModeHolder_le033c0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual bool vf_0x68();
};

static inline UnkMusicMgr_b0019* MusicManager() {
    if (!g_unk_71726bc560) {
        void* mem = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(mem);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(mem);
    }
    return reinterpret_cast<UnkMusicMgr_b0019*>(g_unk_71726bc560);
}

static inline UnkTunableRumbleManager_b0068* RumbleManager() {
    if (!g_unk_7101d47348) {
        UnkRumbleManager* m = static_cast<UnkRumbleManager*>(fn_71000839a0(0x2210, 0x3f));
        fn_71000733e4(m);
        g_unk_7101d47348 = m;
    }
    return reinterpret_cast<UnkTunableRumbleManager_b0068*>(g_unk_7101d47348);
}

static inline UnkSoundSystemMasks_ovb0074* SoundSystem() {
    return reinterpret_cast<UnkSoundSystemMasks_ovb0074*>(g_unk_7101d4d820);
}

void fn_7100e033c0(UnkGameManager* self, bool pause, bool keep) {
    if (pause) {
        fn_7100fdba30(MusicManager());
        UnkSoundSystemMasks_ovb0074* sound = SoundSystem();
        alignas(8) u32 mask = 1;
        fn_7100108464(sound, true, &mask);
        fn_7100108b34(SoundSystem(), 5, true, 0.2f);
        UnkTunableRumbleManager_b0068* rumble = RumbleManager();
        alignas(8) u32 channels = 0x19;
        fn_7100075020(rumble, true, &channels);
    } else {
        UnkModeHolder_le033c0* mode = static_cast<UnkModeHolder_le033c0*>(self->field_0x498);
        if (mode && mode->vf_0x68() && keep)
            return;
        fn_7100fdbbd0(MusicManager(), true);
        fn_7100108b34(SoundSystem(), 5, false, 0.2f);
        UnkSoundSystemMasks_ovb0074* sound = SoundSystem();
        alignas(8) u32 mask = 1;
        fn_7100108464(sound, false, &mask);
        UnkTunableRumbleManager_b0068* rumble = RumbleManager();
        alignas(8) u32 channels = 0x19;
        fn_7100075020(rumble, false, &channels);
    }
}
