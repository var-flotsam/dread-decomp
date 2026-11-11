// Unit 2577 of tools/tu_map.py (CSubAreaManager::CTunableSubAreaManager, no reliable name): the sub-area
// manager's request queue, its string-id slots, id lists and tables, the lookups of its objects and the
// reflected helpers that remove entries. Names and layouts are placeholders (the full ones are in
// src/game/CSubAreaManager.cpp).
#include <new>

#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/file/UnkFileDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CChangeStageNavMeshItemComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU2611.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::reflection::UnkReflValue;

struct CSubAreaManager;
struct UnkSubArea;

// ---- fn_7100ee448c: pops the oldest queued request ----
struct UnkReqExtra_b189 {
    u16 field_0x0;
    u8 field_0x2;
};

struct UnkReqSlot_b189 {
    UnkStringData* id;
    UnkReqExtra_b189 extra;

    void ResetId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (id && id != &pool->empty) {
                --id->ref_count;
                if (id->ref_count == 0) {
                    fn_71000803a4(pool, id);
                    pool = g_unk_7101d47108;
                }
            }
            id = &pool->empty;
        }
    }
    UnkReqSlot_b189(UnkReqSlot_b189&& other) {
        id = other.id;
        UnkStringData* empty = &g_unk_7101d47108->empty;
        __builtin_memcpy(&other.id, &empty, sizeof(empty));
        __builtin_memcpy(&extra, &other.extra, 3);
    }
};

struct UnkReqQueue_b189 {
    u8 pad_0x0[0x8];
    UnkReqSlot_b189* read;
    u8 pad_0x10[8];
    u64 count;
    UnkReqSlot_b189 storage[64];
};

UnkReqSlot_b189 fn_7100ee448c(UnkReqQueue_b189* self) {
    UnkReqSlot_b189 r(static_cast<UnkReqSlot_b189&&>(*self->read));
    self->read->ResetId();
    ++self->read;
    if (self->read == self->storage + 64)
        self->read = self->storage;
    --self->count;
    return r;
}

// ---- the two id slots ----
struct UnkMgrIds_b189 {
    u8 pad_0x0[0x9c0];
    CStrId field_0x9c0;
    CStrId field_0x9c8;
    CStrId field_0x9d0;
};

void fn_7100eeb24c(UnkMgrIds_b189* self) {
    self->field_0x9c0 = g_unk_7101d470e0;
    self->field_0x9c8 = self->field_0x9d0;
}

void fn_7100eeb15c(UnkMgrIds_b189* self, const CStrId* id) {
    self->field_0x9c8 = g_unk_7101d470e0;
    self->field_0x9c0 = *id;
}

// ---- fn_7100eeac18 ----
struct UnkFourIds_b189 {
    CStrId field_0x0;
    CStrId field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
};

__attribute__((noinline)) void fn_7100eeac18(UnkFourIds_b189* self) {
    self->field_0x18.Reset();
    self->field_0x10.Reset();
    self->field_0x8.Reset();
    self->field_0x0.Reset();
}

// ---- fn_7100eecd08 ----
struct CSubAreaManager;
void fn_7100ee34f0(CSubAreaManager* self);
void fn_7100ef1c10(void* list, u32 count);

struct UnkMgrReset_b189 {
    u8 pad_0x0[0x990];
    u8 field_0x990[0x28];
    u8 field_0x9b8;
    u8 pad_0x9b9[7];
    CStrId field_0x9c0;
    CStrId field_0x9c8;
    CStrId field_0x9d0;
};

void fn_7100eecd08(UnkMgrReset_b189* self) {
    fn_7100ee34f0(reinterpret_cast<CSubAreaManager*>(self));
    self->field_0x9b8 = 0;
    self->field_0x9c0.Reset();
    self->field_0x9c8.Reset();
    self->field_0x9d0.Reset();
    fn_7100ef1c10(self->field_0x990, 0);
}

// ---- fn_7100ee48ec ----
using UnkPathIdMember_b189 = base::UnkStrIdMemberOf<g_unk_7101d47200>;
extern base::file::UnkFileDevice* g_unk_7101d4d828;
void fn_710008987c(base::file::UnkFileDevice*, const CFilePathStrId*, u32);

