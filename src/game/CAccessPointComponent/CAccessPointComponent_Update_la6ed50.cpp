// CAccessPointComponent's unit (tools/tu_map.py, unit 1581): the update while the access point is in use,
// as CMapAcquisitionComponent's fn_7100a876fc: counts the time (+0x914); unless the char class's music
// (+0x698) is still playing and the tunable wait is set and not over, has g_unk_7101d47368 run
// fn_7100d8e0a0, plays two timeline ids on the user (+0xe8) and calls its own slot 0x2b8. Names and
// layouts are placeholders.
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

struct UnkAccessPointMusic_la6ed50 {
    u8 pad_0x0[0x10];
    char text[0x34];
    u32 use_count;  // 0x44
};

struct UnkAccessPointClass_la6ed50 {
    u8 pad_0x0[0x190];
    UnkAccessPointMusic_la6ed50* music;  // 0x190
};

struct UnkAccessPoint_la6ed50 {
    void** vtable;
    u8 pad_0x8[0xe0];
    UnkWeakHandle* user;  // 0xe8
    u8 pad_0xf0[0x698 - 0xf0];
    UnkAccessPointClass_la6ed50* char_class;  // 0x698
    u8 pad_0x6a0[0x914 - 0x6a0];
    f32 time;  // 0x914

    void vf_0x2b8(bool flag) {
        reinterpret_cast<void (*)(UnkAccessPoint_la6ed50*, bool)>(vtable[0x2b8 / 8])(this, flag);
    }
};
static_assert(__builtin_offsetof(UnkAccessPoint_la6ed50, time) == 0x914, "time");

extern const u64 g_unk_710164b550;
extern const u64 g_unk_710164b558;

inline const u32& AllMusicLayers_la6ed50() {
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

void fn_7100a6ed50(UnkAccessPoint_la6ed50* self, f32 dt) {
    self->time += dt;
    UnkAccessPointClass_la6ed50* char_class = self->char_class;
    void* music = MusicManager();
    ++char_class->music->use_count;
    const char* name = char_class->music->text;
    alignas(8) u32 mask = AllMusicLayers_la6ed50();
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
        fn_7100d7c94c(reinterpret_cast<UnkActor*>(self->user->target), &g_unk_710164b550);
        UnkWeakTarget* target = self->user ? self->user->target : nullptr;
        fn_7100d7c828(reinterpret_cast<UnkActor*>(target), &g_unk_710164b558, false);
    }
    self->vf_0x2b8(false);
}
