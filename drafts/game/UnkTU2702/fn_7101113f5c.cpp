// Unit 2702 (CAIComponent's neighbourhood): stores a callback-set element at an index of the
// callback-set array, growing the array first (value-initialised new elements) when the index is
// past the end and growing is allowed; the element is moved in. Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "game/UnkCallbackSetVector.h"
#include "types.h"

// The element's move assignment: shares the owner, copies the callbacks and empties the source,
// then copies the word.
inline void MoveAssign(UnkCallbackSetElem& dst, UnkCallbackSetElem& src) {
    fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&dst.set.owner),
                  reinterpret_cast<const base::UnkRefHandle*>(&src.set.owner));
    UnkErasedVector* list = &src.set.callbacks;
    dst.set.field_0x30 = src.set.field_0x30;
    fn_710023b29c(&dst.set.callbacks, list);
    src.set.field_0x30 = 0;
    fn_7100ad3588(list);
    if (UnkRefCounted* p = src.set.owner) {
        if (--p->ref_count == 0)
            p->Delete();
        src.set.owner = nullptr;
    }
    dst.field_0x40 = src.field_0x40;
}

void fn_7101113f5c(UnkCallbackSetVector* v, const u32* index, UnkCallbackSetElem* value) {
    u32 i = *index;
    if (v->field_0x14 && v->count <= i) {
        u32 capacity = v->capacity;
        if (capacity <= i && v->step != 0)
            capacity = (i / v->step + 1) * v->step;
        fn_71011143d8(v, capacity);
        u32 n = i + 1;
        fn_71011143d8(v, n);
        for (u32 k = v->count; k < n; ++k)
            new (&v->data[k]) UnkCallbackSetElem();
        v->count = n;
    }
    MoveAssign(v->data[i], *value);
}
