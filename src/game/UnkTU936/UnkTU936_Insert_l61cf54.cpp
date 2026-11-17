// Unit 936 (tools/tu_map.py): inserts a copy of a 0x20-byte polymorphic entry (a vtable, a string id and two
// words) at an index of an array of them, moving the later entries up one place; inserting at the end is an
// inlined push-back. Either way the array grows first when it is full. Names and layouts are placeholders.
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

using UnkEntries_l61cf54 = base::global::CRntVector<UnkEntry_l61d2c8>;

// Copies the entries into a new array of `capacity` and frees the old one; returns the new array.
static inline UnkEntry_l61d2c8* MoveTo(UnkEntries_l61cf54* self, u32 capacity) {
    UnkEntry_l61d2c8* p = static_cast<UnkEntry_l61d2c8*>(self->allocator->vf_0x40((u64)capacity * sizeof(UnkEntry_l61d2c8), 8, self->flags));
    if (self->data) {
        UnkEntry_l61d2c8* src = self->data;
        UnkEntry_l61d2c8* dst = p;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkEntry_l61d2c8(*src);
            src->~UnkEntry_l61d2c8();
        }
        self->allocator->vf_0x48(self->data);
    }
    return p;
}

void fn_710061cf54(UnkEntries_l61cf54* self, const UnkEntry_l61d2c8* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        // Appending: the push-back, inlined.
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = self->GrownCapacity(index);
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        if (cap < new_capacity) {
            UnkEntry_l61d2c8* p = MoveTo(self, new_capacity);
            index = self->count;
            self->data = p;
            self->capacity = new_capacity;
        }
        new (&self->data[index]) UnkEntry_l61d2c8(*value);
        ++self->count;
        return;
    }
    if (cap <= n && self->step != 0) {
        u32 grown = self->GrownCapacity(n);
        if (cap < grown) {
            UnkEntry_l61d2c8* p = MoveTo(self, grown);
            n = self->count;
            self->data = p;
            self->capacity = grown;
        }
    }
    // The entries from the last down to the index move up one place (the index runs through -1 when it is 0).
    for (u32 i = n - 1; i >= index; --i) {
        UnkEntry_l61d2c8* from = &self->data[i];
        new (from + 1) UnkEntry_l61d2c8(*from);
        self->data[i].~UnkEntry_l61d2c8();
        if (i == 0)
            break;
    }
    new (&self->data[index]) UnkEntry_l61d2c8(*value);
    ++self->count;
}
