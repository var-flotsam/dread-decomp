// CBTObserverComponent's unit (tools/tu_map.py unit 2216): the observer object's containers and its
// set-up and tear-down (its constructor is fn_7100ca6844 in src/game/CBTObserverComponent.cpp).
// - The keyed entries at +0x128 (0x80-byte entries of three vectors and a flag, the first holding
//   slots that point back at the entry): clearing them (fn_7100ca720c), moving one (fn_7100ca9380)
//   and growing the arrays (fn_7100ca9158).
// - The map of observer groups at +0x8 (string id -> vector of observers, chained in buckets and
//   in a list): clearing it (fn_7100ca7900), adding a group (fn_7100ca7510, fn_7100caa690) and
//   removing one (fn_7100caa83c).
// - The variant of a string id and a vector, held or referenced: copy-assignment (fn_7100ca9c0c),
//   construction from another (fn_7100caa14c) and move-assignment (fn_7100ca9dc8).
// - The observers by id at +0x90: adding one (fn_7100ca6f00), updating them (fn_7100ca6e64),
//   notifying one (fn_7100ca7388) and dropping them all (fn_7100ca6dd0).
// - The destructor (fn_7100ca6964) and the set-up of the block models and callbacks (fn_7100ca6ba0).
// Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/UnkPairArrays.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CBTObserverComponent.h"
#include "game/CBreakableTileGroup.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU127.h"
#include "game/UnkTU253.h"
#include "game/UnkTU2566.h"
#include "game/UnkTU372.h"
#include "game/UnkTU883.h"
#include "types.h"

using base::UnkStrIdMember;
using base::global::CFilePathStrId;
using base::global::CStrId;

// ---- The keyed entries ----

struct UnkBTObserverEntry_ovb0055 {
    base::UnkVector<void**> links;  // 0x0: slots that point back at the entry
    base::UnkVector<void*> field_0x28;
    base::UnkVector<void*> field_0x50;
    bool field_0x78;

    ~UnkBTObserverEntry_ovb0055() {
        for (void*** it = links.data, ***end = links.data + links.count; it != end; ++it)
            **it = nullptr;
        links.count = 0;
        field_0x28.count = 0;
        field_0x50.count = 0;
        field_0x50.Free();
        field_0x28.Free();
        links.Free();
    }
};
static_assert(sizeof(UnkBTObserverEntry_ovb0055) == 0x80, "UnkBTObserverEntry_ovb0055 layout");

struct UnkBTObserverEntries_ovb0055 {
    UnkBTObserverEntry_ovb0055* values;
    u32* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

__attribute__((noinline)) void fn_7100ca720c(UnkBTObserverEntries_ovb0055* self) {
    if (self->values) {
        for (u64 i = 0; i < self->count; ++i)
            self->values[i].~UnkBTObserverEntry_ovb0055();
    }
    self->count = 0;
}

// Constructs `self` from `other`, pointing other's back-pointing slots at `self`.
__attribute__((noinline)) void fn_7100ca9380(UnkBTObserverEntry_ovb0055* self, UnkBTObserverEntry_ovb0055* other) {
    new (&self->links) base::UnkVector<void**>(0xf);
    new (&self->field_0x28) base::UnkVector<void*>(0xf);
    new (&self->field_0x50) base::UnkVector<void*>(0xf);
    self->field_0x78 = other->field_0x78;
    for (void*** it = other->links.data, ***end = other->links.data + other->links.count; it != end; ++it) {
        void** slot = *it;
        // Stored untyped: the original reloads the count afterwards.
        __builtin_memcpy(slot, &self, sizeof(self));
        self->links.PushBack(slot);
    }
    other->links.count = 0;
    other->field_0x28.count = 0;
    other->field_0x50.count = 0;
}

void fn_7100ca9158(base::UnkPairArrays* pairs, u32 capacity) {
    auto* self = reinterpret_cast<UnkBTObserverEntries_ovb0055*>(pairs);
    if (self->capacity >= capacity)
        return;
    auto* values = static_cast<UnkBTObserverEntry_ovb0055*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkBTObserverEntry_ovb0055), 8, self->flags));
    auto* ids = static_cast<u32*>(self->allocator->vf_0x40((u64)capacity * sizeof(u32), 8, self->flags));
    if (self->values) {
        UnkBTObserverEntry_ovb0055* src = self->values;
        UnkBTObserverEntry_ovb0055* dst = values;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            fn_7100ca9380(dst, src);
            src->~UnkBTObserverEntry_ovb0055();
        }
        self->allocator->vf_0x48(self->values);
    }
    if (self->ids) {
        __builtin_memcpy(ids, self->ids, (u64)self->count * sizeof(u32));
        self->allocator->vf_0x48(self->ids);
    }
    self->values = values;
    self->ids = ids;
    self->capacity = capacity;
}

