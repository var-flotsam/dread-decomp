// Getters of the 0x10-byte TAnimTreeLinkDef behind a collection iterator value, as a value of kind 2
// (as in fn_7100c8e85c, src/game_aca57c/fn_7100ad4b54.cpp): the value is stored inline at 8, or
// through the pointer there when the state after it is 2. fn_71002d9bdc, fn_71002d9e50.
#include "types.h"

struct UnkValue {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
};

void fn_71000a18d4(UnkValue* out, void* type, const void* value, u32 flag);

void* fn_71002d7ab4();  // animtree::CAnimTreeDef::TAnimTreeLinkDef::refl_GetClass
extern u8 g_unk_71726a9e80[] __attribute__((visibility("hidden")));

template <typename T>
struct UnkArgument {
    u8 unk_0x0[8];
    T* field_0x8;
};

struct UnkLinkDefIterValue {
    u64 unk_0x0;
    union {
        u8 value[0x10];
        void* ref;
    };
    s32 state;

    __attribute__((always_inline)) void* Get() { return state == 2 ? ref : value; }
};

__attribute__((always_inline)) static inline UnkValue GetLinkDef(UnkArgument<UnkLinkDefIterValue>* argument) {
    void* def = argument->field_0x8->Get();
    fn_71002d7ab4();
    UnkValue result;
    result.field_0x0 = 0;
    result.field_0x8 = 0;
    result.field_0x10 = 2;
    fn_71000a18d4(&result, g_unk_71726a9e80, def, 1);
    return result;
}

UnkValue fn_71002d9bdc(void* unused, UnkArgument<UnkLinkDefIterValue>* argument) { return GetLinkDef(argument); }

UnkValue fn_71002d9e50(void* unused, UnkArgument<UnkLinkDefIterValue>* argument) { return GetLinkDef(argument); }
