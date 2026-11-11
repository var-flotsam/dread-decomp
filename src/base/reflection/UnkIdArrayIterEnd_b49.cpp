// The end functions of reflected arrays of bare string ids (slot 0x168), next to the Begin of
// src/base/reflection/CEnumType/fn_7100099468.cpp (fn_71000ce8b8, fn_71000d0f1c) and with its
// types: the iterator refers to the element past the end when there is one, then is copied out
// and synced out of line, and the temporary's id is released. fn_71000ce990 over the default
// pool's ids, fn_71000d0ff4 over CFilePathStrId's. Names are placeholders.
#include "UnkClone.h"
#include "base/UnkCloneCalls.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkVector.h"
#include "game/UnkTU83.h"

struct UnkIdRecordOwner_0xf0;
using UnkFilePathIdElem = UnkCloneStrIdElem<UnkIdRecordOwner_0xf0, 0, 8, UnkCloneStrIdB<UnkIdRecordOwner_0xf0>>;
using UnkIdElem = UnkCloneStrIdElem<UnkIdRecordOwner_0xf0, 0, 8, UnkCloneStrId<UnkIdRecordOwner_0xf0>>;

// External linkage, inlined by the compiler: forced or internal, the first two loads swap.
template <class Elem, void (*Sync)(void*)>
void UnkIdArrayEnd_b49(const base::UnkVector<Elem>* v, base::UnkCollectionIter<Elem>* out) {
    base::UnkCollectionIter<Elem> it(v->count, v->data + v->count);
    if (out != &it) {
        out->index = it.index;
        out->element = it.element;
        Sync(out);
    }
}

void fn_71000d0ff4(const base::UnkVector<UnkFilePathIdElem>* v, base::UnkCollectionIter<UnkFilePathIdElem>* out) {
    UnkIdArrayEnd_b49<UnkFilePathIdElem, fn_71000be6c0>(v, out);
}

void fn_71000ce990(const base::UnkVector<UnkIdElem>* v, base::UnkCollectionIter<UnkIdElem>* out) {
    UnkIdArrayEnd_b49<UnkIdElem, fn_71000bce50>(v, out);
}
