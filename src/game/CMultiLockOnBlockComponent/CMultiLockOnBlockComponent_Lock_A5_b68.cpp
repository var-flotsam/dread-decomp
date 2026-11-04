// CMultiLockOnBlockComponent's unit: finds the lock point for `key` in the hash map at +0xe0 (bucket = key modulo
// the bucket count, chained through +0x38); on a hit, marks it locked the first time (counting locks at +0x44 and
// setting +0x3d of the object fn_710031ca1c finds for the key), clears its timer, gives the owner's timeline
// (fn_7100d7c828) the point's key and stores the flag. Names and layouts are placeholders.
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkLockOnPoint_A5_b68 {
    u64 key;
    bool locked;
    u8 pad_0x9[3];
    u32 field_0xc;
    u64 field_0x10;
    u8 pad_0x18[8];
    bool field_0x20;
    u8 pad_0x21[0x17];
    UnkLockOnPoint_A5_b68* next;
};

struct UnkLockOnTarget_A5_b68 {
    u8 pad_0x0[0x3d];
    bool field_0x3d;
};

UnkLockOnTarget_A5_b68* fn_710031ca1c(u64 key);

// The lock count: a store to it is taken to alias the point's key, which is read again afterwards.
typedef s32 __attribute__((may_alias)) UnkAliasS32_A5_b68;

struct UnkMultiLockOnBlock_A5_b68 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0xc];
    s32 field_0x44;
    u8 pad_0x48[0x98];
    UnkLockOnPoint_A5_b68** buckets;
    u32 count;
    u32 bucket_count;
};

void fn_710088341c(UnkMultiLockOnBlock_A5_b68* self, u64 key, bool flag) {
    if (!self->count)
        return;
    u32 index = (u32)key % self->bucket_count;
    UnkLockOnPoint_A5_b68** slot = &self->buckets[index];
    if (!slot)
        return;
    for (UnkLockOnPoint_A5_b68* p = *slot; p; p = p->next) {
        if (p->key == key) {
            if (!p->locked) {
                p->locked = true;
                ++*reinterpret_cast<UnkAliasS32_A5_b68*>(&self->field_0x44);
                if (UnkLockOnTarget_A5_b68* t = fn_710031ca1c(p->key))
                    t->field_0x3d = true;
            }
            p->field_0xc = 0;
            fn_7100d7c828(self->owner, &p->field_0x10, false);
            p->field_0x20 = flag;
            return;
        }
    }
}
