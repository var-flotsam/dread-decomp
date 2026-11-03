// CAnimationComponent unit (tools/tu_map.py, the name from reflection): passes each element of the
// array of the object at +0x40 (data at +0xc8, count at +0xd8, both re-read every step) to
// fn_71002d296c; the method behind the reflected fn_710117c12c (CAnimationComponent_b41.cpp). Names
// and layouts are placeholders.
#include "types.h"

void fn_71002d296c(void* self);

struct UnkAnimArray_b48a3 {
    u8 pad_0x0[0xc8];
    void** data;
    u8 pad_0xd0[8];
    u32 count;
};

struct UnkAnimArrayOwner_b48a3 {
    u8 pad_0x0[0x40];
    UnkAnimArray_b48a3* array;
};

void fn_7101176b98(UnkAnimArrayOwner_b48a3* self) {
    UnkAnimArray_b48a3* array = self->array;
    if (array->count != 0) {
        void** it = array->data;
        do {
            fn_71002d296c(*it);
            ++it;
            array = self->array;
        } while (it != array->data + array->count);
    }
}
