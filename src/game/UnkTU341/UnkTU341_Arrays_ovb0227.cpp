// Unit 341 (no reliable name; hint 'CActor'): insertion into engine arrays (base::UnkVector's layout) of
// ref-counted pointers (fn_7100241ddc, fn_7100241fbc; growth out of line in fn_7100241b30) and of callback
// sets (vtable 0x7101a20950, 0x40 bytes; Insert, GrowAt and the copying insert fn_7100246eb8 grow in place,
// moving elements with the set's move constructor fn_71002458a4 and destroying them through its first
// virtual function), and the string list a reflected default value holds, built from an initializer list
// (fn_7100228ca0). Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::CRntString;

struct UnkRcObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        u32 field_0x8;
    };
};

struct UnkRcRef {
    UnkRcObject* ptr;

    UnkRcRef() : ptr(nullptr) {}
    UnkRcRef(const UnkRcRef& other) {
        __builtin_memset(&ptr, 0, sizeof(ptr));
        UnkRcObject* o = other.ptr;
        if (o) {
            ++o->field_0x8;
            Release();
            ptr = o;
        }
    }
    ~UnkRcRef() { Release(); }

    void Release() {
        UnkRcObject* o = ptr;
        if (o && --o->field_0x8 == 0)
            o->vf_0x10();
    }
};

struct UnkRcRefArray {
    UnkRcRef* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkTag_7100241b30;
template <class Tag>
struct UnkSlotArray;
void fn_7100241b30(UnkSlotArray<UnkTag_7100241b30>* self, u32 capacity);

__attribute__((always_inline)) static inline void InsertAt(UnkRcRefArray* self, const UnkRcRef& value, u32 index) {
    u32 n = self->count;
    u32 cap = self->capacity;
    if (n == index) {
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (index / self->step + 1) * self->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        fn_7100241b30(reinterpret_cast<UnkSlotArray<UnkTag_7100241b30>*>(self), new_capacity);
        new (&self->data[self->count]) UnkRcRef(value);
    } else {
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        fn_7100241b30(reinterpret_cast<UnkSlotArray<UnkTag_7100241b30>*>(self), new_capacity);
        for (u32 i = self->count - 1; i >= index; --i) {
            UnkRcRef* q = &self->data[i];
            new (q + 1) UnkRcRef(*q);
            self->data[i].~UnkRcRef();
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkRcRef(value);
    }
    ++self->count;
}

void fn_7100241ddc(UnkRcRefArray* self, const UnkRcRef* value, u32 index) { InsertAt(self, *value, index); }
void fn_7100241fbc(UnkRcRefArray* self, const UnkRcRef* value, u32 index) { InsertAt(self, *value, index); }

struct UnkRefCounted {
    virtual void vf0();
    virtual void vf8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
};

struct UnkCallbackBase {
    virtual void vf0();
    UnkOwnedErasedVector callbacks;
    u32 field_0x30;

    UnkCallbackBase() : field_0x30(0) {}
};

struct UnkCallbackSet_7101a20950 : UnkCallbackBase {
    virtual void vf8();
    UnkRefCounted* owner;

    UnkCallbackSet_7101a20950() : owner(nullptr) {}
    UnkCallbackSet_7101a20950(const UnkCallbackSet_7101a20950& other) : owner(nullptr) {
        UnkRefCounted* p = other.owner;
        if (p != owner) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted* old = owner) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            owner = p;
        }
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
    }

    UnkCallbackSet_7101a20950(UnkCallbackSet_7101a20950&& other);  // fn_71002458a4

    __attribute__((always_inline)) ~UnkCallbackSet_7101a20950() {
        if (UnkRefCounted* p = owner) {
            if (--p->ref_count == 0)
                p->Delete();
        }
    }
};

