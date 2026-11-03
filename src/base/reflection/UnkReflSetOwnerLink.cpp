// A reflection property setter (base::reflection names; tools/tu_map.py): the pointer picked from the
// argument's holder (inline, or behind a pointer for 9 or more entries or kind 1) gets the object at 8
// stored in its field at +0x18. Returns an empty result of kind 2. Names and layouts are placeholders.
#include "types.h"

struct UnkValueSet_rso {
    u8 unk_0x0[0x18];
    u64 count;
};

// An argument: the value is inline unless it is kind 1 or its type is 9 bytes or more.
struct UnkValueSource_rso {
    void* holder;
    UnkValueSet_rso* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_rso {
    u64 empty[2];
    u64 kind_and_flag;
};

template <class T>
static inline __attribute__((always_inline)) T* PickArg_rso(UnkValueSource_rso* src) {
    if (src->kind == 1 || src->set->count >= 9)
        return static_cast<T*>(src->holder);
    return reinterpret_cast<T*>(src);
}

struct UnkReflLinked_rso {
    u8 unk_0x0[0x18];
    void* owner;
};

struct UnkReflOwner_rso {
    u8 unk_0x0[8];
    void* target;
};

UnkResult_0x18_rso fn_7100b803c0(void* unused, UnkReflOwner_rso* owner, UnkValueSource_rso** source) {
    void* target = owner->target;
    UnkValueSource_rso* src = *source;
    UnkReflLinked_rso* linked = *PickArg_rso<UnkReflLinked_rso*>(src);
    linked->owner = target;
    UnkResult_0x18_rso result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}
