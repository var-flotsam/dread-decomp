// Copies of fn_710000dc80 (src/game_s7/fn_710000dc80.cpp) for another class: getters building a
// value of kind 2 that refers (flag 1) to a navmesh::CDynamicSmartLinkGroup::SDynamicSmartLinkRulesSet
// held by an iterator value, in place at 0x8 or through the pointer there when the state (after the
// 0x10-byte value) is 2. The class getter is called first: fn_71002678c8, fn_7100267b5c. Also, as
// fn_7100d2c880 (src/game_aca57c/fn_7100ad4b54.cpp), a bool value: whether the word at 0x40 of the
// argument's object is above 1 (fn_710011e3b8).
#include "types.h"

struct UnkValue {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
};

void fn_71000a18d4(UnkValue* out, void* type, const void* value, u32 flag);

void* fn_7100265b1c();  // navmesh::CDynamicSmartLinkGroup::SDynamicSmartLinkRulesSet::refl_GetClass
extern u8 g_unk_71726a2bf8[] __attribute__((visibility("hidden")));

template <class T>
struct UnkArgument {
    u8 unk_0x0[8];
    T* field_0x8;
};

template <u32 Size>
struct UnkDefIterValue {
    u64 unk_0x0;
    union {
        u8 value[Size];
        void* ref;
    };
    s32 state;

    __attribute__((always_inline)) void* Get() { return state == 2 ? ref : value; }
};

__attribute__((always_inline)) static inline UnkValue GetRulesSet(UnkArgument<UnkDefIterValue<0x10>>* argument) {
    void* def = argument->field_0x8->Get();
    fn_7100265b1c();
    UnkValue result;
    result.field_0x0 = 0;
    result.field_0x8 = 0;
    result.field_0x10 = 2;
    fn_71000a18d4(&result, g_unk_71726a2bf8, def, 1);
    return result;
}

UnkValue fn_71002678c8(void* unused, UnkArgument<UnkDefIterValue<0x10>>* argument) { return GetRulesSet(argument); }

UnkValue fn_7100267b5c(void* unused, UnkArgument<UnkDefIterValue<0x10>>* argument) { return GetRulesSet(argument); }

__attribute__((always_inline)) static inline UnkValue MakeValue(void* type, const void* value) {
    UnkValue result;
    result.field_0x0 = 0;
    result.field_0x8 = 0;
    result.field_0x10 = 2;
    fn_71000a18d4(&result, type, value, 0);
    return result;
}

void* fn_710000d228();  // the bool type's getter

struct UnkCountObject {
    u8 unk_0x0[0x40];
    u32 field_0x40;
};

UnkValue fn_710011e3b8(void* unused, UnkArgument<UnkCountObject>* argument) {
    bool value = argument->field_0x8->field_0x40 > 1;
    return MakeValue(fn_710000d228(), &value);
}
