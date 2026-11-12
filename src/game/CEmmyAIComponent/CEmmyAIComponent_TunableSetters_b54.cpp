// CEmmyAIComponent unit (tools/tu_map.py, the name from reflection): four reflected setters of a 32-bit
// word. The word picked from the argument's holder (inline, or behind a pointer for 9 or more entries or
// kind 1) goes to the tunables the provider at +0x29ee0 returns from vf_0xd8, at TunableOffset, and then
// to the component itself at SelfOffset. Each returns an empty result of kind 2. Names and layouts are
// placeholders.
#include "types.h"


struct UnkValueSet_b54 {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_b54 {
    void* holder;
    UnkValueSet_b54* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkProvider_b54 {
    void** vtable;
    char* vf_0xd8() { return reinterpret_cast<char* (*)(UnkProvider_b54*)>(vtable[27])(this); }
};

struct UnkOwner_b54 {
    u8 unk_0x0[8];
    char* self;
};


struct UnkResult_0x18_b54 {
    u64 empty[2];
    u64 kind_and_flag;
};

template <u32 SelfOffset, u32 TunableOffset>
static inline __attribute__((always_inline)) UnkResult_0x18_b54 SetWord_b54(UnkOwner_b54* owner,
                                                                            UnkValueSource_b54** source) {
    char* self = owner->self;
    UnkProvider_b54** provider = reinterpret_cast<UnkProvider_b54**>(self + 0x29ee0);
    u32* own = reinterpret_cast<u32*>(self + SelfOffset);
    UnkValueSource_b54* src = *source;
    const u32* v = reinterpret_cast<const u32*>(src);
    if (src->kind == 1 || src->set->count >= 9)
        v = static_cast<const u32*>(src->holder);
    u32 value = *v;
    char* tunables = (*provider)->vf_0xd8();
    *reinterpret_cast<u32*>(tunables + TunableOffset) = value;
    *own = value;
    UnkResult_0x18_b54 result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}

UnkResult_0x18_b54 fn_710056534c(void*, UnkOwner_b54* owner, UnkValueSource_b54** source) {
    return SetWord_b54<0x14c08, 0x3c>(owner, source);
}
UnkResult_0x18_b54 fn_7100565630(void*, UnkOwner_b54* owner, UnkValueSource_b54** source) {
    return SetWord_b54<0x14c10, 0xac>(owner, source);
}
UnkResult_0x18_b54 fn_7100565714(void*, UnkOwner_b54* owner, UnkValueSource_b54** source) {
    return SetWord_b54<0x14c2c, 0x144>(owner, source);
}
UnkResult_0x18_b54 fn_71005657f8(void*, UnkOwner_b54* owner, UnkValueSource_b54** source) {
    return SetWord_b54<0x14c30, 0x148>(owner, source);
}
