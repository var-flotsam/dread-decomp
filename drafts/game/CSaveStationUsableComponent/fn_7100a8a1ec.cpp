// CSaveStationUsableComponent's unit (tools/tu_map.py 1596): the end of a save. Fades the game manager's effect
// player from black with the char class's timing; outside the main menu calls its own slot 0x2b8, otherwise plays
// the char class's music again, unmutes the sound system, clears the time and plays the user's timeline id.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/math/CVector.h"
#include "base/reflection.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/snd/CSoundSystem.h"
#include "game/CBossAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU23.h"
#include "game/UnkTU2656.h"
#include "game/UnkTimelineComponent.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;

struct UnkMgr_0xf18;

void CSoundSystem_SetGroupsMuted(UnkSoundSystemMasks_ovb0074* self, bool on, const u32* mask);

// A mode word passed by address (the word after it is not set).
struct UnkMusicMode {
    u32 kind;
    u32 field_0x4;
};

extern const u64 g_unk_710164b5b8;

// The game manager's current mode name.
struct UnkSaveStationManager_la8a1ec {
    u8 pad_0x0[0x438];
    CRntString mode;  // 0x438
};

struct UnkSaveStationMusic_la8a1ec {
    u8 pad_0x0[0x10];
    CRntString name;  // 0x10
    u8 pad_0x40[4];
    u32 use_count;    // 0x44
};

struct UnkSaveStationClass_la8a1ec {
    u8 pad_0x0[0x190];
    UnkSaveStationMusic_la8a1ec* music;  // 0x190
    u8 pad_0x198[0x14];
    f32 fade_delay;     // 0x1ac
    f32 fade_duration;  // 0x1b0
};

struct UnkSaveStation_la8a1ec {
    void** vtable;
    u8 pad_0x8[0x8];
    base::reflection::UnkReflObject* char_class;  // 0x10
    u8 pad_0x18[0xd0];
    UnkWeakHandle* user;  // 0xe8
    u8 pad_0xf0[0x464];
    f32 time;   // 0x554
    f32 field_0x558;

    void vf_0x2b8(bool flag) {
        reinterpret_cast<void (*)(UnkSaveStation_la8a1ec*, bool)>(vtable[0x2b8 / 8])(this, flag);
    }
};

// The char class, when it is a CCharClassSaveStationUsableComponent.
static inline UnkSaveStationClass_la8a1ec* CharClass(base::reflection::UnkReflObject* object) {
    return fn_710009d50c(object->vf_0x0(), fn_7100a8886c()) ? reinterpret_cast<UnkSaveStationClass_la8a1ec*>(object)
                                                            : nullptr;
}

static inline void* MusicManager() {
    void* music = g_unk_71726bc560;
    if (!music) {
        music = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(music);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(music);
    }
    return music;
}

void fn_7100a8a1ec(UnkSaveStation_la8a1ec* self) {
    u8* fx = g_unk_7101d47310->field_0x1040;
    f32 delay = CharClass(self->char_class)->fade_delay;
    f32 duration = CharClass(self->char_class)->fade_duration;
    base::math::CVector3D black;
    black.x = 0.0f;
    black.y = 0.0f;
    black.z = 0.0f;
    fn_7101053c14(fx, &black, delay, duration, 1.0f);
    if (!CRntString_EqualsCString(&reinterpret_cast<UnkSaveStationManager_la8a1ec*>(g_unk_7101d47310)->mode, "MAINMENU")) {
        self->vf_0x2b8(false);
        return;
    }
    base::reflection::UnkReflObject* object = self->char_class;
    UnkSaveStationClass_la8a1ec* char_class = object ? CharClass(object) : nullptr;
    void* music = MusicManager();
    ++char_class->music->use_count;
    const CRntString* name = &char_class->music->name;
    UnkMusicMode mode;
    mode.kind = base::reflection::refl_GetEnumZero<0x717274f508>();
    fn_7100fd90d8(-1.0f, -1.0f, music, name, 4, &mode, 0, 0, nullptr);
    UnkSoundMgr* sound = g_unk_7101d4d820;
    alignas(8) u32 mask = 1;
    CSoundSystem_SetGroupsMuted(reinterpret_cast<UnkSoundSystemMasks_ovb0074*>(sound), true, &mask);
    self->time = 0.0f;
    if (self->user && self->user->target)
        fn_7100d7c828(reinterpret_cast<UnkActor*>(self->user->target), &g_unk_710164b5b8, false);
    self->field_0x558 = 0.0f;
}
