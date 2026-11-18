// CDoorLifeComponent's unit (tools/tu_map.py unit 1376): the door's destructor (constructor fn_71008ce828).
// Releases the overcharge effects when the door was overcharged, drops its navigation links, stops the first
// timer, then destroys the members last first and runs the life component base's destructor.
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkDestructors.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CItemLifeComponent.h"
#include "game/UnkModelManager.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU2739.h"
#include "game/UnkTU341.h"
#include "game/UnkTU39.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;

// As src/game/UnkTU1376/UnkTU1376_Ctor_l8ce828.cpp and src/clones/ declare them.
extern void* g_unk_7101a7e840[];
extern char g_unk_7101a7eb60[];
extern char g_unk_7101a7ebb8[];

// As src/game/CDoorShieldLifeComponent/CDoorShieldLifeComponent_Links_b51.cpp declares them.
struct UnkNode;
void fn_71002b5698(UnkNode* self);
void fn_710027e18c(UnkNavMesh* navmesh, UnkNode* link);

// As src/game/UnkTU1376/UnkTU1376_Start_l8d109c.cpp and UnkTU1376_Release_l8cf3ac.cpp declare them.
void fn_71008dd384(base::UnkDeferredSet<void>* set, void* item);
struct UnkListSets_l8cf3ac;
void fn_71008cf3ac(UnkListSets_l8cf3ac* self);

struct UnkDoorCallbacks_l8ceda0;
template <>
struct base::UnkCallbackTraits<UnkDoorCallbacks_l8ceda0> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkDoorCallbackList_l8ceda0 = base::UnkCallbackListShort<UnkDoorCallbacks_l8ceda0>;

// The door's state machine; destroying it forgets its states.
struct UnkDoorStates_l8ceda0 {
    char* vtable;
    u64 field_0x8;
    void* current;
    void* previous;
    u32 count;
    u32 states[0x20];
    s32 pending;
    bool field_0xa8;
    bool field_0xa9;

    ~UnkDoorStates_l8ceda0() {
        vtable = g_unk_7101a7ebb8 + 0x10;
        current = nullptr;
        previous = nullptr;
        count = 0;
    }
};
static_assert(sizeof(UnkDoorStates_l8ceda0) == 0xb0, "UnkDoorStates_l8ceda0 layout");

struct UnkDoorStateBase_l8ceda0 {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;  // the id
    base::reflection::UnkReflValue value;

    ~UnkDoorStateBase_l8ceda0() {
        vtable = g_unk_7101a7eb60 + 0x10;
        fn_71000a1c60(&value);
    }
};

struct UnkDoorState_l8ceda0 : UnkDoorStateBase_l8ceda0 {
    base::UnkClearedPair pairs[4];
};
static_assert(sizeof(UnkDoorState_l8ceda0) == 0x70, "UnkDoorState_l8ceda0 layout");

// The string id and table fn_71008dbbdc builds.
struct UnkDoorIdList_l8ceda0 {
    u8 field_0x0;
    base::UnkStrIdMember id;
    UnkOwnedStrIdTable table;
};

// The links follow their count directly (4-byte aligned).
typedef UnkNode* __attribute__((aligned(4))) UnkDoorNodePtr_l8ceda0;

// The navigation links the door added; destroying them forgets them.
struct UnkDoorLinks_l8ceda0 {
    u32 count;
    UnkDoorNodePtr_l8ceda0 links[8];

    ~UnkDoorLinks_l8ceda0() { count = 0; }
};

// A function bound to an object, registered in a set while it is.
struct UnkDoorSetBound_l8ceda0 {
    base::UnkDeferredSet<void>* set;
    void* object;
    void* function;

    ~UnkDoorSetBound_l8ceda0() {
        if (set) {
            fn_71008dd384(set, &set);
            set = nullptr;
        }
        function = nullptr;
        object = nullptr;
    }
};

// A function bound to an object, registered in a link list while it is.
struct UnkDoorListBound_l8ceda0 {
    UnkLinkList* list;
    void* object;
    void* function;

    ~UnkDoorListBound_l8ceda0() {
        if (list) {
            fn_71002aeb9c(list, &list);
            list = nullptr;
        }
        function = nullptr;
        object = nullptr;
    }
};

// A timer (fn_710002f4e4 builds it).
struct UnkDoorTimer_l8ceda0 {
    u8 raw[0xb0];

    ~UnkDoorTimer_l8ceda0() { fn_710002f67c(this); }
};

// An element of the array at +0xbd8.
struct UnkDoorEntry_l8ceda0 {
    virtual ~UnkDoorEntry_l8ceda0();
    u8 pad_0x8[0x38];
};

