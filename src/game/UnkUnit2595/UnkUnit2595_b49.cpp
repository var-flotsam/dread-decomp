// Unit 2595 (tools/tu_map.py; no class name known): a lazily created 0x60-byte singleton (constructor
// fn_7100ec7aec, pointer at g_unk_71729dc210). When its flag at 0x59 is set, starts an effect on the game
// manager's effect player (fn_7101053850) at the origin, then runs the singleton's step fn_7100ec9640.
// Names and layout are placeholders.
#include "base/UnkVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2595.h"
#include "game/UnkTU2656.h"
#include "types.h"

struct UnkSingleton2595_b49a6 {
    u8 pad_0x0[0x59];
    bool field_0x59;
    u8 pad_0x5a[0x6];

    UnkSingleton2595_b49a6();  // fn_7100ec7aec
};

// As src/game/CChozoWarriorXUltimateGrabAttack/fn_710048a194.cpp declares it.
struct UnkVec;
void fn_7100ec9640(UnkSingleton2595_b49a6* self);

struct UnkVec3_b49a6 {
    u64 xy;
    u32 z;
};

static inline UnkSingleton2595_b49a6* Singleton2595() {
    if (!g_unk_71729dc210) {
        void* mem = fn_71000839a0(sizeof(UnkSingleton2595_b49a6), 0x3f);
        g_unk_71729dc210 = new (mem) UnkSingleton2595_b49a6();
    }
    return g_unk_71729dc210;
}

// Starts the effect at the origin when the singleton's flag is set.
static inline bool PlayEffect2595(f32 scale) {
    if (!Singleton2595()->field_0x59)
        return false;
    void* effects = g_unk_7101d47310->field_0x1040;
    UnkVec3_b49a6 origin;
    origin.xy = 0;
    origin.z = 0;
    fn_7101053850(effects, reinterpret_cast<UnkVec*>(&origin), 1, 0.3f, scale);
    return true;
}

void fn_7100f14050() {
    if (PlayEffect2595(0.0f))
        fn_7100ec9640(Singleton2595());
}

struct UnkEffectUser2595_b49a6 {
    u8 pad_0x0[0x258];
    f32 field_0x258;
};

void fn_7100f148d0(UnkEffectUser2595_b49a6* self) {
    if (PlayEffect2595(1.0f))
        self->field_0x258 = 2.0f;
}
