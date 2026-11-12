// CCollisionComponent's unit (tools/tu_map.py unit 1319): the component's records (+0x48), collider
// groups (+0x70) and registered objects (+0x98): construction of the record array, destruction,
// clearing, loading from the collision file named by the CCharClassCollisionComponent, refreshing
// records and objects, collider-state tracks, flag propagation to groups, unregistering an object,
// placing a record's object, 2D bounds and the overlap visitor. Names and layouts are placeholders.
#include <new>

#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CCollisionComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU112.h"
#include "game/UnkTU2505.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/UnkTU54.h"
#include "game/UnkTU58.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CBroadphaseObject.h"
#include "game/logic/collision/CCollider.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;
using base::reflection::CClass;
using base::global::CStrId;
using base::global::CRntString;

// Refreshing every record through vf_0x170 (or clearing its +0x1c) and resetting every registered object.
UnkReflObject*& fn_7100862088(base::UnkVector<UnkReflObject*>* self, u32 index);
struct UnkColliderNode_A6_b70;
void fn_7100860c64(UnkCollisionContainer* self);

typedef void (*UnkCollisionRefresh_ovb0107)(UnkCollisionContainer* self, UnkCollisionRecord* record, s32 mode);
typedef u32 (*UnkCollisionQuery_ovb0107)(UnkCollisionObject* object);

static inline bool Flag(UnkCollisionContainer* self, u32 offset) {
    return *reinterpret_cast<bool*>(reinterpret_cast<char*>(self) + offset);
}

static inline void RefreshRecords(UnkCollisionContainer* self, s32 mode) {
    for (u32 i = 0; i < self->records.count; ++i) {
        UnkCollisionRecord* record = &fn_71007253b8(&self->records, i);
        if (static_cast<u8>(record->field_0x18)) {
            UnkCollisionObject* object = record->field_0x28;
            if ((*reinterpret_cast<UnkCollisionQuery_ovb0107**>(object))[0x58 / 8](object))
                (*reinterpret_cast<UnkCollisionRefresh_ovb0107**>(self))[0x170 / 8](self, record, mode);
            else
                *reinterpret_cast<u32*>(reinterpret_cast<char*>(record) + 0x1c) = 0;
        }
    }
}

static inline void ResetObjects(UnkCollisionContainer* self) {
    auto* objects = reinterpret_cast<base::UnkVector<UnkReflObject*>*>(&self->objects);
    for (u32 i = 0; i < self->objects.count; ++i) {
        UnkReflObject* object = fn_7100862088(objects, i);
        if (object)
            fn_7100d35af4(reinterpret_cast<UnkColliderNode_A6_b70*>(object));
    }
}

void fn_7100866580(UnkCollisionContainer* self) {
    if (Flag(self, 0xc8))
        RefreshRecords(self, 1);
    ResetObjects(self);
}

void fn_71008614c0(UnkCollisionContainer* self) {
    fn_7100860c64(self);
    fn_71008615b4(self);
    if (Flag(self, 0xc8))
        RefreshRecords(self, 1);
    ResetObjects(self);
}

void fn_71008617d0(UnkCollisionContainer* self) {
    fn_71008618cc(self);
    fn_71008615b4(self);
    if (Flag(self, 0xc8) && Flag(self, 0xc9))
        RefreshRecords(self, 0);
    ResetObjects(self);
}

// Following the owner's collider-state (CCharClassModifyColliderStateTrack) and disable-colliders tracks.

void* fn_710117b130(UnkAnimationComponent* self, const CClass* cls);
void fn_7100861bb4(UnkCollisionContainer* self, void* track);
void fn_7100861e38(UnkCollisionContainer* self, void* track);
struct UnkStruct_7100861148;

