// Units 271 and 281 (tools/tu_map.py): handlers that pass the object held at a fixed offset, if
// any, to a function of their unit, reporting whether there was one.
#include "types.h"

void fn_71001ebb34(void* obj);
void fn_71001ef028(void* obj);

struct UnkHolder10 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkHolderA0 {
    u8 pad_0x0[0xa0];
    void* field_0xa0;
};

bool fn_71001f0364(UnkHolder10* self) {
    if (!self->field_0x10)
        return false;
    fn_71001ebb34(self->field_0x10);
    return true;
}

bool fn_7100202d24(UnkHolderA0* self) {
    if (!self->field_0xa0)
        return false;
    fn_71001ef028(self->field_0xa0);
    return true;
}