// ---- The map of observer groups ----

struct UnkHashedStr_ovb0055 {
    u8 unk_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

// A group's vector of observers (copied element by element as raw words).
struct UnkObserverIdList_ovb0055 {
    void** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkObserverIdList_ovb0055(const UnkObserverIdList_ovb0055& other)
        : data(nullptr), count(0), capacity(0), step(other.step), field_0x14(other.field_0x14), flags(other.flags),
          allocator(other.allocator) {
        void** p = nullptr;
        if (u32 n = other.capacity) {
            p = static_cast<void**>(allocator->vf_0x40((u64)n * sizeof(void*), 8, flags));
            if (data) {
                __builtin_memcpy(p, data, (u64)count * sizeof(void*));
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
        __builtin_memcpy(p, other.data, (u64)other.count * sizeof(void*));
        count = other.count;
    }

    UnkObserverIdList_ovb0055& operator=(const UnkObserverIdList_ovb0055& other) {
        if (this != &other) {
            count = 0;
            u32 n = other.capacity;
            void** p;
            if (capacity < n) {
                p = static_cast<void**>(allocator->vf_0x40((u64)n * sizeof(void*), 8, flags));
                if (data) {
                    __builtin_memcpy(p, data, (u64)count * sizeof(void*));
                    allocator->vf_0x48(data);
                }
                data = p;
                capacity = n;
            } else {
                p = data;
            }
            __builtin_memcpy(p, other.data, (u64)other.count * sizeof(void*));
            count = other.count;
        }
        return *this;
    }

    void Free() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkObserverIdNode_ovb0055 {
    UnkStrIdMember id;
    UnkObserverIdList_ovb0055 items;         // 0x8
    UnkObserverIdNode_ovb0055* next;         // 0x30
    UnkObserverIdNode_ovb0055* prev;         // 0x38
    UnkObserverIdNode_ovb0055* bucket_next;  // 0x40
};

struct UnkObserverIdMap_ovb0055 {
    base::UnkAllocator* allocator;        // 0x0
    u32 flags;                            // 0x8
    u32 count;                            // 0xc
    UnkObserverIdNode_ovb0055** buckets;  // 0x10
    u32 field_0x18;
    u32 bucket_count;  // 0x1c
    u8 unk_0x20[0x18];
    UnkObserverIdNode_ovb0055* tail;  // 0x38
    UnkObserverIdNode_ovb0055* head;  // 0x40
};

__attribute__((noinline)) void fn_7100ca7900(UnkObserverIdMap_ovb0055* self) {
    if (self->count) {
        for (UnkObserverIdNode_ovb0055* node = self->head; node;) {
            UnkObserverIdNode_ovb0055* next = node->next;
            node->items.Free();
            node->id.~UnkStrIdMemberOf();
            self->allocator->vf_0x48(node);
            node = next;
        }
        __builtin_memset(self->buckets, 0, (u64)self->bucket_count * sizeof(void*));
        self->count = 0;
        self->tail = nullptr;
        self->head = nullptr;
    }
}

UnkObserverIdNode_ovb0055* fn_7100caa690(UnkObserverIdMap_ovb0055* map, const CStrId* key,
                                         const UnkObserverIdList_ovb0055* value) {
    UnkObserverIdNode_ovb0055** slot = nullptr;
    if (map->field_0x18) {
        const auto* h = reinterpret_cast<const UnkHashedStr_ovb0055*>(key->data);
        u32 index = (h->hash_lo ^ h->hash_hi) % map->bucket_count;
        slot = map->buckets + index;
    }
    UnkObserverIdNode_ovb0055* first = *slot;
    auto* node = static_cast<UnkObserverIdNode_ovb0055*>(map->allocator->vf_0x40(0x48, 8, map->flags));
    if (node) {
        new (&node->id) CStrId(*key);
        new (&node->items) UnkObserverIdList_ovb0055(*value);
        node->next = nullptr;
        node->prev = nullptr;
        node->bucket_next = nullptr;
    }
    node->bucket_next = first;
    *slot = node;
    if (map->head) {
        if (map->tail) {
            // Stored untyped: the original reloads the tail afterwards.
            __builtin_memcpy(&node->prev, &map->tail, sizeof(node->prev));
            map->tail->next = node;
        }
    } else {
        map->head = node;
    }
    map->tail = node;
    ++map->count;
    return node;
}

// Removes the group `key` names; returns the group after it in the list.
UnkObserverIdNode_ovb0055* fn_7100caa83c(UnkObserverIdMap_ovb0055* map, const CStrId* key) {
    const auto* h = reinterpret_cast<const UnkHashedStr_ovb0055*>(key->data);
    u32 index = (h->hash_lo ^ h->hash_hi) % map->bucket_count;
    UnkObserverIdNode_ovb0055** slot = map->buckets + index;
    UnkObserverIdNode_ovb0055* prev = nullptr;
    UnkObserverIdNode_ovb0055* node = *slot;
    if (!node)
        return nullptr;
    if (node->id.data != key->data) {
        do {
            prev = node;
            node = node->bucket_next;
            if (!node)
                return nullptr;
        } while (node->id.data != key->data);
    }
    (prev ? prev->bucket_next : *slot) = node->bucket_next;
    if (map->head == node)
        map->head = node->next;
    // The links are stored untyped: the original reloads them after each store.
    if (map->tail == node)
        __builtin_memcpy(&map->tail, &node->prev, sizeof(map->tail));
    if (node->prev)
        __builtin_memcpy(&node->prev->next, &node->next, sizeof(node->next));
    UnkObserverIdNode_ovb0055* next = nullptr;
    if (node->next) {
        __builtin_memcpy(&node->next->prev, &node->prev, sizeof(node->prev));
        next = node->next;
    }
    node->items.Free();
    node->id.~UnkStrIdMemberOf();
    map->allocator->vf_0x48(node);
    --map->count;
    return next;
}

// The same map seen from its owner (+0x8), adding an empty group: the node is the key, then the
// group's vector (returned), then the links.
struct UnkObserverGroupKey_ovb0055 {
    CStrId id;
    explicit UnkObserverGroupKey_ovb0055(const CStrId& key) : id(key) {}
};

struct UnkObserverGroupItems_ovb0055 {
    u8* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    explicit UnkObserverGroupItems_ovb0055(base::UnkAllocator* allocator_)
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(0xf), allocator(allocator_) {}
};

struct UnkObserverGroupNode_ovb0055 : UnkObserverGroupKey_ovb0055, UnkObserverGroupItems_ovb0055 {
    UnkObserverGroupNode_ovb0055* next;         // 0x30
    UnkObserverGroupNode_ovb0055* prev;         // 0x38
    UnkObserverGroupNode_ovb0055* bucket_next;  // 0x40

    UnkObserverGroupNode_ovb0055(const CStrId& key, base::UnkAllocator* allocator)
        : UnkObserverGroupKey_ovb0055(key), UnkObserverGroupItems_ovb0055(allocator), next(nullptr), prev(nullptr),
          bucket_next(nullptr) {}
};

struct UnkObserverGroupMap_ovb0055 {
    base::UnkAllocator* allocator;           // 0x0
    u32 flags;                               // 0x8
    u32 count;                               // 0xc
    UnkObserverGroupNode_ovb0055** buckets;  // 0x10
    u32 field_0x18;
    u32 bucket_count;  // 0x1c
    u8 unk_0x20[0x18];
    UnkObserverGroupNode_ovb0055* tail;  // 0x38
    UnkObserverGroupNode_ovb0055* head;  // 0x40
};

struct UnkObserverGroupOwner_ovb0055 {
    u8 unk_0x0[0x8];
    UnkObserverGroupMap_ovb0055 map;  // 0x8
};

UnkObserverGroupItems_ovb0055* fn_7100ca7510(UnkObserverGroupOwner_ovb0055* self, const CStrId* key) {
    UnkObserverGroupMap_ovb0055& map = self->map;
    base::UnkAllocator* allocator = fn_7100083990();
    UnkObserverGroupNode_ovb0055** slot = nullptr;
    if (map.field_0x18) {
        const auto* h = reinterpret_cast<const UnkHashedStr_ovb0055*>(key->data);
        u32 index = (h->hash_lo ^ h->hash_hi) % map.bucket_count;
        slot = map.buckets + index;
    }
    UnkObserverGroupNode_ovb0055* first = *slot;
    auto* node = static_cast<UnkObserverGroupNode_ovb0055*>(map.allocator->vf_0x40(0x48, 8, map.flags));
    if (node)
        new (node) UnkObserverGroupNode_ovb0055(*key, allocator);
    node->bucket_next = first;
    *slot = node;
    if (map.head) {
        if (map.tail) {
            // Stored untyped: the original reloads the tail afterwards.
            __builtin_memcpy(&node->prev, &map.tail, sizeof(node->prev));
            map.tail->next = node;
        }
    } else {
        map.head = node;
    }
    map.tail = node;
    ++map.count;
    return node;
}

// ---- The variant of a string id and a vector: held (state 1) or referenced (state 2) ----

struct UnkIdListVariant_ovb0055 {
    struct Named {
        UnkStrIdMember id;
        UnkObserverIdList_ovb0055 list;
    };
    union {
        Named named;
        struct {
            UnkStrIdMember* id;
            UnkObserverIdList_ovb0055* list;
        } refs;
    };
    s32 state;  // 0x30

    UnkStrIdMember& Id() { return state == 2 ? *refs.id : named.id; }
    const UnkStrIdMember& Id() const { return state == 2 ? *refs.id : named.id; }
    UnkObserverIdList_ovb0055& List() { return state == 2 ? *refs.list : named.list; }
    const UnkObserverIdList_ovb0055& List() const { return state == 2 ? *refs.list : named.list; }
};

UnkIdListVariant_ovb0055* fn_7100ca9c0c(UnkIdListVariant_ovb0055* self, const UnkIdListVariant_ovb0055* other) {
    if (other != self) {
        self->Id() = other->Id();
        self->List() = other->List();
    }
    return self;
}

// Takes the other variant's id and copies its list.
void fn_7100caa14c(UnkIdListVariant_ovb0055* self, UnkIdListVariant_ovb0055* other) {
    self->state = 0;
    switch (other->state) {
    case 1:
        self->state = 1;
        new (&self->named.id) UnkStrIdMember(static_cast<UnkStrIdMember&&>(other->named.id));
        new (&self->named.list) UnkObserverIdList_ovb0055(other->named.list);
        break;
    case 2: {
        self->state = 1;
        UnkStrIdMember* id = other->refs.id;
        UnkObserverIdList_ovb0055* list = other->refs.list;
        new (&self->named.id) UnkStrIdMember(static_cast<UnkStrIdMember&&>(*id));
        new (&self->named.list) UnkObserverIdList_ovb0055(*list);
        break;
    }
    }
}

// The same variant over the engine's vector, for move-assignment.
struct UnkIdVecVariant_ovb0055 {
    union {
        struct {
            UnkStrIdMember id;
            base::UnkVector<void*> vec;
        } named;
        struct {
            UnkStrIdMember* id;
            base::UnkVector<void*>* vec;
        } refs;
    };
    s32 state;  // 0x30

    UnkIdVecVariant_ovb0055() {}
    ~UnkIdVecVariant_ovb0055() {}

    UnkStrIdMember& Id() { return state == 2 ? *refs.id : named.id; }
    base::UnkVector<void*>& Vec() { return state == 2 ? *refs.vec : named.vec; }
};

// Releases `to`, then takes `from`'s id, leaving the empty string (stored untyped: the original
// reloads the variant's state afterwards).
static inline void MoveId(UnkStrIdMember& to, UnkStrIdMember& from) {
    to.~UnkStrIdMemberOf();
    to.data = from.data;
    base::global::UnkStringData* empty = &g_unk_7101d47108->empty;
    __builtin_memcpy(&from.data, &empty, sizeof(empty));
}

UnkIdVecVariant_ovb0055* fn_7100ca9dc8(UnkIdVecVariant_ovb0055* self, UnkIdVecVariant_ovb0055* other) {
    switch (other->state) {
    case 1:
        MoveId(self->Id(), other->named.id);
        self->Vec() = static_cast<base::UnkVector<void*>&&>(other->named.vec);
        break;
    case 2: {
        UnkStrIdMember* id = other->refs.id;
        MoveId(self->Id(), *id);
        self->Vec() = static_cast<base::UnkVector<void*>&&>(*other->refs.vec);
        break;
    }
    }
    return self;
}

// ---- The observers by id ----

struct UnkBTObserver_ovb0055 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    u8 unk_0x8[0xc];
    bool enabled;  // 0x14
};

struct UnkBreakableTileGroupData;
void fn_7100ca07a8(UnkBreakableTileGroupData* self);
void fn_7100ca06f0(UnkBTObserver_ovb0055* observer);
void fn_7100ca08b0(UnkBTObserver_ovb0055* observer, u32 id);
void fn_7100ca0aec(UnkBTObserver_ovb0055* observer);
void fn_7100ca0a84(UnkBTObserver_ovb0055* observer, f32 dt);

struct UnkObserverById_ovb0055 {
    u8 unk_0x0[0x90];
    UnkBTObserver_ovb0055** values;  // 0x90
    u32* ids;                        // 0x98
    u32 count;                       // 0xa0
    u32 capacity;                    // 0xa4
    u32 step;                        // 0xa8
    u32 flags;                       // 0xac
    base::UnkAllocator* allocator;   // 0xb0
    u32 next_id;                     // 0xb8
};

// Creates an observer under `id`, or under the next free id when it is 0; returns the id.
u32 fn_7100ca6f00(UnkObserverById_ovb0055* self, u32 id) {
    u32 next = id + 1;
    if (self->next_id > next)
        next = self->next_id;
    if (id == 0)
        id = next++;
    self->next_id = next;
    auto* observer = static_cast<UnkBTObserver_ovb0055*>(fn_71000839a0(0xe0, 5));
    fn_7100ca06f0(observer);
    fn_7100ca08b0(observer, id);
    u32 n = self->count;
    if (n == self->capacity) {
        u32 capacity = self->step + n;
        if (n < capacity) {
            auto* values = static_cast<UnkBTObserver_ovb0055**>(
                self->allocator->vf_0x40((u64)capacity * sizeof(void*), 8, self->flags));
            auto* ids = static_cast<u32*>(self->allocator->vf_0x40((u64)capacity * sizeof(u32), 8, self->flags));
            if (self->values) {
                __builtin_memcpy(values, self->values, (u64)self->count * sizeof(void*));
                self->allocator->vf_0x48(self->values);
            }
            if (self->ids) {
                __builtin_memcpy(ids, self->ids, (u64)self->count * sizeof(u32));
                self->allocator->vf_0x48(self->ids);
            }
            n = self->count;
            self->values = values;
            self->ids = ids;
            self->capacity = capacity;
        }
    }
    self->ids[n] = id;
    __builtin_memcpy(&self->values[self->count], &observer, sizeof(observer));
    ++self->count;
    return id;
}

struct UnkObserverPlayerState_ovb0055 {
    u8 unk_0x0[0x70];
    s32 state;  // 0x70
};

struct UnkObserverPlayer_ovb0055 {
    u8 unk_0x0[0x1b8];
    UnkObserverPlayerState_ovb0055* state;  // 0x1b8
};

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);

// While the player is in a state other than 2, updates the enabled observers and resets the others.
void fn_7100ca6e64(UnkObserverById_ovb0055* self, f32 dt) {
    auto* player = reinterpret_cast<UnkObserverPlayer_ovb0055*>(fn_7100dec788(g_unk_7101d47310));
    if (!player || !player->state || player->state->state == 2)
        return;
    for (UnkBTObserver_ovb0055** it = self->values, **end = self->values + self->count; it != end; ++it) {
        if ((*it)->enabled)
            fn_7100ca0a84(*it, dt);
        else
            fn_7100ca0aec(*it);
    }
}

struct UnkQueryObj;
struct UnkMapData;
struct UnkMinimapOwner;
void fn_7100e9d1f8(UnkMinimapOwner* self, s32 kind);
UnkQueryObj* fn_7100ca1e50(UnkBTObserver_ovb0055* observer, u32 a, u32 b);

struct UnkObserverListener_ovb0055 {
    virtual void vf_0x0(u32 id, u32 a, u32 b);
};

struct UnkObserverNotify_ovb0055 {
    u8 unk_0x0[0x90];
    UnkBTObserver_ovb0055** values;  // 0x90
    u32* ids;                        // 0x98
    u32 count;                       // 0xa0
    u8 unk_0xa4[0x2c];
    UnkObserverListener_ovb0055* listener;  // 0xd0