struct UnkMgrPaths_b189 {
    u8 pad_0x0[0x3b8];
    base::UnkVector<UnkPathIdMember_b189> field_0x3b8;
};

void fn_7100ee48ec(UnkMgrPaths_b189* self) {
    base::file::UnkFileDevice* device = g_unk_7101d4d828;
    if (!device) {
        device = static_cast<base::file::UnkFileDevice*>(fn_71000839a0(0x1348, 0x3f));
        fn_71000893a8(device);
        g_unk_7101d4d828 = device;
    }
    for (UnkPathIdMember_b189 *it = self->field_0x3b8.data, *end = it + self->field_0x3b8.count; it != end; ++it)
        fn_710008987c(device, reinterpret_cast<const CFilePathStrId*>(it), 0);
    self->field_0x3b8.Clear();
}

// ---- reflected removals of an owned entry by index ----
static inline __attribute__((always_inline)) void* ArgPtr_b189(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

// ---- fn_7100ef0cdc: removes the owned object at an index ----
struct UnkOwnedObject_b189 {
    virtual void vf_0x0();
    virtual void vf_0x8();
};
struct UnkOwnedObjectPtr_b189 {
    union {
        UnkOwnedObject_b189* ptr;
    };
    UnkOwnedObjectPtr_b189(UnkOwnedObjectPtr_b189&& o) {
        UnkOwnedObject_b189* t = o.ptr;
        o.ptr = nullptr;
        ptr = t;
    }
    ~UnkOwnedObjectPtr_b189() {
        UnkOwnedObject_b189* p = ptr;
        ptr = nullptr;
        if (p)
            p->vf_0x8();
    }
};

struct UnkOwnerA_b189 {
    u8 pad_0x0[0x830];
    base::UnkVector<UnkOwnedObjectPtr_b189> field_0x830;
};

static inline void EraseAtA(base::UnkVector<UnkOwnedObjectPtr_b189>* self, u32 index) {
    if (index < self->count) {
        u64 i = index;
        self->data[i].~UnkOwnedObjectPtr_b189();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkOwnedObjectPtr_b189* cur = &self->data[i];
            new (cur) UnkOwnedObjectPtr_b189(static_cast<UnkOwnedObjectPtr_b189&&>(cur[1]));
            (&self->data[i])[1].~UnkOwnedObjectPtr_b189();
        }
        self->count = self->count - 1;
    }
}

UnkReflValue fn_7100ef0cdc(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    UnkOwnerA_b189* owner = *reinterpret_cast<UnkOwnerA_b189**>(obj + 8);
    u32 index = *static_cast<const u32*>(ArgPtr_b189(a));
    bool found;
    if (owner->field_0x830.count > index) {
        EraseAtA(&owner->field_0x830, index);
        found = true;
    } else {
        found = false;
    }
    const base::reflection::CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &found, false);
    return r;
}

// ---- fn_7100ef11f4: the same over the owned id sets ----
struct UnkNamedIds;
void fn_7100ef1e1c(UnkNamedIds* self);
struct UnkNamedIdsPtr_b189 {
    union {
        UnkNamedIds* ptr;
    };
    UnkNamedIdsPtr_b189(UnkNamedIdsPtr_b189&& o) {
        UnkNamedIds* t = o.ptr;
        o.ptr = nullptr;
        ptr = t;
    }
    ~UnkNamedIdsPtr_b189() {
        UnkNamedIds* p = ptr;
        ptr = nullptr;
        if (p) {
            fn_7100ef1e1c(p);
            operator delete(p);
        }
    }
};

struct UnkOwnerB_b189 {
    u8 pad_0x0[0x888];
    base::UnkVector<UnkNamedIdsPtr_b189> field_0x888;
};

