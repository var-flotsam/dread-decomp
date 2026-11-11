// Shaped as fn_7100633198 (src/game/CPoisonFlyDiveAttack/CPoisonFlyDiveAttack_2.cpp), in the
// CCharClassPlayCurrentEnvironmentMusicEvent and CCharClassPlayCurrentEnvironmentSoundEvent units:
// create the lazily built manager g_unk_7101d47368 (0x4c8 bytes with flags 0x3f, constructed by fn_7100d8ced8) when
// there is none yet, then call one of its methods and return what it returns. Names are
// placeholders.
#include "base/UnkVector.h"
#include "base/reflection.h"
#include "types.h"

extern void* g_unk_7101d47368;

void* fn_7100d8dff0(void* manager);

__attribute__((always_inline)) static inline void* GetEnvironmentManager() {
    void* manager = g_unk_7101d47368;
    if (!manager) {
        manager = fn_71000839a0(0x4c8, 0x3f);
        fn_7100d8ced8(manager);
        g_unk_7101d47368 = manager;
    }
    return manager;
}

void* fn_7100cc959c() {
    return fn_7100d8e0a0(GetEnvironmentManager());
}

void* fn_7100cc9b2c() {
    return fn_7100d8dff0(GetEnvironmentManager());
}
