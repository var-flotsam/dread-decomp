// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): moves an owned entry into the
// array at the given index, shifting the later ones up. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkTU2577.h"
#include "types.h"

struct UnkNamedIds;

struct UnkOwnedEntry_b190 {
    union {
        UnkNamedIds* ptr;
    };
    explicit UnkOwnedEntry_b190(UnkNamedIds* p) : ptr(p) {}
    UnkOwnedEntry_b190(UnkOwnedEntry_b190&& o) {
        UnkNamedIds* moved = o.ptr;
        o.ptr = nullptr;
        ptr = moved;
    }
    ~UnkOwnedEntry_b190() {
        UnkNamedIds* old = ptr;
        ptr = nullptr;
        if (old) {
            fn_7100ef1e1c(old);
            operator delete(old);
        }
    }
};

// Allocates new_capacity entries and moves the entries over, freeing the old array.
static inline UnkOwnedEntry_b190* MoveTo(base::global::CRntVector<UnkOwnedEntry_b190>* v, u32 new_capacity) {
    UnkOwnedEntry_b190* p = static_cast<UnkOwnedEntry_b190*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkOwnedEntry_b190), 8, v->flags));
    if (v->data) {
        UnkOwnedEntry_b190* src = v->data;
        UnkOwnedEntry_b190* dst = p;
        for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
            new (dst) UnkOwnedEntry_b190(static_cast<UnkOwnedEntry_b190&&>(*src));
            src->~UnkOwnedEntry_b190();
        }
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_7100ef1f24(base::global::CRntVector<UnkOwnedEntry_b190>* v, UnkOwnedEntry_b190&& value, u32 index) {
    using Entry = UnkOwnedEntry_b190;
    u32 cap = v->capacity;
    u32 n = v->count;
    Entry* p;
    if (n == index) {
        u32 new_capacity = cap;
        if (cap <= n && v->step != 0)
            new_capacity = v->GrownCapacity(index);
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
    } else {
        if (cap <= n && v->step != 0) {
            u32 grown = v->GrownCapacity(n);
            if (cap < grown) {
                Entry* q = MoveTo(v, grown);
                v->data = q;
                v->capacity = grown;
                n = v->count;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            Entry* q = &v->data[i];
            new (q + 1) Entry(static_cast<Entry&&>(*q));
            v->data[i].~Entry();
            if (i == 0)
                break;
        }
        p = v->data;
    }
    new (&p[index]) Entry(static_cast<Entry&&>(value));
    ++v->count;
}
