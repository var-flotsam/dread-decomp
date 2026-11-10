// Unit 2734 (tools/tu_map.py; the sub-area GUI cache, game/UnkSubAreaGuiCache.h): the cache keeps its
// nodes in a hash table on the key (+0x10) and in a list (+0x40); a node's entry (+0x8) holds the
// actor slots of one actor definition. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CFilePathStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkSubAreaGuiCache.h"
#include <stdio.h>

#include "types.h"
#include "UnkLeftoverReg.h"

using base::global::CStrId;

struct UnkActorDefFactory;
extern UnkActorDefFactory* g_unk_71726a0220;

struct UnkGuiDef_b52;
UnkGuiDef_b52* fn_7101193e88(UnkActorDefFactory* factory, const void* key);

struct UnkRemovedValue_b60;
void fn_7100debc28(UnkGameManager*, UnkRemovedValue_b60*);
struct UnkBoundObj;
void fn_7100232124(UnkBoundObj* obj);

// The polymorphic object at +0x28 of an actor.
struct UnkGuiOwned_b0231 {
    virtual const base::reflection::CType* vf_0x0();
    u8 pad_0x8[0x90];
    base::global::UnkStringData* key;  // 0x98
    u8 pad_0xa0[0x28];
    bool field_0xc8;
};

struct UnkGuiActor_b0231 {
    u8 pad_0x0[0x28];
    UnkGuiOwned_b0231* owned;  // 0x28
    u8 pad_0x30[0x68];
    u8 key[0x8];               // 0x98 (read as a pointer below)
};

struct UnkGuiActorKey_b0231 {
    u8 pad_0x0[0x30];
    u32 hash_a;
    u32 hash_b;
};

struct UnkGuiActorObj_b0231 {
    u8 pad_0x0[0x28];
    UnkGuiOwned_b0231* owned;  // 0x28
    u8 pad_0x30[0x68];
    UnkGuiActorKey_b0231* key;  // 0x98
    u8 pad_0xa0[0x115];
    bool field_0x1b5;
};

struct UnkGuiSlot_b0231 {
    UnkGuiActorObj_b0231* actor;
    u8 pad_0x8[4];
    bool in_use;  // 0xc
    u8 pad_0xd[3];
    CStrId id;  // 0x10
};
static_assert(sizeof(UnkGuiSlot_b0231) == 0x18, "slot");

// A cache entry (a node's +0x8).
struct UnkGuiEntry_b0231 {
    u8 pad_0x0[0x8];
    UnkGuiSlot_b0231* slots;  // 0x8
    u32 count;                // 0x10
};

struct UnkGuiDef_b0231 {
    u8 pad_0x0[0x54];
    u32 field_0x54;
    u8 pad_0x58[0x18];
    base::UnkStrIdMember* children;  // 0x70
    u32 child_count;                 // 0x78
};

struct UnkGuiNode_b0231 {
    CStrId key;                        // 0x0 (pool entry; its +0x30 holds the hash)
    CStrId entry_id;                   // 0x8
    UnkGuiSlot_b0231* slots;           // 0x10
    u32 slot_count;                    // 0x18
    u32 slot_capacity;                 // 0x1c
    u8 pad_0x20[0x10];
    base::UnkAllocator* slot_allocator;  // 0x30
    u8 pad_0x38[8];
    union {
        UnkGuiNode_b0231* list_next;       // 0x40
    };
    union {
        UnkGuiNode_b0231* list_prev;       // 0x48
    };
    UnkGuiNode_b0231* chain_next;      // 0x50
};

struct UnkGuiCacheView_b0231 {
    base::UnkAllocator* allocator;  // 0x0
    u8 pad_0x8[4];
    u32 field_0xc;
    UnkGuiNode_b0231** buckets;  // 0x10
    u32 count;
    u32 divisor;
    u8 pad_0x20[0x18];
    UnkGuiNode_b0231* tail;  // 0x38
    UnkGuiNode_b0231* head;  // 0x40

