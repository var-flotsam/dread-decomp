// Equality of two reflected values holding an id and a list of 64-bit ids, each member inline or
// (mode 2) through a pointer. Used as a type descriptor's compare (fn_71004a84c8, in
// class_descriptors_00.cpp, matched by hand stream 2); these are its two identical copies for
// other types. Names are placeholders.
#include "game/UnkIdVec.h"
#include "types.h"

// Each member is held inline or, in mode 2, through a pointer.
struct UnkHolder_5d9dec {
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
    const UnkHolder_5d9dec* a = static_cast<const UnkHolder_5d9dec*>(a_);
    const UnkHolder_5d9dec* b = static_cast<const UnkHolder_5d9dec*>(b_);
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

bool fn_71005d9dec(const void* a, const void* b) {
    return Equal(a, b);
}

bool fn_71005ee098(const void* a, const void* b) {
    return Equal(a, b);
}
