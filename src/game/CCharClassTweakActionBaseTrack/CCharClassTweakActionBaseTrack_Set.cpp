// CCharClassTweakActionBaseTrack unit (tools/tu_map.py, the name from reflection): a property setter.
// The byte picked from the argument's holder (inline, or behind a pointer for 9 or more entries or
// kind 1) is stored doubled as a word at +0x24 of the object at 8. Returns an empty result of kind 2.
// Names and layouts are placeholders.
#include "types.h"

struct UnkValueSet_tab {
    u8 unk_0x0[0x18];
    u64 count;
};

// An argument: the value is inline unless it is kind 1 or its type is 9 bytes or more.
struct UnkValueSource_tab {
    void* holder;
    UnkValueSet_tab* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_tab {
    u64 empty[2];
    u64 kind_and_flag;
};

template <class T>
static inline __attribute__((always_inline)) T* PickArg_tab(UnkValueSource_tab* src) {
    if (src->kind == 1 || src->set->count >= 9)
        return static_cast<T*>(src->holder);
    return reinterpret_cast<T*>(src);
}

struct UnkTweakActionTrack_tab {
    u8 unk_0x0[0x24];
    u32 field_0x24;
};

struct UnkTweakActionTrackOwner_tab {
    u8 unk_0x0[8];
    UnkTweakActionTrack_tab* target;
};

UnkResult_0x18_tab fn_7100d2c8d4(void* unused, UnkTweakActionTrackOwner_tab* owner, UnkValueSource_tab** source) {
    UnkValueSource_tab* src = *source;
    UnkTweakActionTrack_tab* target = owner->target;
    u8 value = *PickArg_tab<u8>(src);
    target->field_0x24 = value << 1;
    UnkResult_0x18_tab result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}
