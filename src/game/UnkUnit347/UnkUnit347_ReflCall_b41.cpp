// Unit 347 (tools/tu_map.py, no name hint): a reflected function, the shape of fn_7101115b5c
// (CAIComponent), that passes its first two arguments (null when an argument has no type) to
// fn_71002634f8 on the object at 8 and returns its pointer result as a reflected value described by fn_71002295ac.
// Names are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

void* fn_71002634f8(char* self, void* a, void* b);
struct UnkEnumType;
UnkEnumType* fn_71002295ac();  // the signature src/game_s7/fn_710024cc40.cpp declares

static inline __attribute__((always_inline)) void* ArgPtr_b41a1(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_7100264040(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    void* p0 = ArgPtr_b41a1(&a[0]);
    void* p1 = ArgPtr_b41a1(&a[1]);
    void* v = fn_71002634f8(target, p0, p1);
    const auto* type = reinterpret_cast<const base::reflection::CType*>(fn_71002295ac());
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &v, false);
    return r;
}
