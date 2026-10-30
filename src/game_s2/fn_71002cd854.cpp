// Grows an array of 0x48-byte elements to hold at least `capacity` of them, moving each element over
// (an array member, a string id and three words) and destroying the old one.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkList {
    base::UnkVector<void*> list;
    UnkList(const UnkList& other) asm("fn_71002cbc20");  // fn_71002cbc20
    ~UnkList() { list.Free(); }
};

struct UnkElement {
    UnkList field_0x0;
    // Released by hand: the destructor decrements and tests in two steps, like CStrId's move.
    union {
        CStrId field_0x28;
    };
    u64 field_0x30[3];

    UnkElement(UnkElement&& other) : field_0x0(other.field_0x0), field_0x28(other.field_0x28) {
        __builtin_memcpy(field_0x30, other.field_0x30, sizeof(field_0x30));
    }
    ~UnkElement() {
        base::global::UnkStringPool* pool = g_unk_7101d47108;
        if (pool) {
            base::global::UnkStringData* data = field_0x28.data;
            if (data && data != &pool->empty) {
                --field_0x28.data->ref_count;
                if (field_0x28.data->ref_count == 0) {
                    fn_71000803a4(pool, field_0x28.data);
                    pool = g_unk_7101d47108;
                }
            }
            field_0x28.data = &pool->empty;
        }
    }
};

// The same growth as UnkVector::Reserve, written out: the original inlines all of it.
void fn_71002cd854(base::UnkVector<UnkElement>* self, u32 capacity) {
    if (self->capacity < capacity) {
        UnkElement* fresh = static_cast<UnkElement*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkElement), 8, self->flags));
        UnkElement* dst = fresh;
        if (self->data) {
            UnkElement* src = self->data;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkElement(static_cast<UnkElement&&>(*src));
                src->~UnkElement();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = fresh;
        self->capacity = capacity;
    }
}
