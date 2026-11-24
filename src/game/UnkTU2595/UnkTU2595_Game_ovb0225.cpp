// Unit 2595 (tools/tu_map.py; no class name known), see src/game/UnkUnit2595/UnkUnit2595_b49.cpp: functions that
// act on the game manager through its lazily created singleton (fn_7100ec7aec, g_unk_71729dc210). Names and
// layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/CBossAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkSingleton2595_b0225.h"
#include "game/UnkTU2595.h"
#include "game/UnkTU2656.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"
#include "game/UnkMask.h"

struct UnkSingleton2595_b49a6;

static inline UnkSingleton2595_b0225* Singleton2595() {
    UnkSingleton2595_b0225*& slot = reinterpret_cast<UnkSingleton2595_b0225*&>(g_unk_71729dc210);
    if (!slot) {
        void* mem = fn_71000839a0(sizeof(UnkSingleton2595_b0225), 0x3f);
        slot = new (mem) UnkSingleton2595_b0225();
    }
    return slot;
}

struct UnkVec_710048a194;

struct UnkVec3_b0225 {
    u64 xy;
    u32 z;
};

// With the singleton's flag set, plays the effect at the origin and has the music manager change channel 4.
void fn_7100f1435c() {
    if (!Singleton2595()->field_0x59)
        return;
    void* effects = g_unk_7101d47310->field_0x1040;
    UnkVec3_b0225 origin;
    origin.xy = 0;
    origin.z = 0;
    fn_7101053850(effects, reinterpret_cast<UnkVec_710048a194*>(&origin), 1, 0.3f, 1.0f);
    void* instance = g_unk_71726bc560;
    if (!instance) {
        instance = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(instance);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(instance);
    }
    CTunableMusicManager_StopMusicOnAllSlots(static_cast<UnkMusicMgr_b0019*>(instance), UnkMask(4), UnkMaskOf<UnkMusicMaskTag>::All(), 0.3f);
}