static inline void EraseAtB(base::UnkVector<UnkNamedIdsPtr_b189>* self, u32 index) {
    if (index < self->count) {
        u64 i = index;
        self->data[i].~UnkNamedIdsPtr_b189();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkNamedIdsPtr_b189* cur = &self->data[i];
            new (cur) UnkNamedIdsPtr_b189(static_cast<UnkNamedIdsPtr_b189&&>(cur[1]));
            (&self->data[i])[1].~UnkNamedIdsPtr_b189();
        }
        self->count = self->count - 1;
    }
}

UnkReflValue fn_7100ef11f4(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    UnkOwnerB_b189* owner = *reinterpret_cast<UnkOwnerB_b189**>(obj + 8);
    u32 index = *static_cast<const u32*>(ArgPtr_b189(a));
    bool found;
    if (owner->field_0x888.count > index) {
        EraseAtB(&owner->field_0x888, index);
        found = true;
    } else {
        found = false;
    }
    const base::reflection::CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &found, false);
    return r;
}

// ---- fn_7100ef1d24: sets the number of sub-area records ----
struct UnkGeneratorRecord_ovb0120;
struct UnkSubAreaRecord_0x188;
void fn_7100faa3d8(UnkSubAreaRecord_0x188* self);
struct UnkSubAreaRecord_0x188 {
    u8 data[0x188];
    UnkSubAreaRecord_0x188() { fn_7100fa99bc(reinterpret_cast<UnkGeneratorRecord_ovb0120*>(this)); }
    UnkSubAreaRecord_0x188(UnkSubAreaRecord_0x188&& other) {
        fn_7100fab18c(reinterpret_cast<UnkGeneratorRecord_ovb0120*>(this),
                      reinterpret_cast<UnkGeneratorRecord_ovb0120*>(&other));
    }
    ~UnkSubAreaRecord_0x188() { fn_7100faa3d8(this); }
};

void fn_7100ef1d24(base::UnkVector<UnkSubAreaRecord_0x188>* self, u32 n) {
    self->Reserve(n);
    for (u32 i = self->count; i < n; ++i)
        new (&self->data[i]) UnkSubAreaRecord_0x188();
    self->count = n;
}

// ---- the sub-areas' mask updates ----
struct UnkMask {
    u32 bits;
    UnkMask() : bits(0) {}
    UnkMask(u32 b) : bits(b) {}
    UnkMask(const UnkMask& o) : bits(o.bits) {}
};
struct UnkSubArea;
void fn_7100fac120(UnkSubArea* area, UnkMask mask, bool a, bool b, void* c, bool d);
void fn_7100facf04(UnkSubArea* area, UnkMask mask, bool on);

struct UnkSubAreaRec_b189 {
    u8 pad_0x0[0x188];
};
struct UnkMaskMgr_b189 {
    u8 pad_0x0[0x110];
    u32 field_0x110;
    u8 pad_0x114[0x2dc];
    UnkSubAreaRec_b189* field_0x3f0;
    UnkSubAreaRec_b189* field_0x3f8;
    u8 pad_0x400[0x460];
    UnkSubAreaRec_b189* recs;
    u32 count;
    u8 pad_0x86c[0x16c];
    u32 field_0x9d8;
};

struct UnkCameraEvent_b0058 {
    s32 kind;
    u32 unk_0x4;
};

void fn_7100eecf7c(void* manager, bool on, const UnkCameraEvent_b0058* event) {
    UnkMaskMgr_b189* self = static_cast<UnkMaskMgr_b189*>(manager);
    u32 current = self->field_0x9d8;
    u32 bits = event->kind;
    self->field_0x9d8 = !on ? (current & ~bits) : (bits | current);
    if (self->count == 0)
        return;
    for (UnkSubAreaRec_b189* rec = self->recs; rec != self->recs + self->count; ++rec) {
        if (rec != self->field_0x3f0 && (self->field_0x110 == 0 || rec != self->field_0x3f8))
            fn_7100fac120(reinterpret_cast<UnkSubArea*>(rec), UnkMask(event->kind), on, true, self->field_0x3f0,
                          false);
        fn_7100facf04(reinterpret_cast<UnkSubArea*>(rec), UnkMask(event->kind), on);
    }
}

// ---- the object table and its lookups ----
bool fn_7100d65fb0(void*, const base::math::CVector2D*);
struct UnkTracker_71008618cc;
struct UnkReflObject;

