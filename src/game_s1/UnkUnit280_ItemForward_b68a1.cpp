// Unit 280 of tools/tu_map.py, beside src/game_s1/fn_71001fcd4c.cpp: fn_71001fd7b4 finds the item with
// a given id in an array of pointers and, when fn_7100202574 gives something for it, passes the other
// two arguments on to fn_71002028c4; always false. Names and argument types are placeholders.
#include "base/reflection.h"
#include "types.h"

// As src/game_s1/fn_71001fcd4c.cpp lays them out.
struct UnkItem {
    s32 id;
};

struct UnkPtrArray {
    UnkItem** data;
    u32 count;
};

__attribute__((always_inline)) static inline UnkItem* Find_b68a1(const UnkPtrArray* items, s32 id) {
    for (UnkItem **it = items->data, **end = it + items->count; it != end; ++it) {
        if ((*it)->id == id)
            return *it;
    }
    return nullptr;
}

void fn_71002028c4(UnkItem* item, void* arg, u32 value);

bool fn_71001fd7b4(const UnkPtrArray* items, void* arg, s32 id, u32 value) {
    UnkItem* found = Find_b68a1(items, id);
    if (found && fn_7100202574(found))
        fn_71002028c4(found, arg, value);
    return false;
}
