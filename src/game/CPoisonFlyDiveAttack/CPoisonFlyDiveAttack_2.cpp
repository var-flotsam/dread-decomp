// In the CPoisonFlyDiveAttack unit: creates the lazily built manager (g_unk_71726af468, 0x2610
// bytes, as fn_71003f8210 does) when there is none yet, then hands its member at +0x80 to
// fn_710063c670 and returns what that returns. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU958.h"

void fn_7100633198() {
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 3));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    fn_710063c670(manager->pad_0x0 + 0x80);
}