struct UnkDoorLifeD_l8ceda0 {
    void** vtable;
    u8 pad_0x8[0x380];
    UnkDoorCallbackList_l8ceda0 callbacks_a;  // 0x388
    UnkDoorCallbackList_l8ceda0 callbacks_b;  // 0x3c8
    u8 pad_0x408[0x10];
    bool overcharged;  // 0x418
    u8 pad_0x419[7];
    UnkDoorStates_l8ceda0 machine;  // 0x420
    UnkDoorState_l8ceda0 state1;    // 0x4d0
    UnkDoorState_l8ceda0 state2;    // 0x540
    UnkDoorState_l8ceda0 state3;    // 0x5b0
    u8 pad_0x620[0x40];
    UnkDoorIdList_l8ceda0 ids;  // 0x660
    u8 pad_0x698[0x728 - 0x698];
    CRntVector<void*> field_0x728;  // 0x728
    UnkDoorLinks_l8ceda0 links;     // 0x750
    u8 pad_0x794[0x7c8 - 0x794];
    u8 field_0x7c8[0x200];  // four list sets (fn_71008cf3ac releases them)
    UnkDoorSetBound_l8ceda0 field_0x9c8;
    UnkDoorSetBound_l8ceda0 field_0x9e0;
    void* field_0x9f8;
    base::UnkRefPtrMember field_0xa00;
    base::UnkRefPtrMember field_0xa08;
    u8 pad_0xa10[8];
    UnkDoorTimer_l8ceda0 timer_a;  // 0xa18
    u8 pad_0xac8[0x30];
    UnkDoorTimer_l8ceda0 timer_b;  // 0xaf8
    UnkDoorListBound_l8ceda0 field_0xba8;
    UnkDoorListBound_l8ceda0 field_0xbc0;
    CRntVector<UnkDoorEntry_l8ceda0> entries;  // 0xbd8
};
static_assert(__builtin_offsetof(UnkDoorLifeD_l8ceda0, machine) == 0x420, "layout");
static_assert(__builtin_offsetof(UnkDoorLifeD_l8ceda0, ids) == 0x660, "layout");
static_assert(__builtin_offsetof(UnkDoorLifeD_l8ceda0, links) == 0x750, "layout");
static_assert(__builtin_offsetof(UnkDoorLifeD_l8ceda0, field_0x9c8) == 0x9c8, "layout");
static_assert(__builtin_offsetof(UnkDoorLifeD_l8ceda0, timer_a) == 0xa18, "layout");
static_assert(__builtin_offsetof(UnkDoorLifeD_l8ceda0, timer_b) == 0xaf8, "layout");
static_assert(__builtin_offsetof(UnkDoorLifeD_l8ceda0, entries) == 0xbd8, "layout");

static inline UnkFxCache* FxCache() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return reinterpret_cast<UnkFxCache*>(g_unk_71726b0cd8);
}

static inline void ReleaseEffect(const char* name) {
    UnkFxCache* cache = FxCache();
    CFilePathStrId path(name, true);
    fn_71011aa5a4(cache, &path);
}

static inline UnkNavMeshManager* NavManager() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

void fn_71008ceda0(UnkDoorLifeD_l8ceda0* self) {
    self->vtable = &g_unk_7101a7e840[2];
    if (self->overcharged) {
        ReleaseEffect("actors/props/door/fx/door_open_overcharge.bcptl");
        ReleaseEffect("actors/props/door/fx/door_open_overcharge_activ.bcptl");
    }
    if (UnkNavMesh* navmesh = NavManager()->navmesh) {
        for (UnkDoorNodePtr_l8ceda0 *it = self->links.links, *end = it + self->links.count; it != end; ++it) {
            UnkNode* link = *it;
            fn_710027e18c(navmesh, link);
            if (link) {
                fn_71002b5698(link);
                fn_7100083c00(link);
            }
        }
        self->links.count = 0;
    }
    fn_710002fa90(&self->timer_a);

    self->entries.Free();
    self->field_0xbc0.~UnkDoorListBound_l8ceda0();
    self->field_0xba8.~UnkDoorListBound_l8ceda0();
    self->timer_b.~UnkDoorTimer_l8ceda0();
    self->timer_a.~UnkDoorTimer_l8ceda0();
    self->field_0xa08.~UnkRefPtrMember();
    self->field_0xa00.~UnkRefPtrMember();
    self->field_0x9e0.~UnkDoorSetBound_l8ceda0();
    self->field_0x9c8.~UnkDoorSetBound_l8ceda0();
    fn_71008cf3ac(reinterpret_cast<UnkListSets_l8cf3ac*>(self->field_0x7c8));
    self->links.~UnkDoorLinks_l8ceda0();
    self->field_0x728.Free();
    self->ids.~UnkDoorIdList_l8ceda0();
    self->state3.~UnkDoorState_l8ceda0();
    self->state2.~UnkDoorState_l8ceda0();
    self->state1.~UnkDoorState_l8ceda0();
    self->machine.~UnkDoorStates_l8ceda0();
    self->callbacks_b.~UnkDoorCallbackList_l8ceda0();
    self->callbacks_a.~UnkDoorCallbackList_l8ceda0();
    fn_71008e05e8(self);
}
