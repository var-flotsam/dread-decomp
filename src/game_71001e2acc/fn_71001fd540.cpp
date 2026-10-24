// Find the item with a given id in an array of pointers and forward it with two flags and an
// argument to a handler, false without an item (as in fn_71001fd2ec, src/game_s1/fn_71001fcd4c.cpp):
// fn_71001fd540 (handler fn_7100202d9c), fn_71001fd584 (fn_7100202dd4).
#include "types.h"

struct UnkItem {
    s32 id;
};

struct UnkPtrArray {
    UnkItem** data;
    u32 count;
};

__attribute__((always_inline)) static inline UnkItem* Find(const UnkPtrArray* items, s32 id) {
    for (UnkItem **it = items->data, **end = it + items->count; it != end; ++it) {
        if ((*it)->id == id)
            return *it;
    }
    return nullptr;
}

bool fn_7100202d9c(UnkItem* item, bool a, void* arg, bool b);
bool fn_7100202dd4(UnkItem* item, bool a, void* arg, bool b);

bool fn_71001fd540(const UnkPtrArray* items, bool a, void* arg, s32 id, bool b) {
    UnkItem* found = Find(items, id);
    if (!found)
        return false;
    return fn_7100202d9c(found, a, arg, b);
}

bool fn_71001fd584(const UnkPtrArray* items, bool a, void* arg, s32 id, bool b) {
    UnkItem* found = Find(items, id);
    if (!found)
        return false;
    return fn_7100202dd4(found, a, arg, b);
}
