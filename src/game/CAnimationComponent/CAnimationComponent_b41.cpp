// CAnimationComponent unit (tools/tu_map.py, the name from reflection): a reflected function that
// passes each element of the array of the object at +0x40 (data at +0xc8, count at +0xd8) to
// fn_71002d296c; returns an empty value. Names and layouts are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

void fn_71002d296c(void* self);

struct UnkAnimArray_b41a3 {
    u8 pad_0x0[0xc8];
    void** data;
    u8 pad_0xd0[8];
    u32 count;
};

UnkReflValue fn_710117c12c(void*, char* obj) {
    char* self = *reinterpret_cast<char**>(obj + 8);
    UnkAnimArray_b41a3* array = *reinterpret_cast<UnkAnimArray_b41a3**>(self + 0x40);
    if (array->count != 0) {
        void** it = array->data;
        do {
            fn_71002d296c(*it);
            ++it;
            array = *reinterpret_cast<UnkAnimArray_b41a3**>(self + 0x40);
        } while (it != array->data + array->count);
    }
    return base::reflection::refl_EmptyValue();
}
