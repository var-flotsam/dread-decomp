// CMapAcquisitionComponent's unit: the start of the map acquisition. Plays the game manager's fade with the
// char class's two times; in the main menu mode starts the char class's music, sets the sound system's mask,
// resets the times and plays a timeline id on the user (+0xe8), elsewhere calls its own slot 0x2b8.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
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

struct UnkActor;
struct UnkMgr_0xf18;
struct UnkSoundSystemMasks_ovb0074;

void fn_71001082a8(UnkSoundSystemMasks_ovb0074* self, bool on, const u32* mask);

struct UnkMusicMode {
    u32 kind;
    u32 field_0x4;
};

extern const u64 g_unk_710164b5a8;

// The game manager's mode name, seen at +0x438.
struct UnkGameManagerMode_la8756c {
    u8 pad_0x0[0x438];
    CRntString mode;  // 0x438
};

struct UnkMapAcquisitionMusic_la8756c {
    u8 pad_0x0[0x10];
    CRntString name;  // 0x10
    u8 pad_0x40[0x4];
    u32 use_count;  // 0x44
};
static_assert(__builtin_offsetof(UnkMapAcquisitionMusic_la8756c, use_count) == 0x44, "layout");

struct UnkMapAcquisitionClass_la8756c {
    u8 pad_0x0[0x190];
    UnkMapAcquisitionMusic_la8756c* music;  // 0x190
    u8 pad_0x198[0x1ac - 0x198];
    f32 fade_delay;     // 0x1ac
    f32 fade_duration;  // 0x1b0
};

struct UnkMapAcquisition_la8756c {
    void** vtable;
    u8 pad_0x8[0xe0];
    UnkWeakHandle* user;  // 0xe8
    u8 pad_0xf0[0x7f8 - 0xf0];
    UnkMapAcquisitionClass_la8756c* char_class;  // 0x7f8
    f32 field_0x800;
    u8 pad_0x804[0xc];
    f32 time;  // 0x810

    void vf_0x2b8(bool flag) {
        reinterpret_cast<void (*)(UnkMapAcquisition_la8756c*, bool)>(vtable[0x2b8 / 8])(this, flag);
    }
};

static inline void* MusicManager() {
    void* music = g_unk_71726bc560;
    if (!music) {
        music = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(music);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(music);
    }
    return music;
}

void fn_7100a8756c(UnkMapAcquisition_la8756c* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    UnkMapAcquisitionClass_la8756c* fade = self->char_class;
    f32 delay = fade->fade_delay;
    f32 duration = fade->fade_duration;
    void* fx = manager->field_0x1040;
    base::math::CVector3D black;
    black.x = 0.0f;
    black.y = 0.0f;
    black.z = 0.0f;
    fn_7101053c14(fx, &black, delay, duration, 1.0f);
    if (!fn_710007f140(&reinterpret_cast<UnkGameManagerMode_la8756c*>(g_unk_7101d47310)->mode, "MAINMENU")) {
        self->vf_0x2b8(false);
        return;
    }
    UnkMapAcquisitionClass_la8756c* char_class = self->char_class;
    void* music = MusicManager();
    ++char_class->music->use_count;
    const CRntString* name = &char_class->music->name;
    UnkMusicMode mode;
    mode.kind = base::reflection::refl_GetEnumZero<0x717274f508>();
    fn_7100fd90d8(-1.0f, -1.0f, music, name, 4, &mode, 0, 0, nullptr);
    UnkSoundSystemMasks_ovb0074* sound = reinterpret_cast<UnkSoundSystemMasks_ovb0074*>(g_unk_7101d4d820);
    alignas(8) u32 mask = 1;
    fn_71001082a8(sound, true, &mask);
    self->time = 0.0f;
    if (self->user && self->user->target)
        fn_7100d7c828(reinterpret_cast<UnkActor*>(self->user->target), &g_unk_710164b5a8, false);
    self->field_0x800 = 0.0f;
}
