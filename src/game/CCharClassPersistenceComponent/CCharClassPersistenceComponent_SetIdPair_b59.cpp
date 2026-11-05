// CCharClassPersistenceComponent's unit (tools/tu_map.py): reflected setters (the invokers of
// fn_71009c932c and its neighbours in src/reflection/function_inits_02.cpp) that copy a 16-byte
// {string id, u64} pair from the first argument into the object at +0x60, +0x70 or +0x80, and return
// an empty value (refl_SetFieldValue's shape, with CStrId's copy-assignment). Names are placeholders.
#include "base/reflection/UnkReflValue.h"

struct UnkStrIdPair_b59 {
    base::global::CStrId id;
    u64 value;
};

template <u32 Offset>
base::reflection::UnkReflValue SetFieldStrIdPair_b59(void*, char* obj, base::reflection::UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    base::reflection::UnkReflValue* arg = args[0];
    const UnkStrIdPair_b59* value = base::reflection::refl_ArgValue<UnkStrIdPair_b59>(arg);
    *reinterpret_cast<UnkStrIdPair_b59*>(target + Offset) = *value;
    return base::reflection::refl_EmptyValue();
}

base::reflection::UnkReflValue fn_71009c9c14(void* self, char* obj, base::reflection::UnkReflValue* const* args) {
    return SetFieldStrIdPair_b59<0x60>(self, obj, args);
}

base::reflection::UnkReflValue fn_71009c9d00(void* self, char* obj, base::reflection::UnkReflValue* const* args) {
    return SetFieldStrIdPair_b59<0x70>(self, obj, args);
}

base::reflection::UnkReflValue fn_71009c9dec(void* self, char* obj, base::reflection::UnkReflValue* const* args) {
    return SetFieldStrIdPair_b59<0x80>(self, obj, args);
}
