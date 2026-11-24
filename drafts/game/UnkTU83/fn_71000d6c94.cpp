// Unit 83 (base::reflection): adds an entry (a string id and a reflected value built from an argument value)
// to a chained hash table, in the bucket its id's hash picks, and to the table's list of entries; returns
// the new entry. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkStringKey83 {
    u8 pad_0x0[0x30];
    u32 field_0x30;
    u32 field_0x34;
};

struct UnkEntry83 {
    CStrId key;
    UnkReflValue value;  // 0x8
    UnkEntry83* next;    // 0x20
    UnkEntry83* prev;    // 0x28
    UnkEntry83* chain;   // 0x30

    UnkEntry83(const CStrId& id, const UnkReflValue& arg) : key(id) {
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = arg.kind;
        const void* data;
        if (!arg.type)
            data = nullptr;
        else if (arg.field_0x14 == 1 || *reinterpret_cast<const u64*>(arg.type + 0x18) >= 9)
            data = arg.field_0x0;
        else
            data = &arg;
        fn_71000a18d4(&value, reinterpret_cast<const CType*>(arg.type), data, false);
        prev = nullptr;
        chain = nullptr;
        next = nullptr;
    }
};

struct UnkTable83 {
    base::IAllocator* allocator;  // 0x0
    u32 flags;                      // 0x8
    u32 count;                      // 0xc
    UnkEntry83** buckets;           // 0x10
    u32 bucket_count;               // 0x18
    u32 bucket_capacity;            // 0x1c
    u8 pad_0x20[0x18];
    UnkEntry83* tail;  // 0x38
    UnkEntry83* head;  // 0x40
};

UnkEntry83* fn_71000d6c94(UnkTable83* self, const CStrId* id, const UnkReflValue* arg) {
    UnkEntry83** bucket = nullptr;
    if (self->bucket_count != 0) {
        const UnkStringKey83* key = reinterpret_cast<const UnkStringKey83*>(id->data);
        u32 index = (key->field_0x30 ^ key->field_0x34) % self->bucket_capacity;
        bucket = &self->buckets[index];
    }
    UnkEntry83* old = *bucket;
    auto* node = static_cast<UnkEntry83*>(self->allocator->vf_0x40(sizeof(UnkEntry83), 8, self->flags));
    if (node)
        new (node) UnkEntry83(*id, *arg);
    node->chain = old;
    *bucket = node;
    if (!self->head)
        self->head = node;
    else if (self->tail) {
        node->prev = self->tail;
        self->tail->next = node;
    }
    self->tail = node;
    ++self->count;
    return node;
}