    UnkGuiNode_b0231** BucketFor(base::global::UnkStringData* key) {
        if (count == 0)
            return nullptr;
        const u32* hash = reinterpret_cast<const u32*>(reinterpret_cast<const u8*>(key) + 0x30);
        u32 idx = (hash[0] ^ hash[1]) % divisor;
        return buckets + idx;
    }
};

struct UnkUnusedArg_b49 : UnkLeftoverReg {};
struct UnkSweepEntry_b49;
struct UnkSweepList_b49;
struct UnkSweepNode_b49;
bool fn_710119d5f0(UnkUnusedArg_b49, UnkSweepEntry_b49* entry, bool flag);
UnkSweepNode_b49* fn_710119d7e4(UnkSweepList_b49* self, UnkSweepNode_b49* node);
void fn_710119e83c(UnkGuiNode_b0231* node);
void fn_710119dff8(UnkSubAreaGuiCache* self, UnkCountedResource* def, bool flag);

// Whether the actor's definition class matches.
static inline UnkGuiOwned_b0231* CastOwned(UnkGuiOwned_b0231* o) {
    return fn_710009d50c(o->vf_0x0(), fn_7100254b14()) ? o : nullptr;
}

// fn_710119d910: drops the nodes whose entry fn_710119d5f0 accepts and whose definition's use count equals the entry's, again while any went.
void fn_710119d910(UnkSubAreaGuiCache* cache) {
    auto* self = reinterpret_cast<UnkGuiCacheView_b0231*>(cache);
    bool any;
    do {
        any = false;
        UnkActorDefFactory* factory = g_unk_71726a0220;
        for (UnkGuiNode_b0231* node = self->head; node;) {
            void* entry = &node->entry_id;
            auto* def = reinterpret_cast<UnkGuiDef_b0231*>(fn_7101193e88(factory, entry));
            if (def && def->field_0x54 == node->slot_count &&
                fn_710119d5f0(UnkUnusedArg_b49(), reinterpret_cast<UnkSweepEntry_b49*>(entry), false)) {
                node = reinterpret_cast<UnkGuiNode_b0231*>(
                    fn_710119d7e4(reinterpret_cast<UnkSweepList_b49*>(self), reinterpret_cast<UnkSweepNode_b49*>(node)));
                any = true;
            } else {
                node = node->list_next;
            }
        }
    } while (any && self->field_0xc);
}

// Takes the node out of the hash table and the list, destroys it (fn_710119e83c) and frees it; gives
// the node that followed it in the list, null when there is no node under the key.
UnkSweepNode_b49* fn_710119d7e4(UnkSweepList_b49* list, UnkSweepNode_b49* node_) {
    auto* self = reinterpret_cast<UnkGuiCacheView_b0231*>(list);
    auto* node = reinterpret_cast<UnkGuiNode_b0231*>(node_);
    auto* key = node->key.data;
    const u32* hash = reinterpret_cast<const u32*>(reinterpret_cast<const u8*>(key) + 0x30);
    u32 idx = (hash[0] ^ hash[1]) % self->divisor;
    // reads the bucket array after the index is known, as the original does
    asm volatile("" ::: "memory");
    UnkGuiNode_b0231** slot = self->buckets + idx;
    UnkGuiNode_b0231* found = *slot;
    UnkGuiNode_b0231* prev = nullptr;
    if (!found)
        return nullptr;
    if (found->key.data != key) {
        do {
            prev = found;
            found = found->chain_next;
            if (!found)
                return nullptr;
        } while (found->key.data != key);
    }
    UnkGuiNode_b0231* after = found->chain_next;
    *(prev ? &prev->chain_next : slot) = after;
    if (self->head == found)
        self->head = found->list_next;
    if (self->tail == found)
        self->tail = found->list_prev;
    if (found->list_prev)
        found->list_prev->list_next = found->list_next;
    UnkGuiNode_b0231* next;
    if (found->list_next) {
        found->list_next->list_prev = found->list_prev;
        next = found->list_next;
    } else {
        next = nullptr;
    }
    fn_710119e83c(found);
    self->allocator->vf_0x48(found);
    self->field_0xc--;
    return reinterpret_cast<UnkSweepNode_b49*>(next);
}

