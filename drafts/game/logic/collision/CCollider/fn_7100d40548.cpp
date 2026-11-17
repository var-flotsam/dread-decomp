// game::logic::collision::CCollider unit: copy constructor of a growable array of reference-counted
// pointers (base::global::CRntVector's layout): takes the source's settings, reserves its capacity
// and appends a counted copy of each element, growing like PushBack (including its hang).
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

struct UnkRefObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        s32 ref_count;
    };
};

struct UnkRefPtr {
    UnkRefObject* ptr = nullptr;

    UnkRefPtr() = default;
    UnkRefPtr(const UnkRefPtr& other) { *this = other; }
    UnkRefPtr& operator=(const UnkRefPtr& other) {
        UnkRefObject* p = other.ptr;
        if (p)
            ++p->ref_count;
        if (UnkRefObject* old = ptr) {
            if (--old->ref_count == 0)
                old->vf_0x10();
        }
        ptr = p;
        return *this;
    }
    ~UnkRefPtr() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

template <class Tag>
using UnkSlotArray = base::global::CRntVector<UnkRefPtr>;

struct UnkTag_71007ed25c;
void fn_71007ed25c(UnkSlotArray<UnkTag_71007ed25c>* self, u32 capacity);

void fn_7100d40548(UnkSlotArray<UnkTag_71007ed25c>* self, const UnkSlotArray<UnkTag_71007ed25c>& other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other.step;
    self->field_0x14 = other.field_0x14;
    self->flags = other.flags;
    self->allocator = other.allocator;
    fn_71007ed25c(self, other.capacity);
    for (const UnkRefPtr *it = other.data, *end = it + other.count; it != end; ++it) {
        u32 n = self->count;
        u32 new_capacity = self->capacity;
        if (new_capacity <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        fn_71007ed25c(self, new_capacity);
        new (&self->data[self->count]) UnkRefPtr(*it);
        ++self->count;
    }
}
