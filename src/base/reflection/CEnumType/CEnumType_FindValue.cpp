// base::reflection::CEnumType: looking an enumerator up by name in its list at +0xf0 (0x10-byte
// entries, a name id and the value): whether it exists and its value (fn_7100099724), its value or
// -1 (fn_710009976c), and the same stored through a reflected value's pointer at +0x8
// (fn_710009996c). Names are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CEnumType.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CEnumType;

struct UnkEnumEntry {
    CStrId name;
    s32 value;
};

static inline s32 ValueOf(const CEnumType* self, const CStrId* name) {
    const UnkEnumEntry* entries = reinterpret_cast<const UnkEnumEntry*>(self->m_values.data);
    for (const UnkEnumEntry *it = entries, *end = entries + self->m_values.count; it != end; ++it)
        if (it->name.data == name->data)
            return it->value;
    return -1;
}

bool fn_7100099724(const CEnumType* self, const CStrId* name, s32* out) {
    const UnkEnumEntry* entries = reinterpret_cast<const UnkEnumEntry*>(self->m_values.data);
    for (const UnkEnumEntry *it = entries, *end = entries + self->m_values.count; it != end; ++it) {
        if (it->name.data == name->data) {
            *out = it->value;
            return true;
        }
    }
    return false;
}

s32 fn_710009976c(const CEnumType* self, const CStrId* name) { return ValueOf(self, name); }

// A reflected value: the value's storage at +0x8.
struct UnkEnumValueRef {
    void* field_0x0;
    s32* value;
};

void fn_710009996c(const CEnumType* self, const UnkEnumValueRef* out, const CStrId* name) {
    *out->value = ValueOf(self, name);
}
