// base::reflection::CEnumType: the name id of an enumerator by its value, or the empty id (the
// list at +0xf0 as in CEnumType_FindValue.cpp). Names are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CEnumType.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CEnumType;

struct UnkEnumEntry_b66a1 {
    CStrId name;
    s32 value;
};

CStrId fn_71000999cc(const CEnumType* self, s32 value) {
    const UnkEnumEntry_b66a1* entries = reinterpret_cast<const UnkEnumEntry_b66a1*>(self->m_values.data);
    for (const UnkEnumEntry_b66a1 *it = entries, *end = entries + self->m_values.count; it != end; ++it)
        if (it->value == value)
            return it->name;
    return g_unk_7101d470e0;
}
