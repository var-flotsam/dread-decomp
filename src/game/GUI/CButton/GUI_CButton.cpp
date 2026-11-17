// GUI::CButton (unit 2745 of tools/tu_map.py, the name from reflection): small checks on the
// button's string ids at +0x230 and +0x238. While an id is set, they defer to fn_71011bf744 with
// the id's index (0 or 1); otherwise they do nothing. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/GUI/CButton.h"
#include "types.h"

struct UnkGuiButtonIds {
    u8 unk_0x0[0x230];
    base::global::UnkStringData* ids[2];  // CStrIds' data
};

void fn_71011bf71c(UnkGuiButtonIds* self) {
    if (self->ids[0] != &g_unk_7101d47108->empty)
        fn_71011bf744(self, 0);
}

void fn_71011bfb60(UnkGuiButtonIds* self) {
    if (self->ids[1] != &g_unk_7101d47108->empty)
        fn_71011bf744(self, 1);
}

void fn_71011bfc30(UnkGuiButtonIds* self) {
    if (self->ids[0] != &g_unk_7101d47108->empty)
        fn_71011bf744(self, 0);
}

void fn_71011bfc58(UnkGuiButtonIds* self) {
    if (self->ids[1] != &g_unk_7101d47108->empty)
        fn_71011bf744(self, 1);
}
