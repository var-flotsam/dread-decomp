// Unit 1159 of tools/tu_map.py (0x7100744c80-0x71007457cc, no reflected class): a setter that stores a
// duration twice, assigns two colours (+0x140 and +0x130), a flag at +0x150 and, when asked, raises the
// dirty byte at +0x24. Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "types.h"

struct UnkColorFade1159 {
    u8 pad_0x0[0x24];
    bool field_0x24;
    u8 pad_0x25[0x128 - 0x25];
    f32 field_0x128;
    f32 field_0x12c;
    base::color::CColor4F field_0x130;
    base::color::CColor4F field_0x140;
    bool field_0x150;
};

void fn_71007453a8(UnkColorFade1159* self, const base::color::CColor4F* from, const base::color::CColor4F* to,
                   bool mark_dirty, bool flag, f32 time) {
    self->field_0x12c = time;
    self->field_0x128 = time;
    fn_7100030728(&self->field_0x140, from);
    fn_7100030728(&self->field_0x130, to);
    self->field_0x150 = flag;
    if (mark_dirty)
        self->field_0x24 = true;
}
