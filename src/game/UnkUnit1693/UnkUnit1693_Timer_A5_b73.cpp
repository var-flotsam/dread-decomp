// Unit 1693 (tools/tu_map.py, no name hint; the screen of UnkUnit1693_CompanyName_b49.cpp): the update. Once
// the screen has been shown for more than 4 seconds it removes its key from the GUI root's key list (+0xca8)
// and tells the menu to move on (fn_7100b35704 with 1, true). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/GUI/CMainMenu.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU1693.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkUnit1693Timer_A5_b73 {
    u8 pad_0x0[0x10];
    f32 time;
};

void fn_7100b4d368(UnkUnit1693Timer_A5_b73* self, UnkReflObject* menu, f32 dt) {
    self->time += dt;
    if (self->time > 4.0f) {
        fn_710002ffb4(&g_unk_717271e8f8->field_0xca8, &g_unk_71727645a0);
        fn_7100b35704(menu, 1, true);
    }
}
