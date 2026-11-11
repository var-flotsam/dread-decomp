// In the CPoisonFlyAIComponent unit, two users of the part at +0x80 of the manager
// g_unk_71726af468 (created on first use, allocated with flags 0x3f or 3): fn_7100639bc8, a
// reflected function with no result, runs fn_710063cab8 on it and returns an empty value (kind 2,
// by reference); fn_7100639274 runs the base step fn_71010ff2b8 and then fn_710063c670 on it.
// Names are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU958.h"
#include "types.h"

void fn_710063cab8(u8* part);

template <u32 Flags>
static inline __attribute__((always_inline)) UnkManager_71726af468* GetManager() {
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, Flags));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    return manager;
}

base::reflection::UnkReflValue fn_7100639bc8() {
    fn_710063cab8(GetManager<0x3f>()->pad_0x0 + 0x80);
    base::reflection::UnkReflValue result;
    result.field_0x0 = nullptr;
    result.type = nullptr;
    result.kind = 2;
    result.field_0x14 = 1;
    return result;
}

void fn_7100639274(char* self) {
    fn_71010ff2b8(self);
    fn_710063c670(GetManager<3>()->pad_0x0 + 0x80);
}
