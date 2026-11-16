// Unit 2599 (hint 'CEnvironmentData::SFogTransition'): inserts a copy of a {vtable, string id, 4-byte
// value} element at an index of an array of them, moving the later elements up one place (growing the
// array first when it is full); appending is fn_7100f70c48. Names and layouts are placeholders.
#include <new>

#include "UnkClone.h"
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "types.h"

extern char g_unk_7101cab360[];

struct UnkClone_fn_7100f70e18;
void fn_7100f70c48(base::global::CRntVector<base::UnkIdValueElem<UnkClone_fn_7100f70e18, UnkCloneElem<UnkClone_fn_7100f70e18, 4>>>*, const base::UnkIdValueElem<UnkClone_fn_7100f70e18, UnkCloneElem<UnkClone_fn_7100f70e18, 4>>&);
template <> struct base::UnkIdValueTraits<UnkClone_fn_7100f70e18> {
    static char* vtable() { return g_unk_7101cab360; }
    static void push_back(base::global::CRntVector<base::UnkIdValueElem<UnkClone_fn_7100f70e18, UnkCloneElem<UnkClone_fn_7100f70e18, 4>>>* v, const base::UnkIdValueElem<UnkClone_fn_7100f70e18, UnkCloneElem<UnkClone_fn_7100f70e18, 4>>& x) { fn_7100f70c48(v, x); }
};

using UnkIdValue_f70fbc = base::UnkIdValueElem<UnkClone_fn_7100f70e18, UnkCloneElem<UnkClone_fn_7100f70e18, 4>>;

void fn_7100f70fbc(base::global::CRntVector<UnkIdValue_f70fbc>* self, const UnkIdValue_f70fbc* value, u32 index) {
    using T = UnkIdValue_f70fbc;
    u32 n = self->count;
    if (n == index) {
        fn_7100f70c48(self, *value);
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
