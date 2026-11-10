// base::reflection::CCollectionType's unit (tools/tu_map.py): the types a collection type refers to (vtable slot 0x30): the base's (fn_710009d020), then the key and element types.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/CCollectionType.h"
#include "types.h"

using base::reflection::CCollectionType;
using base::reflection::CType;

void fn_710009d020(const CType* self, base::UnkVector<const CType*>* out);

using UnkRefsFn_ovb0137 = void (*)(const CType*, base::UnkVector<const CType*>*, bool);

void fn_7100098944(const CCollectionType* self, base::UnkVector<const CType*>* out, bool flag) {
    reinterpret_cast<UnkRefsFn_ovb0137>(&fn_710009d020)(self, out, flag);
    const CType* key = self->m_key;
    out->PushBack(key);
    const CType* element = self->m_element;
    out->PushBack(element);
}