struct UnkAreaKey_b189 {
    u8 pad_0x0[0x30];
    u32 lo;
    u32 hi;
};
struct UnkAreaClass_b189 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
};
struct UnkAreaObject_b189 {
    u8 pad_0x0[0x158];
    UnkAreaClass_b189* field_0x158;
};
struct UnkAreaValue_b189 {
    u8 pad_0x0[0x10];
    UnkAreaObject_b189* field_0x10;
};
struct UnkAreaNode_b189 {
    UnkAreaKey_b189* key;
    UnkAreaValue_b189* value;
    u8 pad_0x10[0x10];
    UnkAreaNode_b189* next;
};


struct UnkAreaTracker_b189 {
    u8 pad_0x0[8];
    UnkAreaObject_b189** data;
    u32 count;
};

struct UnkAreaMgr_b189 {
    u8 pad_0x0[0x8e8];
    UnkAreaNode_b189** buckets;  // 0x8e8
    u32 size;                    // 0x8f0
    u32 capacity;                // 0x8f4
    u8 pad_0x8f8[0x38];
    UnkAreaTracker_b189* field_0x930;
};

static inline UnkAreaObject_b189* LookupObject(UnkAreaMgr_b189* self, UnkAreaKey_b189* key) {
    if (self->size == 0)
        return nullptr;
    u32 index = (key->lo ^ key->hi) % self->capacity;
    UnkAreaNode_b189** bucket = self->buckets + index;
    if (!bucket)
        return nullptr;
    UnkAreaNode_b189* n = *bucket;
    if (!n)
        return nullptr;
    // Contains(), then Get(): two walks of the chain.
    UnkAreaNode_b189* probe = n;
    for (; probe; probe = probe->next)
        if (probe->key == key)
            break;
    if (!probe)
        return nullptr;
    while (n->key != key)
        n = n->next;
    UnkAreaValue_b189* v = n->value;
    return v ? v->field_0x10 : nullptr;
}

// ---- fn_7100ee2864 ----
struct UnkAreaEntry_b189 {
    UnkAreaKey_b189* value;
};
UnkAreaObject_b189** fn_7100d5a990(UnkAreaTracker_b189* tracker, UnkAreaObject_b189* object);
void fn_7100d59994(UnkTracker_71008618cc*, UnkReflObject*);

void fn_7100ee2864(UnkAreaMgr_b189* self, UnkAreaEntry_b189* entry) {
    UnkAreaObject_b189* object = LookupObject(self, entry->value);
    if (!object)
        return;
    UnkAreaObject_b189** it = fn_7100d5a990(self->field_0x930, object);
    if (self->field_0x930->data + self->field_0x930->count == it)
        fn_7100d59994(reinterpret_cast<UnkTracker_71008618cc*>(self->field_0x930),
                      reinterpret_cast<UnkReflObject*>(object));
}

// ---- fn_7100eeb730 ----
struct UnkAreaCur_b189 {
    u8 pad_0x0[0xb0];
    UnkAreaKey_b189** keys;
    u32 count;
};
struct UnkAreaMgr2_b189 : UnkAreaMgr_b189 {};

struct UnkAreaMgrB_b189 {
    u8 pad_0x0[0x3f0];
    UnkAreaCur_b189* current;
};

UnkAreaObject_b189* fn_7100eeb730(UnkAreaMgr_b189* self, const base::math::CVector2D* pos) {
    UnkAreaCur_b189* cur = reinterpret_cast<UnkAreaMgrB_b189*>(self)->current;
    if (!cur || cur->count == 0)
        return nullptr;
    for (UnkAreaKey_b189 **it = cur->keys, **end = it + cur->count; it != end; ++it) {
        UnkAreaObject_b189* object = LookupObject(self, *it);
        if (!object)
            continue;
        UnkAreaClass_b189* cls = object->field_0x158;
        char* c = cls->vf_0x18() == 0x10 ? reinterpret_cast<char*>(cls) : nullptr;
        if (fn_7100d65fb0(c + 0xa8, pos))
            return object;
    }
    return nullptr;
}

