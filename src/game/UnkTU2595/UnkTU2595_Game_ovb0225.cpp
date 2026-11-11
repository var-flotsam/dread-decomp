// Unit 2595 (tools/tu_map.py; no class name known), see src/game/UnkUnit2595/UnkUnit2595_b49.cpp: functions that
// act on the game manager through its lazily created singleton (fn_7100ec7aec, g_unk_71729dc210). Names and
// layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2595.h"
#include "game/UnkTU2656.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

struct UnkSingleton2595_b0225 {
    u8 pad_0x0[0x59];
    bool field_0x59;
    u8 pad_0x5a[0x6];

    UnkSingleton2595_b0225();  // fn_7100ec7aec
};


struct UnkSingleton2595_b49a6;

static inline UnkSingleton2595_b0225* Singleton2595() {
    UnkSingleton2595_b0225*& slot = reinterpret_cast<UnkSingleton2595_b0225*&>(g_unk_71729dc210);
    if (!slot) {
        void* mem = fn_71000839a0(sizeof(UnkSingleton2595_b0225), 0x3f);
        slot = new (mem) UnkSingleton2595_b0225();
    }
    return slot;
}

struct UnkVec;

// The channel mask as src/base/reflection/reflection_environment_ovb0056.cpp defines it.
struct UnkLoadingMask {
    u32 bits;
    UnkLoadingMask() : bits(0) {}
    UnkLoadingMask(u32 b) : bits(b) {}
    UnkLoadingMask(const UnkLoadingMask& o) : bits(o.bits) {}
};
inline const UnkLoadingMask& UnkLoadingMaskNone() {
    static UnkLoadingMask s asm("g_unk_71726c4090");
    return s;
}
inline const UnkLoadingMask& UnkLoadingMaskAll() {
    static UnkLoadingMask s asm("g_unk_71726c4080")(~UnkLoadingMaskNone().bits & 0x1ff);
    return s;
}
extern void* g_unk_71726bc560;

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
    fn_7101053850(effects, reinterpret_cast<UnkVec*>(&origin), 1, 0.3f, 1.0f);
    void* instance = g_unk_71726bc560;
    if (!instance) {
        instance = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(instance);
        g_unk_71726bc560 = instance;
    }
    fn_7100fd7b44(instance, UnkLoadingMask(4), UnkLoadingMaskAll(), 0.3f);
}
