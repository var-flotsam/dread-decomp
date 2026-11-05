// base::reflection unit (tools/tu_map.py): clears the pointer at +0x88 of every element of a (data, count) list
// of pointers. Names and layouts are placeholders.
#include "types.h"

struct UnkElem88_A6 {
    char pad[0x88];
    struct { u32 a, b; } field_0x88;
};
struct UnkPtrList_A6 {
    char pad[8];
    UnkElem88_A6** data;
    u32 count;
};

void fn_7100b774b8(UnkPtrList_A6* self) {
    if (self->count == 0) return;
    for (UnkElem88_A6** it = self->data; it != self->data + self->count; ++it) __builtin_memset(&(*it)->field_0x88, 0, 8);
}
