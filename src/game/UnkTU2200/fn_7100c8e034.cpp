// Unit 2200 (CCutSceneDef neighbourhood): fn_7100c8e034 is the move assignment of a 0xa0-byte
// cut-scene entry: each string id is released and takes the other's, and both tables take over the
// other's arrays. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkKeyedStrIds.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2200.h"
#include "types.h"

using base::global::UnkStringPool;

struct UnkCutSceneTable_lc886c0;

// Releases `self`'s id, then takes `other`'s, leaving `other` the pool's empty id.
template <UnkStringPool*& Pool>
inline void TakeId_lc8e034(base::UnkStrIdMemberOf<Pool>& self, base::UnkStrIdMemberOf<Pool>& other) {
    self.~UnkStrIdMemberOf();
    self.data = other.data;
    other.data = &Pool->empty;
}

// The keyed string-id table at +0x28, with its move assignment.
struct UnkKeyedIdTable_laed78c : UnkKeyedStrIds {
    // Releases every id; the arrays are kept.
    void Clear() {
        if (ids) {
            for (u32 i = 0; i < count; ++i)
                ids[i].~UnkStrIdMemberOf();
        }
        count = 0;
    }

    // Takes over other's arrays, leaving it empty.
    UnkKeyedIdTable_laed78c& operator=(UnkKeyedIdTable_laed78c&& other) {
        Clear();
        allocator = other.allocator;
        ids = other.ids;
        keys = other.keys;
        capacity = other.capacity;
        count = other.count;
        field_0x1c = other.field_0x1c;
        field_0x18 = other.field_0x18;
        other.count = 0;
        other.keys = nullptr;
        other.ids = nullptr;
        return *this;
    }
};
static_assert(sizeof(UnkKeyedIdTable_laed78c) == 0x28, "UnkKeyedIdTable_laed78c layout");

// The entry's second table: cleared by fn_7100c88920, then takes over the other's arrays.
struct UnkCutSceneTable_lc8e034 {
    void* field_0x0;
    void* field_0x8;
    u32 count;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    base::UnkAllocator* allocator;

    UnkCutSceneTable_lc8e034& operator=(UnkCutSceneTable_lc8e034&& other) {
        fn_7100c88920(reinterpret_cast<UnkCutSceneTable_lc886c0*>(this));
        allocator = other.allocator;
        field_0x8 = other.field_0x8;
        field_0x0 = other.field_0x0;
        field_0x14 = other.field_0x14;
        count = other.count;
        field_0x1c = other.field_0x1c;
        field_0x18 = other.field_0x18;
        other.count = 0;
        other.field_0x0 = nullptr;
        other.field_0x8 = nullptr;
        return *this;
    }
};
static_assert(sizeof(UnkCutSceneTable_lc8e034) == 0x28, "layout");

using UnkPathIdMember_lc8e034 = base::UnkStrIdMemberOf<g_unk_7101d47200>;

struct UnkCutSceneEntry_lc8e034 {
    base::UnkStrIdMember field_0x0;
    UnkPathIdMember_lc8e034 field_0x8;
    base::UnkStrIdMember field_0x10;
    base::UnkStrIdMember field_0x18;
    u16 field_0x20;
    u32 field_0x24;
    UnkKeyedIdTable_laed78c field_0x28;
    u64 field_0x50;
    base::UnkStrIdMember field_0x58;
    base::UnkStrIdMember field_0x60;
    u32 field_0x68;
    UnkCutSceneTable_lc8e034 field_0x70;
    UnkPathIdMember_lc8e034 field_0x98;

};
static_assert(__builtin_offsetof(UnkCutSceneEntry_lc8e034, field_0x70) == 0x70, "layout");
static_assert(sizeof(UnkCutSceneEntry_lc8e034) == 0xa0, "layout");

void* fn_7100c8e034(void* dst, const void* src) {
    auto* self = static_cast<UnkCutSceneEntry_lc8e034*>(dst);
    auto* other = const_cast<UnkCutSceneEntry_lc8e034*>(static_cast<const UnkCutSceneEntry_lc8e034*>(src));
    TakeId_lc8e034(self->field_0x0, other->field_0x0);
    TakeId_lc8e034(self->field_0x8, other->field_0x8);
    TakeId_lc8e034(self->field_0x10, other->field_0x10);
    TakeId_lc8e034(self->field_0x18, other->field_0x18);
    self->field_0x20 = other->field_0x20;
    self->field_0x24 = other->field_0x24;
    self->field_0x28 = static_cast<UnkKeyedIdTable_laed78c&&>(other->field_0x28);
    self->field_0x50 = other->field_0x50;
    TakeId_lc8e034(self->field_0x58, other->field_0x58);
    TakeId_lc8e034(self->field_0x60, other->field_0x60);
    self->field_0x68 = other->field_0x68;
    self->field_0x70 = static_cast<UnkCutSceneTable_lc8e034&&>(other->field_0x70);
    TakeId_lc8e034(self->field_0x98, other->field_0x98);
    return self;
}
