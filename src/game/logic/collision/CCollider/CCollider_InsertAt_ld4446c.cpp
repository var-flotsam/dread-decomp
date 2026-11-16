// CCollider's unit (2503): moves an array in at an index of an array of arrays, moving the later ones up one
// place (growing the outer array first when it is full); appending is fn_7100d44688. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntVector;

// An array of plain words, freed with its owner.
struct UnkWordVec_ld4446c {
    CRntVector<void*> v;

    UnkWordVec_ld4446c(UnkWordVec_ld4446c&& other) : v(static_cast<CRntVector<void*>&&>(other.v)) {}
    ~UnkWordVec_ld4446c() { v.Free(); }
};

void fn_7100d44688(void*, void*);

void fn_7100d4446c(CRntVector<UnkWordVec_ld4446c>* self, UnkWordVec_ld4446c* value, u32 index) {
    using E = UnkWordVec_ld4446c;
    u32 n = self->count;
    if (n == index) {
        fn_7100d44688(self, value);
        return;
    }
    u32 cap = self->capacity;
    if (cap <= n && self->step != 0) {
        u32 grown = (n / self->step + 1) * self->step;
        if (cap < grown) {
            E* p = static_cast<E*>(self->allocator->vf_0x40((u64)grown * sizeof(E), 8, self->flags));
            if (E* s = self->data) {
                E* o = p;
                for (u32 i = 0; i < self->count; ++i, ++s, ++o) {
                    new (o) E(static_cast<E&&>(*s));
                    s->~E();
                }
                self->allocator->vf_0x48(self->data);
            }
            self->data = p;
            self->capacity = grown;
        }
    }
    // The arrays from the last down to the index move up one place (the index runs through -1 when it is 0).
    for (u32 i = self->count - 1; i >= index; --i) {
        E* q = &self->data[i];
        new (q + 1) E(static_cast<E&&>(*q));
        self->data[i].~E();
        if (i == 0)
            break;
    }
    new (&self->data[index]) E(static_cast<E&&>(*value));
    ++self->count;
}
