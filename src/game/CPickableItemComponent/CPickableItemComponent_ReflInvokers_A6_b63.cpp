// CPickableItemComponent's unit (1493): reflected functions' invokers that pass their two arguments (as
// pointers) to a virtual of the object at +8 (slots 0x198 and 0x1a0) and return the int it answers. Names are
// placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

static inline __attribute__((always_inline)) void* ArgPtr_A6_b63(UnkReflValue* arg, const char* type) {
    if (!type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

template <u32 Slot>
static inline __attribute__((always_inline)) UnkReflValue InvokeInt2_A6_b63(char* obj, UnkReflValue* const* args) {
    typedef s32 (*Fn)(void*, void*, void*);
    void* self = *reinterpret_cast<void**>(obj + 8);
    void** vtable = *reinterpret_cast<void***>(self);
    asm("" ::: "memory");  // the object and its vtable are read before the arguments
    UnkReflValue* a = args[0];
    const char* first_type = a[0].type;
    asm("" ::: "memory");
    Fn fn = reinterpret_cast<Fn>(vtable[Slot / 8]);
    asm("" ::: "memory");  // the virtual is read with the first argument's type
    void* first = ArgPtr_A6_b63(&a[0], first_type);
    void* second = ArgPtr_A6_b63(&a[1], a[1].type);
    s32 result = fn(self, first, second);
    const CType* int_type = fn_7100014768();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, int_type, &result, false);
    return r;
}

UnkReflValue fn_71009d413c(void*, char* obj, UnkReflValue* const* args) {
    return InvokeInt2_A6_b63<0x198>(obj, args);
}

UnkReflValue fn_71009d4204(void*, char* obj, UnkReflValue* const* args) {
    return InvokeInt2_A6_b63<0x1a0>(obj, args);
}
