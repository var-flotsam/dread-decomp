// CEmmyAIComponent unit (tools/tu_map.py, the name from reflection): a reflected setter of an 8-bit
// flag. The byte picked from the argument's holder (inline, or behind a pointer for 9 or more entries or
// kind 1) goes to the tunables the provider at +0x29ee0 returns from vf_0xd8, at TunableOffset, and then
// to the component itself at SelfOffset. It returns an empty result of kind 2. Names and layouts are
// placeholders.
#include "types.h"


struct UnkValueSet_b55 {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_b55 {
    void* holder;
    UnkValueSet_b55* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkProvider_b55 {
    void** vtable;
    char* vf_0xd8() { return reinterpret_cast<char* (*)(UnkProvider_b55*)>(vtable[27])(this); }
};

struct UnkOwner_b55 {
    u8 unk_0x0[8];
    char* self;
};


struct UnkResult_0x18_b55 {
    u64 empty[2];
    u64 kind_and_flag;
};

template <u32 SelfOffset, u32 TunableOffset>
static inline __attribute__((always_inline)) UnkResult_0x18_b55 SetByte_b55(UnkOwner_b55* owner,
                                                                            UnkValueSource_b55** source) {
    char* self = owner->self;
    UnkProvider_b55** provider = reinterpret_cast<UnkProvider_b55**>(self + 0x29ee0);
    u8* own = reinterpret_cast<u8*>(self + SelfOffset);
    asm("" : "+r"(source) : "r"(provider), "r"(own));
    UnkValueSource_b55* src = *source;
    const u8* v = reinterpret_cast<const u8*>(src);
    if (src->kind == 1 || src->set->count >= 9)
        v = static_cast<const u8*>(src->holder);
    u8 value = *v;
    char* tunables = (*provider)->vf_0xd8();
    *reinterpret_cast<u8*>(tunables + TunableOffset) = value;
    *own = value;
    UnkResult_0x18_b55 result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}

UnkResult_0x18_b55 fn_7100565168(void*, UnkOwner_b55* owner, UnkValueSource_b55** source) {
    return SetByte_b55<0x14779, 0x8>(owner, source);
}
