// Copies of fn_71001fd8a0 (src/flag_id/fn_71001fd8a0.cpp) for other handlers: each finds the item
// with a given id in an array of pointers and forwards it, with the other arguments, to a handler;
// false (or null) if it's absent. The handlers' argument types are placeholders. fn_71001fd6c0
// and fn_71001fd680 take arguments they don't pass on.
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

bool fn_7100202eb0(UnkItem* item, bool value);
bool fn_7100202e7c(UnkItem* item, bool value, void* arg2, void* arg3);
bool fn_7100202e48(UnkItem* item, void* arg1, bool value);
void* fn_710020289c(UnkItem* item, u32 index);
void* fn_71002004f4(UnkItem* item, void* arg1, void* arg2, void* arg3, bool value);
bool fn_7100202d24(UnkItem* item, void* arg);

bool fn_71001fd6c0(const UnkPtrArray* items, bool value, void* unused2, void* unused3, void* unused4, s32 id) {
    UnkItem* found = Find(items, id);
    if (!found)
        return false;
    return fn_7100202eb0(found, value);
}

bool fn_71001fd680(const UnkPtrArray* items, bool value, void* arg2, void* arg3, s32 id) {
    UnkItem* found = Find(items, id);
    if (!found)
        return false;
    return fn_7100202e7c(found, value, arg2, arg3);
}

bool fn_71001fd640(const UnkPtrArray* items, void* arg1, bool value, s32 id) {
    UnkItem* found = Find(items, id);
    if (!found)
        return false;
    return fn_7100202e48(found, arg1, value);
}

void* fn_71001fce38(const UnkPtrArray* items, s32 id, u32 index) {
    UnkItem* found = Find(items, id);
    if (!found)
        return nullptr;
    return fn_710020289c(found, index);
}

void* fn_71001fcd4c(const UnkPtrArray* items, void* arg1, void* arg2, void* arg3, s32 id, bool value) {
    UnkItem* found = Find(items, id);
    if (!found)
        return nullptr;
    return fn_71002004f4(found, arg1, arg2, arg3, value);
}

// Also false without an argument.
bool fn_71001fd2ec(const UnkPtrArray* items, s32 id, void* arg) {
    UnkItem* found = Find(items, id);
    if (!arg || !found)
        return false;
    return fn_7100202d24(found, arg);
}
