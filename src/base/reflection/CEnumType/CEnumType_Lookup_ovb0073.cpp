// base::reflection::CEnumType: an enumerator looked up by its name given as a string (the list at
// +0xf0 as in CEnumType_FindValue.cpp): whether it exists and its value (fn_7100099890), and its
// value or -1 (fn_71000997b4). Names are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CEnumType.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CEnumType;

struct UnkEnumEntry_ovb0073 {
    CStrId name;
    s32 value;
};

static inline const UnkEnumEntry_ovb0073* Entries_ovb0073(const CEnumType* self) {
    return reinterpret_cast<const UnkEnumEntry_ovb0073*>(self->m_values.data);
}

// Whether the enumerator named `name` exists, and its value.
bool fn_7100099890(const CEnumType* self, const char* name, s32* out) {
    CStrId id(name, true);
    for (const UnkEnumEntry_ovb0073 *it = Entries_ovb0073(self), *end = it + self->m_values.count; it != end; ++it) {
        if (it->name.data == id.data) {
            *out = it->value;
            return true;
        }
    }
    return false;
}

// The value of the enumerator named `name`, or -1.
s32 fn_71000997b4(const CEnumType* self, const char* name) {
    CStrId id(name, true);
    for (const UnkEnumEntry_ovb0073 *it = Entries_ovb0073(self), *end = it + self->m_values.count; it != end; ++it)
        if (it->name.data == id.data)
            return it->value;
    return -1;
}
