// Unit 2702 (CAIComponent's neighbourhood): moves an iterator value whose element is a callback set
// and a word; the element has only a copy constructor, so the move copies (the owner is shared).
// Names and layouts are placeholders.
#include <new>

#include "base/UnkCollectionIter.h"
#include "game/UnkCallbackSet_7101a20950.h"
#include "types.h"

struct UnkCopiedSetElem_l113b40 {
    UnkCallbackSet_7101a20950 set;
    u32 field_0x40;

    UnkCopiedSetElem_l113b40(const UnkCopiedSetElem_l113b40& other) = default;
};

void fn_7101113b40(void* dst, const void* src) {
    base::refl_IterValueMove<UnkCopiedSetElem_l113b40>(dst, src);
}
