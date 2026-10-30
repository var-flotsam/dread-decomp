// Two copies of fn_71002d9bdc (src/game/animtree/CAnimTreeDef/fn_71002d9bdc.cpp) for the 0x10-byte
// CFXPool::CDefinition behind a collection iterator value, as a value of kind 2: the value is stored
// inline at 8, or through the pointer there when the state after it is 2. fn_710104f390, fn_710104f5a0.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

extern const CType g_unk_71729fdbd0 __attribute__((visibility("hidden")));  // CFXPool::CDefinition's type

struct UnkFxDefIterValue {
    u64 unk_0x0;
    union {
        u8 value[0x10];
        void* ref;
    };
    s32 state;

    __attribute__((always_inline)) void* Get() { return state == 2 ? ref : value; }
};

struct UnkFxDefArgument {
    u8 unk_0x0[8];
    UnkFxDefIterValue* field_0x8;
};

__attribute__((always_inline)) static inline UnkReflValue GetDefinition(UnkFxDefArgument* argument) {
    void* def = argument->field_0x8->Get();
    fn_710104d52c();
    UnkReflValue result;
    result.field_0x0 = nullptr;
    result.type = nullptr;
    result.kind = 2;
    fn_71000a18d4(&result, &g_unk_71729fdbd0, def, 1);
    return result;
}

UnkReflValue fn_710104f390(void* unused, UnkFxDefArgument* argument) { return GetDefinition(argument); }

UnkReflValue fn_710104f5a0(void* unused, UnkFxDefArgument* argument) { return GetDefinition(argument); }
