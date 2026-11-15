// Dropped-item manager's unit (2562): grows the paired array of 0x78-byte entries and their 8-byte keys
// to hold at least `capacity`; each entry is moved and its three id pairs released. Names and layouts
// are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkTU32.h"
#include "types.h"

typedef u64 __attribute__((may_alias)) UnkAliasWord_le2fa5c;

struct UnkIdPairs_le2fa5c {
    u8 pair0[0x18];
    u8 pair1[0x18];
    u8 pair2[0x18];

    ~UnkIdPairs_le2fa5c() {
        fn_7100026f70(reinterpret_cast<UnkIdPair*>(pair2));
        fn_7100026f70(reinterpret_cast<UnkIdPair*>(pair1));
        fn_7100026f70(reinterpret_cast<UnkIdPair*>(pair0));
    }
};

struct UnkEntry_le2fa5c {
    u8 pad_0x0[0x30];
    UnkIdPairs_le2fa5c pairs;  // 0x30
};
static_assert(sizeof(UnkEntry_le2fa5c) == 0x78, "size");

struct UnkEntryArray_le2fa5c {
    UnkEntry_le2fa5c* values;
    UnkAliasWord_le2fa5c* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100e2f4bc(UnkEntry_le2fa5c* dst, UnkEntry_le2fa5c* src);

void fn_7100e2fa5c(UnkEntryArray_le2fa5c* self, u32 capacity) {
    if (self->capacity < capacity) {
        UnkEntry_le2fa5c* values =
            static_cast<UnkEntry_le2fa5c*>(self->allocator->vf_0x40((u64)capacity * sizeof(UnkEntry_le2fa5c), 8, self->flags));
        UnkAliasWord_le2fa5c* keys =
            static_cast<UnkAliasWord_le2fa5c*>(self->allocator->vf_0x40((u64)capacity * sizeof(u64), 8, self->flags));
        if (self->values) {
            UnkEntry_le2fa5c* src = self->values;
            UnkEntry_le2fa5c* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                fn_7100e2f4bc(dst, src);
                src->pairs.~UnkIdPairs_le2fa5c();
            }
            self->allocator->vf_0x48(self->values);
        }
        if (UnkAliasWord_le2fa5c* old = self->keys) {
            u64 j = 0;
            for (u32 i = 0; i < self->count; ++i, ++j)
                keys[j] = old[j];
            self->allocator->vf_0x48(self->keys);
        }
        self->values = values;
        self->keys = keys;
        self->capacity = capacity;
    }
}
