// Copies of fn_71005d9dec (src/game_s7/fn_71005d9dec.cpp), which the binary
// repeats at other addresses: fn_71004a8988, fn_71004be4e0, fn_71004be9a0, fn_71005da2ac, fn_71005e306c, fn_71005e352c, fn_71005ee558.
// Equality of two reflected values holding an id and a list of 64-bit ids, each member inline or
// (mode 2) through a pointer. Used as a type descriptor's compare (fn_71004a84c8, in
// class_descriptors_00.cpp, matched by hand stream 2); these are its two identical copies for
// other types. Names are placeholders.
#include "types.h"

struct UnkIdVec {
    u64* data;
    u32 count;
};

// Each member is held inline or, in mode 2, through a pointer.
struct UnkHolder {
    union {
        u32 value;
        const u32* ptr;
    } id;
    union {
        UnkIdVec value;
        const UnkIdVec* ptr;
    } list;
    u8 pad[0x30 - 0x18];
    u32 mode;
    const u32& Id() const { return mode == 2 ? *id.ptr : id.value; }
    const UnkIdVec& List() const { return mode == 2 ? *list.ptr : list.value; }
};

static inline bool Equal(const void* a_, const void* b_) {
    const UnkHolder* a = static_cast<const UnkHolder*>(a_);
    const UnkHolder* b = static_cast<const UnkHolder*>(b_);
    if (a->Id() != b->Id())
        return false;
    const UnkIdVec& x = a->List();
    const UnkIdVec& y = b->List();
    if (x.count != y.count)
        return false;
    for (u64 i = 0; i < x.count; i++)
        if (y.data[i] != x.data[i])
            return false;
    return true;
}

bool fn_71004a8988(const void* a, const void* b) {
    return Equal(a, b);
}

bool fn_71004be4e0(const void* a, const void* b) {
    return Equal(a, b);
}

bool fn_71004be9a0(const void* a, const void* b) {
    return Equal(a, b);
}

bool fn_71005da2ac(const void* a, const void* b) {
    return Equal(a, b);
}

bool fn_71005e306c(const void* a, const void* b) {
    return Equal(a, b);
}

bool fn_71005e352c(const void* a, const void* b) {
    return Equal(a, b);
}

bool fn_71005ee558(const void* a, const void* b) {
    return Equal(a, b);
}

