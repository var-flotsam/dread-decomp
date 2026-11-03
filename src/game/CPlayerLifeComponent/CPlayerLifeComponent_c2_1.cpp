// Reads or writes one of an object's named settings. Unknown keys go to the base handler
// (fn_71011608b4). Layouts are placeholders.
#include "../../game_s7/UnkPropS7.h"

using namespace unk_prop;

struct UnkObj_7101165a40 {
    u8 unk_0x0[0x314];
    s32 field_0x314;
};

u32 fn_71011608b4(UnkObj_7101165a40* self, UnkProp* p);

u32 fn_7101165a40(UnkObj_7101165a40* self, UnkProp* p) {
    if (!p->set && Is(p, Key(0xcf8)))
        return fn_710007cb4c(p->stream, "i", self->field_0x314) != 0;
    return fn_71011608b4(self, p);
}
