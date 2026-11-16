// Unit 2666 Lua binding: fades the music manager's channels ("f|bs": time, a flag choosing channels 12
// or 2, an optional layer name) on every layer. The layer name is looked up but its mask is not used:
// the original passes the all-layers mask whatever the name. Names and layouts are placeholders.
#include "base/UnkLoadingMask.h"
#include "base/global/CRntVector.h"
#include "game/CBossAIComponent.h"
#include "game/UnkTU2677.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

struct lua_State;

s32 fn_7101097578(const char* name);

struct UnkMaskStaticZero_l091ae0 {
    u32 value;
    UnkMaskStaticZero_l091ae0() : value(0) {}
};

inline const UnkMaskStaticZero_l091ae0& MaskNone_l091ae0() {
    static UnkMaskStaticZero_l091ae0 s_value asm("g_unk_71726c4090");
    return s_value;
}

inline u32 MaskAll_l091ae0() {
    static u32 s_mask asm("g_unk_71726c4080") = ~MaskNone_l091ae0().value & 0x1ff;
    return s_mask;
}

s32 fn_7101091ae0(lua_State* L) {
    f32 time = -1.0f;
    bool flag = false;
    const char* name = nullptr;
    if (fn_71010c0270(L, "f|bs", &time, &flag, &name)) {
        // The named layer's mask is computed and then not used (faithful to the original).
        u32 layers = name ? 1u << fn_7101097578(name) : MaskAll_l091ae0();
        (void)layers;
        UnkLoadingMask channels(flag ? 12 : 2);
        void* manager = g_unk_71726bc560;
        if (!manager) {
            manager = fn_71000839a0(0xf18, 0x3f);
            fn_7100fd3804(manager);
            g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(manager);
        }
        f32 duration = time;
        fn_7100fd7b44(manager, channels, UnkLoadingMask(MaskAll_l091ae0()), duration);
    }
    return 0;
}
