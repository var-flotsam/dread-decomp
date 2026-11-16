// Unit 365 (navmesh::CNavMeshGenerator): on first use collects the weak handles of the targets of
// every listed actor whose definition's AI component is set (each definition looked up once), then
// tells whether any of those targets' trackers reports either half of `query`. Names and layouts are
// placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU315.h"
#include "game/UnkTU344.h"
#include "game/UnkTU365.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// A counted reference whose move constructor starts empty and takes a new reference to the other's resource.
struct UnkNavResRef_b64a2 {
    UnkCountedResource* ptr;

    UnkNavResRef_b64a2(UnkNavResRef_b64a2&& other) {
        ptr = nullptr;
        if (ptr != other.ptr) {
            UnkCountedResource* p = other.ptr;
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
    }
    ~UnkNavResRef_b64a2() {
        UnkCountedResource* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};

// A counted reference like UnkResRef (base/UnkResKey.h) whose destructor reads the pointer once.
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

void fn_71002a790c(base::global::CRntVector<UnkNavResRef_b64a2>* self, u32 capacity);

struct UnkTag_7100241b30;
template <class Tag>
struct UnkSlotArray;
void fn_7100241b30(UnkSlotArray<UnkTag_7100241b30>* self, u32 capacity);

struct UnkActorDef345;
struct UnkActorDefEntry345;
UnkActorDefEntry345* fn_7100255368(const UnkActorDef345* self, const u64* id);

// A counted reference to a weak handle.
struct UnkWeakRef_l28a764 {
    UnkWeakHandle* ptr;

    UnkWeakRef_l28a764(UnkWeakHandle* p) : ptr(p) {
        if (p)
            ++p->ref_count;
    }
    UnkWeakRef_l28a764(const UnkWeakRef_l28a764& other) : ptr(nullptr) { AssignRef(&ptr, other.ptr); }
    ~UnkWeakRef_l28a764() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};

struct UnkTargetHolder_l28a764 {
    u8 pad_0x0[0x10];
    UnkWeakTarget* target;  // 0x10
};

struct UnkActor_l28a764 {
    u8 pad_0x0[0x48];
    base::global::CStrId type;  // 0x48
    u8 pad_0x50[0x168];
    u8 part[0x10];  // 0x1b8, a UnkBreakablePart

    UnkTargetHolder_l28a764* Target() {
        return reinterpret_cast<UnkTargetHolder_l28a764*>(*fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(part)));
    }
};
static_assert(__builtin_offsetof(UnkActor_l28a764, part) == 0x1b8, "layout");

struct UnkActorEntry_l28a764 {
    u8 pad_0x0[0x10];
    UnkActor_l28a764* actor;  // 0x10
};

struct UnkActorList_l28a764 {
    UnkActorEntry_l28a764** data;
    u32 count;
};
UnkActorList_l28a764* fn_71011267fc();

// The AI component settings of an actor definition.
struct UnkAIComponentDef_l28a764 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x399];
    bool field_0x3a1;
};

struct UnkQueryTarget_l28a764 {
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
    virtual bool vf_0x50(const void* key);
};

struct UnkTracker_l28a764 {
    u8 pad_0x0[0x30];
    UnkQueryTarget_l28a764* field_0x30;
};

struct UnkQuery_l28a764 {
    u64 field_0x0;
    u64 field_0x8;
};

__attribute__((visibility("hidden"))) extern bool g_unk_71726a47e8;
__attribute__((visibility("hidden"))) extern base::global::CRntVector<UnkWeakRef_l28a764> g_unk_71726a4828;

// The id of the AI component in the game manager's id table. Needs `u64 field_0x6c0;` in
// UnkGameManagerIdTable (include/game/UnkGameManagerIdTable.h), which does not name it yet.
static inline const u64* AIComponentId() {
    return &GameManagerIdTable().field_0x6c0;
}

static inline bool Contains(const base::global::CRntVector<UnkNavResRef_b64a2>& list, const UnkSpawnDefRef_b52& def) {
    for (u32 i = 0; i < list.count; ++i)
        if (list.data[i].ptr == def.ptr)
            return true;
    return false;
}

static inline void Add(base::global::CRntVector<UnkNavResRef_b64a2>& list, UnkSpawnDefRef_b52& def) {
    u32 n = list.count;
    u32 capacity = list.capacity;
    if (capacity <= n && list.step != 0)
        capacity = (n / list.step + 1) * list.step;
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    fn_71002a790c(&list, capacity);
    new (&list.data[list.count]) UnkNavResRef_b64a2(static_cast<UnkNavResRef_b64a2&&>(reinterpret_cast<UnkNavResRef_b64a2&>(def)));
    ++list.count;
}

static inline void Add(base::global::CRntVector<UnkWeakRef_l28a764>& list, const UnkWeakRef_l28a764& ref) {
    u32 n = list.count;
    u32 capacity = list.capacity;
    if (capacity <= n && list.step != 0)
        capacity = (n / list.step + 1) * list.step;
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    fn_7100241b30(reinterpret_cast<UnkSlotArray<UnkTag_7100241b30>*>(&list), capacity);
    new (&list.data[list.count]) UnkWeakRef_l28a764(ref);
    ++list.count;
}

static void Collect() {
    base::global::CRntVector<UnkNavResRef_b64a2> defs{15};
    UnkActorList_l28a764* actors = fn_71011267fc();
    UnkActorEntry_l28a764** end = actors->data + actors->count;
    for (UnkActorEntry_l28a764** it = actors->data; it != end; ++it) {
        UnkActor_l28a764* actor = *it ? (*it)->actor : nullptr;
        UnkTargetHolder_l28a764* first = actor ? actor->Target() : nullptr;
        if (first && first->target) {
            UnkSpawnDefRef_b52 def = fn_71011938bc(g_unk_71726a0220, &actor->type, &g_unk_7101d470e8);
            if (!Contains(defs, def))
                Add(defs, def);
            if (def.ptr) {
                auto* ai = reinterpret_cast<UnkAIComponentDef_l28a764*>(
                    fn_7100255368(reinterpret_cast<const UnkActorDef345*>(def.ptr), AIComponentId()));
                if (ai && fn_710009d50c(ai->vf_0x0(), fn_7100408b2c()) && ai->field_0x3a1) {
                    UnkTargetHolder_l28a764* holder = actor->Target();
                    if (holder && holder->target) {
                        UnkWeakRef_l28a764 handle(GetWeakHandle(holder->target));
                        Add(g_unk_71726a4828, handle);
                    }
                }
            }
        }
        fn_71002191a4(g_unk_7101d47300, 1);
    }
    defs.Free();
    fn_71002191a4(g_unk_7101d47300, 1);
}

bool fn_710028a764(void* self, const UnkQuery_l28a764* query) {
    if (!g_unk_71726a47e8) {
        g_unk_71726a47e8 = true;
        Collect();
    }
    UnkWeakRef_l28a764* end = g_unk_71726a4828.data + g_unk_71726a4828.count;
    UnkWeakRef_l28a764* it = g_unk_71726a4828.data;
    for (; it != end; ++it) {
        UnkWeakHandle* handle = it->ptr;
        if (!handle || !handle->target)
            continue;
        auto* tracker = reinterpret_cast<UnkTracker_l28a764*>(fn_710028aca8(handle->target));
        if (!tracker || !tracker->field_0x30)
            continue;
        if (tracker->field_0x30->vf_0x50(&query->field_0x0) || tracker->field_0x30->vf_0x50(&query->field_0x8))
            break;
    }
    return it != end;
}