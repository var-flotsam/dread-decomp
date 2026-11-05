// Unit 36 of tools/tu_map.py: marks the worker object busy (+0x9, see UnkUnit36_Busy.cpp) and calls
// fn_710002c220 with the inverse of `flag`. Names are placeholders.
#include "types.h"

void fn_710002c220(bool value);

struct UnkUnit36Head_b65a1 {
    u8 pad_0x0[0x9];
    bool busy;
};

void fn_710002a03c(UnkUnit36Head_b65a1* self, bool flag) {
    self->busy = true;
    fn_710002c220(!flag);
}