// Releases the node's ids and its slot array.
void fn_710119e83c(UnkGuiNode_b0231* node) {
    if (node->slots) {
        for (u32 i = 0; i < node->slot_count; ++i)
            node->slots[i].id.Reset();
        node->slot_count = 0;
        node->slot_allocator->vf_0x48(node->slots);
        node->slot_capacity = 0;
    }
    node->entry_id.Reset();
    node->key.Reset();
}

// Frees what the entry's slots hold (the actors still taken are handed back first: `release` clears
// every slot with an actor, otherwise those whose definition class is not marked at +0xc8 stop it)
// and empties the entry. False when an actor's definition is marked.
bool fn_710119d5f0(UnkUnusedArg_b49, UnkSweepEntry_b49* entry_, bool release) {
    auto* entry = reinterpret_cast<UnkGuiEntry_b0231*>(entry_);
    if (entry->count != 0) {
        UnkGameManager* manager = g_unk_7101d47310;
        UnkGuiSlot_b0231* slot = entry->slots;
        UnkGuiSlot_b0231* end = slot + entry->count;
        if (release) {
            for (; slot != end; ++slot) {
                UnkGuiActorObj_b0231* actor = slot->actor;
                if (!actor)
                    continue;
                if (slot->in_use)
                    fn_7100232124(reinterpret_cast<UnkBoundObj*>(actor));
                else if (!actor->field_0x1b5)
                    fn_7100debc28(manager, reinterpret_cast<UnkRemovedValue_b60*>(actor));
            }
        } else {
            for (; slot != end; ++slot) {
                if (slot->actor) {
                    if (UnkGuiOwned_b0231* owned = slot->actor->owned) {
                        if (CastOwned(owned)) {
                            if (CastOwned(slot->actor->owned)->field_0xc8)
                                return false;
                        }
                    }
                }
                UnkGuiActorObj_b0231* actor = slot->actor;
                if (!actor)
                    continue;
                if (slot->in_use)
                    fn_7100232124(reinterpret_cast<UnkBoundObj*>(actor));
                else if (!actor->field_0x1b5)
                    fn_7100debc28(manager, reinterpret_cast<UnkRemovedValue_b60*>(actor));
            }
        }
        for (u32 i = 0; i < entry->count; ++i)
            if (g_unk_7101d47108)
                entry->slots[i].id.Reset();
    }
    entry->count = 0;
    return true;
}

// Hands back the actors of the entry under `id` that are free and not in use, and with `recurse` those
// of the definitions its definition lists. True when any went.
bool fn_710119e684(UnkSubAreaGuiCache* cache, const base::UnkStrIdMember* id, bool recurse) {
    auto* self = reinterpret_cast<UnkGuiCacheView_b0231*>(cache);
    UnkGuiNode_b0231** bucket = self->BucketFor(id->data);
    if (!bucket || !*bucket)
        return false;
    UnkGuiNode_b0231* node = *bucket;
    UnkGameManager* manager = g_unk_7101d47310;
    while (node->key.data != id->data) {
        node = node->chain_next;
        if (!node)
            return false;
    }
    u32 any = 0;
    for (UnkGuiSlot_b0231 *it = node->slots, *end = it + node->slot_count; it != end; ++it) {
        if (it->actor) {
            if (UnkGuiOwned_b0231* owned = it->actor->owned) {
                if (CastOwned(owned)) {
                    if (CastOwned(it->actor->owned)->field_0xc8)
                        break;
                }
            }
        }
        UnkGuiActorObj_b0231* actor = it->actor;
        if (actor && !it->in_use && !actor->field_0x1b5) {
            fn_7100debc28(manager, reinterpret_cast<UnkRemovedValue_b60*>(actor));
            any = 1;
        }
    }
    if (recurse) {
        auto* def = reinterpret_cast<UnkGuiDef_b0231*>(fn_7101193e88(g_unk_71726a0220, &node->entry_id));
        for (base::UnkStrIdMember *it = def->children, *end = it + def->child_count; it != end; ++it)
            fn_710119e684(cache, it, true);
    }
    return any & 1;
}

