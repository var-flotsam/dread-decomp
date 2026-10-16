// Finds the item with a given id and forwards it to a handler; false if absent.
#include "types.h"

struct UnkItem {
    s32 id;
};

struct UnkPtrArray {
    UnkItem** data;
    u32 count;
};

bool fn_7100202f00(UnkItem* item);

bool fn_71001fd8a0(const UnkPtrArray* items, s32 id) {
    UnkItem* found = nullptr;
    for (UnkItem **it = items->data, **end = it + items->count; it != end; ++it) {
        if ((*it)->id == id) {
            found = *it;
            break;
        }
    }
    if (!found)
        return false;
    return fn_7100202f00(found);
}
