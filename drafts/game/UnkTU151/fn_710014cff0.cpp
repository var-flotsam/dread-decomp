// Unit 151 (tools/tu_map.py): looks up the entry with a string id (and a kind, unless the kind is
// 0x7fffffff, "any") in the array at +0x7cc and copies its values out. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CStrId.h"
#include "game/UnkTU151.h"
#include "types.h"

struct __attribute__((packed)) UnkEntry_b0249 {
    base::UnkStrIdMember id;  // 0x0
    union {
        void* owned;  // 0x8
        f32* values;
    };
    u32 count;      // 0x10
    s32 kind;       // 0x14
    u64 location;   // 0x18
};
static_assert(sizeof(UnkEntry_b0249) == 0x20, "entry layout");

struct __attribute__((packed)) UnkEntryArray_b0249 {
    u32 count;
    UnkEntry_b0249 items[1];
};

void fn_710014d838(UnkEntryArray_b0249* self, u32 count);

// The entry at `index`, the array grown to hold it first.
static inline UnkEntry_b0249& At(UnkEntryArray_b0249* entries, u64 index) {
    if (index >= entries->count)
        fn_710014d838(entries, index + 1);
    return entries->items[index];
}

struct UnkSetupData_l14cff0 {
    u8 pad_0x0[0x7cc];
    UnkEntryArray_b0249 entries;  // 0x7cc
};

// The kind that matches any entry.
static const s32 kAnyKind = 0x7fffffff;

// The index of the entry with `key` (and `kind`, unless it is kAnyKind), or -1.
static inline u32 FindEntry(UnkEntryArray_b0249* entries, const base::UnkStrIdMember& key, s32 kind) {
    u64 count = entries->count;
    if (count == 0)
        return -1;
    if (kind == kAnyKind) {
        for (u64 i = 0; i < count; ++i) {
            if (At(entries, i).id == key)
                return i;
        }
    } else {
        for (u64 i = 0; i < count; ++i) {
            UnkEntry_b0249& entry = At(entries, i);
            if (entry.id == key && entry.kind == kind)
                return i;
        }
    }
    return -1;
}

bool fn_710014cff0(UnkSetupSub_b0237* sub, void* a, void* b, u32 c) {
    UnkEntryArray_b0249* entries = &reinterpret_cast<UnkSetupData_l14cff0*>(sub)->entries;
    u32 i = FindEntry(entries, *static_cast<const base::UnkStrIdMember*>(a), c);
    if (i >= entries->count)
        return false;

    UnkEntry_b0249& entry = entries->items[i];
    __builtin_memcpy(b, entry.values, entry.count * sizeof(f32));
    return true;
}
