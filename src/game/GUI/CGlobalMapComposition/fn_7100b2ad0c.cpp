// Sets a menu's two string ids (0x780, 0x788), marks it changed (0x7a4) and sets its state at
// 0x588 to 4.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkIdMenu {
    u8 unk_0x0[0x588];
    s32 field_0x588;
    u8 unk_0x58c[0x1f4];
    CStrId field_0x780;
    CStrId field_0x788;
    u8 unk_0x790[0x14];
    bool field_0x7a4;
};

void fn_7100b2ad0c(UnkIdMenu* self, const CStrId& a, const CStrId& b) {
    self->field_0x780 = a;
    self->field_0x788 = b;
    self->field_0x7a4 = true;
    self->field_0x588 = 4;
}