    UnkBTObserver_ovb0055** FindSlot(u32 id) {
        for (u64 i = 0; i < count; ++i) {
            if (ids[i] == id)
                return &values[i];
        }
        return &values[count];
    }
};

// Tells the listener, then the observer `id` names; a hit it reports is passed on to the map.
void fn_7100ca7388(UnkGameManagerSub_0x1af8* manager, u32 id, u32 a, u32 b) {
    auto* self = reinterpret_cast<UnkObserverNotify_ovb0055*>(manager);
    if (self->listener)
        self->listener->vf_0x0(id, a, b);
    UnkBTObserver_ovb0055** slot = self->FindSlot(id);
    if (slot == &self->values[self->count] || !*slot)
        return;
    if (UnkQueryObj* hit = fn_7100ca1e50(*slot, a, b))
        fn_7100e9d1f8(reinterpret_cast<UnkMinimapOwner*>(fn_7100e8d8b0()), fn_7100c9e960(hit));
}

struct UnkObserverReset_ovb0055 {
    u8 unk_0x0[0x8];
    UnkObserverIdMap_ovb0055 groups;  // 0x8
    u8 unk_0x50[0x40];
    UnkBTObserver_ovb0055** observers;  // 0x90
    u8 unk_0x98[0x8];
    u32 observer_count;  // 0xa0
    u8 unk_0xa4[0x14];
    u32 next_id;  // 0xb8
    u8 unk_0xbc[0x6c];
    UnkBTObserverEntries_ovb0055 entries;  // 0x128
};

void fn_7100ca6dd0(UnkObserverReset_ovb0055* self) {
    fn_7100ca7900(&self->groups);
    for (UnkBTObserver_ovb0055** it = self->observers, **end = self->observers + self->observer_count; it != end;
         ++it) {
        UnkBTObserver_ovb0055* observer = *it;
        fn_7100ca07a8(reinterpret_cast<UnkBreakableTileGroupData*>(observer));
        if (observer)
            observer->vf_0x10();
    }
    self->observer_count = 0;
    self->next_id = 1;
    fn_7100ca720c(&self->entries);
}

// ---- Destruction and set-up ----

struct UnkLinkList;
struct UnkCallback;
void fn_71005c4828(void*, void**);

// The group map's buckets and list (destroyed last) ...
struct UnkObserverGroupsBase_ovb0055 {
    UnkObserverIdMap_ovb0055 map;

