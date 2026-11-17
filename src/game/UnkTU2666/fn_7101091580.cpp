// Unit 2666 (Lua bindings): plays a music track by name on the music manager, the optional second
// argument naming how it treats the music stack. Names and layouts are placeholders.
#include <cstring>

#include "base/UnkLoadingMask.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/CBossAIComponent.h"
#include "game/UnkMask.h"
#include "game/UnkTU2677.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "game/sound/UnkMusicKind.h"
#include "types.h"

using base::global::CRntString;

// The channel mask as src/game/UnkLoadingScreen.cpp defines it (passed through a copy), and its two
// guarded constants.
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

// The music stack mode named `name` (0x7fffffff when unknown).
static inline s32 StackModeFromName(const char* name) {
    if (strcmp(name, "None") == 0)
        return 0;
    if (strcmp(name, "PLAY_NEXT") == 0)
        return 1;
    if (strcmp(name, "CLEAR_STACK") == 0)
        return 2;
    if (strcmp(name, "REMOVE_MUSIC_FROM_STACK") == 0)
        return 3;
    return 0x7fffffff;
}

s32 fn_7101091580(lua_State* L) {
    const char* name = nullptr;
    const char* modeName = nullptr;
    if (fn_71010c0ad0(L, 0, 1, "s|s", &name, &modeName)) {
        s32 mode = modeName ? StackModeFromName(modeName) : 1;
        u32 bits = 1u << mode;
        void* manager = MusicManager();
        CRntString key(name);
        fn_7100fd82dc(static_cast<UnkMusicMgr_b0019*>(manager), &key, UnkKind(bits),
                      UnkMask(UnkLoadingMaskOf<UnkLoadingMaskTag>::All().bits), 0.0f);
    }
    return 0;
}