// ---- fn_7100eecb08: erases the node of a string-id hash map ----
struct UnkHashNode_b189 {
    base::UnkStrIdMember key;
    base::UnkVector<void*> field_0x8;
    // Union members: the original reloads them after each link store.
    union {
        UnkHashNode_b189* next;  // 0x30: list order
    };
    union {
        UnkHashNode_b189* prev;  // 0x38
    };
    UnkHashNode_b189* chain;  // 0x40: bucket chain
};
static_assert(__builtin_offsetof(UnkHashNode_b189, next) == 0x30, "node layout");

struct UnkHashMap_b189 {
    base::UnkAllocator* allocator;
    u32 field_0x8;
    u32 count;
    UnkHashNode_b189** buckets;
    u32 field_0x18;
    u32 bucket_count;
    u8 unk_0x20[0x18];
    UnkHashNode_b189* tail;
    UnkHashNode_b189* head;
};

UnkHashNode_b189* fn_7100eecb08(UnkHashMap_b189* m, const base::UnkStrIdMember* key) {
    base::global::UnkStringData* k = key->data;
    const char* raw = reinterpret_cast<const char*>(k);
    u32 h = *reinterpret_cast<const u32*>(raw + 0x30) ^ *reinterpret_cast<const u32*>(raw + 0x34);
    u32 i = h % m->bucket_count;
    UnkHashNode_b189** slot = m->buckets + i;
    UnkHashNode_b189* n = *slot;
    if (!n)
        return nullptr;
    UnkHashNode_b189* prev = nullptr;
    if (n->key.data != k) {
        do {
            prev = n;
            n = n->chain;
            if (!n)
                return nullptr;
        } while (n->key.data != k);
    }
    *(prev ? &prev->chain : slot) = n->chain;
    if (m->head == n)
        m->head = n->next;
    if (m->tail == n)
        m->tail = n->prev;
    if (n->prev)
        n->prev->next = n->next;
    if (n->next)
        n->next->prev = n->prev;
    UnkHashNode_b189* next = n->next;
    n->field_0x8.Free();
    n->key.~UnkStrIdMemberOf();
    m->allocator->vf_0x48(n);
    --m->count;
    return next;
}

// ---- fn_7100eead58 ----
// The string data's text (0x10) and a use counter (0x44).
struct UnkStrDataView_b189 {
    u8 pad_0x0[0x10];
    const char* text;
    u8 pad_0x18[0x2c];
    // A union member: the use-count store does not keep the ids' data pointers in registers.
    union {
        u32 uses;
    };
};
struct UnkStrIdView_b189 {
    UnkStrDataView_b189* data;
};

// The ids are destroyed by fn_7100eeac18, so they are union members (no destructor of their own).
struct UnkChangeEvent_b189 {
    union {
        CStrId field_0x0;
    };
    union {
        CStrId field_0x8;
    };
    union {
        CStrId field_0x10;
    };
    union {
        CStrId field_0x18;
    };
    u8 field_0x20;
    UnkChangeEvent_b189(const CStrId& a, const CStrId& b, const CStrId& c, const CStrId& d)
        : field_0x0(a), field_0x8(b), field_0x10(c), field_0x18(d), field_0x20(0) {}
    ~UnkChangeEvent_b189() {}
};

struct UnkSubAreaChange_b190;

static inline const char* TextOf(const CStrId& id) {
    const UnkStrIdView_b189& v = reinterpret_cast<const UnkStrIdView_b189&>(id);
    ++v.data->uses;
    return v.data->text ? v.data->text : g_unk_710162e2ed;
}

void fn_7100eead58(CSubAreaManager* self, const CStrId& prev_a, const CStrId& prev_b, const CStrId& cur_a,
                   const CStrId& cur_b) {
    fn_7100000790("CurrentScenario.SubAreaChangeRequest", "ssss", TextOf(prev_a), TextOf(prev_b), TextOf(cur_a),
                  TextOf(cur_b));
    UnkChangeEvent_b189 event(prev_a, prev_b, cur_a, cur_b);
    fn_7100ef1608(reinterpret_cast<char*>(self) + 0x9e0, reinterpret_cast<const UnkSubAreaChange_b190*>(&event));
    fn_7100eeac18(reinterpret_cast<UnkFourIds_b189*>(&event));
}

