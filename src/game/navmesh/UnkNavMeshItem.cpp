// From unit 372 of tools/tu_map.py (0x71002b2e9c-0x71002b4934, no reflected class; it names
// "NAVMESHITEM" and "BreakableTile"): a navigation-mesh item, the record src/game_s2/fn_71002b2e9c.cpp
// default-constructs. Names and layouts are placeholders (the same offsets as that file's
// UnkRecord).
// Holds fn_71002b2f68 (the constructor from its values).
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

// An entry of the item's list (0x18 bytes): a key, a weak reference to what it names and a value.
struct UnkNavMeshItemEntry {
    CStrId key;
    UnkWeakHandle* handle;
    u32 value;

    UnkNavMeshItemEntry(const UnkNavMeshItemEntry& other) : key(other.key) {
        handle = nullptr;
        AssignRef(&handle, other.handle);
        value = other.value;
    }
};
static_assert(sizeof(UnkNavMeshItemEntry) == 0x18, "UnkNavMeshItemEntry layout");

// The list as the constructor gets it: the count, with the entries right after it (in the item the
// count is at +0x124 and the entries at +0x128).
struct UnkNavMeshItemEntries {
    u32 count;

    const UnkNavMeshItemEntry& At(u64 i) const {
        return reinterpret_cast<const UnkNavMeshItemEntry*>(reinterpret_cast<const char*>(this) + 4)[i];
    }
};

struct UnkNavMeshItemPair {
    u32 field_0x0;
    u32 field_0x4;
};

struct UnkNavMeshItemElem {
    u8 unk_0x0[8];
};

struct UnkNavMeshItem {
    u64 field_0x0;
    CStrId field_0x8;
    s32 field_0x10;
    u8 pad_0x14[4];
    u8 field_0x18[0xb0];  // built by fn_710002f4e4
    base::UnkVector<UnkNavMeshItemElem> field_0xc8;
    base::UnkVector<UnkNavMeshItemElem> field_0xf0;
    u32 field_0x118;
    u32 field_0x11c;
    u32 field_0x120;
    u32 count;  // +0x124
    UnkNavMeshItemEntry entries[8];  // +0x128
    u16 field_0x1e8;
    u8 field_0x1ea;
    u8 pad_0x1eb;
    u16 field_0x1ec;
    u8 field_0x1ee;
    u8 pad_0x1ef;
    u64 field_0x1f0;
    UnkNavMeshItem* field_0x1f8;  // the callback's object
    u32 (*field_0x200)(UnkNavMeshItem* self, void* event);
};
static_assert(__builtin_offsetof(UnkNavMeshItem, field_0x1e8) == 0x1e8, "UnkNavMeshItem placeholder layout");
static_assert(__builtin_offsetof(UnkNavMeshItem, field_0x200) == 0x200, "UnkNavMeshItem placeholder layout");

void fn_710002f4e4(void* self);
u32 fn_71002b44fc(UnkNavMeshItem* self, void* event);

// Builds the item from its values: the two vectors are empty (step 10, flags 0x57), the entries
// are copied from `entries`, and the event callback is fn_71002b44fc on the item.
// NON_MATCHING(minor): register allocation only: the entry copy loop's source and destination
// pointers are in swapped registers (x23/x26).
void fn_71002b2f68(UnkNavMeshItem* self, u64 field_0x0, const CStrId& id, s32 field_0x10, bool flag,
                   const UnkNavMeshItemEntries* entries, const UnkNavMeshItemPair* pair, u16 field_0x1ec,
                   u8 field_0x1ee) {
    self->field_0x0 = field_0x0;
    new (&self->field_0x8) CStrId(id);
    self->field_0x10 = field_0x10;
    fn_710002f4e4(self->field_0x18);
    new (&self->field_0xc8) base::UnkVector<UnkNavMeshItemElem>(0x57);
    new (&self->field_0xf0) base::UnkVector<UnkNavMeshItemElem>(0x57);
    self->field_0x118 = pair->field_0x0;
    self->field_0x11c = pair->field_0x4;
    self->field_0x120 = flag;
    for (u64 i = 0; i < entries->count; ++i)
        new (&self->entries[i]) UnkNavMeshItemEntry(entries->At(i));
    self->count = entries->count;
    self->field_0x1e8 = 0;
    self->field_0x1ea = 0;
    self->field_0x1f0 = 0;
    self->field_0x1f8 = self;
    self->field_0x200 = fn_71002b44fc;
    self->field_0x1ec = field_0x1ec;
    self->field_0x1ee = field_0x1ee;
}
