// Unit 2622 (no reliable name): the scenario load request's entries (clear, append, reserve) and the directory iterator step. Names and layouts are placeholders.
#include <new>
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "game/CGameManager/CTunableGameManager.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;

struct UnkEntry_b0235;
void fn_7100fc6fc0(UnkEntry_b0235* self, const UnkEntry_b0235* other);

struct UnkRequestListNode_b0246 {
    void* value;
    UnkRequestListNode_b0246* next;
};

struct UnkRequestList_b0246 {
    void** vtable;
    // A union member: the original reloads the count after storing a new node's links.
    union {
        u32 count;
    };
    UnkRequestListNode_b0246* head;
    UnkRequestListNode_b0246* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    // Frees every node.
    void Clear() {
        for (UnkRequestListNode_b0246* n = head; n;) {
            UnkRequestListNode_b0246* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
    __attribute__((always_inline)) ~UnkRequestList_b0246() {
        vtable = reinterpret_cast<void**>(g_unk_7101aa9ac0 + 0x10);
        Clear();
    }
};

// One request (0x208 bytes): the scenario's strings and the blackboards it is given.
struct UnkLoadEntry_b0246 {
    CRntString scenario;  // 0x0
    CRntString name;      // 0x30
    CRntString project;   // 0x60
    CRntString s2;        // 0x90
    CRntString s3;        // 0xc0
    UnkRequestList_b0246 blackboards;  // 0xf0
    CRntString c;         // 0x120
    u8 unk_0x150[0x88];
    CRntString field_0x1d8;

    __attribute__((always_inline)) ~UnkLoadEntry_b0246() {}
};
static_assert(sizeof(UnkLoadEntry_b0246) == 0x208, "UnkLoadEntry_b0246 layout");

struct UnkLoadRequest_b0246 {
    u32 kind;
    bool flag;
    void (*fn)();
    base::UnkVector<UnkLoadEntry_b0246> entries;  // 0x10
    bool pending;  // 0x38
};
static_assert(__builtin_offsetof(UnkLoadRequest_b0246, pending) == 0x38, "UnkLoadRequest_b0246 layout");


// The element as raw storage: copied out of line, destroyed in place.
struct alignas(8) UnkEntryRaw_b0246 {
    u8 raw[0x208];

    UnkEntryRaw_b0246(const UnkEntryRaw_b0246& other) {
        fn_7100fc6fc0(reinterpret_cast<UnkEntry_b0235*>(this), reinterpret_cast<const UnkEntry_b0235*>(&other));
    }
    __attribute__((always_inline)) ~UnkEntryRaw_b0246() { reinterpret_cast<UnkLoadEntry_b0246*>(this)->~UnkLoadEntry_b0246(); }
};

__attribute__((noinline)) void fn_7100fc7d94(base::UnkVector<UnkEntryRaw_b0246>* self, u32 capacity);

struct UnkScenarioLoadEntry;
struct UnkString_7100fc4b34;

// The part of an entry from +0x150 on: a flag, then buffers that start zeroed.
struct UnkEntryTail_b0246 {
    bool valid;      // 0x150
    u8 pad_0x151[7];
    u8 flag;         // 0x158
    u8 pad_0x159[3];
    u8 buffer_a[0x25];  // 0x15c
    u8 pad_0x181[3];
    u32 field_0x184;
    u8 buffer_b[0x1e];  // 0x188
    u8 pad_0x1a6[1];
    u8 buffer_c[7];  // 0x1a0 .. placeholder
};

static inline void InitEntry_b0246(UnkLoadEntry_b0246* e) {
    new (&e->scenario) CRntString(g_unk_710162e2ed);
    new (&e->name) CRntString(g_unk_710162e2ed);
    new (&e->project) CRntString(g_unk_710162e2ed);
    new (&e->s2) CRntString(g_unk_710162e2ed);
    new (&e->s3) CRntString(g_unk_710162e2ed);
    e->blackboards.vtable = reinterpret_cast<void**>(g_unk_7101aa9ac0 + 0x10);
    e->blackboards.count = 0;
    e->blackboards.head = nullptr;
    e->blackboards.tail = nullptr;
    e->blackboards.allocator = fn_7100083990();
    e->blackboards.flags = 3;
    new (&e->c) CRntString(g_unk_710162e2ed);
    u8* raw = reinterpret_cast<u8*>(e);
    raw[0x150] = 1;
    raw[0x158] = 0;
    *reinterpret_cast<u32*>(raw + 0x184) = 0;
    __builtin_memset(raw + 0x15c, 0, 0x25);
    *reinterpret_cast<u32*>(raw + 0x1a0) = 0;
    __builtin_memset(raw + 0x1a3, 0, 4);
    raw[0x1a7] = 1;
    *reinterpret_cast<u64*>(raw + 0x1b8) = 0;
    *reinterpret_cast<u16*>(raw + 0x1c0) = 0;
    __builtin_memset(raw + 0x1a8, 0, 16);
    *reinterpret_cast<u32*>(raw + 0x1d4) = 0;
    __builtin_memset(raw + 0x1c4, 0, 16);
    fn_710007eba0(reinterpret_cast<UnkString_7100fc4b34*>(&e->field_0x1d8), 0);
    __builtin_memset(raw + 0x196, 0, 8);
    __builtin_memset(raw + 0x188, 0, 16);
}

UnkScenarioLoadEntry* fn_7100fc77b0(base::UnkVector<UnkScenarioLoadEntry>* entries_) {
    auto* entries = reinterpret_cast<base::UnkVector<UnkEntryRaw_b0246>*>(entries_);
    u32 n = entries->count;
    u32 want = n + 1;
    fn_7100fc7d94(entries, want);
    if (entries->count < want) {
        for (u32 i = entries->count; i < want; ++i)
            InitEntry_b0246(reinterpret_cast<UnkLoadEntry_b0246*>(&entries->data[i]));
    }
    entries->count = want;
    UnkEntryRaw_b0246* end = &entries->data[want];
    return reinterpret_cast<UnkScenarioLoadEntry*>(want ? end - 1 : end);
}

void fn_7100fc7d94(base::UnkVector<UnkEntryRaw_b0246>* self, u32 capacity) {
    if (self->capacity < capacity) {
        auto* p = static_cast<UnkEntryRaw_b0246*>(self->allocator->vf_0x40((u64)capacity * sizeof(UnkEntryRaw_b0246), 8, self->flags));
        if (self->data) {
            UnkEntryRaw_b0246* src = self->data;
            UnkEntryRaw_b0246* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkEntryRaw_b0246(static_cast<UnkEntryRaw_b0246&&>(*src));
                src->~UnkEntryRaw_b0246();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = capacity;
    }
}

struct UnkScenarioLoadRequest;

void fn_7100fc73f4(UnkScenarioLoadRequest* request, void (*fn)()) {
    UnkLoadRequest_b0246* self = reinterpret_cast<UnkLoadRequest_b0246*>(request);
    self->kind = 5;
    self->flag = true;
    self->fn = fn;
    for (u64 i = 0; i < self->entries.count; ++i)
        self->entries.data[i].~UnkLoadEntry_b0246();
    self->entries.count = 0;
    self->pending = true;
}

using base::global::CRntString;

struct UnkDirIter_b0235;

struct UnkIterPart_b0246 {
    u8 pad_0x0[0x10];
    CRntString name;  // 0x10
    u8 pad_0x40[4];
    union {
        u32 uses;  // 0x44
    };
};

struct UnkIterInner_b0246 {
    u8 pad_0x0[8];
    UnkIterPart_b0246* part;  // 0x8
};

struct UnkIterSub_b0246 {
    u8 pad_0x0[8];
    UnkIterInner_b0246* inner;  // 0x8
    UnkIterSub_b0246* next;     // 0x10
};

struct UnkIterObj_b0246 {
    u8 pad_0x0[0x3c];
    u32 count;  // 0x3c
    u8 pad_0x40[0x30];
    UnkIterSub_b0246* first;  // 0x70
};

struct UnkIterNode_b0246 {
    UnkIterObj_b0246* obj;
    UnkIterNode_b0246* next;
};

struct UnkIterDir_b0246 {
    u8 pad_0x0[0x120];
    CRntString filter;  // 0x120
};

struct UnkIterOwner_b0246 {
    u8 pad_0x0[0x10];
    UnkIterDir_b0246* dir;  // 0x10
};

struct UnkIterState_b0246 {
    UnkIterOwner_b0246* owner;  // 0x0
    u32 index;                  // 0x8
    UnkIterNode_b0246* node;    // 0x10
    UnkIterSub_b0246* sub;      // 0x18
};

bool fn_7100fc7b78(UnkDirIter_b0235* iter) {
    UnkIterState_b0246* it = reinterpret_cast<UnkIterState_b0246*>(iter);
    if (!it->owner)
        return false;
    UnkIterNode_b0246* node = it->node;
    if (!node)
        goto tail;
    if (node->obj->count != 0) {
        for (;;) {
            UnkIterSub_b0246* sub = it->sub->next;
            it->sub = sub;
            if (!sub)
                break;
            const CRntString* filter = &it->owner->dir->filter;
            if (!filter->data || *filter->data == 0)
                goto done;
            UnkIterInner_b0246* inner = sub->inner;
            inner->part->uses++;
            if (fn_710007f220(&inner->part->name, &it->owner->dir->filter))
                continue;
            if (it->sub)
                goto done;
            break;
        }
        node = it->node;
    }
    node = node->next;
    it->node = node;
    if (node)
        it->sub = node->obj->first;
done:
    it->index++;
    if (!it->owner)
        return false;
tail:
    // keeps the node read at the end from being folded into the early exit
    asm volatile("" ::: "memory");
    return it->node != nullptr;
}