    __attribute__((always_inline)) ~UnkObserverGroupsBase_ovb0055() {
        fn_7100ca7900(&map);
        reinterpret_cast<base::UnkVector<void*>*>(&map.buckets)->Free();
    }
};

// ... and the pool its nodes come from.
struct UnkObserverGroups_ovb0055 : UnkObserverGroupsBase_ovb0055 {
    base::UnkBlockPool nodes;  // 0x48

    __attribute__((always_inline)) ~UnkObserverGroups_ovb0055() {
        nodes.field_0x3c = true;
        fn_7100ca7900(&map);
        map.allocator = nullptr;
    }
};

// A callback whose link is dropped from its list on destruction.
template <class Link, void (*Release)(Link*, Link**)>
struct UnkObserverCallback_ovb0055 {
    Link* link;
    void* object;
    void (*function)();

    __attribute__((always_inline)) ~UnkObserverCallback_ovb0055() {
        if (link) {
            Release(link, &link);
            link = nullptr;
        }
        object = nullptr;
        function = nullptr;
    }
};

struct UnkObserverPairs_ovb0055 : base::UnkPairArrays {
    __attribute__((always_inline)) ~UnkObserverPairs_ovb0055() {
        count = 0;
        if (values)
            allocator->vf_0x48(values);
        if (keys)
            allocator->vf_0x48(keys);
    }
};

struct UnkObserverEntryPairs_ovb0055 : base::UnkPairArrays {
    __attribute__((always_inline)) ~UnkObserverEntryPairs_ovb0055() {
        fn_7100ca720c(reinterpret_cast<UnkBTObserverEntries_ovb0055*>(this));
        if (values)
            allocator->vf_0x48(values);
        if (keys)
            allocator->vf_0x48(keys);
    }
};

struct UnkBTObserverComponent_ovb0055 {
    char* vtable;
    UnkObserverGroups_ovb0055 groups;    // 0x8
    UnkObserverPairs_ovb0055 observers;  // 0x90
    u32 field_0xb8;
    u64 field_0xc0;
    u32 field_0xc8;
    u64 field_0xd0;
    u32 field_0xd8;
    bool field_0xdc;
    UnkObserverCallback_ovb0055<UnkLinkList, fn_71002b46ec> callback_0xe0;
    UnkObserverCallback_ovb0055<void, fn_71005c4828> callback_0xf8;
    UnkObserverCallback_ovb0055<void, fn_71005c4828> callback_0x110;
    UnkObserverEntryPairs_ovb0055 entries;  // 0x128

