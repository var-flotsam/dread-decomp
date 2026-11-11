// Unit 2640 of tools/tu_map.py: the end of an array of 0x18-byte elements led by a file path id,
// next to its begin fn_7100ffc2c0 (src/base/reflection/UnkIdArrayBegin.cpp) and with its types: the
// iterator past the last element, copied to the output unless it is the output and synced out of line
// (fn_7100fefde0), the temporary's id released. Names are placeholders.
#include "UnkClone.h"
#include "base/UnkCloneCalls.h"
#include "base/UnkVector.h"
#include "game/UnkTU2640.h"

struct UnkBeginIdArrayOwner;
using UnkEndFilePathIdElem_0x18 = UnkCloneStrIdElem<UnkBeginIdArrayOwner, 0, 0x18, UnkCloneStrIdB<UnkBeginIdArrayOwner>>;

void fn_7100ffc398(const base::UnkVector<UnkEndFilePathIdElem_0x18>* v,
                   base::UnkCollectionIter<UnkEndFilePathIdElem_0x18>* out) {
    base::UnkCollectionIter<UnkEndFilePathIdElem_0x18> it(v->count, v->data + v->count);
    if (out != &it) {
        out->index = it.index;
        out->element = it.element;
        fn_7100fefde0(out);
    }
}