// ---- fn_7100ee4550 ----
struct UnkIdSet;
struct UnkPoppedRequest;

void fn_7100ee6884(CSubAreaManager* self);

// What the request id's lookup hands back (+0x38 holds the previous state's data).
struct UnkSubareaNamedEntry;
struct UnkSubareaNamedList;
UnkSubareaNamedEntry* fn_7100fab838(UnkSubareaNamedList*, const CStrId*);

struct UnkGroupEntry_b189 {
    u8 pad_0x0[0x10];
    u8 field_0x10[1];
};
struct UnkGroup_b189 {
    u8 pad_0x0[8];
    u64 field_0x8;
    UnkGroupEntry_b189** field_0x10;
    u32 field_0x18;
};
struct UnkGroupVector_b189 {
    UnkGroup_b189** data;
    u32 count;
};
struct UnkPtrList_b189 {
    struct Item {
        u8 pad_0x0[0x10];
        void* field_0x10;
    };
    Item** data;
    u32 count;
};

struct UnkNamedObjectPtr_b190;
UnkNamedObjectPtr_b190* fn_7100eee97c(base::UnkVector<UnkNamedObjectPtr_b190>*, u32);
void fn_7100eee1f8(void*, CStrId*, const CStrId*, bool, bool);
struct UnkSubAreaEntityList_b190;

struct UnkSetCounts_b189 {
    u8 pad_0x0[0x30];
    u32 field_0x30;
    u8 pad_0x34[0x2c];
    u32 field_0x60;
};

struct UnkMgr4550_b189 {
    u8 pad_0x0[0x110];
    u32 field_0x110;
    u8 pad_0x114[0x44];
    u32 field_0x158[1];  // the id set (+0x158)
    u8 pad_0x15c[0x294];
    UnkSubArea* field_0x3f0;
    UnkSubArea* field_0x3f8;
    UnkSubArea* field_0x400;
    bool field_0x408;
    bool field_0x409;
    u8 pad_0x40a[0x426];
    UnkGroupVector_b189 field_0x830;
};

static inline u32 CountAt(UnkMgr4550_b189* self, u32 off) {
    return *reinterpret_cast<u32*>(reinterpret_cast<char*>(self) + off);
}

void fn_7100ee4550(CSubAreaManager* manager, const UnkPoppedRequest* request, bool a, bool b) {
    UnkMgr4550_b189* self = reinterpret_cast<UnkMgr4550_b189*>(manager);
    if (self->field_0x3f0) {
        UnkSubArea* found = reinterpret_cast<UnkSubArea*>(
            fn_7100fab838(reinterpret_cast<UnkSubareaNamedList*>(self->field_0x3f0), reinterpret_cast<const CStrId*>(request)));
        if (found && self->field_0x110 == 0) {
            fn_7100ee8044(manager, reinterpret_cast<char*>(self->field_0x3f0) + 0x38, found,
                          reinterpret_cast<UnkIdSet*>(reinterpret_cast<char*>(self) + 0x158));
            if (CountAt(self, 0x188) != 0 || CountAt(self, 0x160) != 0 || CountAt(self, 0x1b0) != 0 ||
                CountAt(self, 0x1d8) != 0 || CountAt(self, 0x228) != 0 || CountAt(self, 0x200) != 0) {
                UnkSubArea* cur = self->field_0x3f0;
                self->field_0x400 = found;
                self->field_0x408 = a;
                self->field_0x409 = b;
                self->field_0x3f8 = cur;
                fn_7100ee6884(manager);
                return;
            }
        }
    }
    if (self->field_0x830.count) {
        const CStrId* id = reinterpret_cast<const CStrId*>(request);
        for (u32 i = 0; i < self->field_0x830.count; ++i) {
            UnkGroup_b189* group = *reinterpret_cast<UnkGroup_b189**>(
                fn_7100eee97c(reinterpret_cast<base::UnkVector<UnkNamedObjectPtr_b190>*>(&self->field_0x830), i));
            if (group->field_0x8 == reinterpret_cast<const u64&>(*id)) {
                for (u64 j = 0; j < group->field_0x18; ++j)
                    fn_7100eee1f8(manager, reinterpret_cast<CStrId*>(group->field_0x10[j]->field_0x10), id, a, b);
                break;
            }
        }
    }
    UnkPtrList_b189* list = reinterpret_cast<UnkPtrList_b189*>(fn_71009ba31c());
    for (UnkPtrList_b189::Item **p = list->data, **e = p + list->count; p != e; ++p) {
        UnkPtrList_b189::Item* item = *p;
        fn_71009b9ee8(item ? item->field_0x10 : nullptr);
    }
}

