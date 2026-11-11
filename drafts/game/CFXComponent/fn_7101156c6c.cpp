// CFXComponent unit: erases one entry of a two-array container (entries plus their string ids)
// by moving the last entry into its slot. Names and layouts are placeholders.

#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkFXAllocator {
    virtual void vf_0x0() = 0;
    virtual void vf_0x8() = 0;
    virtual void vf_0x10() = 0;
    virtual void vf_0x18() = 0;
    virtual void vf_0x20() = 0;
    virtual void vf_0x28() = 0;
    virtual void vf_0x30() = 0;
    virtual void vf_0x38() = 0;
    virtual void vf_0x40() = 0;
    virtual void vf_0x48(void* ptr) = 0;  // free
};

struct UnkFXItem {
    u64 field_0x0;
    CStrId id;
};

struct UnkFXEntry {
    UnkFXItem* items;
    u32 count;
    u32 capacity;
    u32 field_0x10;
    u8 field_0x14;
    u32 field_0x18;
    UnkFXAllocator* allocator;

    UnkFXEntry(UnkFXEntry&& other)
        : items(other.items), count(other.count), capacity(other.capacity),
          field_0x10(other.field_0x10), field_0x14(other.field_0x14),
          field_0x18(other.field_0x18), allocator(other.allocator) {
        other.items = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    ~UnkFXEntry() {
        if (items) {
            for (u64 i = 0; i < count; ++i)
                items[i].id.Reset();
            count = 0;
            allocator->vf_0x48(items);
            capacity = 0;
        }
    }
};
static_assert(sizeof(UnkFXEntry) == 0x28, "");

struct UnkFXEntryList {
    UnkFXEntry* entries;
    CStrId* ids;
    u32 count;
};

UnkFXEntry* fn_7101156c6c(UnkFXEntryList* self, UnkFXEntry* it) {
    u32 index = it - self->entries;
    self->entries[index].~UnkFXEntry();
    self->ids[index].Reset();
    if (self->count - 1 != index) {
        UnkFXEntry& src = self->entries[self->count - 1];
        new (&self->entries[index]) UnkFXEntry(static_cast<UnkFXEntry&&>(src));
        self->entries[self->count - 1].~UnkFXEntry();
        CStrId* ids = self->ids;
        ids[index].data = ids[self->count - 1].data;
        ids[self->count - 1].data = &g_unk_7101d47108->empty;
        self->ids[self->count - 1].Reset();
    }
    self->count--;
    return &self->entries[index];
}
