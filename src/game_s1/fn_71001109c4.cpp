// Reflection setter (an invoker) of a pair of f32 at offset 0 of the object: copies the first
// argument's value in and returns an empty value. Like base::reflection::refl_SetFieldValue, for
// an 8-byte struct copied member by member. Called only through its function descriptor.

#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkF32Pair_71001109c4 {
    f32 x;
    f32 y;
};

UnkReflValue fn_71001109c4(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    UnkReflValue* arg = args[0];
    auto* field = reinterpret_cast<UnkF32Pair_71001109c4*>(target);
    const UnkF32Pair_71001109c4* value = base::reflection::refl_ArgValue<UnkF32Pair_71001109c4>(arg);
    field->x = value->x;
    field->y = value->y;
    return base::reflection::refl_EmptyValue();
}

// A getter (an invoker) returning the sum of the floats at 0x24 and 0x20 as a reflected float
// (fn_7100110a98).
UnkReflValue fn_7100110a98(void*, char* obj) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    f32 value = *reinterpret_cast<f32*>(target + 0x24) + *reinterpret_cast<f32*>(target + 0x20);
    const base::reflection::CType* type = fn_710000abbc();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}

// Invokers of the same shape in other units: a value built after the work, with its type's getter.
template <const base::reflection::CType* (*Getter)(), class T>
__attribute__((always_inline)) static inline UnkReflValue MakeValue(T value) {
    const base::reflection::CType* type = Getter();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}

static inline char* Target(char* obj) { return *reinterpret_cast<char**>(obj + 8); }

// Clear a word of the object and return 1 as an int.
template <u32 Offset>
__attribute__((always_inline)) static inline UnkReflValue ClearWord(char* obj) {
    *reinterpret_cast<u32*>(Target(obj) + Offset) = 0;
    return MakeValue<fn_7100014768, s32>(1);
}

UnkReflValue fn_7100f0f090(void*, char* obj) { return ClearWord<0x144>(obj); }
UnkReflValue fn_7100f0f1dc(void*, char* obj) { return ClearWord<0x148>(obj); }
UnkReflValue fn_7100f0f2dc(void*, char* obj) { return ClearWord<0xf0>(obj); }

// A flag of the object, and one two pointers away.
UnkReflValue fn_7101115c1c(void*, char* obj) {
    return MakeValue<fn_710000d228, bool>(*reinterpret_cast<bool*>(Target(obj) + 0x178d));
}
UnkReflValue fn_71007a0bdc(void*, char* obj) {
    char* p = *reinterpret_cast<char**>(Target(obj) + 0x50);
    return MakeValue<fn_710000d228, bool>(*reinterpret_cast<bool*>(p + 0x10f));
}

// Calls on the object: one returning nothing (the value is 1 as an int), one returning a flag.
void fn_71006004d4(void* self);
void fn_71008d5200(void* self);
bool fn_71007aa610(void* self);
bool fn_71008d0ef4(void* self);

UnkReflValue fn_71006020c0(void*, char* obj) {
    fn_71006004d4(Target(obj));
    return MakeValue<fn_7100014768, s32>(1);
}
UnkReflValue fn_71008dcbb8(void*, char* obj) {
    fn_71008d5200(Target(obj));
    return MakeValue<fn_7100014768, s32>(1);
}
UnkReflValue fn_71007adbe8(void*, char* obj) { return MakeValue<fn_710000d228, bool>(fn_71007aa610(Target(obj))); }
UnkReflValue fn_71008dcc08(void*, char* obj) { return MakeValue<fn_710000d228, bool>(fn_71008d0ef4(Target(obj))); }

void fn_7101126af4(void* self);
bool fn_710112c3dc(void* self);

UnkReflValue fn_7101132928(void*, char* obj) {
    fn_7101126af4(Target(obj));
    return MakeValue<fn_7100014768, s32>(1);
}
UnkReflValue fn_71011329bc(void*, char* obj) { return MakeValue<fn_710000d228, bool>(fn_710112c3dc(Target(obj))); }
