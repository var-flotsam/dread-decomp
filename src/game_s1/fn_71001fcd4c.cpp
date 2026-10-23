// Copies of fn_71001fd8a0 (src/flag_id/fn_71001fd8a0.cpp) for other handlers: each finds the item
// with a given id in an array of pointers and forwards it, with the other arguments, to a handler;
// false (or null, nothing, 1.0) if it's absent. The handlers' argument types are placeholders.
// fn_71001fd6c0 and fn_71001fd680 take arguments they don't pass on.
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

bool fn_7100202fcc(UnkItem* item, void* arg1, void* arg2);
bool fn_7100202e18(UnkItem* item, void* arg);
bool fn_7100202e08(UnkItem* item, void* arg);
bool fn_71002026cc(UnkItem* item, void* arg);
void* fn_7100202574(UnkItem* item);
bool fn_7100200144(UnkItem* item);
void fn_7100202ec4(UnkItem* item, bool value);
void fn_7100202d5c(UnkItem* item, bool value);
void fn_7100202d44(UnkItem* item, bool value);
void fn_710020014c(UnkItem* item, bool value);
void fn_7100200138(UnkItem* item, bool value);
void fn_7100202fa0(UnkItem* item, u64 value);
f32 fn_7100202f54(UnkItem* item);

bool fn_71001fda90(const UnkPtrArray* items, void* arg1, void* arg2, s32 id) {
    UnkItem* found = Find(items, id);
    if (!found)
        return false;
    return fn_7100202fcc(found, arg1, arg2);
}

template <bool (*Handler)(UnkItem*, void*)>
__attribute__((always_inline)) static inline bool Forward(const UnkPtrArray* items, void* arg, s32 id) {
    UnkItem* found = Find(items, id);
    if (!found)
        return false;
    return Handler(found, arg);
}

bool fn_71001fd604(const UnkPtrArray* items, void* arg, s32 id) { return Forward<fn_7100202e18>(items, arg, id); }

bool fn_71001fd5c8(const UnkPtrArray* items, void* arg, s32 id) { return Forward<fn_7100202e08>(items, arg, id); }

bool fn_71001fce78(const UnkPtrArray* items, void* arg, s32 id) { return Forward<fn_71002026cc>(items, arg, id); }

void* fn_71001fcdc4(const UnkPtrArray* items, s32 id) {
    UnkItem* found = Find(items, id);
    if (!found)
        return nullptr;
    return fn_7100202574(found);
}

bool fn_71001fc8c8(const UnkPtrArray* items, s32 id) {
    UnkItem* found = Find(items, id);
    if (!found)
        return false;
    return fn_7100200144(found);
}

// Handlers without a result.
template <void (*Handler)(UnkItem*, bool)>
__attribute__((always_inline)) static inline void ForwardFlag(const UnkPtrArray* items, bool value, s32 id) {
    if (UnkItem* found = Find(items, id))
        Handler(found, value);
}

void fn_71001fd82c(const UnkPtrArray* items, bool value, s32 id) { ForwardFlag<fn_7100202ec4>(items, value, id); }

void fn_71001fd504(const UnkPtrArray* items, bool value, s32 id) { ForwardFlag<fn_7100202d5c>(items, value, id); }

void fn_71001fd4c8(const UnkPtrArray* items, bool value, s32 id) { ForwardFlag<fn_7100202d44>(items, value, id); }

void fn_71001fc904(const UnkPtrArray* items, bool value, s32 id) { ForwardFlag<fn_710020014c>(items, value, id); }

void fn_71001fc88c(const UnkPtrArray* items, bool value, s32 id) { ForwardFlag<fn_7100200138>(items, value, id); }

void fn_71001fda54(const UnkPtrArray* items, const u64* value, s32 id) {
    if (UnkItem* found = Find(items, id))
        fn_7100202fa0(found, *value);
}

f32 fn_71001fd94c(const UnkPtrArray* items, s32 id) {
    UnkItem* found = Find(items, id);
    if (!found)
        return 1.0f;
    return fn_7100202f54(found);
}

// With the handler written inline: sets the two flags at 0x143 and 0x144 of the item
// (fn_71001fc7f4).
bool fn_71001fc7f4(const UnkPtrArray* items, bool flag_0x143, bool flag_0x144, s32 id) {
    UnkItem* found = Find(items, id);
    if (!found)
        return false;
    reinterpret_cast<bool*>(found)[0x143] = flag_0x143;
    reinterpret_cast<bool*>(found)[0x144] = flag_0x144;
    return true;
}
