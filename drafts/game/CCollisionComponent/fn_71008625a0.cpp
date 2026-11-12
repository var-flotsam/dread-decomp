// CCollisionComponent unit: creates a 0x330-byte record, appends it to the component's list and
// stores an owner plus a shared string id in it. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"

using base::global::UnkStringData;

char* fn_7100d45754(char* p0);

struct UnkAllocator {
    struct VTable {
        u8 pad_0x0[0x40];
        void* (*alloc)(UnkAllocator*, u64 size, u64 align, u32 tag);
    };
    VTable* vtable;
};

struct UnkRecord {
    struct VTable {
        u8 pad_0x0[0x18];
        void (*init)(UnkRecord*);
    };
    VTable* vtable;
    u8 pad_0x8[8];
    void* owner;
    u64 field_0x18;
    UnkStringData* name;
};

struct UnkRecordNode {
    UnkRecord* record;
    UnkRecordNode* next;
};

struct UnkStringHolder {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkCollisionOwner {
    u8 pad_0x0[0x40];
    union {
        u32 count;
    };
    u8 pad_0x44[4];
    UnkRecordNode* head;
    UnkRecordNode* tail;
    UnkAllocator* allocator;
    u32 tag;
    u8 pad_0x64[4];
    u32 alloc_flags;
};

struct UnkNameRef {
    UnkStringData* data;
};

// NON_MATCHING(minor): same instructions; register assignment (x19/x20 swapped) and one store order differ
UnkRecord* fn_71008625a0(UnkCollisionOwner* self, void* owner, UnkNameRef* name) {
    UnkRecord* rec = (UnkRecord*)fn_71000839a0(0x330, self->alloc_flags);
    fn_7100d45754((char*)rec);
    if (rec) {
        rec->vtable->init(rec);
        UnkRecordNode* node = (UnkRecordNode*)self->allocator->vtable->alloc(self->allocator, 0x10, 8, self->tag);
        if (node) {
            node->record = rec;
            node->next = nullptr;
            if (self->count == 0) {
                self->head = node;
                self->tail = node;
                self->count = 1;
            } else {
                self->tail->next = node;
                node->next = nullptr;
                self->tail = node;
                self->count = self->count + 1;
            }
        }
    }
    rec->owner = owner;
    UnkStringData* cur = rec->name;
    if (cur != name->data) {
        UnkStringData* incoming = name->data;
        UnkStringData* empty = &g_unk_7101d47108->empty;
        if (incoming != empty) {
            incoming->ref_count++;
            cur = rec->name;
        }
        if (cur != empty) {
            cur->ref_count--;
            if (rec->name->ref_count == 0)
                fn_71000803a4(g_unk_7101d47108, rec->name);
        }
        __builtin_memcpy(&rec->name, &incoming, sizeof(incoming));
    }
    rec->field_0x18 = (*(UnkStringHolder* const*)name)->field_0x30;
    return rec;
}
