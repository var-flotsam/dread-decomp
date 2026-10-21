#pragma once

// A polymorphic element holding a string id (in its base class, which has its own vtable) and
// a T: UnkVector's resize, grow-at and copy constructor over it (fn_7100f79bb0, fn_7100f44150,
// fn_7100f5fa48 and their copies) and insert (fn_7100f601b8). The id is held raw, as a union member, so its copy and
// stores follow the original's order; the destructor is the vtable's second slot. Default construction zero-fills the element first, as value-initialisation does.

#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

namespace base {

// base_vtable(), vtable(): the two vtables; init(value): the derived class's member initialiser
// (default construction only); resize(v, n), push_back(v, e): the out-of-line calls.
template <class Tag>
struct UnkStrIdValueTraits;

template <class Tag, class T>
struct UnkStrIdValueElem {
    using Traits = UnkStrIdValueTraits<Tag>;
    char* vtable;
    union {
        global::UnkStringData* id;
    };
    T value;

    UnkStrIdValueElem() {
        __builtin_memset(this, 0, sizeof(*this));
        vtable = Traits::base_vtable() + 0x10;
        if (!g_unk_7101d47108)
            global::UnkEnsureStringPool();
        id = &g_unk_7101d47108->empty;
        vtable = Traits::vtable() + 0x10;
        Traits::init(value);
    }
    UnkStrIdValueElem(const UnkStrIdValueElem& other) {
        vtable = Traits::base_vtable() + 0x10;
        id = other.id;
        if (&g_unk_7101d47108->empty != id)
            ++id->ref_count;
        vtable = Traits::vtable() + 0x10;
        value = other.value;
    }
    ~UnkStrIdValueElem() {
        reinterpret_cast<void (*)(UnkStrIdValueElem*)>(reinterpret_cast<void**>(vtable)[1])(this);
    }
};

template <class Tag, class T>
inline void refl_StrIdValueReallocate(UnkVector<UnkStrIdValueElem<Tag, T>>* v, u32 n) {
    using E = UnkStrIdValueElem<Tag, T>;
    E* p = static_cast<E*>(v->allocator->vf_0x40((u64)n * sizeof(E), 8, v->flags));
    if (E* s = v->data) {
        E* o = p;
        for (u32 i = 0; i < v->count; ++i, ++s, ++o) {
            new (o) E(*s);
            s->~E();
        }
        v->allocator->vf_0x48(v->data);
    }
    v->data = p;
    v->capacity = n;
}

template <class Tag, class T>
void refl_StrIdValueResize(UnkVector<UnkStrIdValueElem<Tag, T>>* v, u32 n) {
    using E = UnkStrIdValueElem<Tag, T>;
    if (v->capacity < n)
        refl_StrIdValueReallocate(v, n);
    for (u32 i = v->count; i < n; ++i)
        new (&v->data[i]) E();
    v->count = n;
}

template <class Tag, class T>
UnkStrIdValueElem<Tag, T>& refl_StrIdValueGrowAt(UnkVector<UnkStrIdValueElem<Tag, T>>* v, u32 index) {
    if (v->field_0x14 && v->count <= index) {
        u32 cap = v->capacity;
        if (cap <= index && v->step != 0) {
            u32 grown = (index / v->step + 1) * v->step;
            if (cap < grown)
                refl_StrIdValueReallocate(v, grown);
        }
        UnkStrIdValueTraits<Tag>::resize(v, index + 1);
    }
    return v->data[index];
}

template <class Tag, class T>
void refl_StrIdValueVecCopy(UnkVector<UnkStrIdValueElem<Tag, T>>* self,
                            const UnkVector<UnkStrIdValueElem<Tag, T>>* other) {
    using E = UnkStrIdValueElem<Tag, T>;
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    if (u32 n = other->capacity)
        refl_StrIdValueReallocate(self, n);
    for (const E *e = other->data, *end = e + other->count; e != end; ++e)
        UnkStrIdValueTraits<Tag>::push_back(self, e);
}

// Copies `value` in at `index`, shifting the later elements up (fn_7100f601b8 and its copies).
// Appending grows like UnkVector::PushBack, hang included; each branch constructs the element
// itself. A copy that calls its push-back for the append (fn_7100f5ff2c) allocates one register
// differently in the shared tail and isn't matched yet.
template <class Tag, class T>
void refl_StrIdValueInsert(UnkVector<UnkStrIdValueElem<Tag, T>>* v, const UnkStrIdValueElem<Tag, T>& value,
                           u32 index) {
    using E = UnkStrIdValueElem<Tag, T>;
    u32 cap = v->capacity;
    u32 n = v->count;
    if (n == index) {
        u32 new_capacity = cap;
        if (cap <= n && v->step != 0)
            new_capacity = (index / v->step + 1) * v->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        E* p;
        if (cap < new_capacity) {
            p = static_cast<E*>(v->allocator->vf_0x40((u64)new_capacity * sizeof(E), 8, v->flags));
            if (E* s = v->data) {
                E* o = p;
                for (u32 i = 0; i < v->count; ++i, ++s, ++o) {
                    new (o) E(*s);
                    s->~E();
                }
                v->allocator->vf_0x48(v->data);
            }
            index = v->count;
            v->data = p;
            v->capacity = new_capacity;
        } else {
            p = v->data;
        }
        new (&p[index]) E(value);
    } else {
        if (cap <= n && v->step != 0) {
            u32 grown = (n / v->step + 1) * v->step;
            if (cap < grown)
                refl_StrIdValueReallocate(v, grown);
        }
        for (u32 i = v->count - 1; i >= index; --i) {
            E* q = &v->data[i];
            new (q + 1) E(*q);
            v->data[i].~E();
            if (i == 0)
                break;
        }
        new (&v->data[index]) E(value);
    }
    ++v->count;
}

}  // namespace base
