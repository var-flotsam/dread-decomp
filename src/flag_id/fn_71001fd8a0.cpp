// Finds the item with a given id and forwards it to a handler; false if absent.
#include "types.h"

struct UnkItem_71001fd8a0 {
    s32 id;
};

struct UnkPtrArray_71001fd8a0 {
    UnkItem_71001fd8a0** data;
    u32 count;
};

bool fn_7100202f00(UnkItem_71001fd8a0* item);

bool fn_71001fd8a0(const UnkPtrArray_71001fd8a0* items, s32 id) {
    UnkItem_71001fd8a0* found = nullptr;
    for (UnkItem_71001fd8a0 **it = items->data, **end = it + items->count; it != end; ++it) {
        if ((*it)->id == id) {
            found = *it;
            break;
        }
    }
    if (!found)
        return false;
    return fn_7100202f00(found);
}