// ---- fn_7100eecca0 ----
using base::global::UnkStrIdBase;

extern UnkStrIdBase g_unk_71729daa38[/* 9; unsized: x86-64 would assume 16-byte alignment */];

// The result is a done flag in w0; the existing declarations (src/game/UnkSubareaSetupState.cpp) return void.
bool TryAcquire(UnkGameManagerSub_0x608* self, u32 kind, const CStrId& id, bool arg3) asm("_Z15fn_71011a32d8P25UnkGameManagerSub_0x608jRKN4base6global5CStrIdEb");
bool TryRelease(UnkGameManagerSub_0x608* self, u32 kind, const CStrId& id) asm("_Z15fn_71011a3574P25UnkGameManagerSub_0x608jRKN4base6global5CStrIdE");

void fn_7100eecca0(void*, bool enable) {
    UnkGameManagerSub_0x608* p = g_unk_7101d47310->field_0x608;
    const CStrId& id = reinterpret_cast<const CStrId&>(g_unk_71729daa38[4]);
    if (enable) {
        while (!TryAcquire(p, 0, id, false)) {
        }
    } else {
        while (!TryRelease(p, 0, id)) {
        }
    }
}

// ---- fn_7100ee2bd8: collects the sub-area records of the objects at a position ----
struct UnkRecB_b189 {
    u8 pad_0x0[0x105];
    u8 field_0x105;
};

struct UnkObjClassB_b189 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
};

struct UnkObjB_b189 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual u32 vf_0x58();
    u8 pad_0x8[0x18];
    CStrId field_0x20;
    u8 pad_0x28[0x130];
    UnkObjClassB_b189* field_0x158;
};

struct UnkObjEntryB_b189 {
    u8 pad_0x0[0x10];
    UnkObjB_b189* object;
};

struct UnkObjListB_b189 {
    u8 pad_0x0[8];
    UnkObjEntryB_b189** data;
    u32 count;
};

struct UnkObjMgrB_b189 {
    u8 pad_0x0[0x930];
    UnkObjListB_b189* field_0x930;
};

UnkRecB_b189* fn_7100ee2d84(UnkObjMgrB_b189* self, const CStrId* id);

bool fn_7100ee2bd8(UnkObjMgrB_b189* self, base::UnkVector<UnkRecB_b189*>* out, const base::math::CVector2D* pos,
                   u32 mask, UnkObjB_b189* skip) {
    bool any = false;
    UnkObjListB_b189* list = self->field_0x930;
    for (UnkObjEntryB_b189 **it = list->data, **end = it + list->count; it != end; ++it) {
        UnkObjEntryB_b189* e = *it;
        UnkObjB_b189* o = e->object;
        u32 flags = o->vf_0x58() & mask;
        if (o == skip || flags == 0)
            continue;
        UnkObjClassB_b189* cls = o->field_0x158;
        char* c = cls->vf_0x18() == 0x10 ? reinterpret_cast<char*>(cls) : nullptr;
        if (!fn_7100d65fb0(c + 0xa8, pos))
            continue;
        UnkRecB_b189* r = fn_7100ee2d84(self, &o->field_0x20);
        if (r && r->field_0x105 == 0) {
            out->PushBack(r);
            any = true;
        }
    }
    return any;
}
