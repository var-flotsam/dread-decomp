// Begin over arrays of bare string ids in other units: the same as fn_71000d0f1c (CFilePathStrId's
// pool) and fn_71000ce8b8 (the default pool) in src/base/reflection/CEnumType/fn_7100099468.cpp,
// each with its own unit's out-of-line iterator sync; the temporary's id is released after it.
#include "UnkClone.h"
#include "base/UnkCloneCalls.h"
#include "base/UnkVector.h"

struct UnkBeginIdArrayOwner;

// Elements of 0x10 and 0x18 bytes holding a string id first.
using UnkBeginFilePathIdElem_0x10 = UnkCloneStrIdElem<UnkBeginIdArrayOwner, 0, 0x10, UnkCloneStrIdB<UnkBeginIdArrayOwner>>;
using UnkBeginFilePathIdElem_0x18 = UnkCloneStrIdElem<UnkBeginIdArrayOwner, 0, 0x18, UnkCloneStrIdB<UnkBeginIdArrayOwner>>;
using UnkBeginIdElem_0x10 = UnkCloneStrIdElem<UnkBeginIdArrayOwner, 0, 0x10, UnkCloneStrId<UnkBeginIdArrayOwner>>;

template <class Elem, void (*Sync)(void*)>
__attribute__((always_inline)) static inline void Begin(const base::UnkVector<Elem>* v,
                                                        base::UnkCollectionIter<Elem>* out) {
    base::UnkCollectionIter<Elem> it(0, v->data);
    if (out != &it) {
        out->index = it.index;
        out->element = it.element;
        Sync(out);
    }
}

void fn_7100ea7c6c(void*);  // CMinimapManager's unit
void fn_710104e720(void*);  // CFXPool::CDefinition's unit
void fn_7100fefde0(void*);

void fn_7100eb01cc(const base::UnkVector<UnkBeginIdElem_0x10>* v, base::UnkCollectionIter<UnkBeginIdElem_0x10>* out) {
    Begin<UnkBeginIdElem_0x10, fn_7100ea7c6c>(v, out);
}

void fn_71010500d8(const base::UnkVector<UnkBeginFilePathIdElem_0x10>* v,
                   base::UnkCollectionIter<UnkBeginFilePathIdElem_0x10>* out) {
    Begin<UnkBeginFilePathIdElem_0x10, fn_710104e720>(v, out);
}

void fn_7100ffc2c0(const base::UnkVector<UnkBeginFilePathIdElem_0x18>* v,
                   base::UnkCollectionIter<UnkBeginFilePathIdElem_0x18>* out) {
    Begin<UnkBeginFilePathIdElem_0x18, fn_7100fefde0>(v, out);
}
