// Calls a handler for every 4-byte entry of an array.
#include "types.h"

struct UnkEntry_0x4 {
    u16 field_0x0;
    u8 field_0x2;
    u8 field_0x3;
};

struct UnkArray_0x4 {
    UnkEntry_0x4* data;
    u32 count;
};

void fn_7100288320(void* self, u16 a, u8 b);

void fn_71002885e8(void* self, const UnkArray_0x4* entries) {
    for (const UnkEntry_0x4 *it = entries->data, *end = it + entries->count; it != end; ++it)
        fn_7100288320(self, it->field_0x0, it->field_0x2);
}
