// Unit 280 (tools/tu_map.py): the scene manager's entry arrays (0x28-byte entries, src/engine/scene/CSceneManager.cpp)
// and their out-of-line operations. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkSceneEntry;

// An entry (0x28 bytes) copied field by field.
typedef u64 __attribute__((may_alias)) UnkAliasU64_ovb0254;
typedef u32 __attribute__((may_alias)) UnkAliasU32_ovb0254;
struct UnkEntry_ovb0254 {
    u64 field_0x0;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    bool field_0x24;

    UnkEntry_ovb0254() : field_0x0(0), field_0x8(-1), field_0xc(0), field_0x10(0), field_0x14(0), field_0x18(0), field_0x1c(0), field_0x20(0x3f800000), field_0x24(false) {}

    __attribute__((always_inline)) void Copy(const UnkEntry_ovb0254& o) {
        char* d = reinterpret_cast<char*>(this);
        u64 v0 = o.field_0x0;
        *reinterpret_cast<UnkAliasU32_ovb0254*>(d + 0x8) = o.field_0x8;
        *reinterpret_cast<UnkAliasU64_ovb0254*>(d + 0x0) = v0;
        *reinterpret_cast<UnkAliasU32_ovb0254*>(d + 0xc) = o.field_0xc;
        *reinterpret_cast<UnkAliasU32_ovb0254*>(d + 0x10) = o.field_0x10;
        *reinterpret_cast<UnkAliasU32_ovb0254*>(d + 0x14) = o.field_0x14;
        *reinterpret_cast<UnkAliasU32_ovb0254*>(d + 0x18) = o.field_0x18;
        *reinterpret_cast<UnkAliasU32_ovb0254*>(d + 0x1c) = o.field_0x1c;
        *reinterpret_cast<UnkAliasU32_ovb0254*>(d + 0x20) = o.field_0x20;
        *reinterpret_cast<u8*>(d + 0x24) = o.field_0x24;
    }
    UnkEntry_ovb0254(const UnkEntry_ovb0254& o) { Copy(o); }
    UnkEntry_ovb0254(UnkEntry_ovb0254&& o) { Copy(o); }
};
static_assert(sizeof(UnkEntry_ovb0254) == 0x28, "entry layout");

using EntryVec_ovb0254 = base::UnkVector<UnkEntry_ovb0254>;

void fn_71001fe390(base::UnkVector<UnkSceneEntry>* self_, const UnkSceneEntry* entry_) {
    EntryVec_ovb0254* self = reinterpret_cast<EntryVec_ovb0254*>(self_);
    const UnkEntry_ovb0254* entry = reinterpret_cast<const UnkEntry_ovb0254*>(entry_);
    u32 n = self->count;
    u32 old_capacity = self->capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    UnkEntry_ovb0254* p;
    if (old_capacity < new_capacity) {
        p = static_cast<UnkEntry_ovb0254*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkEntry_ovb0254), 8, self->flags));
        if (self->data) {
            UnkEntry_ovb0254* src = self->data;
            UnkEntry_ovb0254* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                asm("" : "+r"(dst));
                new (dst) UnkEntry_ovb0254(static_cast<UnkEntry_ovb0254&&>(*src));
            }
            self->allocator->vf_0x48(self->data);
        }
        n = self->count;
        self->data = p;
        self->capacity = new_capacity;
    } else {
        p = self->data;
    }
    UnkEntry_ovb0254* slot = &p[n];
    // keeps the element address computed before the copy's loads
    asm("" : "+r"(slot));
    new (slot) UnkEntry_ovb0254(*entry);
    ++self->count;
}

// Copy-assigns an entry array: reserve the source's capacity, then append its entries.
base::UnkVector<UnkSceneEntry>* fn_71001fdacc(base::UnkVector<UnkSceneEntry>* self_,
                                              const base::UnkVector<UnkSceneEntry>* other_) {
    EntryVec_ovb0254* self = reinterpret_cast<EntryVec_ovb0254*>(self_);
    const EntryVec_ovb0254* other = reinterpret_cast<const EntryVec_ovb0254*>(other_);
    if (self != other) {
        self->count = 0;
        u32 cap = other->capacity;
        if (self->capacity < cap) {
            UnkEntry_ovb0254* p = static_cast<UnkEntry_ovb0254*>(
                self->allocator->vf_0x40((u64)cap * sizeof(UnkEntry_ovb0254), 8, self->flags));
            if (self->data) {
                UnkEntry_ovb0254* src = self->data;
                UnkEntry_ovb0254* dst = p;
                for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                    asm("" : "+r"(dst));
                    new (dst) UnkEntry_ovb0254(static_cast<UnkEntry_ovb0254&&>(*src));
                }
                self->allocator->vf_0x48(self->data);
            }
            self->data = p;
            self->capacity = cap;
        }
        for (const UnkEntry_ovb0254 *it = other->data, *end = it + other->count; it != end; ++it)
            fn_71001fe390(self_, reinterpret_cast<const UnkSceneEntry*>(it));
    }
    return self_;
}

static inline void Reallocate_ovb0254(EntryVec_ovb0254* self, u32 new_capacity) {
    UnkEntry_ovb0254* p = static_cast<UnkEntry_ovb0254*>(
        self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkEntry_ovb0254), 8, self->flags));
    if (self->data) {
        UnkEntry_ovb0254* src = self->data;
        UnkEntry_ovb0254* dst = p;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            asm("" : "+r"(dst));
            new (dst) UnkEntry_ovb0254(static_cast<UnkEntry_ovb0254&&>(*src));
        }
        self->allocator->vf_0x48(self->data);
    }
    self->data = p;
    self->capacity = new_capacity;
}

// The entry at `index`; an index past the end first grows the array (to the next multiple of the step,
// then to exactly index + 1) and fills the new entries with defaults.
UnkSceneEntry* fn_71001fca5c(base::UnkVector<UnkSceneEntry>* self_, u32 index) {
    EntryVec_ovb0254* self = reinterpret_cast<EntryVec_ovb0254*>(self_);
    if (self->field_0x14 && self->count <= index) {
        u32 cap = self->capacity;
        if (cap <= index && self->step != 0) {
            u32 grown = (index / self->step + 1) * self->step;
            if (cap < grown) {
                Reallocate_ovb0254(self, grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            Reallocate_ovb0254(self, n);
        for (u32 i = self->count; i < n; ++i)
            new (self->data + i) UnkEntry_ovb0254();
        self->count = n;
    }
    return reinterpret_cast<UnkSceneEntry*>(&self->data[index]);
}
