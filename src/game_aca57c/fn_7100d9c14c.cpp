// Getters building a value of kind 2 that holds a word of the object at +0x8 of their argument
// (fn_71000a18d4 fills in the rest from the global descriptor of the word's type).
#include "types.h"

struct UnkValue {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
};

struct UnkArgument {
    u8 pad_0x0[8];
    u8* field_0x8;
};

void fn_71000a18d4(UnkValue* out, void* descriptor, const u32* value, u32 flag);

template <u32 Offset, u8* Descriptor>
__attribute__((always_inline)) static inline UnkValue GetWord(UnkArgument* argument) {
    u32 value = *reinterpret_cast<u32*>(argument->field_0x8 + Offset);
    UnkValue result;
    result.field_0x0 = 0;
    result.field_0x8 = 0;
    result.field_0x10 = 2;
    fn_71000a18d4(&result, Descriptor, &value, 0);
    return result;
}

extern u8 g_unk_71727ba2e0[];
extern u8 g_unk_71727ba480[];
extern u8 g_unk_71727ca958[];
extern u8 g_unk_71727caab8[];

UnkValue fn_7100d9c14c(void* unused, UnkArgument* argument) { return GetWord<0x0, g_unk_71727ba2e0>(argument); }
UnkValue fn_7100d9c1e0(void* unused, UnkArgument* argument) { return GetWord<0x4, g_unk_71727ba480>(argument); }
UnkValue fn_7100eaa018(void* unused, UnkArgument* argument) { return GetWord<0x98, g_unk_71727ca958>(argument); }
UnkValue fn_7100eaca98(void* unused, UnkArgument* argument) { return GetWord<0x0, g_unk_71727caab8>(argument); }