struct UnkSetArray {
    UnkCallbackSet_7101a20950* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) UnkCallbackSet_7101a20950* MoveTo(u32 new_capacity) {
        UnkCallbackSet_7101a20950* p = static_cast<UnkCallbackSet_7101a20950*>(
            allocator->vf_0x40((u64)new_capacity * sizeof(UnkCallbackSet_7101a20950), 8, flags));
        if (data) {
            UnkCallbackSet_7101a20950* src = data;
            UnkCallbackSet_7101a20950* dst = p;
            for (u32 i = 0; i < count; ++i) {
                new (dst) UnkCallbackSet_7101a20950(static_cast<UnkCallbackSet_7101a20950&&>(*src));
                src->vf0();
                ++dst;
                ++src;
            }
            allocator->vf_0x48(data);
        }
        return p;
    }

    // The element at `index`; an index past the end first grows the array (to the next multiple of the
    // step, then to exactly index + 1) and default-constructs the new elements.
    __attribute__((always_inline)) UnkCallbackSet_7101a20950* GrowAt(u32 index) {
        if (field_0x14 && count <= index) {
            u32 cap = capacity;
            if (cap <= index && step != 0) {
                u32 grown = (index / step + 1) * step;
                if (cap < grown) {
                    UnkCallbackSet_7101a20950* q = MoveTo(grown);
                    data = q;
                    capacity = grown;
                    cap = grown;
                }
            }
            u32 n = index + 1;
            if (cap < n) {
                UnkCallbackSet_7101a20950* q = MoveTo(n);
                data = q;
                capacity = n;
            }
            for (u32 i = count; i < n; ++i)
                new (&data[i]) UnkCallbackSet_7101a20950();
            count = n;
        }
        return &data[index];
    }

    __attribute__((always_inline)) void Insert(UnkCallbackSet_7101a20950&& value, u32 index) {
        u32 cap = capacity;
        u32 n = count;
        UnkCallbackSet_7101a20950* p;
        if (n == index) {
            u32 new_capacity = cap;
            if (cap <= n && step != 0)
                new_capacity = (index / step + 1) * step;
            if ((s32)new_capacity <= (s32)index)
                for (;;) {
                }
            if (cap < new_capacity) {
                p = MoveTo(new_capacity);
                index = count;
                data = p;
                capacity = new_capacity;
            } else {
                p = data;
            }
        } else {
            if (cap <= n && step != 0) {
                u32 grown = (n / step + 1) * step;
                if (cap < grown) {
                    UnkCallbackSet_7101a20950* q = MoveTo(grown);
                    data = q;
                    capacity = grown;
                    n = count;
                }
            }
            for (u32 i = n - 1; i >= index; --i) {
                UnkCallbackSet_7101a20950* q = &data[i];
                new (q + 1) UnkCallbackSet_7101a20950(static_cast<UnkCallbackSet_7101a20950&&>(*q));
                data[i].vf0();
                if (i == 0)
                    break;
            }
            p = data;
        }
        new (&p[index]) UnkCallbackSet_7101a20950(static_cast<UnkCallbackSet_7101a20950&&>(value));
        ++count;
    }
};

void fn_71002470ec(UnkSetArray* self, UnkCallbackSet_7101a20950* value, u32 index) {
    self->Insert(static_cast<UnkCallbackSet_7101a20950&&>(*value), index);
}

UnkCallbackSet_7101a20950* fn_7100247340(UnkSetArray* self, u32 index) { return self->GrowAt(index); }

struct UnkEntry_b41a1;

// Inserts a copy of `value` at `index` (appending when the index is the count).
void fn_7100246eb8(UnkSetArray* self, const UnkCallbackSet_7101a20950* value, u32 index) {
    u32 n = self->count;
    if (n == index) {
        fn_7100246ba8(reinterpret_cast<base::UnkVector<UnkEntry_b41a1>*>(self),
                      reinterpret_cast<const UnkEntry_b41a1*>(value));
        return;
    }
    u32 cap = self->capacity;
    if (cap <= n && self->step != 0) {
        u32 grown = (n / self->step + 1) * self->step;
        if (cap < grown) {
            UnkCallbackSet_7101a20950* q = self->MoveTo(grown);
            self->data = q;
            self->capacity = grown;
            n = self->count;
        }
    }
    for (u32 i = n - 1; i >= index; --i) {
        UnkCallbackSet_7101a20950* q = &self->data[i];
        new (q + 1) UnkCallbackSet_7101a20950(static_cast<UnkCallbackSet_7101a20950&&>(*q));
        self->data[i].vf0();
        if (i == 0)
            break;
    }
    new (&self->data[index]) UnkCallbackSet_7101a20950(*value);
    ++self->count;
}

using base::global::CRntString;

void fn_7100228ca0(base::reflection::UnkReflList<CRntString>* self, const CRntString* items, u64 size, u32 step,
                   base::UnkAllocator* allocator, u32 flags) {
    auto* v = reinterpret_cast<base::UnkVector<CRntString>*>(self);
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = step;
    self->field_0x14 = step != 0;
    self->flags = flags;
    self->allocator = allocator ? allocator : fn_7100083990();
    v->Reserve(size);
    for (const CRntString *it = items, *end = items + size; it != end; ++it) {
        CRntString copy(*it);
        v->PushBackCopy(copy);
    }
}
