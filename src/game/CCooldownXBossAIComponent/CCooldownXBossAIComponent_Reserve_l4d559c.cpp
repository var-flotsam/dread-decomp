// CCooldownXBossAIComponent's unit (tools/tu_map.py): grows a keyed array of 0x40-byte records (six
// string ids, a counted reference and a flag) and its string-id keys to hold at least `capacity`; each
// record is moved over and destroyed (fn_71004d0b00), each key moved and released.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCooldownXBossAIComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkCooldownXRecord_ovb0088;

// A counted reference: copying it takes a reference on the new object and drops the old one.
struct UnkCountedRef_l4d559c {
    UnkRefObject* object;

    UnkCountedRef_l4d559c() : object(nullptr) {}
    UnkCountedRef_l4d559c& operator=(const UnkCountedRef_l4d559c& other) {
        if (object != other.object) {
            UnkRefObject* taken = other.object;
            if (taken)
                ++taken->ref_count;
            if (UnkRefObject* old = object) {
                if (--old->ref_count == 0)
                    old->vf_0x10();
            }
            object = taken;
        }
        return *this;
    }
};

// A string id taken over from another, which is left with the empty id.
struct UnkMovedId_l4d559c {
    UnkStringData* data;

    UnkMovedId_l4d559c(UnkMovedId_l4d559c&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
};

struct UnkMovedIds_l4d559c {
    UnkMovedId_l4d559c ids[6];
};

struct UnkCooldownXRecord_l4d559c {
    UnkMovedIds_l4d559c names;
    UnkCountedRef_l4d559c ref;  // 0x30
    bool flag;                  // 0x38

    UnkCooldownXRecord_l4d559c(UnkCooldownXRecord_l4d559c&& other) : names(static_cast<UnkMovedIds_l4d559c&&>(other.names)) {
        ref = other.ref;
        flag = other.flag;
    }
};
static_assert(sizeof(UnkCooldownXRecord_l4d559c) == 0x40, "size");

using UnkCooldownXRecords_l4d559c = base::global::CRntSmallDictionary<CStrId, UnkCooldownXRecord_l4d559c>;

void fn_71004d559c(UnkCooldownXRecords_l4d559c* self, u32 capacity) {
    if (self->capacity < capacity) {
        UnkCooldownXRecord_l4d559c* values = static_cast<UnkCooldownXRecord_l4d559c*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkCooldownXRecord_l4d559c), 8, self->flags));
        CStrId* keys = static_cast<CStrId*>(self->allocator->vf_0x40((u64)capacity * sizeof(CStrId), 8, self->flags));
        if (self->values) {
            UnkCooldownXRecord_l4d559c* src = self->values;
            UnkCooldownXRecord_l4d559c* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkCooldownXRecord_l4d559c(static_cast<UnkCooldownXRecord_l4d559c&&>(*src));
                CCooldownXBossAIComponent_DestroyRecordRefAndIds(reinterpret_cast<UnkCooldownXRecord_ovb0088*>(src));
            }
            self->allocator->vf_0x48(self->values);
        }
        if (CStrId* old = self->keys) {
            CStrId* dst = keys;
            for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
                CStrId::MoveConstructReloading(dst, *old);
                old->Reset();
            }
            self->allocator->vf_0x48(self->keys);
        }
        self->values = values;
        self->keys = keys;
        self->capacity = capacity;
    }
}