    __attribute__((always_inline)) ~UnkBTObserverComponent_ovb0055() { vtable = g_unk_7101aa52b0 + 0x10; }
};
static_assert(sizeof(UnkBTObserverComponent_ovb0055) == 0x150, "UnkBTObserverComponent_ovb0055 layout");

void fn_7100ca6964(UnkBTObserverComponent_ovb0055* self) {
    self->~UnkBTObserverComponent_ovb0055();
}

struct UnkModelData;
struct UnkModel {
    u8 pad_0x0[0x138];
    UnkModelData* field_0x138;
};

void fn_71006cae2c(UnkRenderPool* pool, const CFilePathStrId& path, UnkModel** out, base::global::CStrId* name);
extern CFilePathStrId g_unk_7172796d70[10];  // the breakable block models' paths
struct UnkTileEventSet;
extern UnkTileEventSet g_unk_7172795e08;  // the breakable tiles' event set
extern void* g_unk_7101d47360;
void fn_7100ee1f34(void*);

struct UnkObserverSetUp_ovb0055 {
    u8 unk_0x0[0xc0];
    UnkModel** models;  // 0xc0
    u32 model_count;    // 0xc8
    u8 unk_0xcc[0x14];
    UnkLinkList* link_0xe0;
    u8 unk_0xe8[0x10];
    void* link_0xf8;
    u8 unk_0x100[0x10];
    void* link_0x110;
};

// The manager behind g_unk_7101d47360, created on first use.
static inline void* CallbackManager() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = manager;
    }
    return manager;
}

