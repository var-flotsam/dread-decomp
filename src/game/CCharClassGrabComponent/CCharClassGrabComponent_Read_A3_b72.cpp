// CCharClassGrabComponent unit (tools/tu_map.py, the name from reflection): fn_7100790cbc reads a record from
// a file (fn_710007c714): two 12-byte vectors, two words and two bools; each set bool sets bit 2 of the flags
// at +0x20 or +0x24. Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "types.h"

using base::global::CRntFile;

struct UnkGrabRecord_A3_b72 {
    u8 field_0x0[0xc];
    u8 field_0xc[0xc];
    u32 field_0x18;
    u32 field_0x1c;
    u32 flags_0x20;
    u32 flags_0x24;
};

void fn_7100790cbc(UnkGrabRecord_A3_b72* self, CRntFile* file) {
    bool first = false;
    bool second = false;
    fn_710007c714(file, self->field_0x0, 0xc, 1);
    fn_710007c714(file, self->field_0xc, 0xc, 1);
    fn_710007c714(file, &self->field_0x18, 4, 1);
    fn_710007c714(file, &self->field_0x1c, 4, 1);
    fn_710007c714(file, &first, 1, 1);
    fn_710007c714(file, &second, 1, 1);
    if (first)
        self->flags_0x20 |= 4;
    if (second)
        self->flags_0x24 |= 4;
}
