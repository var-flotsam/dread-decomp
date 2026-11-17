// Unit 936 (tools/tu_map.py): the entry at an index of an array of 0x20-byte polymorphic entries (a vtable, a
// string id and two words), growing the array up to it with default entries: CRntVector::GrowAt written out.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU936.h"
#include "types.h"

// A vtable, a counted string id and two words. Slot 1 of the vtable destroys it.
struct UnkEntry_l61d2c8 {
    void (**vtable)(UnkEntry_l61d2c8*);
    base::global::UnkStringData* id;
    struct Words {
        u64 field_0x0;
        u64 field_0x8;
    } words;

    UnkEntry_l61d2c8(const UnkEntry_l61d2c8& other) {
        vtable = reinterpret_cast<void (**)(UnkEntry_l61d2c8*)>(g_unk_7101a56178 + 0x10);
        id = other.id;
        if (&g_unk_7101d47108->empty != id)
            ++id->ref_count;
        words = other.words;
    }
    ~UnkEntry_l61d2c8() { vtable[1](this); }
};

// The default constructor, out of line.
void fn_71006107bc(UnkEntry_l61d2c8* entry);

using UnkEntries_l61d2c8 = base::global::CRntVector<UnkEntry_l61d2c8>;

// Copies the entries into a new array of `n` and frees the old one.
static inline void Reallocate(UnkEntries_l61d2c8* self, u32 n) {
    auto* p = static_cast<UnkEntry_l61d2c8*>(self->allocator->vf_0x40((u64)n * sizeof(UnkEntry_l61d2c8), 8, self->flags));
    if (self->data) {
        UnkEntry_l61d2c8* src = self->data;
        UnkEntry_l61d2c8* dst = p;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkEntry_l61d2c8(*src);
            src->~UnkEntry_l61d2c8();
        }
        self->allocator->vf_0x48(self->data);
    }
    self->data = p;
    self->capacity = n;
}

UnkEntry_l61d2c8* fn_710061d2c8(UnkEntries_l61d2c8* self, u32 index) {
    if (self->field_0x14 && self->count <= index) {
        // First to the next multiple of the step, then to exactly index + 1 entries.
        u32 cap = self->capacity;
        if (cap <= index && self->step != 0) {
            u32 grown = (index / self->step + 1) * self->step;
            if (cap < grown) {
                Reallocate(self, grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            Reallocate(self, n);
        for (u32 i = self->count; i < n; ++i)
            fn_71006107bc(&self->data[i]);
        self->count = n;
    }
    return &self->data[index];
}
