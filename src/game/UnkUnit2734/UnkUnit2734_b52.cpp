// Unit 2734 (tools/tu_map.py; the sub-area GUI cache, game/UnkSubAreaGuiCache.h): marking entries by id
// and sweeping the marked ones. The cache keeps its nodes in a hash table on the id (+0x10) and in a
// list (+0x40); each node's entry (+0x8) names an actor definition the factory (g_unk_71726a0220)
// resolves. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "game/UnkSubAreaGuiCache.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"
#include "UnkLeftoverReg.h"

struct UnkActorDefFactory;
extern UnkActorDefFactory* g_unk_71726a0220;  // as src/game/UnkActorDefFactory.cpp declares it

struct UnkSweepEntry_b49;
// As src/game/UnkUnit2734/UnkUnit2734_Sweep_b49.cpp declares them: the first argument is unused.
struct UnkUnusedArg_b49 : UnkLeftoverReg {};
struct UnkSweepList_b49;
struct UnkSweepNode_b49;
bool fn_710119d5f0(UnkUnusedArg_b49, UnkSweepEntry_b49* entry, bool flag);
UnkSweepNode_b49* fn_710119d7e4(UnkSweepList_b49* self, UnkSweepNode_b49* node);

struct UnkGuiDef_b52 {
    u8 pad_0x0[0x54];
    u32 field_0x54;
    u8 pad_0x58[0x18];
    base::UnkStrIdMember* children;  // 0x70
    u32 child_count;                 // 0x78
};
UnkGuiDef_b52* fn_7101193e88(UnkActorDefFactory* factory, const void* key);

struct UnkGuiNode_b52 {
    base::global::UnkStringData* key;
    u8 entry[0x10];  // 0x8
    u32 field_0x18;
    u8 pad_0x1c[0x24];
    UnkGuiNode_b52* list_next;   // 0x40
    u8 pad_0x48[0x8];
    UnkGuiNode_b52* chain_next;  // 0x50
};

struct UnkGuiCacheView_b52 {
    u8 pad_0x0[0xc];
    u32 field_0xc;
    UnkGuiNode_b52** buckets;  // 0x10
    u32 count;
    u32 divisor;
    u8 pad_0x20[0x20];
    UnkGuiNode_b52* head;  // 0x40

    UnkGuiNode_b52** BucketFor(base::global::UnkStringData* key) {
        if (count == 0)
            return nullptr;
        const u32* hash = reinterpret_cast<const u32*>(reinterpret_cast<const u8*>(key) + 0x30);
        u32 idx = (hash[0] ^ hash[1]) % divisor;
        return buckets + idx;
    }
};

static UnkSweepNode_b49* Drop(UnkGuiCacheView_b52* self, UnkGuiNode_b52* node) {
    return fn_710119d7e4(reinterpret_cast<UnkSweepList_b49*>(self), reinterpret_cast<UnkSweepNode_b49*>(node));
}

// Marks the entry under `id` (and, with `recurse`, the definitions its definition lists) and drops it
// when fn_710119d5f0 accepts it. False when there is no entry under `id`.
bool fn_710119d9b8(UnkSubAreaGuiCache* cache, const base::UnkStrIdMember* id, bool recurse) {
    auto* self = reinterpret_cast<UnkGuiCacheView_b52*>(cache);
    UnkGuiNode_b52** bucket = self->BucketFor(id->data);
    if (!bucket)
        return false;
    UnkGuiNode_b52* node = *bucket;
    for (; node; node = node->chain_next)
        if (node->key == id->data)
            break;
    if (!node)
        return false;
    u8* entry = node->entry;
    UnkGuiDef_b52* def = fn_7101193e88(g_unk_71726a0220, entry);
    if (recurse) {
        for (base::UnkStrIdMember *it = def->children, *end = it + def->child_count; it != end; ++it)
            fn_710119d9b8(cache, it, true);
    }
    if (fn_710119d5f0(UnkUnusedArg_b49(), reinterpret_cast<UnkSweepEntry_b49*>(entry), false))
        Drop(self, node);
    return true;
}

struct UnkGfxFrameSource;
extern UnkGfxFrameSource* g_unk_7101d47328;
struct UnkStruct_71002334fc;
void fn_71002334fc(UnkStruct_71002334fc*, bool);
struct UnkStruct_7100233684;
base::reflection::UnkReflObject* fn_7100233684(UnkStruct_7100233684* self);
void fn_710023272c(void* actor);

// An actor slot of a cached entry (0x18 bytes): the actor, the frame it was handed out in, in use.
struct UnkGuiActorSlot_b52 {
    void* actor;
    s32 frame;
    bool in_use;
    u8 pad_0xd[0xb];
};

struct UnkGuiSlotNode_b52 {
    base::global::UnkStringData* key;
    u8 pad_0x8[0x8];
    UnkGuiActorSlot_b52* slots;  // 0x10
    u32 slot_count;              // 0x18
    u8 pad_0x1c[0x1c];
    s32 in_use_count;            // 0x38
    u8 pad_0x3c[0x14];
    UnkGuiSlotNode_b52* chain_next;  // 0x50
};

struct UnkActorModelHolder_b52 {
    u8 pad_0x0[0x50];
    u8* model;  // 0x50
};

static inline UnkGuiActorSlot_b52* FindInUse(UnkGuiSlotNode_b52* node) {
    for (UnkGuiActorSlot_b52 *it = node->slots, *end = it + node->slot_count; it != end; ++it)
        if (it->in_use)
            return it;
    return nullptr;
}

// Gives back one in-use actor of the entry under `id`: the slot is freed and stamped with the current
// frame, and the actor is reset (fn_710023272c), shown (fn_71002334fc) and its model's +0x114 cleared
// when +0x11c is set. Null when there is none.
void* fn_710119db5c(UnkSubAreaGuiCache* cache, const base::UnkStrIdMember* id) {
    auto* self = reinterpret_cast<UnkGuiCacheView_b52*>(cache);
    UnkGuiNode_b52** bucket = self->BucketFor(id->data);
    if (!bucket)
        return nullptr;
    auto* node = reinterpret_cast<UnkGuiSlotNode_b52*>(*bucket);
    for (; node; node = node->chain_next)
        if (node->key == id->data)
            break;
    if (!node)
        return nullptr;
    s32 count = node->in_use_count;
    if (count <= 0)
        return nullptr;
    UnkGuiActorSlot_b52* slot = FindInUse(node);
    if (!slot)
        return nullptr;
    node->in_use_count = count - 1;
    void* actor = slot->actor;
    slot->frame = *reinterpret_cast<const s32*>(reinterpret_cast<const u8*>(g_unk_7101d47328) + 0x2c);
    slot->in_use = false;
    fn_710023272c(actor);
    fn_71002334fc(static_cast<UnkStruct_71002334fc*>(actor), true);
    auto* holder = reinterpret_cast<UnkActorModelHolder_b52*>(fn_7100233684(static_cast<UnkStruct_7100233684*>(actor)));
    if (holder && holder->model && holder->model[0x11c])
        holder->model[0x114] = 0;
    return actor;
}
