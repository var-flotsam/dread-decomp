// Unit 2573: appends an actor to the owner's target list at +0xf8, holding the actor's weak handle and
// the id its billboard collision component gives for a key (an invalid id when it has none).
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

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

void CSamusGunComponent_ReserveRefEntryVector(base::global::CRntVector<UnkTU2573TargetEntry>* entries, u32 capacity);
u32 fn_710086427c(UnkReflObject* collision, void* key);

void fn_7100ece654(UnkTU2573TargetList* self, UnkTU2573Actor* actor, UnkTU2573KeyRef* keyRef) {
    if (actor == nullptr)
        return;
    UnkTU2573HandleRef handle(fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(actor)));

    u32 id = kInvalidTargetId;
    UnkReflObject* collision = actor->collision;
    if (collision != nullptr && fn_710009d50c(collision->vf_0x0(), fn_7100723914())) {
        UnkTU2573Key* key = keyRef->key;
        id = fn_710086427c(collision, key ? key->field_0x10 : nullptr);
    }

    base::global::CRntVector<UnkTU2573TargetEntry>& v = self->targets;
    u32 n = v.count;
    u32 capacity = v.capacity;
    if (capacity <= n && v.step != 0)
        capacity = (n / v.step + 1) * v.step;
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    CSamusGunComponent_ReserveRefEntryVector(&v, capacity);
    new (&v.data[v.count]) UnkTU2573TargetEntry(handle, UnkTU2573TargetInfo(id));
    ++v.count;
}
