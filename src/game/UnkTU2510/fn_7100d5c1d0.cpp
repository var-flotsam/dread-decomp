// Unit 2510 (collision world): one collider against every collider of a sub-tree's plain list, keeping those
// whose layers and bounds overlap it and adding their contact results. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/UnkVisitResult_b0040.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "game/logic/collision/UnkColliderResult.h"
#include "types.h"

using base::reflection::UnkReflObject;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

static inline void InitSkip() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
}

static inline bool Overlaps(const UnkBvhCollider* c, const UnkBox* box) {
    return c->bounds.max.x >= box->min.x && c->bounds.min.x <= box->max.x && c->bounds.max.y >= box->min.y &&
           c->bounds.min.y <= box->max.y;
}

// The colliders of the list, one by one: those on the mask's layers whose bounds overlap `box` are tested
// against `collider`. Stops early once a test asks to.
UnkFlags UnkSubTree::Query(UnkFlags flags, UnkBvhCollider* collider, u32 arg2, u32 mask, const UnkBox* box,
                           void* arg) {
    UnkFlags result(flags);
    UnkBvhEntry** end = this->end();
    for (UnkBvhEntry** it = begin(); !(result.bits & 2) && it != end; ++it) {
        UnkBvhEntry* entry = *it;
        UnkBvhCollider* other = entry ? entry->collider : nullptr;
        if (!other || !other->enabled)
            continue;
        if (!(other->GetLayers() & mask) || other == collider)
            continue;
        if (other->boundsDirty)
            other->UpdateBounds();
        if (!Overlaps(other, box))
            continue;
        InitSkip();
        result.bits |= fn_7100d5761c(reinterpret_cast<UnkReflObject*>(collider), reinterpret_cast<UnkReflObject*>(other),
                                     collider->GetLayers(), mask, arg, arg2).bits;
    }
    return result;
}