static inline void Attach(void** link, void* manager, u32 offset) {
    if (*link) {
        fn_71005c4828(*link, link);
        *link = nullptr;
    }
    void* list = static_cast<char*>(manager) + offset;
    *link = list;
    fn_71005c41bc(list, reinterpret_cast<UnkCallback*>(link));
}

// Loads the ten block models and hooks up the three callbacks.
void fn_7100ca6ba0(UnkObserverSetUp_ovb0055* self) {
    self->models = static_cast<UnkModel**>(fn_7100083ba0(0x50, 0x1e));
    for (u64 i = 0; i < 10; ++i) {
        UnkModel* model;
        CFilePathStrId path(g_unk_7172796d70[i]);
        UnkRenderManager* manager = g_unk_7101d47300;
        UnkModel** slot = &self->models[i];
        model = nullptr;
        fn_71006cae2c(manager->field_0x218, path, &model, &g_unk_7101d470e0);
        fn_7100219ae0(manager, model ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<char*>(model) + 8) : nullptr,
                      0);
        *slot = model;
        fn_71001c0968(self->models[i]->field_0x138, 1);
        fn_71001436a0(self->models[i], 0);
    }
    self->model_count = 10;
    UnkLinkList** link = &self->link_0xe0;
    if (*link) {
        fn_71002b46ec(*link, link);
        *link = nullptr;
    }
    UnkLinkList* events = reinterpret_cast<UnkLinkList*>(&g_unk_7172795e08);
    *link = events;
    fn_71002b4538(events, link);
    Attach(&self->link_0xf8, CallbackManager(), 0xa60);
    Attach(&self->link_0x110, CallbackManager(), 0x9e0);
}
