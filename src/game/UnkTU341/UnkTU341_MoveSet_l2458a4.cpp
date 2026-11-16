// UnkTU341 (tools/tu_map.py unit 341): the callback set's move constructor: a copy of the other
// set, after which the other's callbacks are cleared and its owner released.
// Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "game/UnkCallbackSet_7101a20950.h"
#include "types.h"

UnkCallbackSet_7101a20950::UnkCallbackSet_7101a20950(UnkCallbackSet_7101a20950&& other)
    : owner(nullptr) {
    UnkRefCounted* q = other.owner;
    if (q != owner) {
        if (q)
            ++q->ref_count;
        if (UnkRefCounted* old = owner) {
            if (--old->ref_count == 0)
                old->Delete();
        }
        owner = q;
    }
    UnkErasedVector* from = &other.callbacks;
    field_0x30 = other.field_0x30;
    fn_710023b29c(&callbacks, from);
    other.field_0x30 = 0;
    fn_7100ad3588(from);
    if (UnkRefCounted* p = other.owner) {
        if (--p->ref_count == 0)
            p->Delete();
        other.owner = nullptr;
    }
}
