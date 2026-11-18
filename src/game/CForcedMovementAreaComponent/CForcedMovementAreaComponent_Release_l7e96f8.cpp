// CForcedMovementAreaComponent unit (tools/tu_map.py): a release step. Hands +0x30 to the object
// fn_710043acb4 finds for +0x18 (fn_71008659c4), takes the area's listener (+0x38) off every object the
// references in the buckets at +0x48 hold, unregisters the area from the game manager under the unit's
// id and runs the base step (fn_710024fad0). Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "game/CActorComponent.h"
#include "game/CChozoCommanderSentenceSphereAttack.h"
#include "game/CCollisionComponent.h"
#include "game/CForcedMovementAreaComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// As src/game/CCentralUnitComponent/CCentralUnitComponent_b41.cpp declares it.
struct UnkSelf_cu41;

struct UnkAreaListenersTag_l7e96f8;
using UnkAreaListeners_l7e96f8 = base::UnkSList<UnkAreaListenersTag_l7e96f8>;
using UnkAreaListenerNode_l7e96f8 = UnkAreaListeners_l7e96f8::Node;

static inline s32 IndexOf(UnkAreaListeners_l7e96f8* list, u64 value) {
    u32 i = 0;
    for (UnkAreaListenerNode_l7e96f8* n = list->head; n; n = n->next, ++i) {
        if (n->value == value)
            return static_cast<s32>(i);
    }
    return -1;
}

static inline UnkAreaListenerNode_l7e96f8* Find(UnkAreaListeners_l7e96f8* list, u64 value) {
    for (UnkAreaListenerNode_l7e96f8* n = list->head; n; n = n->next) {
        if (n->value == value)
            return n;
    }
    return nullptr;
}

static inline void Erase(UnkAreaListeners_l7e96f8* list, UnkAreaListenerNode_l7e96f8* node) {
    UnkAreaListenerNode_l7e96f8* prev = nullptr;
    UnkAreaListenerNode_l7e96f8* it = list->head;
    while (it) {
        UnkAreaListenerNode_l7e96f8* cur = it;
        UnkAreaListenerNode_l7e96f8* next = it->next;
        it = next;
        if (cur != node) {
            prev = cur;
            continue;
        }
        if (prev) {
            prev->next = next;
            if (list->tail == node)
                list->tail = prev;
        } else {
            list->head = next;
        }
        if (--list->count == 0) {
            list->head = nullptr;
            list->tail = nullptr;
        }
        list->allocator->vf_0x48(node);
        return;
    }
}

struct UnkAreaTargetOwner_l7e96f8 {
    u8 pad_0x0[0x128];
    UnkAreaListeners_l7e96f8 listeners;  // 0x128
};

struct UnkAreaTargetRef_l7e96f8 : UnkRefObject {
    UnkAreaTargetOwner_l7e96f8* owner;  // 0x10
};

// A counted reference held for the length of a scope.
struct UnkAreaTargetHold_l7e96f8 {
    UnkAreaTargetRef_l7e96f8* ref;

    explicit UnkAreaTargetHold_l7e96f8(UnkAreaTargetRef_l7e96f8* r) : ref(r) { ++ref->ref_count; }
    ~UnkAreaTargetHold_l7e96f8() {
        if (--ref->ref_count == 0)
            ref->vf_0x10();
    }
};

struct UnkAreaBucket_l7e96f8 {
    UnkAreaTargetRef_l7e96f8** refs;
    u32 count;
    u8 pad_0xc[0x1c];
};
static_assert(sizeof(UnkAreaBucket_l7e96f8) == 0x28, "layout");

struct UnkForcedMovementArea_l7e96f8 {
    u8 pad_0x0[0x18];
    void* field_0x18;
    u8 pad_0x20[0x10];
    u8 field_0x30[8];
    u8 listener[0x10];  // 0x38, the part registered with the targets
    UnkAreaBucket_l7e96f8* buckets;  // 0x48
    u8 pad_0x50[8];
    u32 bucket_count;  // 0x58
};
static_assert(__builtin_offsetof(UnkForcedMovementArea_l7e96f8, bucket_count) == 0x58, "layout");

void fn_71007e96f8(UnkForcedMovementArea_l7e96f8* self) {
    if (void* object = fn_710043acb4(self->field_0x18))
        fn_71008659c4(static_cast<UnkCollisionContainer*>(object), 0x10000, 0x10, self->field_0x30);
    UnkAreaBucket_l7e96f8* buckets_end = self->buckets + self->bucket_count;
    for (UnkAreaBucket_l7e96f8* bucket = self->buckets; bucket != buckets_end; ++bucket) {
        UnkAreaTargetRef_l7e96f8** refs_end = bucket->refs + bucket->count;
        for (UnkAreaTargetRef_l7e96f8** it = bucket->refs; it != refs_end; ++it) {
            if (!*it)
                continue;
            UnkAreaTargetHold_l7e96f8 target(*it);
            if (UnkAreaTargetOwner_l7e96f8* owner = target.ref->owner) {
                UnkAreaListeners_l7e96f8* list = &owner->listeners;
                u64 key = reinterpret_cast<u64>(self->listener);
                if (IndexOf(list, key) >= 0) {
                    if (UnkAreaListenerNode_l7e96f8* node = Find(list, key))
                        Erase(list, node);
                }
            }
        }
    }
    fn_7100de9730(g_unk_7101d47310, reinterpret_cast<UnkSpawnGroupKey*>(&g_unk_717271fdd8), self);
    fn_710024fad0(reinterpret_cast<UnkSelf_cu41*>(self));
}
