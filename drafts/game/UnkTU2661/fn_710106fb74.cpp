// Unit 2661 (CCutscene::CTunableCutscene): moves a 0x30-byte polymorphic holder into an engine
// array at the given index, shifting the later ones up. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkTU2661.h"
#include "types.h"

// The holder's 35 bytes after its vtable, moved as one block.
struct __attribute__((packed)) UnkTunableBody_l06fb74 {
    u8 bytes[35];
};

// A 0x30-byte element with a virtual destructor (vtable g_unk_7101cac988) and plain data after it.
struct UnkTunableHolder_l06fb74 {
    virtual ~UnkTunableHolder_l06fb74();
    UnkTunableBody_l06fb74 body;  // 0x8


    UnkTunableHolder_l06fb74(UnkTunableHolder_l06fb74&& other) {
        *reinterpret_cast<char**>(this) = reinterpret_cast<char*>(g_unk_7101cac988) + 0x10;
        body = other.body;
    }
};
static_assert(sizeof(UnkTunableHolder_l06fb74) == 0x30, "holder size");
using Holder = UnkTunableHolder_l06fb74;
using UnkHolders_l06fb74 = base::global::CRntVector<Holder>;

// Allocates new_capacity holders and moves the holders over, freeing the old array.
static inline Holder* MoveTo(UnkHolders_l06fb74* v, u32 new_capacity) {
    Holder* p = static_cast<Holder*>(v->allocator->vf_0x40((u64)new_capacity * sizeof(Holder), 8, v->flags));
    if (v->data) {
        Holder* src = v->data;
        Holder* dst = p;
        for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
            new (dst) Holder(static_cast<Holder&&>(*src));
            src->~Holder();
        }
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_710106fb74(UnkHolders_l06fb74* v, Holder* value, u32 index) {
    u32 cap = v->capacity;
    u32 n = v->count;
    if (n == index) {
        Holder* p;
        u32 new_capacity = cap;
        if (cap <= n && v->step != 0)
            new_capacity = (index / v->step + 1) * v->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        if (cap < new_capacity) {
            p = MoveTo(v, new_capacity);
            index = v->count;
            v->data = p;
            v->capacity = new_capacity;
        } else {
            p = v->data;
        }
        new (&p[index]) Holder(static_cast<Holder&&>(*value));
    } else {
        if (cap <= n && v->step != 0) {
            u32 grown = (n / v->step + 1) * v->step;
            if (cap < grown) {
                Holder* q = MoveTo(v, grown);
                v->data = q;
                v->capacity = grown;
                n = v->count;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            Holder* q = &v->data[i];
            new (q + 1) Holder(static_cast<Holder&&>(*q));
            v->data[i].~Holder();
            if (i == 0)
                break;
        }
        new (&v->data[index]) Holder(static_cast<Holder&&>(*value));
    }
    ++v->count;
}
