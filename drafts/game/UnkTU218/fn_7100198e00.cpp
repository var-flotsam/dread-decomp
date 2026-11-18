// Unit 218: the destructor of the object fn_7100198aac builds: frees the values of its node map, its three
// owned pointer arrays, its counted references, its array of 0x20-byte members and its keyed records, then
// destroys its maps and runs the base destructor (fn_710019e82c). Names and layouts are placeholders.
#include <string.h>

#include "base/UnkCountedResource.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU211.h"
#include "game/UnkTU221.h"
#include "game/UnkTU226.h"
#include "game/UnkTU228.h"
#include "types.h"

struct UnkKeyedRecordArray_b45a1;
void fn_710019b91c(UnkKeyedRecordArray_b45a1* self);
void fn_7100196e7c(void* member);
void fn_7100196f78(void* value);
void fn_7100199168(char* map);
void fn_7100192df4(char* map);

extern void* g_unk_7101a1e0a8[];

struct UnkOwned_l198e00 {
    ~UnkOwned_l198e00() { fn_710019adec(this); }
};

// A counted reference: dropping the last one releases the resource.
struct UnkResRef_l198e00 {
    UnkCountedResource* ptr;

    ~UnkResRef_l198e00() {
        UnkCountedResource* resource = ptr;
        if (resource && --resource->ref_count == 0)
            fn_710001737c(resource);
    }
};

struct UnkMapValue_l198e00 {
    ~UnkMapValue_l198e00() { fn_7100196f78(this); }
};

struct UnkMember_l198e00 {
    u8 pad_0x0[0x20];

    ~UnkMember_l198e00() { fn_7100196e7c(this); }
};

struct UnkPolymorphic_l198e00 {
    virtual ~UnkPolymorphic_l198e00();
};

struct UnkMapNode_l198e00 {
    u64 key;
    UnkMapValue_l198e00* value;  // 0x8
    UnkMapNode_l198e00* next;    // 0x10
};

struct UnkNodeMap_l198e00 {
    base::UnkAllocator* allocator;  // 0x0
    u32 flags;
    u32 count;  // 0xc
    base::global::CRntVector<UnkMapNode_l198e00*> buckets;  // 0x10
    UnkMapNode_l198e00* tail;  // 0x38
    UnkMapNode_l198e00* head;  // 0x40

    void DeleteValues() {
        for (UnkMapNode_l198e00* node = head; node; node = node->next) {
            if (UnkMapValue_l198e00* value = node->value)
                delete value;
        }
    }

    // Frees every node and empties the buckets.
    void Clear() {
        if (count) {
            UnkMapNode_l198e00* node = head;
            while (node) {
                UnkMapNode_l198e00* next = node->next;
                allocator->vf_0x48(node);
                node = next;
            }
            memset(buckets.data, 0, buckets.capacity * u64(8));
            count = 0;
            tail = nullptr;
            head = nullptr;
        }
    }

    ~UnkNodeMap_l198e00() {
        Clear();
        buckets.Free();
    }
};
static_assert(sizeof(UnkNodeMap_l198e00) == 0x48, "UnkNodeMap_l198e00 layout");

struct UnkObject218_l198e00 {
    void* vtable;
    u8 base_0x8[0x20];
    u32 owned_count;    // 0x28
    u32 decal_count;    // 0x2c
    u32 object_count;   // 0x30
    u8 pad_0x34[4];
    UnkMember_l198e00* members;           // 0x38
    UnkOwned_l198e00** owned;             // 0x40
    UnkDecalResource** decals;            // 0x48
    UnkPolymorphic_l198e00** objects;     // 0x50
    base::global::CRntVector<UnkResRef_l198e00> refs;  // 0x58
    UnkKeyedRecordArray_b45a1* records;   // 0x80
    u8 pad_0x88[0x50];
    char h0[0x48];             // 0xd8
    UnkNodeMap_l198e00 h1;     // 0x120
    char h2[0x48];             // 0x168
};
static_assert(__builtin_offsetof(UnkObject218_l198e00, h2) == 0x168, "UnkObject218_l198e00 layout");

void fn_7100198e00(UnkObject218_l198e00* self) {
    self->vtable = reinterpret_cast<char*>(g_unk_7101a1e0a8) + 0x10;
    self->h1.DeleteValues();
    self->h1.Clear();

    for (u64 i = 0; i < self->owned_count; ++i) {
        if (UnkOwned_l198e00* owned = self->owned[i])
            delete owned;
        self->owned[i] = nullptr;
    }
    fn_7100083990()->vf_0x48(self->owned);
    self->owned = nullptr;

    for (u64 i = 0; i < self->decal_count; ++i) {
        fn_71001a080c(g_unk_716e352450, self->decals[i]);
        self->decals[i] = nullptr;
    }
    fn_7100083990()->vf_0x48(self->decals);
    self->decals = nullptr;

    self->refs.Clear();

    if (self->objects) {
        for (u32 i = 0; i < self->object_count; ++i) {
            if (self->objects[i]) {
                delete self->objects[i];
                self->objects[i] = nullptr;
            }
        }
        fn_7100083990()->vf_0x48(self->objects);
        self->objects = nullptr;
    }

    delete[] self->members;
    self->members = nullptr;

    if (UnkKeyedRecordArray_b45a1* records = self->records) {
        fn_710019b91c(records);
        operator delete(records);
    }
    self->records = nullptr;

    fn_7100192df4(self->h2);
    self->h1.~UnkNodeMap_l198e00();
    fn_7100199168(self->h0);
    self->refs.Free();
    fn_710019e82c(self);
}