struct UnkCollisionComponent_ovb0107 {
    u8 unk_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 unk_0x38[0x10];
    base::UnkVector<UnkCollisionRecord> records;  // 0x48
    u8 unk_0x70[0x58];
    bool registered;  // 0xc8
    bool active;      // 0xc9
    bool saved;       // 0xca
    bool disabled;    // 0xcb
    u8 unk_0xcc[4];
    void* stateTrack;  // 0xd0
    u8 unk_0xd8[0x50];
    bool dirty;  // 0x128
};
static_assert(__builtin_offsetof(UnkCollisionComponent_ovb0107, dirty) == 0x128, "layout");

typedef void (*UnkCollisionSetState_ovb0107)(UnkCollisionObject* object, u32 state);
typedef void (*UnkOwnerHook_ovb0107)(void* object);

static inline void Reset(UnkCollisionComponent_ovb0107* self) {
    fn_7100861148(reinterpret_cast<UnkStruct_7100861148*>(self));
}

void fn_71008615b4(UnkCollisionContainer* container) {
    auto* self = reinterpret_cast<UnkCollisionComponent_ovb0107*>(container);
    UnkActor* owner = self->owner;
    auto* animation = static_cast<UnkAnimationComponent*>(owner->field_0x220);
    if (animation) {
        void* track = fn_710117b130(animation, fn_7100d212c8());
        void* current = self->stateTrack;
        if (track) {
            if (current != track) {
                if (current)
                    fn_7100861bb4(container, track);
                else
                    fn_7100861e38(container, track);
                self->stateTrack = track;
            }
        } else if (current) {
            for (u32 i = 0; i < self->records.count; ++i) {
                UnkCollisionObject* object = fn_71007253b8(&self->records, i).field_0x28;
                u32 state = static_cast<u32>(fn_71007253b8(&self->records, i).field_0x20);
                (*reinterpret_cast<UnkCollisionSetState_ovb0107**>(object))[0x60 / 8](object, state);
            }
            void* hook = *reinterpret_cast<void**>(reinterpret_cast<char*>(self->owner) + 0x1f0);
            if (hook)
                (*reinterpret_cast<UnkOwnerHook_ovb0107**>(hook))[0x258 / 8](hook);
            self->stateTrack = nullptr;
        }
        bool disabled = fn_710117b348(animation, fn_7100cf4948());
        if (disabled) {
            if (!self->disabled) {
                self->disabled = true;
                self->saved = self->registered;
                Reset(self);
            } else if (self->registered) {
                self->saved = true;
                Reset(self);
            }
        } else if (self->disabled) {
            bool saved = self->saved;
            self->disabled = false;
            if (saved != self->registered) {
                if (saved)
                    fn_71008618cc(self);
                else
                    Reset(self);
            }
        }
    }
    self->dirty = false;
}

// Constructing the record array with a growth step, allocation flags and an initial capacity.

void fn_710086023c(char* self, u32 capacity, u32 step, u32 flags) {
    auto* records = reinterpret_cast<base::UnkVector<UnkCollisionRecord>*>(self);
    records->data = nullptr;
    records->count = 0;
    records->capacity = 0;
    records->step = step;
    records->field_0x14 = step != 0;
    records->flags = flags;
    records->allocator = fn_7100083990();
    records->Reserve(capacity);
}

// Handing every collider group and every record's object back to the collision manager.

struct UnkManager_7100863a8c;
void fn_7100d46804(UnkManager_7100863a8c* self);
void fn_7100d46c60(UnkManager_7100863a8c* self, UnkCollisionObject* entry);
void fn_7100d46e4c(UnkManager_7100863a8c* self, UnkCollisionObject* group);
UnkCollisionObject*& fn_7100863cb4(base::UnkVector<UnkCollisionObject*>* self, u32 index);

struct UnkCollisionComponent_Clear_ovb0107 {
    u8 unk_0x0[0x48];
    base::UnkVector<UnkCollisionRecord> records;   // 0x48
    base::UnkVector<UnkCollisionObject*> groups;   // 0x70
    base::UnkVector<UnkCollisionObject*> objects;  // 0x98
};

