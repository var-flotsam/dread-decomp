// base::reflection::CPointerType's unit (tools/tu_map.py unit 80): the types a pointer type refers to (vtable
// slot 0x30): the base's (fn_710009d020), the pointee, and, when the pointee has entries in its array at 0x90,
// the types fn_710009d3b8 collects from it. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/CPointerType.h"
#include "types.h"

using base::global::CRntVector;
using base::reflection::CPointerType;
using base::reflection::CType;

void CType_AppendParentType(const CType* self, base::global::CRntVector<const CType*>* out);
// Collects the types `type` refers to through its array at 0x90.
void fn_710009d3b8(const CType* type, CRntVector<const CType*>* out);

using UnkRefsFn_l09c150 = void (*)(const CType*, CRntVector<const CType*>*, bool);

void fn_710009c150(const CPointerType* self, CRntVector<const CType*>* out, bool flag) {
    reinterpret_cast<UnkRefsFn_l09c150>(&CType_AppendParentType)(self, out, flag);
    const CType* pointee = self->m_pointee;
    out->PushBack(pointee);
    if (self->m_pointee->m_array_0x90.count == 0)
        return;
    CRntVector<const CType*> refs(0xf);
    refs.Reserve(100);
    fn_710009d3b8(self->m_pointee, &refs);
    for (const CType* type : refs)
        out->PushBack(type);
    refs.Free();
}
