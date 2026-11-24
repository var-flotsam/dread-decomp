// Unit 2640: replaces the record stored under a key in a hash map keyed by object pointers (hashed by the two
// words at +0x30 of the object): the record's id and array are move-assigned from `value`. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkElem78 {
    u8 unk_0x0[0x78];
};

template <class Elem>
struct UnkElemArray {
    Elem* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::IAllocator* allocator;
};

void fn_71010017f4(UnkElemArray<UnkElem78>* self);  // clear

struct UnkTU2640MapKey {
    u8 unk_0x0[0x30];
    u32 field_0x30;
    u32 field_0x34;
};

struct UnkTU2640Record {
    CStrId id;
    UnkElemArray<UnkElem78> items;  // 0x8

    UnkTU2640Record& operator=(UnkTU2640Record&& other) {
        UnkElemArray<UnkElem78>& src = other.items;
        id.ReleaseAndTake(other.id);
        if (this != &other) {
            fn_71010017f4(&items);
            items.allocator = src.allocator;
            items.flags = src.flags;
            items.step = src.step;
            items.field_0x14 = src.field_0x14;
            items.count = src.count;
            items.capacity = src.capacity;
            items.data = src.data;
            src.data = nullptr;
            src.count = 0;
            src.capacity = 0;
        }
        return *this;
    }
};

struct UnkTU2640MapNode {
    UnkTU2640MapKey* key;
    UnkTU2640Record value;
    u8 unk_0x38[0x10];
    UnkTU2640MapNode* next;  // 0x48
};

struct UnkTU2640Map {
    u8 unk_0x0[0x10];
    UnkTU2640MapNode** buckets;
    u32 bucket_count;
    u32 modulus;
};

void fn_710100375c(UnkTU2640Map* self, UnkTU2640MapKey* const* key, UnkTU2640Record* value) {
    if (self->bucket_count == 0)
        return;
    u32 index = ((*key)->field_0x30 ^ (*key)->field_0x34) % self->modulus;
    UnkTU2640MapNode** bucket = self->buckets + index;
    if (!bucket)
        return;
    for (UnkTU2640MapNode* node = *bucket; node; node = node->next) {
        if (node->key == *key) {
            node->value = static_cast<UnkTU2640Record&&>(*value);
            return;
        }
    }
}