static inline UnkManager_7100863a8c* Manager() {
    UnkManager_7100863a8c* m = g_unk_71726a4788;
    if (!m) {
        m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return m;
}

void fn_7100860524(UnkCollisionComponent_Clear_ovb0107* self) {
    for (u32 i = 0; i < self->groups.count; ++i)
        fn_7100d46e4c(Manager(), fn_7100863cb4(&self->groups, i));
    for (u32 i = 0; i < self->records.count; ++i)
        fn_7100d46c60(Manager(), fn_71007253b8(&self->records, i).field_0x28);
    self->groups.count = 0;
    for (u64 i = 0; i < self->records.count; ++i)
        self->records.data[i].~UnkCollisionRecord();
    self->records.count = 0;
    self->objects.count = 0;
}

// Adding flags to the object registered under an id and passing them on to it and its groups.

struct UnkCollisionNode_ovb0107;

// The groups a collider is in: a count followed directly by the pointers.
struct UnkNodeList_ovb0107 {
    u32 count;

    UnkCollisionNode_ovb0107** begin() {
        return reinterpret_cast<UnkCollisionNode_ovb0107**>(reinterpret_cast<char*>(this) + 4);
    }
    UnkCollisionNode_ovb0107** end() { return begin() + count; }
};

struct UnkCollisionNode_ovb0107 {
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
    virtual void vf_0x60(u32 flags);
    u8 unk_0x8[0x15c];
    UnkNodeList_ovb0107 parents;  // 0x164
};

typedef void (*UnkCollisionRefresh_Flags_ovb0107)(UnkCollisionContainer* self, UnkCollisionRecord* record, s32 mode);

void fn_71008633e0(UnkCollisionContainer* self, UnkCollisionNode_ovb0107* node, u32 flags);

static inline UnkCollisionNode_ovb0107* Node(UnkCollisionContainer* self, u32 i) {
    return reinterpret_cast<UnkCollisionNode_ovb0107*>(fn_71007253b8(&self->records, i).field_0x28);
}

__attribute__((always_inline)) static inline void AddFlags(UnkCollisionContainer* self, u32 i, const u32* extra) {
    u32 flags = Node(self, i)->vf_0x58() | *extra;
    Node(self, i)->vf_0x60(flags);
    UnkCollisionRecord* record = &fn_71007253b8(&self->records, i);
    (*reinterpret_cast<UnkCollisionRefresh_Flags_ovb0107**>(self))[0x170 / 8](self, record, 0);
    fn_71008633e0(self, Node(self, i), flags);
    for (UnkCollisionNode_ovb0107** p = Node(self, i)->parents.begin(); p != Node(self, i)->parents.end(); ++p) {
        UnkCollisionNode_ovb0107* parent = *p;
        fn_71008633e0(self, parent, parent->vf_0x58());
    }
}

void fn_71008635a4(UnkCollisionContainer* self, const CStrId* id, const u32* extra) {
    for (u32 i = 0; i < self->records.count; ++i) {
        if (fn_71007253b8(&self->records, i).field_0x8.data == id->data)
            AddFlags(self, i, extra);
    }
}

UnkCollisionObject* fn_7100863814(UnkCollisionContainer* self, const CStrId* id, const u32* extra) {
    for (u32 i = 0; i < self->records.count; ++i) {
        if (fn_71007253b8(&self->records, i).field_0x0.data == id->data) {
            AddFlags(self, i, extra);
            return fn_71007253b8(&self->records, i).field_0x28;
        }
    }
    return nullptr;
}

// Passing an object's flags through the manager's mask table to the component's filters.
struct UnkManager_7100863a8c;
void fn_7100d46804(UnkManager_7100863a8c* self);

struct UnkCollisionNode_ovb0107;
void fn_7100d35b14(UnkCollisionNode_ovb0107* node, u32 mask, void* filter);

// The collision manager's flag table as fn_71008633e0 reads it: two parallel word arrays.
struct UnkCollisionManagerMasks_ovb0107 {
    u8 unk_0x0[0xa0];
    u32* masks;  // 0xa0
    u32* flags;  // 0xa8
    u32 count;   // 0xb0
    u8 unk_0xb4[8];
    u32 allocFlags;  // 0xbc
    base::UnkAllocator* allocator;  // 0xc0
};

struct UnkCollisionFilter_ovb0107 {
    u8 unk_0x0[0x44];
};

struct UnkCollisionComponent_Masks_ovb0107 {
    u8 unk_0x0[0x40];
    u8 filter[0xa0];  // 0x40
    UnkCollisionFilter_ovb0107* filters;  // 0xe0
    u32 filterCount;  // 0xe8
    u8 unk_0xec[0x14];
    bool enabled;  // 0x100
};

void fn_71008633e0(UnkCollisionContainer* container, UnkCollisionNode_ovb0107* node, u32 flags) {
    auto* self = reinterpret_cast<UnkCollisionComponent_Masks_ovb0107*>(container);
    if (!self->enabled)
        return;
    auto* m = reinterpret_cast<UnkCollisionManagerMasks_ovb0107*>(Manager());
    base::UnkAllocator* allocator = m->allocator;
    u64 size = 0;
    u32* masks = nullptr;
    u32* bits = nullptr;
    if (u32 count = m->count) {
        u32 allocFlags = m->allocFlags;
        masks = static_cast<u32*>(allocator->vf_0x40((u64)count * 4, 8, allocFlags));
        bits = static_cast<u32*>(allocator->vf_0x40((u64)count * 4, 8, allocFlags));
        size = (u64)m->count * 4;
    }
    __builtin_memcpy(masks, m->masks, size);
    __builtin_memcpy(bits, m->flags, (u64)m->count * 4);
    u32* end = masks + m->count;
    u32* a = masks;
    for (u32* b = bits; a != end; ++b, ++a) {
        if (!(*b & flags) || !self->filterCount)
            continue;
        u32 want = *a;
        const u32* word = reinterpret_cast<const u32*>(self->filters);
        for (UnkCollisionFilter_ovb0107* f = self->filters; f != self->filters + self->filterCount; ++f, ++word) {
            if (*word & want) {
                fn_7100d35b14(node, *word, self->filter);
                goto done;
            }
        }
    }
done:
    if (bits)
        allocator->vf_0x48(bits);
    if (masks)
        allocator->vf_0x48(masks);
}

// Creating a collider group under a name and keeping it.
struct UnkColliderNode;
struct UnkManager_7100863a8c;
void fn_7100d46804(UnkManager_7100863a8c* self);

struct UnkCollisionComponent_Groups_ovb0107 {
    u8 unk_0x0[0x30];
    void* owner;  // 0x30
    u8 unk_0x38[0x38];
    base::UnkVector<UnkColliderNode*> groups;  // 0x70
};

UnkColliderNode* fn_7100862400(UnkCollisionComponent_Groups_ovb0107* self, const CStrId* name) {
    UnkManager_7100863a8c* manager = Manager();
    UnkColliderNode* group;
    {
        void* owner = self->owner;
        CStrId id(*name);
        group = fn_71008625a0(manager, owner, &id);
    }
    fn_7100d35a98(reinterpret_cast<UnkEntry*>(group), true);
    self->groups.PushBack(group);
    return group;
}

// The destructor.

struct UnkCollisionComponent_Clear_ovb0107;
void fn_7100860524(UnkCollisionComponent_Clear_ovb0107* self);

// A shared link target: a reference count at +8, deleted through its slot 2.
struct UnkLinkShared_ovb0107 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 refCount;
    UnkWeakTarget* target;  // 0x10
};

