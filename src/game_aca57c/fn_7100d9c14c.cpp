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

template <u32 Offset>
__attribute__((always_inline)) static inline UnkReflValue GetWord(UnkArgument* argument, const void* descriptor) {
    u32 value = *reinterpret_cast<u32*>(argument->field_0x8 + Offset);
    UnkReflValue result;
    result.field_0x0 = nullptr;
    result.type = nullptr;
    result.kind = 2;
    fn_71000a18d4(&result, static_cast<const CType*>(descriptor), &value, 0);
    return result;
}

extern const CType g_unk_71727ba2e0;
extern const CType g_unk_71727ba480;
extern const CType g_unk_71727ca958;
extern const CType g_unk_71727caab8;

UnkReflValue fn_7100d9c14c(void* unused, UnkArgument* argument) { return GetWord<0x0>(argument, &g_unk_71727ba2e0); }
UnkReflValue fn_7100d9c1e0(void* unused, UnkArgument* argument) { return GetWord<0x4>(argument, &g_unk_71727ba480); }
UnkReflValue fn_7100eaa018(void* unused, UnkArgument* argument) { return GetWord<0x98>(argument, &g_unk_71727ca958); }
UnkReflValue fn_7100eaca98(void* unused, UnkArgument* argument) { return GetWord<0x0>(argument, &g_unk_71727caab8); }

// The same getter in other units, each with its own offset and type (as the units declare the types).
#include "../game_s7/UnkStaticInitS7.h"
#include "game/UnkTUInit.h"

using unk_static_init::UnkEnumType;

extern base::reflection::CType g_unk_7108b6cc88;
extern base::reflection::CType g_unk_7108b6cde8;
extern UnkReflType g_unk_7172791c90;
extern UnkEnumType g_unk_7172730858;
extern UnkEnumType g_unk_7172743e90;
extern base::reflection::CType g_unk_7172752160;
extern base::reflection::CType g_unk_7108b6d110;
extern base::reflection::CType g_unk_7108b6d270;
extern base::reflection::CType g_unk_7108b6cb28;
extern base::reflection::CType g_unk_7108b6cf48;

UnkReflValue fn_7100718544(void* unused, UnkArgument* argument) { return GetWord<0xac0>(argument, &g_unk_7108b6cc88); }
UnkReflValue fn_71007185d0(void* unused, UnkArgument* argument) { return GetWord<0xac4>(argument, &g_unk_7108b6cde8); }
UnkReflValue fn_710075d374(void* unused, UnkArgument* argument) { return GetWord<0xf8>(argument, &g_unk_7172791c90); }
UnkReflValue fn_710078dfbc(void* unused, UnkArgument* argument) { return GetWord<0x78>(argument, &g_unk_7172730858); }
UnkReflValue fn_7100a13530(void* unused, UnkArgument* argument) { return GetWord<0x420>(argument, &g_unk_7172743e90); }
UnkReflValue fn_7100a94804(void* unused, UnkArgument* argument) { return GetWord<0x53c>(argument, &g_unk_7172752160); }
UnkReflValue fn_7100a94890(void* unused, UnkArgument* argument) { return GetWord<0x540>(argument, &g_unk_7172752160); }
UnkReflValue fn_710103acd4(void* unused, UnkArgument* argument) { return GetWord<0x568>(argument, &g_unk_7108b6d110); }
UnkReflValue fn_710103ad60(void* unused, UnkArgument* argument) { return GetWord<0x56c>(argument, &g_unk_7108b6d270); }
UnkReflValue fn_710103adec(void* unused, UnkArgument* argument) { return GetWord<0x570>(argument, &g_unk_7108b6cb28); }
UnkReflValue fn_710103ae78(void* unused, UnkArgument* argument) { return GetWord<0x574>(argument, &g_unk_7108b6cf48); }
