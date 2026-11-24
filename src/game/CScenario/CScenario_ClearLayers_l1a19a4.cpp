// CScenario unit: flushes the scenario's actor lists, then empties the bound-object maps of its three layers,
// detaching each attached object from its map and removing it from the game manager.
// Names and layouts are placeholders.
#include <string.h>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;

struct UnkScenario_l1a1e34;
void CScenario_ClearActorLists(UnkScenario_l1a1e34* self);

struct UnkBoundObject_l1a19a4 {
    u8 pad_0x0[0xc8];
    void* owner;  // 0xc8
    u8 pad_0xd0[0xe5];
    u8 detached;  // 0x1b5
};

struct UnkIdNode_l1a19a4 {
    CStrId id;                        // 0x0
    UnkBoundObject_l1a19a4* object;   // 0x8
    UnkIdNode_l1a19a4* next;          // 0x10
};

struct UnkIdMap_l1a19a4 {
    base::IAllocator* allocator;    // 0x0
    u8 pad_0x8[4];
    u32 count;                        // 0xc
    void* buckets;                    // 0x10
    u8 pad_0x18[4];
    u32 bucket_count;                 // 0x1c
    u8 pad_0x20[0x18];
    UnkIdNode_l1a19a4* tail;          // 0x38
    UnkIdNode_l1a19a4* head;          // 0x40

    // Frees every node and empties the buckets.
    void Clear() {
        if (count) {
            UnkIdNode_l1a19a4* node = head;
            while (node) {
                UnkIdNode_l1a19a4* next = node->next;
                node->id.Reset();
                allocator->vf_0x48(node);
                node = next;
            }
            memset(buckets, 0, bucket_count * u64(8));
            count = 0;
            tail = nullptr;
            head = nullptr;
        }
    }
};

struct UnkBoundMap_l1a19a4 {
    void* vtable;                     // 0x0
    CStrId name;                      // 0x8
    UnkIdMap_l1a19a4 map;             // 0x10
    u8 pad_0x58[8];

    // Detaches the objects still attached to this map and removes them from the game manager.
    void ReleaseObjects() {
        for (UnkIdNode_l1a19a4* node = map.head; node; node = node->next) {
            UnkBoundObject_l1a19a4* object = node->object;
            if (object->detached)
                continue;
            if (object->owner == this)
                object->owner = nullptr;
            fn_7100debc28(g_unk_7101d47310, reinterpret_cast<UnkRemovedValue_b60*>(object));
        }
    }
};
static_assert(sizeof(UnkBoundMap_l1a19a4) == 0x60, "UnkBoundMap_l1a19a4 layout");

struct UnkScenarioLayer_l1a19a4 {
    UnkBoundMap_l1a19a4* entries;
    u8 pad_0x8[0x8];
    u32 count;  // 0x10
    u8 pad_0x14[0xb4];
};
static_assert(sizeof(UnkScenarioLayer_l1a19a4) == 0xc8, "UnkScenarioLayer_l1a19a4 layout");

struct UnkScenario_l1a19a4 {
    u8 pad_0x0[0x28];
    UnkScenarioLayer_l1a19a4 layers[3];  // 0x28
};

void fn_71011a19a4(void* scenario) {
    UnkScenario_l1a19a4* self = static_cast<UnkScenario_l1a19a4*>(scenario);
    CScenario_ClearActorLists(reinterpret_cast<UnkScenario_l1a1e34*>(self));
    for (UnkScenarioLayer_l1a19a4& layer : self->layers) {
        for (UnkBoundMap_l1a19a4 *it = layer.entries, *end = layer.entries + layer.count; it != end; ++it)
        {
            it->ReleaseObjects();
            it->map.Clear();
        }
    }
}