struct UnkCollisionFilterEntry_ovb0107 {
    u32 mask;
    u8 unk_0x4[0x40];
};

struct UnkCollisionComponent_Dtor_ovb0107 {
    void** vtable;
    u8 unk_0x8[0x38];
    void** vtable2;  // 0x40
    base::UnkVector<UnkCollisionRecord> records;  // 0x48
    base::UnkVector<void*> groups;                // 0x70
    base::UnkVector<void*> objects;               // 0x98
    u8 unk_0xc0[0x18];
    UnkCollisionFilterEntry_ovb0107* filters;  // 0xd8
    u32* masks;                                // 0xe0
    u32 filterCount;                           // 0xe8
    u8 unk_0xec[0xc];
    base::UnkAllocator* filterAllocator;  // 0xf8
    u8 unk_0x100[8];
    void** listener;  // 0x108
    u8 unk_0x110[0x10];
    UnkLinkShared_ovb0107* link;  // 0x120
};
static_assert(__builtin_offsetof(UnkCollisionComponent_Dtor_ovb0107, link) == 0x120, "layout");

void fn_7100860364(void* object) {
    auto* self = static_cast<UnkCollisionComponent_Dtor_ovb0107*>(object);
    self->vtable = g_unk_7101a79c50 + 2;
    self->vtable2 = g_unk_7101a79c50 + 55;
    fn_7100860524(reinterpret_cast<UnkCollisionComponent_Clear_ovb0107*>(self));
    self->listener = g_unk_7101a22d48 + 2;
    if (UnkLinkShared_ovb0107* link = self->link) {
        if (link->target) {
            fn_710117b3f0(link->target, &self->listener);
            link = self->link;
        }
        if (link && --link->refCount == 0)
            link->vf_0x10();
    }
    if (self->filters) {
        for (u64 i = 0; i < self->filterCount; ++i)
            __builtin_memset(&self->filters[i].mask, 0, sizeof(u32));
    }
    self->filterCount = 0;
    if (self->masks)
        self->filterAllocator->vf_0x48(self->masks);
    if (self->filters)
        self->filterAllocator->vf_0x48(self->filters);
    self->objects.Free();
    self->groups.Free();
    self->records.Free();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}

