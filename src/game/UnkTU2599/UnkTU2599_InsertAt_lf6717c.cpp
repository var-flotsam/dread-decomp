// Unit 2599 (hint 'CEnvironmentData::SFogTransition'): inserts a copy of a {vtable, string id, 8-byte
// value} element at an index of an array of them, moving the later elements up one place (growing the
// array first when it is full); appending is fn_7100f66e08. Names and layouts are placeholders.
#include <new>

#include "UnkClone.h"
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU2599.h"
#include "types.h"

struct UnkClone_fn_7100f66fd8;
void fn_7100f66e08(base::global::CRntVector<base::UnkIdValueElem<UnkClone_fn_7100f66fd8, UnkCloneElem<UnkClone_fn_7100f66fd8, 8>>>*, const base::UnkIdValueElem<UnkClone_fn_7100f66fd8, UnkCloneElem<UnkClone_fn_7100f66fd8, 8>>&);
template <> struct base::UnkIdValueTraits<UnkClone_fn_7100f66fd8> {
    static char* vtable() { return g_unk_7101cab2c0; }
    static void push_back(base::global::CRntVector<base::UnkIdValueElem<UnkClone_fn_7100f66fd8, UnkCloneElem<UnkClone_fn_7100f66fd8, 8>>>* v, const base::UnkIdValueElem<UnkClone_fn_7100f66fd8, UnkCloneElem<UnkClone_fn_7100f66fd8, 8>>& x) { fn_7100f66e08(v, x); }
};

using UnkIdValue_f6717c = base::UnkIdValueElem<UnkClone_fn_7100f66fd8, UnkCloneElem<UnkClone_fn_7100f66fd8, 8>>;

void fn_7100f6717c(base::global::CRntVector<UnkIdValue_f6717c>* self, const UnkIdValue_f6717c* value, u32 index) {
    using T = UnkIdValue_f6717c;
    u32 n = self->count;
    if (n == index) {
        fn_7100f66e08(self, *value);
        return;
    }
    if (self->capacity <= n && self->step != 0) {
        u32 capacity = (n / self->step + 1) * self->step;
        if (self->capacity < capacity) {
            base::refl_IdValueMoveTo(self, capacity);
            n = self->count;
        }
    }
    // The elements from the last down to the index move up one place (the index runs through -1 when it is 0).
    for (u32 i = n - 1; i >= index; --i) {
        T* from = &self->data[i];
        new (from + 1) T(*from);
        self->data[i].Destroy();
        if (i == 0)
            break;
    }
    T* e = &self->data[index];
    new (e) T(*value);
    ++self->count;
}
