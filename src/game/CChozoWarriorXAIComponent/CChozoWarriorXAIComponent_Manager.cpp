// In the CChozoWarriorXAIComponent unit: a step that runs the base step fn_7100498e44 and then hands
// the lazily created manager g_unk_71726af468 (allocated with flags 0x3f) to fn_7100da928c, like
// fn_7100639274 in the CPoisonFlyAIComponent unit. Names are placeholders.
#include "base/UnkVector.h"
#include "game/UnkManager_71726af468.h"
#include "types.h"

void fn_7100498e44(char* self);
void fn_7100da928c(UnkManager_71726af468* manager);

void fn_710048e38c(char* self) {
    fn_7100498e44(self);
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    fn_7100da928c(manager);
}
