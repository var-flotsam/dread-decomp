// Unit 2595: toggles the flag at +0x50 and returns its new value. Names are placeholders.
#include "types.h"

struct UnkUnit2595_b65a6 {
    u8 pad_0x0[0x50];
    bool flag;
};

bool fn_7100f13e2c(UnkUnit2595_b65a6* self) {
    bool was = self->flag;
    self->flag = !was;
    return !was;
}
