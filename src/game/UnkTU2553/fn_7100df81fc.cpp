#include "base/global/CFilePathStrId.h"
#include "game/UnkTU0.h"
#include "types.h"

void fn_7100df81fc(u32 code, u32 value) {
    switch (code) {
    case 2:
        fn_7100000790("msemenu_start.OnSaveDataInitialized", g_unk_710162e2ed);
        break;
    default:
        fn_7100000790("SaveGame.InvalidDeviceSelected", "i", value);
        break;
    case 3:
        fn_7100000790("SaveGame.OnOverridePermissionNeeded", g_unk_710162e2ed);
        break;
    case 4:
        fn_7100000790("SaveGame.OnNoDeviceSelected", g_unk_710162e2ed);
        break;
    case 5:
        fn_7100000790("SaveGame.NotEnoughSpaceDeviceSelected", "i", value);
        break;
    }
}
