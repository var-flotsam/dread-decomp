// Unit 347 (tools/tu_map.py, no name hint): a reflected function (fn_7100264100) that removes the
// entry keyed by its argument (an object hashed by the two words at +0x30) from the chained map at
// +0x10 of the object at 8. The entry's value, unless it is gone or flagged at +0x1b5, first drops
// its back pointer (+0xc8) to the owner and is handed to the game manager (fn_7100debc28); the node
// is then erased (fn_71002430e8). Returns whether there was an entry. Names are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkHashedKey_b60 {
    u8 pad_0x0[0x30];
    u32 field_0x30;
    u32 field_0x34;
};

struct UnkRemovedOwner_b60;

struct UnkRemovedValue_b60 {
    u8 pad_0x0[0xc8];
    UnkRemovedOwner_b60* owner;  // 0xc8
    u8 pad_0xd0[0xe5];
    bool field_0x1b5;
};

struct UnkKeyNode_b60 {
    UnkHashedKey_b60* key;
    UnkRemovedValue_b60* value;
    u8 pad_0x10[0x10];
    UnkKeyNode_b60* next;  // 0x20
};

struct UnkKeyMap_b60 {
    u8 pad_0x0[0x10];
    UnkKeyNode_b60** buckets;  // 0x10
    u32 count;                 // 0x18
    u32 bucket_count;          // 0x1c
};

struct UnkRemovedOwner_b60 {
    u8 pad_0x0[0x10];
    UnkKeyMap_b60 map;  // 0x10
};

void fn_7100debc28(UnkGameManager* manager, UnkRemovedValue_b60* value);

static inline __attribute__((always_inline)) void* ArgPtr_b60(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

static inline __attribute__((always_inline)) bool Remove_b60(UnkRemovedOwner_b60* self,
                                                            UnkHashedKey_b60* const* key) {
    UnkKeyMap_b60* map = &self->map;
    if (map->count == 0)
        return false;
    UnkHashedKey_b60* k = *key;
    u32 h = (k->field_0x30 ^ k->field_0x34) % map->bucket_count;
    UnkKeyNode_b60** bucket = &map->buckets[h];
    if (!bucket)
        return false;
    for (UnkKeyNode_b60* n = *bucket; n; n = n->next) {
        if (n->key == k) {
            UnkRemovedValue_b60* v = n->value;
            if (v && !v->field_0x1b5) {
                if (v->owner == self)
                    v->owner = nullptr;
                fn_7100debc28(g_unk_7101d47310, v);
            }
            fn_71002430e8(map, n);
            return true;
        }
    }
    return false;
}

UnkReflValue fn_7100264100(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    auto* self = *reinterpret_cast<UnkRemovedOwner_b60**>(obj + 8);
    auto* key = static_cast<UnkHashedKey_b60* const*>(ArgPtr_b60(a));
    bool removed = Remove_b60(self, key);
    const base::reflection::CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &removed, false);
    return r;
}
