// The GUI::CMapMenuController unit (tools/tu_map.py, unit 1689): after the base handler
// (fn_7100b43b30), sets the flag at +0x4b4 and, when the id is empty, runs fn_7100eb7358 on the
// object at +0x618. Names and layouts are placeholders.
#include "../../game_s7/UnkGuiS7.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkMapMenuController {
    u8 pad_0x0[0x4b4];
    bool field_0x4b4;
    u8 pad_0x4b5[0x163];
    u8 field_0x618[1];
};

void fn_7100b43b30(UnkMapMenuController* self, const CStrId* id, void* arg);

void fn_7100b3d16c(UnkMapMenuController* self, const CStrId* id, void* arg) {
    fn_7100b43b30(self, id, arg);
    self->field_0x4b4 = true;
    if (id->data == &g_unk_7101d47108->empty)
        unk_gui::fn_7100eb7358(self->field_0x618, 0, 1);
}
