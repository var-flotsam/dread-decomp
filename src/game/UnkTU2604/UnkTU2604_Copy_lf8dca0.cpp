// Unit 2604: CEnvironmentSoundData's copy constructor, as the dictionary of environment sound data constructs
// its values with it: every member is default-constructed, then the ids and settings are assigned and each
// sound of the other's map is added to this one. Names past the class's are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkRenderPassSlots.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::CStrId;

struct UnkTU2604CopySound {  // CEnvironmentSoundData::SSound
    CStrId id;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;

    UnkTU2604CopySound(const UnkTU2604CopySound& other) = default;
};

struct UnkTU2604CopySoundNode {
    CStrId key;
    UnkTU2604CopySound value;      // 0x8
    UnkTU2604CopySoundNode* next;  // 0x28
    UnkTU2604CopySoundNode* prev;  // 0x30
    UnkTU2604CopySoundNode* chain; // 0x38

    UnkTU2604CopySoundNode(const CStrId& k, const UnkTU2604CopySound& v)
        : key(k), value(v), next(nullptr), prev(nullptr), chain(nullptr) {}

    UnkTU2604CopySoundNode*& Next() { return next; }
    UnkTU2604CopySoundNode*& Prev() { return prev; }
};

// CRntDictionary<CStrId, CEnvironmentSoundData::SSound>: chained buckets and a list of all nodes.
struct UnkTU2604CopySoundMap {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;                                    // 0xc
    CRntVector<UnkTU2604CopySoundNode*> buckets;  // 0x10
    UnkTU2604CopySoundNode* tail;                 // 0x38
    UnkTU2604CopySoundNode* head;                 // 0x40

    void Insert(const CStrId& key, const UnkTU2604CopySound& sound) {
        UnkTU2604CopySoundNode** bucket = nullptr;
        if (buckets.count != 0) {
            u32 index = key.Hash() % buckets.capacity;
            bucket = &buckets.data[index];
        }
        UnkTU2604CopySoundNode* old = *bucket;
        auto* node =
            static_cast<UnkTU2604CopySoundNode*>(allocator->vf_0x40(sizeof(UnkTU2604CopySoundNode), 8, flags));
        if (node)
            new (node) UnkTU2604CopySoundNode(key, sound);
        node->chain = old;
        *bucket = node;
        if (!head)
            head = node;
        else if (tail) {
            node->Prev() = tail;
            tail->Next() = node;
        }
        tail = node;
        ++count;
    }
};

// The plain values after the ids; left unset by the constructor.
struct UnkTU2604CopySettings {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;

    UnkTU2604CopySettings() {}
};

struct UnkTU2604CopySoundData {  // CEnvironmentSoundData
    CStrId name;
    CFilePathStrId path;  // 0x8
    CStrId field_0x10;
    UnkTU2604CopySettings settings;  // 0x18
    UnkTU2604CopySoundMap sounds;    // 0x30
};
static_assert(__builtin_offsetof(UnkTU2604CopySoundData, sounds) == 0x30, "layout");

static inline void InitEmptyStrId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

// The map's constructor: allocation flags 0xe and 11 empty buckets.
static void InitSoundMap(UnkTU2604CopySoundMap* map) {
    map->allocator = fn_7100083990();
    map->flags = 0xe;
    map->count = 0;
    new (&map->buckets) CRntVector<UnkTU2604CopySoundNode*>(0xe);
    map->tail = nullptr;
    map->head = nullptr;
    UnkResizeSlots(reinterpret_cast<CRntVector<void*>&>(map->buckets), 11);
}

void fn_7100f8dca0(UnkTU2604CopySoundData* self, const UnkTU2604CopySoundData* other) {
    InitEmptyStrId(&self->name);
    new (&self->path) CFilePathStrId();
    InitEmptyStrId(&self->field_0x10);
    InitSoundMap(&self->sounds);
    self->name = other->name;
    self->path = other->path;
    self->field_0x10 = other->field_0x10;
    self->settings = other->settings;
    for (UnkTU2604CopySoundNode* node = other->sounds.head; node; node = node->next)
        self->sounds.Insert(node->key, node->value);
}
