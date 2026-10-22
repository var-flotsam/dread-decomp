// Makes sure the array of the pool entry with the given 16-bit id (when it is not 0xffff) has
// room for `count` elements.
#include "base/UnkVector.h"
#include "types.h"

struct UnkPool;
base::UnkVector<u64>* fn_71002b1a3c(UnkPool* pool, u32 id);

// NON_MATCHING(minor): the id test is `mvn` + `tst #0xffff` in the original, `add #1` here.
void fn_71002b199c(UnkPool* pool, u32 id, u32 count) {
    if (((id + 1) & 0xffff) == 0)
        return;
    fn_71002b1a3c(pool, (u16)id)->Reserve(count);
}
