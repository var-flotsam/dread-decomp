// Unit 2628 (tools/tu_map.py): asks the music manager (created on first use) to do something once the object
// at +0x2f8 is in state 6. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CBossAIComponent.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

struct UnkMgr_0xf18;

struct UnkMusicState_ovb0219 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkMusicView_ovb0219 {
    u8 pad_0x0[0x2f8];
    UnkMusicState_ovb0219* active;  // 0x2f8
};

static inline void* Instance() {
    void* instance = g_unk_71726bc560;
    if (!instance) {
        instance = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(instance);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(instance);
    }
    return instance;
}

void fn_7100fcebec() {
    UnkMusicView_ovb0219* manager = static_cast<UnkMusicView_ovb0219*>(Instance());
    if (manager->active && manager->active->state == 6)
        fn_7100fdb9e0(static_cast<UnkMgr_0xf18*>(Instance()), 1, 0, -1.0f, 0.5f);
}
