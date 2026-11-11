// CMinimapManager::CTunableMinimapManager's unit (tools/tu_map.py unit 2566): End (slot 0x168) of a
// reflected array of 0x10-byte entries starting with a string id, the shape of fn_71000ce990
// (src/base/reflection/UnkIdArrayIterEnd_b49.cpp) with this unit's out-of-line sync (fn_7100ea7c6c).
// Names are placeholders.
#include "UnkClone.h"
#include "base/UnkCloneCalls.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkVector.h"
#include "game/UnkTU2566.h"

struct UnkIdRecordOwner_0xf0;
using UnkIdElem_b54 = UnkCloneStrIdElem<UnkIdRecordOwner_0xf0, 0, 0x10, UnkCloneStrId<UnkIdRecordOwner_0xf0>>;

void fn_7100eb02a4(const base::UnkVector<UnkIdElem_b54>* v, base::UnkCollectionIter<UnkIdElem_b54>* out) {
    base::UnkCollectionIter<UnkIdElem_b54> it(v->count, v->data + v->count);
    if (out != &it) {
        out->index = it.index;
        out->element = it.element;
        fn_7100ea7c6c(out);
    }
}
