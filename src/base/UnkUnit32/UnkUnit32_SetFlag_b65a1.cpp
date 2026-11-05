// Unit 32 of tools/tu_map.py (no reflected class): sets the flag at +0xcd and, when it changes to
// true, calls fn_7100025d10. Names are placeholders.
#include "types.h"

struct UnkUnit32Object_b65a1 {
    u8 pad_0x0[0xcd];
    bool flag;
};

void fn_7100025d10(UnkUnit32Object_b65a1* self);

void fn_7100025cf0(UnkUnit32Object_b65a1* self, bool flag) {
    if (self->flag != flag) {
        self->flag = flag;
        if (flag)
            fn_7100025d10(self);
    }
}