// The overlap visitor: tests one collision object against the query's shape.
extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

struct UnkShape;

struct UnkShapeTester_A5_b68;

struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};

static inline void InitSkip() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
}

static inline UnkShapeTester_A5_b68* Tester() {
    UnkShapeTester_A5_b68* t = g_unk_71726a47a0;
    if (!t) {
        t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return t;
}

struct UnkCollisionShapeNode_ovb0107 {
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
    u8 unk_0x8[0x62];
    bool enabled;  // 0x6a
    u8 unk_0x6b[0xed];
    void* owner;  // 0x158
};

// The filter the tester calls back for each candidate (vtable g_unk_7101aa8898).
extern void* g_unk_7101aa8898[];
struct UnkShapeFilter_ovb0107 {
    void** vtable;
    UnkCollisionShapeNode_ovb0107* node;
    u64 field_0x10;
    void* filter;
    bool field_0x20;

    UnkShapeFilter_ovb0107(UnkCollisionShapeNode_ovb0107* n, void* f)
        : vtable(g_unk_7101aa8898 + 2), node(n), field_0x10(0), filter(f), field_0x20(false) {}
};

struct UnkShapeTestResult_ovb0107 {
    u32 flags;
    u32 field_0x4;
    UnkShapeTestResult_ovb0107(const UnkShapeTestResult_ovb0107& other);
};

struct UnkCollisionOverlapQuery_ovb0107 {
    u32 mask;      // 0x0
    void* filter;  // 0x8
    u32 result;    // 0x10
    void* owner;   // 0x18
    u8 shape[0x40];  // 0x20
    u16 flags;     // 0x60
};

bool fn_7100865010(UnkCollisionOverlapQuery_ovb0107* self, UnkCollisionShapeNode_ovb0107* node) {
    if (node->enabled && (node->vf_0x58() & self->mask) && self->owner != node->owner) {
        InitSkip();
        if (self->filter) {
            UnkShapeFilter_ovb0107 filter(node, self->filter);
            UnkShapeTester_A5_b68* tester = Tester();
            self->result |= fn_7100d51368(tester, self->owner, self->shape, node->owner,
                                          fn_7100d36a2c(reinterpret_cast<UnkShape*>(node)), &filter,
                                          self->flags).flags;
        } else {
            UnkShapeTester_A5_b68* tester = Tester();
            self->result |= fn_7100d51368(tester, self->owner, self->shape, node->owner,
                                          fn_7100d36a2c(reinterpret_cast<UnkShape*>(node)), nullptr,
                                          self->flags).flags;
        }
    }
    return !(self->result & 2);
}

// Unregistering an object: detaching it from the tracker, removing its record and handing it back.
struct UnkTracker_7100863a8c;

struct UnkManager_7100863a8c;
void fn_7100d46804(UnkManager_7100863a8c* self);
void fn_7100d46c60(UnkManager_7100863a8c* self, UnkCollisionObject* entry);

void fn_7100863e04(UnkCollisionContainer* self, UnkCollisionObject* obj) {
    if (!self->records.count)
        return;
    UnkTracker_7100863a8c* tracker =
        static_cast<UnkTracker_7100863a8c*>(g_unk_7101d47310->field_0x608->field_0x820);
    u32 i;
    for (i = 0; i < self->records.count; ++i) {
        if (fn_71007253b8(&self->records, i).field_0x28 == obj)
            break;
    }
    if (i >= self->records.count)
        return;
    if (fn_7100d5a930(tracker, obj))
        fn_7100d5abfc(tracker, reinterpret_cast<UnkStruct_7100861148_Collider*>(obj));
    if (self->records.count > i) {
        u32 last = self->records.count - 1;
        if (last > i) {
            self->records.data[i].~UnkCollisionRecord();
            new (&self->records.data[i])
                UnkCollisionRecord(static_cast<UnkCollisionRecord&&>(self->records.data[last]));
        }
        for (u32 j = last; self->records.count > j; ++j)
            self->records.data[j].~UnkCollisionRecord();
        self->records.count = last;
    }
    self->objects.Remove(obj);
    fn_7100d46c60(Manager(), obj);
}

// The 2D bounds of the matching enabled objects (records or registered objects).

struct UnkCollisionShape_ovb0107 {
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
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    u8 unk_0x8[0x38];
    f32 minX, minY;  // 0x40
    f32 maxX, maxY;  // 0x48
    u8 unk_0x50[0x10];
    bool dirty;    // 0x60
    u8 unk_0x61[9];
    bool enabled;  // 0x6a
};

struct UnkRecordView_ovb0107 {
    u64 field_0x0;
    void* id;  // 0x8
    u8 unk_0x10[0x18];
    UnkCollisionShape_ovb0107* object;  // 0x28
};

struct UnkIdList_ovb0107 {
    void** data;
    u32 count;
};

struct UnkBounds_ovb0107 {
    f32 minX, minY, maxX, maxY;
    UnkBounds_ovb0107(f32 x0, f32 y0, f32 x1, f32 y1) : minX(x0), minY(y0), maxX(x1), maxY(y1) {}
    UnkBounds_ovb0107(const UnkBounds_ovb0107& o) : minX(o.minX), minY(o.minY), maxX(o.maxX), maxY(o.maxY) {}
};

struct UnkCollisionComponent_Bounds_ovb0107 {
    u8 unk_0x0[0x48];
    UnkRecordView_ovb0107* records;  // 0x48
    u32 recordCount;                 // 0x50
    u8 unk_0x54[0x44];
    UnkCollisionShape_ovb0107** objects;  // 0x98
    u32 objectCount;                      // 0xa0
};

static inline bool Contains(const UnkIdList_ovb0107* list, void* const& id) {
    void** p = list->data;
    for (u64 n = list->count; n; --n, ++p)
        if (*p == id)
            return true;
    return false;
}

static inline void Grow(UnkBounds_ovb0107& b, UnkCollisionShape_ovb0107* shape, f32& minX, f32& minY,
                        f32& maxX, f32& maxY) {
    if (shape->dirty)
        shape->vf_0x98();
    minX = minX < shape->minX ? minX : shape->minX;
    minY = minY < shape->minY ? minY : shape->minY;
    b.minX = minX;
    b.minY = minY;
    maxX = maxX > shape->maxX ? maxX : shape->maxX;
    maxY = maxY > shape->maxY ? maxY : shape->maxY;
    b.maxX = maxX;
    b.maxY = maxY;
}

UnkBounds_ovb0107 fn_71008653d0(UnkCollisionComponent_Bounds_ovb0107* self, u32 mask,
                                const UnkIdList_ovb0107* skip, u32 exclude) {
    UnkBounds_ovb0107 b(3.40282347e+38f, 3.40282347e+38f, -3.40282347e+38f, -3.40282347e+38f);
    f32 minX = 3.40282347e+38f, minY = 3.40282347e+38f;
    f32 maxX = -3.40282347e+38f, maxY = -3.40282347e+38f;
    for (u64 i = 0; i < self->recordCount; ++i) {
        if (!self->records[i].object->enabled)
            continue;
        if (!(self->records[i].object->vf_0x58() & mask))
            continue;
        if (self->records[i].object->vf_0x58() & exclude)
            continue;
        UnkCollisionShape_ovb0107* shape = self->records[i].object;
        if (!shape || Contains(skip, self->records[i].id))
            continue;
        Grow(b, shape, minX, minY, maxX, maxY);
    }
    return b;
}

UnkBounds_ovb0107 fn_7100865558(UnkCollisionComponent_Bounds_ovb0107* self, u32 mask,
                                const UnkIdList_ovb0107* skip, u32 exclude) {
    UnkBounds_ovb0107 b(3.40282347e+38f, 3.40282347e+38f, -3.40282347e+38f, -3.40282347e+38f);
    f32 minX = 3.40282347e+38f, minY = 3.40282347e+38f;
    f32 maxX = -3.40282347e+38f, maxY = -3.40282347e+38f;
    for (u64 i = 0; i < self->objectCount; ++i) {
        if (!self->objects[i]->enabled)
            continue;
        if (!(self->objects[i]->vf_0x58() & mask))
            continue;
        if (self->objects[i]->vf_0x58() & exclude)
            continue;
        UnkCollisionShape_ovb0107* shape = self->objects[i];
        if (!shape || Contains(skip, self->records[i].id))
            continue;
        Grow(b, shape, minX, minY, maxX, maxY);
    }
    return b;
}

struct UnkEntity;

struct UnkCollisionPlaced_ovb0107 {
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
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78(const UnkMatrix44* transform);
};

struct UnkPlacedRecord_ovb0107 {
    u8 unk_0x0[0x10];
    CStrId node;  // 0x10
    u8 unk_0x18;
    bool fixed;   // 0x19
    u8 unk_0x1a[2];
    u32 frame;    // 0x1c
    u8 unk_0x20[8];
    UnkCollisionPlaced_ovb0107* object;  // 0x28
};

struct UnkPlacedOwner_ovb0107 {
    u8 unk_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
    u8 unk_0x78[0x1b0];
    UnkEntity* shape;  // 0x228
};

struct UnkCollisionComponent_Place_ovb0107 {
    u8 unk_0x0[0x30];
    UnkPlacedOwner_ovb0107* owner;  // 0x30
};

static inline bool SameId(const CStrId& a, const CStrId& b) {
    return a.data == b.data;
}

void fn_7100862278(UnkCollisionComponent_Place_ovb0107* self, UnkPlacedRecord_ovb0107* record, bool force) {
    u32 frame = g_unk_7101d58e48;
    if (!force && record->frame >= frame)
        return;
    if (record->fixed)
        return;
    if (record->node.data == &g_unk_7101d47108->empty || SameId(record->node, GameManagerIdTable().field_0xd40)) {
        UnkCollisionPlaced_ovb0107* object = record->object;
        object->vf_0x78(&fn_710002614c(&self->owner->xform));
    } else {
        UnkEntity* shape = self->owner->shape;
        if (!shape)
            goto done;
        UnkMatrix44 transform;
        {
            u64 key = *reinterpret_cast<u64*>(reinterpret_cast<char*>(record->node.data) + 0x30);
            if (!fn_71011735e8(shape, &key, &transform, 0))
                goto done;
        }
        {
            UnkVec3f scale = fn_7100076cec(reinterpret_cast<UnkXform*>(&transform));
            if (scale.x != 0.0f && scale.y != 0.0f && scale.z != 0.0f)
                fn_71000774fc(reinterpret_cast<UnkXform*>(&transform));
        }
        record->object->vf_0x78(&transform);
    }
done:
    record->frame = frame;
}

// Loading the collision file the CCharClassCollisionComponent names.

// A string with 0x100 bytes of inline storage (as src/base/UnkTunableRegistry.cpp has it).
struct UnkTunableLine {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    ~UnkTunableLine() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

// The collision file's reader (as src/game/CSubAreaManager.cpp declares it).
struct UnkCollisionFileReader {
    u8 unk_0x0[0x18];
};

// The component's CCharClassCollisionComponent: the collision file's path and two flags.
struct UnkCharClassCollision_ovb0107 : UnkReflObject {
    u8 unk_0x8[0x50];
    CRntString path;  // 0x58
    u8 unk_0x58_end[0x30 - sizeof(CRntString)];
    bool active;   // 0x88
    bool field_0x89;
};
static_assert(__builtin_offsetof(UnkCharClassCollision_ovb0107, active) == 0x88, "layout");
u32 fn_710079a8b8(UnkCharClassCollision_ovb0107* self, void* key);

struct UnkLoadedShapeOwner_ovb0107 {
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
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88(u32 value);
};

struct UnkLoadedShape_ovb0107 {
    u8 unk_0x0[0x20];
    u8 key[0x138];  // 0x20
    UnkLoadedShapeOwner_ovb0107* owner;  // 0x158
};

struct UnkCollisionComponent_Load_ovb0107 {
    u8 unk_0x0[0x10];
    UnkReflObject* charClass;  // 0x10
    u8 unk_0x18[0x30];
    base::UnkVector<UnkCollisionRecord> records;  // 0x48
    u8 groups[0x28];  // 0x70
    u8 unk_0x98[0x31];
    bool active;  // 0xc9
};
static_assert(__builtin_offsetof(UnkCollisionComponent_Load_ovb0107, active) == 0xc9, "layout");

// Counts a use of the id's string (the word at +0x44 of its pool entry), written untyped: the original
// reloads the id afterwards.
static inline void CountUse(const CStrId& id) {
    char* uses = reinterpret_cast<char*>(id.data) + 0x44;
    u32 n;
    __builtin_memcpy(&n, uses, sizeof(n));
    ++n;
    __builtin_memcpy(uses, &n, sizeof(n));
}

bool fn_710086068c(UnkCollisionComponent_Load_ovb0107* self) {
    UnkReflObject* object = self->charClass;
    auto* charClass = static_cast<UnkCharClassCollision_ovb0107*>(
        object ? (fn_710009d50c(object->vf_0x0(), fn_710079a218()) ? object : nullptr) : nullptr);
    self->active = charClass->active;
    UnkTunableLine path = fn_710013c834(&charClass->path, "\\", "/");
    UnkGameManagerIdTable& table = GameManagerIdTable();
    CountUse(table.field_0x2418);
    if (fn_710007f220(path.Str(),
                      reinterpret_cast<const CRntString*>(reinterpret_cast<char*>(table.field_0x2418.data) + 0x10))) {
        UnkCollisionFileReader reader;
        fn_7100d45fc4(&reader, self);
        fn_7100860830(&reader, path.Str()->data, &self->records, charClass->field_0x89, self->groups);
        for (u32 i = 0; i < self->records.count; ++i) {
            auto* shape = reinterpret_cast<UnkLoadedShape_ovb0107*>(fn_71007253b8(&self->records, i).field_0x28);
            UnkLoadedShapeOwner_ovb0107* owner = shape->owner;
            owner->vf_0x88(fn_710079a8b8(charClass, shape->key));
        }
        fn_7100d45fd4(&reader);
    }
    return true;
}
