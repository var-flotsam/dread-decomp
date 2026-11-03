// Unit 2734 (tools/tu_map.py, no name hint): a table of actors keyed by their definition's key object
// (CActorDef at actor +0x28, key at +0x98, hashed by the xor of its two words at +0x30 into a
// chained bucket array). Activating an actor finds its node and its entry; an entry not yet active is
// marked active, its word cleared, the node's active count raised, and the actor is shut down
// (fn_7100232a48) and handed to fn_7100232654 with the entry's payload. Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

struct UnkActor341;
// As src/game/CActor/CActor_ShutdownComponents.cpp defines it.
void fn_7100232a48(UnkActor341* self);
void fn_7100232654(UnkActor341* actor, void* payload);

typedef u32 __attribute__((may_alias)) AliasU32_b43a3;

struct UnkActorDefKey_b43a3 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkActorDef_b43a3 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x90];
    UnkActorDefKey_b43a3* key;  // 0x98
};

struct UnkActorWithDef_b43a3 {
    u8 pad_0x0[0x28];
    UnkActorDef_b43a3* def;  // 0x28
};

struct UnkActorEntry_b43a3 {
    UnkActor341* actor;
    u32 field_0x8;
    u8 active;
    u64 payload;  // 0x10
};

struct UnkActorNode_b43a3 {
    UnkActorDefKey_b43a3* key;
    u8 pad_0x8[0x8];
    UnkActorEntry_b43a3* entries;  // 0x10
    u32 entry_count;  // 0x18
    u8 pad_0x1c[0x1c];
    u32 active_count;  // 0x38
    u8 pad_0x3c[0x14];
    UnkActorNode_b43a3* next;  // 0x50

    UnkActorEntry_b43a3* Find(const void* actor) {
        for (UnkActorEntry_b43a3 *it = entries, *end = entries + entry_count; it != end; ++it) {
            if (it->actor == actor)
                return it;
        }
        return nullptr;
    }
};

struct UnkActorTable_b43a3 {
    u8 pad_0x0[0x10];
    UnkActorNode_b43a3** buckets;  // 0x10
    u32 count;  // 0x18
    u32 bucket_count;  // 0x1c
};

bool fn_710119dd94(UnkActorTable_b43a3* self, UnkActorWithDef_b43a3* actor) {
    UnkActorDef_b43a3* def = actor->def;
    if (!def)
        return false;
    if (!fn_710009d50c(def->vf_0x0(), fn_7100254b14()))
        return false;
    if (self->count == 0)
        return false;
    UnkActorDefKey_b43a3* key = def->key;
    u32 hash = key->hash_lo ^ key->hash_hi;
    u32 index = hash % self->bucket_count;
    UnkActorNode_b43a3** bucket = self->buckets + index;
    if (!bucket)
        return false;
    UnkActorNode_b43a3* node = *bucket;
    for (;; node = node->next) {
        if (!node)
            return false;
        if (node->key == key)
            break;
    }
    UnkActorEntry_b43a3* e = node->Find(actor);
    if (!e || e->active)
        return false;
    ++*reinterpret_cast<AliasU32_b43a3*>(&node->active_count);  // the original reloads the entry after it
    e->active = 1;
    e->field_0x8 = 0;
    fn_7100232a48(e->actor);
    fn_7100232654(e->actor, &e->payload);
    return true;
}
