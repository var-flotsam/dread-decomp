// Bounds-checked lookup of a 0x50-byte element by index.
#include "types.h"

struct UnkStruct_0x50 {
    u8 unk_0x0[0x50];
};

struct UnkArray_0x50 {
    UnkStruct_0x50* data;
    u32 count;
};

UnkStruct_0x50* fn_7100273218(UnkArray_0x50* array, const u32* index) {
    if (*index >= array->count)
        return nullptr;
    return &array->data[*index];
}
