// Getters building a value of kind 2 that holds a word of the object at +0x8 of their argument
// (fn_71000a18d4 fills in the rest from the global descriptor of the word's type).
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkArgument {
    u8 pad_0x0[8];
    u8* field_0x8;
};

template <u32 Offset, const CType* Descriptor>
__attribute__((always_inline)) static inline UnkReflValue GetWord(UnkArgument* argument) {
    u32 value = *reinterpret_cast<u32*>(argument->field_0x8 + Offset);
    UnkReflValue result;
    result.field_0x0 = nullptr;
    result.type = nullptr;
    result.kind = 2;
    fn_71000a18d4(&result, Descriptor, &value, 0);
    return result;
}

extern const CType g_unk_71727ba2e0;
extern const CType g_unk_71727ba480;
extern const CType g_unk_71727ca958;
extern const CType g_unk_71727caab8;

UnkReflValue fn_7100d9c14c(void* unused, UnkArgument* argument) { return GetWord<0x0, &g_unk_71727ba2e0>(argument); }
UnkReflValue fn_7100d9c1e0(void* unused, UnkArgument* argument) { return GetWord<0x4, &g_unk_71727ba480>(argument); }
UnkReflValue fn_7100eaa018(void* unused, UnkArgument* argument) { return GetWord<0x98, &g_unk_71727ca958>(argument); }
UnkReflValue fn_7100eaca98(void* unused, UnkArgument* argument) { return GetWord<0x0, &g_unk_71727caab8>(argument); }
