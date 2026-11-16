// Unit 2573: whether the owner's target list at +0xf8 already holds an actor (by its weak handle) with
// the id its billboard collision component gives for a key, in an entry still unclaimed.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

#include <stdlib.h>

using base::reflection::UnkReflObject;

static const u32 kInvalidTargetId = 0x80000000;

struct UnkTU2573Key {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkTU2573KeyRef {
    UnkTU2573Key* key;
};

struct UnkTU2573Actor {
    u8 pad_0x0[0x2a0];
    UnkReflObject* collision;  // 0x2a0
};

// A counted reference held for the length of a scope.
struct UnkTU2573HandleRef {
    UnkWeakHandle* p;
    UnkTU2573HandleRef(UnkWeakHandle* h) : p(h) {
        if (p)
            ++p->ref_count;
    }
    ~UnkTU2573HandleRef() {
        if (p && --p->ref_count == 0)
            p->vf_0x10();
    }
};

struct UnkTU2573TargetInfo {
    u32 id;
    u32 field_0x4;
    f32 field_0x8;

    UnkTU2573TargetInfo() = default;
    explicit UnkTU2573TargetInfo(u32 targetId)
        : id(targetId), field_0x4(kInvalidTargetId), field_0x8(3.40282347e+38f) {}
};

struct UnkTU2573TargetEntry {
    UnkWeakHandle* handle;
    UnkTU2573TargetInfo info;

    UnkTU2573TargetEntry(const UnkTU2573HandleRef& h, const UnkTU2573TargetInfo& i) : handle(nullptr) {
        AssignRef(&handle, h.p);
        info = i;
    }
};
static_assert(sizeof(UnkTU2573TargetEntry) == 0x18, "");

struct UnkTU2573TargetList {
    u8 pad_0x0[0xf8];
    base::global::CRntVector<UnkTU2573TargetEntry> targets;  // 0xf8
};

u32 fn_710086427c(UnkReflObject* collision, void* key);

// Ids match when equal, or when the stored one is the negated form of the wanted one.
static inline bool IsSameTargetId(s32 wanted, s32 stored) {
    return wanted == stored || wanted == abs(stored);
}

static inline UnkWeakTarget* HandleTarget(UnkWeakHandle* handle) {
    return handle ? handle->target : nullptr;
}

bool fn_7100ecf1ec(UnkTU2573TargetList* self, UnkTU2573Actor* actor, UnkTU2573KeyRef* keyRef) {
    UnkTU2573HandleRef handle(actor ? fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(actor)) : nullptr);

    u32 id = kInvalidTargetId;
    if (actor != nullptr) {
        UnkReflObject* collision = actor->collision;
        if (collision != nullptr && fn_710009d50c(collision->vf_0x0(), fn_7100723914())) {
            UnkTU2573Key* key = keyRef->key;
            id = fn_710086427c(collision, key ? key->field_0x10 : nullptr);
        }
    }

    base::global::CRntVector<UnkTU2573TargetEntry>& v = self->targets;
    for (UnkTU2573TargetEntry *entry = v.data, *end = v.data + v.count; entry != end; ++entry) {
        if (HandleTarget(handle.p) == HandleTarget(entry->handle) && IsSameTargetId(id, entry->info.id) &&
            entry->info.field_0x4 == kInvalidTargetId)
            return true;
    }
    return false;
}
