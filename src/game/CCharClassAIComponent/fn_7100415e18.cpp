// Builds a value of kind 2 holding the word at +0xc8 of the object at +0x8 of its argument
// (fn_71000a18d4 fills in the rest from the global descriptor).
#include "base/reflection/CEnumType.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkSource {
    u8 pad_0x0[0xc8];
    u32 field_0xc8;
};

struct UnkArgument {
    u8 pad_0x0[8];
    UnkSource* field_0x8;
};

UnkReflValue fn_7100415e18(void* unused, UnkArgument* argument) {
    u32 value = argument->field_0x8->field_0xc8;
    UnkReflValue result;
    result.field_0x0 = nullptr;
    result.type = nullptr;
    result.kind = 2;
    fn_71000a18d4(&result, &g_unk_71727952a8, &value, 0);
    return result;
}
