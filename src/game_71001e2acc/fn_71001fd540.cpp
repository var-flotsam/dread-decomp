// Find the item with a given id in an array of pointers and forward it with two flags and an
// argument to a handler, false without an item (as in fn_71001fd2ec, src/game_s1/fn_71001fcd4c.cpp):
// fn_71001fd540 (handler fn_7100202d9c), fn_71001fd584 (fn_7100202dd4).
#include "types.h"

struct UnkItem_71001fd540 {
    s32 id;
};

struct UnkPtrArray_71001fd540 {
    UnkItem_71001fd540** data;
    u32 count;
};

__attribute__((always_inline)) static inline UnkItem_71001fd540* Find(const UnkPtrArray_71001fd540* items, s32 id) {
    for (UnkItem_71001fd540 **it = items->data, **end = it + items->count; it != end; ++it) {
        if ((*it)->id == id)
            return *it;
    }
    return nullptr;
}

bool fn_7100202d9c(UnkItem_71001fd540* item, bool a, void* arg, bool b);
bool fn_7100202dd4(UnkItem_71001fd540* item, bool a, void* arg, bool b);

bool fn_71001fd540(const UnkPtrArray_71001fd540* items, bool a, void* arg, s32 id, bool b) {
    UnkItem_71001fd540* found = Find(items, id);
    if (!found)
        return false;
    return fn_7100202d9c(found, a, arg, b);
}

bool fn_71001fd584(const UnkPtrArray_71001fd540* items, bool a, void* arg, s32 id, bool b) {
    UnkItem_71001fd540* found = Find(items, id);
    if (!found)
        return false;
    return fn_7100202dd4(found, a, arg, b);
}
