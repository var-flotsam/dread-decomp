// Looks up an object's entry in a hash map at 0x1a70 (keyed by the object, hashed from two
// words at 0x30) and, when it has a value and fn_7100decab0 gives a target with an object at
// 0x248, hands the value to fn_71010e4998.
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkKeyObject {
    u8 unk_0x0[0x30];
    u32 hash_a;
    u32 hash_b;
};

struct UnkMapNode {
    UnkKeyObject* key;
    void* value;
    u8 unk_0x10[0x10];
    UnkMapNode* next;
};

struct UnkMapOwner {
    u8 unk_0x0[0x1a70];
    UnkMapNode** buckets;
    u32 bucket_count;
    u32 bucket_capacity;
};

bool fn_71010e4998(void* target, void* value);

bool fn_7100e039e4(UnkMapOwner* self, UnkKeyObject* const* key) {
    // the owner is the game manager (its map at +0x1a70)
    UnkGameManagerTarget* target = fn_7100decab0(reinterpret_cast<UnkGameManager*>(self));
    if (!target || !target->field_0x248 || !self->bucket_count)
        return false;
    UnkKeyObject* object = *key;
    u32 hash = object->hash_a ^ object->hash_b;
    u32 index = hash % self->bucket_capacity;
    UnkMapNode** buckets = self->buckets;
    UnkMapNode** bucket = buckets + index;
    if (!bucket)
        return false;
    for (UnkMapNode* n = *bucket; n; n = n->next) {
        if (n->key == object) {
            if (!n->value)
                return false;
            return fn_71010e4998(target->field_0x248, n->value);
        }
    }
    return false;
}
