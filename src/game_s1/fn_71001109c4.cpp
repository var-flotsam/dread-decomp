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

// Defined three times: fn_71001109c4 and its copies in two other units, whose pairs sit at 0xb0c and
// 0xa4 (an inline helper changes how the value is returned).
#define UNK_SET_F32_PAIR(name, at)                                                                          \
    UnkReflValue name(void*, char* obj, UnkReflValue* const* args) {                                        \
        char* target = *reinterpret_cast<char**>(obj + 8);                                                  \
        UnkReflValue* arg = args[0];                                                                        \
        auto* field = reinterpret_cast<UnkF32Pair_71001109c4*>(target + at);                                \
        const UnkF32Pair_71001109c4* value = base::reflection::refl_ArgValue<UnkF32Pair_71001109c4>(arg); \
        field->x = value->x;                                                                                \
        field->y = value->y;                                                                                \
        return base::reflection::refl_EmptyValue();                                                        \
    }

UNK_SET_F32_PAIR(fn_71001109c4, 0)
UNK_SET_F32_PAIR(fn_71008a0118, 0xb0c)
UNK_SET_F32_PAIR(fn_7100eaa2b0, 0xa4)

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

// Step the same words up or down by one (and return 1 as an int).
template <u32 Offset, s32 Delta>
__attribute__((always_inline)) static inline UnkReflValue StepWord(char* obj) {
    *reinterpret_cast<s32*>(Target(obj) + Offset) += Delta;
    return MakeValue<fn_7100014768, s32>(1);
}

UnkReflValue fn_7100f0efe0(void*, char* obj) { return StepWord<0x144, 1>(obj); }
UnkReflValue fn_7100f0f038(void*, char* obj) { return StepWord<0x144, -1>(obj); }
UnkReflValue fn_7100f0f12c(void*, char* obj) { return StepWord<0x148, 1>(obj); }
UnkReflValue fn_7100f0f184(void*, char* obj) { return StepWord<0x148, -1>(obj); }
UnkReflValue fn_7100f0f22c(void*, char* obj) { return StepWord<0xf0, 1>(obj); }
UnkReflValue fn_7100f0f284(void*, char* obj) { return StepWord<0xf0, -1>(obj); }

// A flag of the object, and one two pointers away.
UnkReflValue fn_7101115c1c(void*, char* obj) {
    return MakeValue<fn_710000d228, bool>(*reinterpret_cast<bool*>(Target(obj) + 0x178d));
}
UnkReflValue fn_71007a0bdc(void*, char* obj) {
    char* p = *reinterpret_cast<char**>(Target(obj) + 0x50);
    return MakeValue<fn_710000d228, bool>(*reinterpret_cast<bool*>(p + 0x10f));
}

// A flag fn_710020c4a8 reports for a sub-object of the object at +0x58.
bool fn_710020c4a8(void* self);

template <u32 Offset>
__attribute__((always_inline)) static inline UnkReflValue SubFlag(char* obj) {
    char* p = *reinterpret_cast<char**>(Target(obj) + 0x58);
    return MakeValue<fn_710000d228, bool>(fn_710020c4a8(p + Offset));
}

UnkReflValue fn_71008ebbec(void*, char* obj) { return SubFlag<0x20>(obj); }
UnkReflValue fn_71008f2228(void*, char* obj) { return SubFlag<0x28>(obj); }
UnkReflValue fn_71008fa2f4(void*, char* obj) { return SubFlag<0x20>(obj); }

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

// More of the same in other units.
void fn_710093aad0(void* self);
void fn_7100a03118(void* self);
void fn_71011269c0(void* self);
bool fn_710024fbc8(void* self);

UnkReflValue fn_710093b104(void*, char* obj) {
    fn_710093aad0(Target(obj));
    return MakeValue<fn_7100014768, s32>(1);
}
UnkReflValue fn_7100a05828(void*, char* obj) {
    fn_7100a03118(Target(obj));
    return MakeValue<fn_7100014768, s32>(1);
}
UnkReflValue fn_71011328d8(void*, char* obj) {
    fn_71011269c0(Target(obj));
    return MakeValue<fn_7100014768, s32>(1);
}
UnkReflValue fn_71010c57bc(void*, char* obj) { return MakeValue<fn_710000d228, bool>(fn_710024fbc8(Target(obj))); }

// Hand the first argument's value to a method of the object and return what it returns as an int,
// or 0 for a method that returns nothing (four units' copies, each calling its own method).
void fn_710117b3f8(void* self, const void* value);
void fn_7100873bf8(void* self, const void* value);
s32 fn_7101173ddc(void* self, const void* value);
void fn_710117a018(void* self, const void* value);

template <void (*Method)(void*, const void*)>
__attribute__((always_inline)) static inline s32 ReturningZero(void* self, const void* value) {
    Method(self, value);
    return 0;
}

// The value: none without a type, else held in place unless stored out of line or too big. A macro
// writes each invoker's body (an inline helper changes how the result is returned).
#define UNK_CALL_WITH_ARG(method)                                                                \
    char* target = Target(obj);                                                                  \
    UnkReflValue* arg = args[0];                                                                 \
    const void* value;                                                                           \
    if (!arg->type)                                                                              \
        value = nullptr;                                                                         \
    else if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)        \
        value = arg->field_0x0;                                                                  \
    else                                                                                         \
        value = arg;                                                                             \
    return MakeValue<fn_7100014768, s32>(method(target, value));

UnkReflValue fn_710117bba0(void*, char* obj, UnkReflValue* const* args) { UNK_CALL_WITH_ARG(ReturningZero<fn_710117b3f8>) }
UnkReflValue fn_7100874214(void*, char* obj, UnkReflValue* const* args) { UNK_CALL_WITH_ARG(ReturningZero<fn_7100873bf8>) }
UnkReflValue fn_71011746a4(void*, char* obj, UnkReflValue* const* args) { UNK_CALL_WITH_ARG(fn_7101173ddc) }
UnkReflValue fn_710117bc1c(void*, char* obj, UnkReflValue* const* args) { UNK_CALL_WITH_ARG(ReturningZero<fn_710117a018>) }
