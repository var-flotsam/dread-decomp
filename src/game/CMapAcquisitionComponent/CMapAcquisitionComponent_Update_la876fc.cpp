// CMapAcquisitionComponent's unit: the update while the map is acquired, as CSaveStationUsableComponent's
// fn_7100a8a438: counts the time (+0x810); unless the char class's music (+0x7f8) is still playing and
// the tunable wait is set and not over, has g_unk_7101d47368 run fn_7100d8e0a0, plays two timeline ids on
// the user (+0xe8) and calls its own slot 0x2b8. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBossAIComponent.h"
#include "game/CUsableComponent.h"
#include "game/UnkSaveStationTunables_ovb0118.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2523.h"
#include "game/UnkTimelineComponent.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkActor;
struct UnkMgr_0xf18;

struct UnkMapAcquisitionMusic_la876fc {
    u8 pad_0x0[0x10];
    char text[0x34];
    u32 use_count;  // 0x44
};

struct UnkMapAcquisitionClass_la876fc {
    u8 pad_0x0[0x190];
    UnkMapAcquisitionMusic_la876fc* music;  // 0x190
};

struct UnkMapAcquisition_la876fc {
    void** vtable;
    u8 pad_0x8[0xe0];
    UnkWeakHandle* user;  // 0xe8
    u8 pad_0xf0[0x7f8 - 0xf0];
    UnkMapAcquisitionClass_la876fc* char_class;  // 0x7f8
    u8 pad_0x800[0x10];
    f32 time;  // 0x810

    void vf_0x2b8(bool flag) {
        reinterpret_cast<void (*)(UnkMapAcquisition_la876fc*, bool)>(vtable[0x2b8 / 8])(this, flag);
    }
};

extern const u64 g_unk_710164b598;
extern const u64 g_unk_710164b5a0;

inline const u32& AllMusicLayers_la876fc() {
    static u32 s_value = ~base::reflection::refl_GetEnumZero<0x71726c4090>() & 0x1ff;
    return s_value;
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

void fn_7100a876fc(UnkMapAcquisition_la876fc* self, f32 dt) {
    self->time += dt;
    UnkMapAcquisitionClass_la876fc* char_class = self->char_class;
    void* music = MusicManager();
    ++char_class->music->use_count;
    const char* name = char_class->music->text;
    alignas(8) u32 mask = AllMusicLayers_la876fc();
    if (fn_7100fd6f74(music, name, &mask)) {
        f32 wait = g_unk_71727531f0.music_wait;
        if (wait + 0.001f >= 0.0f && wait - 0.001f <= 0.0f)
            return;
        if (!(self->time >= wait))
            return;
    }
    void* lua = g_unk_7101d47368;
    if (!lua) {
        lua = fn_71000839a0(0x4c8, 0x3f);
        fn_7100d8ced8(lua);
        g_unk_7101d47368 = lua;
    }
    fn_7100d8e0a0(lua);
    if (self->user && self->user->target) {
        fn_7100d7c94c(reinterpret_cast<UnkActor*>(self->user->target), &g_unk_710164b598);
        UnkWeakTarget* target = self->user ? self->user->target : nullptr;
        fn_7100d7c828(reinterpret_cast<UnkActor*>(target), &g_unk_710164b5a0, false);
    }
    self->vf_0x2b8(false);
}
