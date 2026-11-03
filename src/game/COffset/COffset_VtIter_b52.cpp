// COffset's unit (tools/tu_map.py unit 936): Begin and End (slots 0x160 and 0x168 in
// src/reflection/CShootDCBones.cpp) over a reflected array of 0x20-byte polymorphic elements holding
// a string id, as base::refl_VtBegin / base::refl_VtEnd: the iterator syncs out of line
// (fn_7100617f20, a refl_IterSync copy), then the temporary's owned value is destroyed inline (the
// base vtable unk_7101a56178 stored, the id released). Names are placeholders.
#include "UnkClone.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"

extern char g_unk_7101a56178[];
void fn_7100617f20(void* it);

struct UnkOffsetElemTag_b52;
template <>
struct base::UnkVtIterTraits<UnkOffsetElemTag_b52> {
    static char* base_vtable() { return g_unk_7101a56178; }
    static void sync(void* it) { fn_7100617f20(it); }
};

using UnkOffsetElem_b52 = base::UnkStrIdVtElem<UnkOffsetElemTag_b52, 0x20>;

void fn_710061d54c(const base::UnkVector<UnkOffsetElem_b52>* v, base::UnkCollectionIter<UnkOffsetElem_b52>* out) {
    base::refl_VtBegin(v, out);
}

void fn_710061d634(const base::UnkVector<UnkOffsetElem_b52>* v, base::UnkCollectionIter<UnkOffsetElem_b52>* out) {
    base::refl_VtEnd(v, out);
}
