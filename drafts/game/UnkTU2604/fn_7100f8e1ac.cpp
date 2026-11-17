// Unit 2604: CEnvironmentSoundData's move constructor, as the dictionary of environment sound data constructs
// its values with it: every member is default-constructed, the ids are moved, and each sound of the other's
// map is added to this one (its key copied, the sound moved). Names past the class's are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/UnkRenderPassSlots.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkTU2604Sound {  // CEnvironmentSoundData::SSound
    UnkMovedStrId id;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;

    UnkTU2604Sound(UnkTU2604Sound&& other) = default;
};

struct UnkTU2604SoundNode {
    CStrId key;
    UnkTU2604Sound value;      // 0x8
    UnkTU2604SoundNode* next;  // 0x28
    UnkTU2604SoundNode* prev;  // 0x30
    UnkTU2604SoundNode* chain; // 0x38

    UnkTU2604SoundNode(const CStrId& k, UnkTU2604Sound&& v)
        : key(k), value(static_cast<UnkTU2604Sound&&>(v)), next(nullptr), prev(nullptr), chain(nullptr) {}
};

// CRntDictionary<CStrId, CEnvironmentSoundData::SSound>: chained buckets and a list of all nodes.
struct UnkTU2604SoundMap {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;                              // 0xc
    CRntVector<UnkTU2604SoundNode*> buckets;  // 0x10
    UnkTU2604SoundNode* tail;               // 0x38
    UnkTU2604SoundNode* head;               // 0x40

    void Insert(const CStrId& key, UnkTU2604Sound&& sound) {
        UnkTU2604SoundNode** bucket = nullptr;
        if (buckets.count != 0)
            bucket = &buckets.data[key.Hash() % buckets.capacity];
        UnkTU2604SoundNode* old = *bucket;
        auto* node = static_cast<UnkTU2604SoundNode*>(allocator->vf_0x40(sizeof(UnkTU2604SoundNode), 8, flags));
        if (node)
            new (node) UnkTU2604SoundNode(key, static_cast<UnkTU2604Sound&&>(sound));
        node->chain = old;
        *bucket = node;
        if (!head)
            head = node;
        else if (tail) {
            node->prev = tail;
            tail->next = node;
        }
        tail = node;
        ++count;
    }
};

struct UnkTU2604SoundData {  // CEnvironmentSoundData
    CStrId name;
    CFilePathStrId path;  // 0x8
    CStrId field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    UnkTU2604SoundMap sounds;  // 0x30
};
static_assert(__builtin_offsetof(UnkTU2604SoundData, sounds) == 0x30, "layout");

static inline void InitEmptyStrId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

// A move of an id into one that is released first; the moved-from id gets the empty id of the pool as it
// was read for the release (not read again).
template <class Id>
static inline void TakeId(Id* dst, Id* src, UnkStringPool* const& pool_global) {
    UnkStringPool* pool = pool_global;
    if (pool) {
        if (dst->data && dst->data != &pool->empty) {
            --dst->data->ref_count;
            if (dst->data->ref_count == 0) {
                fn_71000803a4(pool, dst->data);
                pool = pool_global;
            }
        }
        dst->data = &pool->empty;
    }
    dst->data = src->data;
    src->data = &pool->empty;
}

// The map's constructor: allocation flags 0xe and 11 empty buckets.
static void InitSoundMap(UnkTU2604SoundMap* map) {
    map->allocator = fn_7100083990();
    map->flags = 0xe;
    map->count = 0;
    new (&map->buckets) CRntVector<UnkTU2604SoundNode*>(0xe);
    map->tail = nullptr;
    map->head = nullptr;
    UnkResizeSlots(reinterpret_cast<CRntVector<void*>&>(map->buckets), 11);
}

void fn_7100f8e1ac(UnkTU2604SoundData* self, UnkTU2604SoundData* other) {
    InitEmptyStrId(&self->name);
    new (&self->path) CFilePathStrId();
    InitEmptyStrId(&self->field_0x10);
    InitSoundMap(&self->sounds);
    self->name.ReleaseAndTake(other->name);
    TakeId(&self->path, &other->path, g_unk_7101d47200);
    TakeId(&self->field_0x10, &other->field_0x10, g_unk_7101d47108);
    self->field_0x18 = other->field_0x18;
    self->field_0x20 = other->field_0x20;
    self->field_0x28 = other->field_0x28;
    for (UnkTU2604SoundNode* node = other->sounds.head; node; node = node->next)
        self->sounds.Insert(node->key, static_cast<UnkTU2604Sound&&>(node->value));
}