struct UnkActorDefFactory;
struct UnkSpawnDefRef_b52;
UnkSpawnDefRef_b52 fn_71011938bc(UnkActorDefFactory* factory, const base::global::CStrId* type, const base::global::CFilePathStrId* path);

// What fn_71011938bc returns, as this unit holds it: the resource's count is dropped when the handle
// goes.
struct UnkDefHandle_b0231 {
    UnkCountedResource* ptr;
    ~UnkDefHandle_b0231() {
        UnkCountedResource* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};

// Loads the actor definition `id` names and adds it to the cache (fn_710119dff8) unless the cache is
// disabled. True when there is one.
bool fn_710119e4ec(UnkSubAreaGuiCache* cache, const CStrId* id, bool flag) {
    UnkGameManager* manager = g_unk_7101d47310;
    if (!manager->field_0x608 || *reinterpret_cast<const bool*>(reinterpret_cast<const u8*>(manager) + 0x1aa8))
        return false;
    using Load = UnkDefHandle_b0231 (*)(UnkActorDefFactory*, const CStrId*, const base::global::CFilePathStrId*);
    UnkDefHandle_b0231 def = reinterpret_cast<Load>(fn_71011938bc)(g_unk_71726a0220, id, &g_unk_7101d470e8);
    if (!def.ptr)
        return false;
    fn_710119dff8(cache, def.ptr, flag);
    return def.ptr != nullptr;
}

struct UnkPowerUpLifeEntry_b52;
struct alignas(8) UnkPowerUpLifeType_b52;
UnkPowerUpLifeEntry_b52* fn_710119438c(UnkActorDefFactory* factory, const char* path, CStrId* id, const UnkPowerUpLifeType_b52& type,
                                       void* arg);

typedef u32 __attribute__((may_alias)) UnkAliasU32_b0231;

// The name data of a pooled id: the use count at +0x44 and the text at +0x10.
struct UnkPoolName_b0231 {
    u8 pad_0x0[0x10];
    const char* text;  // 0x10
    u8 pad_0x18[0x2c];
    u32 field_0x44;
};

struct UnkPoolOwner_b0231 {
    UnkPoolName_b0231* name;
    u8 pad_0x8[8];
    s32 index;  // 0x10
};

// Loads the pool entry "pool#<name>@<name><index>" of the id with the actor definition factory.
UnkPowerUpLifeEntry_b52* fn_710119e5b8(UnkUnusedArg_b49, UnkPoolOwner_b0231* owner) {
    char key[0x400];
    ++*reinterpret_cast<UnkAliasU32_b0231*>(&owner->name->field_0x44);
    const char* first = owner->name->text ? owner->name->text : g_unk_710162e2ed;
    ++*reinterpret_cast<UnkAliasU32_b0231*>(&owner->name->field_0x44);
    const char* second = owner->name->text ? owner->name->text : g_unk_710162e2ed;
    sprintf(key, "pool#%s@%s%d", first, second, owner->index);
    UnkActorDefFactory* factory = g_unk_71726a0220;
    base::UnkNamedEntry entry;
    fn_710007e704(&entry, key);
    alignas(8) u32 type = 0x144;
    UnkPowerUpLifeEntry_b52* result = fn_710119438c(factory, reinterpret_cast<const char*>(&entry),
                                                    reinterpret_cast<CStrId*>(owner),
                                                    reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr);
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&entry));
    return result;
}
