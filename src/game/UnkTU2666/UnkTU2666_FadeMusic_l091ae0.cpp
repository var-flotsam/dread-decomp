// Unit 2666 Lua binding: fades the music manager's channels ("f|bs": time, a flag choosing channels 12
// or 2, an optional layer name) on every layer. The layer name is looked up but its mask is not used:
// the original passes the all-layers mask whatever the name. Names and layouts are placeholders.
#include "game/UnkMask.h"
#include "base/global/CRntVector.h"
#include "game/CBossAIComponent.h"
#include "game/UnkTU2666.h"
#include "game/UnkTU2677.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

struct lua_State;

namespace lua_bind {
s32 StopMusicWithFade(lua_State* L) {
    f32 time = -1.0f;
    bool flag = false;
    const char* name = nullptr;
    if (fn_71010c0270(L, "f|bs", &time, &flag, &name)) {
        // The named layer's mask is computed and then not used (faithful to the original).
        u32 layers = name ? 1u << fn_7101097578(name) : UnkMaskOf<UnkMusicMaskTag>::All().bits;
        (void)layers;
        UnkMask channels(flag ? 12 : 2);
        void* manager = g_unk_71726bc560;
        if (!manager) {
            manager = fn_71000839a0(0xf18, 0x3f);
            fn_7100fd3804(manager);
            g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(manager);
        }
        f32 duration = time;
        CTunableMusicManager_StopMusicOnAllSlots(static_cast<UnkMusicMgr_b0019*>(manager), channels, UnkMaskOf<UnkMusicMaskTag>::All(), duration);
    }
    return 0;
}
}  // namespace lua_bind
