// navmesh::CNavMeshGenerator's unit (tools/tu_map.py unit 365): gathers the actor definitions of the
// breakable actors, then registers, per scenario, the ids of the dynamic smart-link rules their AI
// components name. Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU315.h"
#include "game/UnkTU344.h"
#include "game/UnkTU365.h"
#include "types.h"

using namespace base::global;

// A counted reference to a resource whose destructor reads the pointer once (as in
// src/game/CSpawnPointComponent/CSpawnPointComponent_b52.cpp).
struct UnkSpawnDefRef_b52 {
    UnkCountedResource* ptr;

    UnkSpawnDefRef_b52& operator=(const UnkSpawnDefRef_b52& other) {
        UnkCountedResource* p = other.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
    ~UnkSpawnDefRef_b52() {
        UnkCountedResource* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};

// A file-path id (on CFilePathStrId's pool) that moves, and whose release reloads the id and its
// count after the decrement.
struct UnkLinkPath_ovb0128 : UnkFilePathStrIdBase {
    UnkLinkPath_ovb0128() {
        data = nullptr;
        if (!g_unk_7101d47200)
            UnkEnsureFilePathPool();
        data = &g_unk_7101d47200->empty;
    }
    UnkLinkPath_ovb0128(const UnkLinkPath_ovb0128& other) : UnkFilePathStrIdBase(other) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    UnkLinkPath_ovb0128(UnkLinkPath_ovb0128&& other) : UnkFilePathStrIdBase(other) {
        other.data = &g_unk_7101d47200->empty;
    }
    ~UnkLinkPath_ovb0128() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// A counted reference to a resource (base/UnkCountedResource.h); moves copy it.
struct UnkSmartLinkRef_ovb0128 {
    UnkCountedResource* ptr;

    UnkSmartLinkRef_ovb0128() : ptr(nullptr) {}
    UnkSmartLinkRef_ovb0128(const UnkSmartLinkRef_ovb0128& other) : ptr(nullptr) {
        if (UnkCountedResource* p = other.ptr)
            Assign(p);
    }
    ~UnkSmartLinkRef_ovb0128() {
        if (UnkCountedResource* p = ptr) {
            if (--p->ref_count == 0)
                fn_710001737c(p);
        }
    }

    // Takes a reference to p (not null), dropping the held one.
    void Assign(UnkCountedResource* p) {
        ++p->ref_count;
        if (UnkCountedResource* old = ptr) {
            if (--old->ref_count == 0)
                fn_710001737c(old);
        }
        ptr = p;
    }
};

struct UnkSmartLinkEntry_ovb0128 {
    UnkLinkPath_ovb0128 path;
    UnkSmartLinkRef_ovb0128 object;
};

struct UnkBreakablePart {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
    u8 pad_0x8[0x38];
};
static_assert(sizeof(UnkBreakablePart) == 0x40, "UnkBreakablePart placeholder layout");

struct UnkBreakableTarget {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkActorDef345;
struct UnkActorDefEntry345;
struct UnkNavResRef_b64a2;
struct UnkNavLoadSlot360;
struct UnkOwnedList;
template <class V>
struct UnkNavLoadMap;
template <class V>
struct UnkIdMap;
struct UnkSmartLinkSource_l29b5dc;

UnkActorDefEntry345* fn_7100255368(const UnkActorDef345* self, const u64* id);
void fn_71002681a8(base::global::CRntVector<UnkSmartLinkEntry_ovb0128>* self,
                   const base::global::CRntVector<UnkSmartLinkEntry_ovb0128>* other);
void fn_71002a790c(base::global::CRntVector<UnkNavResRef_b64a2>* self, u32 capacity);
void CNavMesh_ReserveCellRecordMapMap(UnkNavLoadMap<UnkNavLoadMap<base::global::CRntVector<UnkNavLoadSlot360>>>* self, u32 capacity);
void fn_710027f54c(UnkIdMap<UnkOwnedList>* self, u32 capacity);

// The scenario an actor is in: its id at +0x30.
struct UnkNavScenario_l29b5dc {
    u8 pad_0x0[0x30];
    CStrId id;  // 0x30
};

struct UnkNavActor_l29b5dc {
    u8 pad_0x0[0x30];
    UnkNavScenario_l29b5dc* scenario;  // 0x30
    u8 pad_0x38[0x10];
    CStrId type;  // 0x48
    u8 pad_0x50[0x1b8 - 0x50];
    UnkBreakablePart part;  // 0x1b8
};

struct UnkNavActorSlot_l29b5dc {
    u8 pad_0x0[0x10];
    UnkNavActor_l29b5dc* actor;  // 0x10
};

// The game's actors, an array of slots with gaps.
struct UnkNavActorList_l29b5dc {
    UnkNavActorSlot_l29b5dc** slots;
    u32 count;
};
UnkNavActorList_l29b5dc* fn_71011267fc();

// An actor definition (a counted resource) and the type it defines.
struct UnkNavActorDef_l29b5dc : UnkCountedResource {
    u8 pad_0x58[0x98 - 0x58];
    CStrId type;  // 0x98
};

// An AI component's definition: its smart links.
struct UnkNavAIComponentDef_l29b5dc : base::reflection::UnkReflObject {
    u8 pad_0x8[0x1d0 - 0x8];
    UnkSmartLinkSource_l29b5dc* links;  // 0x1d0
};

const base::global::CRntVector<UnkSmartLinkEntry_ovb0128>* fn_710026626c(UnkSmartLinkSource_l29b5dc* source);
const CStrId* fn_710026ce70(base::reflection::UnkReflObject* rules);

// A counted reference to an actor definition.
struct UnkNavActorDefRef_l29b5dc {
    UnkNavActorDef_l29b5dc* ptr;

    explicit UnkNavActorDefRef_l29b5dc(UnkNavActorDef_l29b5dc* p) : ptr(nullptr) {
        if (p)
            Set(p);
    }
    // A new reference to the definition the other holds.
    explicit UnkNavActorDefRef_l29b5dc(const UnkSpawnDefRef_b52& other) : ptr(nullptr) {
        if (ptr != other.ptr)
            Set(static_cast<UnkNavActorDef_l29b5dc*>(other.ptr));
    }
    ~UnkNavActorDefRef_l29b5dc() {
        UnkNavActorDef_l29b5dc* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }

    // Takes a reference to p, dropping the held one.
    void Set(UnkNavActorDef_l29b5dc* p) {
        if (p)
            ++p->ref_count;
        UnkNavActorDef_l29b5dc* old = ptr;
        if (old && --old->ref_count == 0)
            fn_710001737c(old);
        ptr = p;
    }
};

using UnkNavActorDefList_l29b5dc = base::global::CRntVector<UnkNavActorDefRef_l29b5dc>;

// The definition list's reserve is the resource-reference array's (fn_71002a790c).
static inline void Reserve(UnkNavActorDefList_l29b5dc* list, u32 capacity) {
    fn_71002a790c(reinterpret_cast<base::global::CRntVector<UnkNavResRef_b64a2>*>(list), capacity);
}

// A map from string ids to values: two arrays of `capacity` entries, grown together out of line.
template <class V>
struct UnkNavIdMap_l29b5dc {
    V* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    explicit UnkNavIdMap_l29b5dc(u32 flags)
        : values(nullptr), keys(nullptr), count(0), capacity(0), step(10), flags(flags),
          allocator(GetDefaultAllocator()) {}
    UnkNavIdMap_l29b5dc(UnkNavIdMap_l29b5dc&& other)
        : values(other.values), keys(other.keys), count(other.count), capacity(other.capacity),
          step(other.step), flags(other.flags), allocator(other.allocator) {
        other.values = nullptr;
        other.keys = nullptr;
        other.count = 0;
        other.capacity = 0;
    }

    u32 IndexOf(const CStrId& key) const {
        u32 i = 0;
        for (; i < count; ++i)
            if (keys[i].data == key.data)
                break;
        return i;
    }
    V* Find(const CStrId& key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i].data == key.data)
                return &values[i];
        return &values[count];
    }
    V* End() { return &values[count]; }
    V& operator[](const CStrId& key) { return values[IndexOf(key)]; }

    void Add(const CStrId& key, V&& value) {
        if (count == capacity)
            Reserve(this, step + count);
        new (&keys[count]) CStrId(key);
        new (&values[count]) V(static_cast<V&&>(value));
        ++count;
    }
};

// Per scenario, the ids of the smart-link rules in use.
using UnkNavRuleIds_l29b5dc = UnkNavIdMap_l29b5dc<base::global::CRntVector<UnkNavLoadSlot360>>;
using UnkNavScenarioRules_l29b5dc = UnkNavIdMap_l29b5dc<UnkNavRuleIds_l29b5dc>;

static inline void Reserve(UnkNavRuleIds_l29b5dc* map, u32 capacity) {
    fn_710027f54c(reinterpret_cast<UnkIdMap<UnkOwnedList>*>(map), capacity);
}
static inline void Reserve(UnkNavScenarioRules_l29b5dc* map, u32 capacity) {
    CNavMesh_ReserveCellRecordMapMap(reinterpret_cast<UnkNavLoadMap<UnkNavLoadMap<base::global::CRntVector<UnkNavLoadSlot360>>>*>(map),
                  capacity);
}

struct UnkNavLinkRules_l29b5dc {
    u8 pad_0x0[0x360];
    UnkNavScenarioRules_l29b5dc rules;  // 0x360
};

// The copy of an AI component's links (fn_71002681a8 copy-constructs it into the storage).
struct UnkNavLinkCopy_l29b5dc {
    alignas(8) u8 storage[sizeof(base::global::CRntVector<UnkSmartLinkEntry_ovb0128>)];

    base::global::CRntVector<UnkSmartLinkEntry_ovb0128>& items() {
        return *reinterpret_cast<base::global::CRntVector<UnkSmartLinkEntry_ovb0128>*>(storage);
    }
};

template <const base::reflection::CClass* (*GetClass)()>
static inline bool IsA(const base::reflection::UnkReflObject* object) {
    return fn_710009d50c(object->vf_0x0(), GetClass());
}

static inline bool Contains(const UnkNavActorDefList_l29b5dc& defs, const UnkCountedResource* def) {
    for (u32 i = 0; i < defs.count; ++i)
        if (defs.data[i].ptr == def)
            return true;
    return false;
}

// Whether the actor's breakable part has a target with an object.
static inline bool HasBreakableTarget(UnkNavActor_l29b5dc* actor) {
    UnkBreakableTarget* target = *fn_71002a76a4(&actor->part);
    return target && target->field_0x10;
}

// The id of the AI component in an actor definition (needs UnkGameManagerIdTable::field_0x6c0, a CStrId).
static inline const u64* AIComponentId() {
    return reinterpret_cast<const u64*>(&GameManagerIdTable().field_0x6c0);
}

void fn_710029b5dc(void* self, UnkNavLinkRules_l29b5dc* out) {
    UnkNavActorDefList_l29b5dc defs(0xf);

    // The definitions of the breakable actors, each once.
    UnkNavActorList_l29b5dc* actors = fn_71011267fc();
    for (UnkNavActorSlot_l29b5dc **it = actors->slots, **end = it + actors->count; it != end; ++it) {
        if (!*it)
            continue;
        UnkNavActor_l29b5dc* actor = (*it)->actor;
        if (!actor || !HasBreakableTarget(actor))
            continue;
        UnkSpawnDefRef_b52 def = fn_71011938bc(g_unk_71726a0220, &actor->type, &g_unk_7101d470e8);
        if (!def.ptr)
            continue;
        if (!Contains(defs, def.ptr)) {
            u32 n = defs.count;
            u32 capacity = defs.capacity;
            if (capacity <= n && defs.step != 0)
                capacity = (n / defs.step + 1) * defs.step;
            if ((s32)n >= (s32)capacity)
                for (;;) {
                }
            Reserve(&defs, capacity);
            new (&defs.data[defs.count]) UnkNavActorDefRef_l29b5dc(def);
            ++defs.count;
        }
    }

    fn_71002191a4(g_unk_7101d47300, 1);

    // The rules each breakable actor's AI component links, per scenario.
    actors = fn_71011267fc();
    for (UnkNavActorSlot_l29b5dc **it = actors->slots, **end = it + actors->count; it != end; ++it) {
        if (!*it)
            continue;
        UnkNavActor_l29b5dc* actor = (*it)->actor;
        if (!actor || !HasBreakableTarget(actor))
            continue;
        UnkNavActorDef_l29b5dc* found = nullptr;
        for (UnkNavActorDefRef_l29b5dc *d = defs.data, *dend = d + defs.count; d != dend; ++d) {
            if (d->ptr->type.data == actor->type.data) {
                found = d->ptr;
                break;
            }
        }
        if (!found)
            continue;
        UnkNavActorDefRef_l29b5dc def(found);
        auto* component = reinterpret_cast<UnkNavAIComponentDef_l29b5dc*>(
            fn_7100255368(reinterpret_cast<const UnkActorDef345*>(def.ptr), AIComponentId()));
        if (!component || !IsA<fn_7100408b2c>(component) || !component->links)
            continue;

        UnkNavLinkCopy_l29b5dc copy;
        base::global::CRntVector<UnkSmartLinkEntry_ovb0128>& links = copy.items();
        fn_71002681a8(&links, fn_710026626c(component->links));
        for (UnkSmartLinkEntry_ovb0128 *l = links.data, *lend = l + links.count; l != lend; ++l) {
            UnkSmartLinkEntry_ovb0128 link = *l;
            auto* object = reinterpret_cast<base::reflection::UnkReflObject*>(link.object.ptr);
            base::reflection::UnkReflObject* rules =
                object && IsA<fn_710026cad8>(object) ? object : nullptr;
            CStrId scenario = actor->scenario->id;
            UnkNavScenarioRules_l29b5dc& map = out->rules;
            if (map.Find(scenario) == map.End()) {
                map.Add(scenario, UnkNavRuleIds_l29b5dc(0xe));
                UnkNavRuleIds_l29b5dc& ids = map[scenario];
                if (ids.Find(*fn_710026ce70(rules)) == ids.End())
                    ids.Add(*fn_710026ce70(rules), base::global::CRntVector<UnkNavLoadSlot360>(0xf));
            }
        }
        links.Free();
    }

    defs.Free();
    fn_71002191a4(g_unk_7101d47300, 1);
}
