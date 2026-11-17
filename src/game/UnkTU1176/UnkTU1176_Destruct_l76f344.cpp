// Unit 1176 (CCentralUnitComponent): the component's destructor. Destroys the members last first (the
// callback sets, arrays, listeners, string ids and the state machine's states), then runs the base's.
// Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CActorComponent.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/UnkTU341.h"
#include "game/UnkTU39.h"
#include "game/navmesh/SNavMeshPath.h"
#include "game_s2/UnkOwnedList.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

extern void* g_unk_7101a6ec78[];
extern char g_unk_7101a6eed8[];
extern char g_unk_7101a6ee80[];
struct UnkPair_7100778228;
void fn_7100778228(UnkPair_7100778228*);

// A reference-counted object: its third virtual releases it.
struct UnkCounted_l76f344 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();
    u32 ref_count;
    UnkWeakTarget* owner;
};

struct UnkCountedRef_l76f344 {
    UnkCounted_l76f344* ptr;

    ~UnkCountedRef_l76f344() {
        if (ptr && --ptr->ref_count == 0)
            ptr->Release();
    }
};

// A stored function: its operations table destroys it in place.
struct UnkFunction_l76f344;
struct UnkFunctionOps_l76f344 {
    void* vf_0x0[8];
    void (*destroy)(UnkFunction_l76f344* self);
};

struct UnkFunction_l76f344 {
    u64 storage;
    const UnkFunctionOps_l76f344* ops;

    ~UnkFunction_l76f344() {
        if (ops) {
            ops->destroy(this);
            ops = nullptr;
        }
    }
};

// A 0x40-byte entry with a virtual destructor.
struct UnkPolyEntry_l76f344 {
    virtual ~UnkPolyEntry_l76f344();
    u8 data[0x38];
};

// A 0x80-byte entry destroyed by fn_7100778228.
struct UnkEntry_l76f344 {
    char data[0x80];
    ~UnkEntry_l76f344() { fn_7100778228(reinterpret_cast<UnkPair_7100778228*>(this)); }
};

// A callback set: an array of functions under the base vtable, a counted reference under its own.
struct UnkCallbackSetBase_l76f344 {
    void** vtable;
    CRntVector<UnkFunction_l76f344> functions;

    ~UnkCallbackSetBase_l76f344() {
        vtable = g_unk_7101a20998 + 2;
        functions.Free();
    }
};

struct UnkCallbackSetA_l76f344 : UnkCallbackSetBase_l76f344 {
    u32 field_0x30;
    UnkCountedRef_l76f344 field_0x38;

    ~UnkCallbackSetA_l76f344() { vtable = g_unk_7101a33fa0 + 2; }
};
static_assert(sizeof(UnkCallbackSetA_l76f344) == 0x40, "UnkCallbackSetA layout");

struct UnkCallbackSetB_l76f344 : UnkCallbackSetBase_l76f344 {
    u32 field_0x30;
    UnkCountedRef_l76f344 field_0x38;

    ~UnkCallbackSetB_l76f344() { vtable = g_unk_7101a20950 + 2; }
};
static_assert(sizeof(UnkCallbackSetB_l76f344) == 0x40, "UnkCallbackSetB layout");

// Callbacks registered with a list, a deferred set or a registry; each unregisters itself.
struct UnkListCallback_l76f344 {
    UnkLinkList* list;
    void* owner;
    void (*fn)(void*);

    ~UnkListCallback_l76f344() {
        if (list) {
            fn_71002aeb9c(list, &list);
            list = nullptr;
        }
        fn = nullptr;
        owner = nullptr;
    }
};

struct UnkRegistryCallback_l76f344 {
    void* registry;
    void* owner;
    void (*fn)(void*);

    ~UnkRegistryCallback_l76f344() {
        if (registry) {
            fn_71004567dc(registry, &registry);
            registry = nullptr;
        }
        fn = nullptr;
        owner = nullptr;
    }
};

struct UnkDeferredCallback_l76f344 {
    base::UnkDeferredSet<void>* set;
    void* owner;
    void (*fn)(void*);

    ~UnkDeferredCallback_l76f344() {
        if (set) {
            fn_71004f3ea4(set, this);
            set = nullptr;
        }
        fn = nullptr;
        owner = nullptr;
    }
};

// A listener: detaches itself from its link's owner and drops the link.
struct UnkListener_l76f344 {
    void** vtable;
    void* field_0x8;
    void* field_0x10;
    UnkCounted_l76f344* link;
    u32 field_0x20;
    u8 field_0x24;

    ~UnkListener_l76f344() {
        vtable = g_unk_7101a22d48 + 2;
        if (!link)
            return;
        if (UnkWeakTarget* target = link->owner) {
            fn_710117b3f0(target, this);
            if (!link)
                return;
        }
        if (--link->ref_count == 0)
            link->Release();
    }
};

// A string id and a table of string ids.
struct UnkStrIdList_l76f344 {
    u8 field_0x0;
    CStrId id;
    UnkStrIdTable table;

    ~UnkStrIdList_l76f344() {
        fn_71004f11f8(&table);
        if (table.keys)
            table.allocator->vf_0x48(table.keys);
        if (table.values)
            table.allocator->vf_0x48(table.values);
    }
};
static_assert(sizeof(UnkStrIdList_l76f344) == 0x38, "UnkStrIdList layout");

struct UnkStateMachine_l76f344;

struct UnkStateHandler_l76f344 {
    void* owner;
    void (*fn)(void*);

    ~UnkStateHandler_l76f344() {
        fn = nullptr;
        owner = nullptr;
    }
};

struct UnkStateBase_l76f344 {
    void** vtable;
    UnkStateMachine_l76f344* machine;
    u32 id;
    base::reflection::UnkReflValue value;

    ~UnkStateBase_l76f344() {
        vtable = reinterpret_cast<void**>(g_unk_7101a6ee80 + 0x10);
        fn_71000a1c60(&value);
    }
};

struct UnkState_l76f344 : UnkStateBase_l76f344 {
    UnkStateHandler_l76f344 handlers[4];
};
static_assert(sizeof(UnkState_l76f344) == 0x70, "UnkState layout");

struct UnkStateMachine_l76f344 {
    void** vtable;
    void* owner;
    UnkState_l76f344* current;
    UnkState_l76f344* previous;
    u32 count;
    u8 storage[16 * sizeof(UnkState_l76f344*)];
    u32 pending_id;
    u8 changing;
    u8 field_0xd9;

    ~UnkStateMachine_l76f344() {
        vtable = reinterpret_cast<void**>(g_unk_7101a6eed8 + 0x10);
        count = 0;
        current = nullptr;
        previous = nullptr;
    }
};
static_assert(sizeof(UnkStateMachine_l76f344) == 0xb0, "UnkStateMachine layout");

struct UnkCentralUnitComponent_l76f344 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkStateMachine_l76f344 machine;  // 0x30
    UnkState_l76f344 states[7];  // 0xe0
    u32 field_0x3f0;
    UnkCallbackSetA_l76f344 sets_0x3f8[5];
    CStrId name;  // 0x538
    UnkCallbackSetA_l76f344 set_0x540;
    CRntVector<UnkEntry_l76f344> entries;  // 0x580
    UnkCallbackSetB_l76f344 set_0x5a8;
    CRntVector<UnkPolyEntry_l76f344> list_0x5e8;
    UnkOwnedList owned;  // 0x610
    bool field_0x638;
    CRntVector<UnkPolyEntry_l76f344> list_0x640;
    UnkCallbackSetB_l76f344 set_0x668;
    CRntVector<UnkPolyEntry_l76f344> list_0x6a8;
    u8 field_0x6d0;
    UnkCountedRef_l76f344 ref_0x6d8;
    UnkListCallback_l76f344 callback_0x6e0;
    UnkCountedRef_l76f344 ref_0x6f8;
    UnkCountedRef_l76f344 ref_0x700;
    UnkListCallback_l76f344 callbacks_0x708[3];
    UnkRegistryCallback_l76f344 callback_0x750;
    UnkListener_l76f344 listener;  // 0x768
    UnkStrIdList_l76f344 str_list;  // 0x790
    UnkDeferredCallback_l76f344 callbacks_0x7c8[2];
    u8 move_slot[0xb0];  // 0x7f8
    u8 block_0x8a8[0xb0];
    void* field_0x958;
    f32 scale_x;
    f32 scale_y;
    u32 field_0x968;
    UnkCallbackSetB_l76f344 set_0x970;
    CStrId empty_name;  // 0x9b0
    UnkCallbackSetB_l76f344 set_0x9b8;
    bool field_0x9f8;
};
static_assert(__builtin_offsetof(UnkCentralUnitComponent_l76f344, sets_0x3f8) == 0x3f8, "layout");
static_assert(__builtin_offsetof(UnkCentralUnitComponent_l76f344, entries) == 0x580, "layout");
static_assert(__builtin_offsetof(UnkCentralUnitComponent_l76f344, ref_0x6d8) == 0x6d8, "layout");
static_assert(__builtin_offsetof(UnkCentralUnitComponent_l76f344, listener) == 0x768, "layout");
static_assert(__builtin_offsetof(UnkCentralUnitComponent_l76f344, callbacks_0x7c8) == 0x7c8, "layout");
static_assert(__builtin_offsetof(UnkCentralUnitComponent_l76f344, set_0x970) == 0x970, "layout");
static_assert(__builtin_offsetof(UnkCentralUnitComponent_l76f344, field_0x9f8) == 0x9f8, "layout");

void fn_710076f344(UnkCentralUnitComponent_l76f344* self) {
    self->vtable = g_unk_7101a6ec78 + 2;
    self->set_0x9b8.~UnkCallbackSetB_l76f344();
    self->empty_name.~CStrId();
    self->set_0x970.~UnkCallbackSetB_l76f344();
    fn_710002f67c(self->block_0x8a8);
    fn_7100030a50(reinterpret_cast<UnkMoveSlot_0xb0*>(self->move_slot));
    self->callbacks_0x7c8[1].~UnkDeferredCallback_l76f344();
    self->callbacks_0x7c8[0].~UnkDeferredCallback_l76f344();
    self->str_list.~UnkStrIdList_l76f344();
    self->listener.~UnkListener_l76f344();
    self->callback_0x750.~UnkRegistryCallback_l76f344();
    for (int i = 2; i >= 0; i--)
        self->callbacks_0x708[i].~UnkListCallback_l76f344();
    self->ref_0x700.~UnkCountedRef_l76f344();
    self->ref_0x6f8.~UnkCountedRef_l76f344();
    self->callback_0x6e0.~UnkListCallback_l76f344();
    self->ref_0x6d8.~UnkCountedRef_l76f344();
    self->list_0x6a8.Free();
    self->set_0x668.~UnkCallbackSetB_l76f344();
    self->list_0x640.Free();
    self->owned.~UnkOwnedList();
    self->list_0x5e8.Free();
    self->set_0x5a8.~UnkCallbackSetB_l76f344();
    self->entries.Free();
    self->set_0x540.~UnkCallbackSetA_l76f344();
    self->name.~CStrId();
    for (int i = 4; i >= 0; i--)
        self->sets_0x3f8[i].~UnkCallbackSetA_l76f344();
    for (int i = 6; i >= 0; i--)
        self->states[i].~UnkState_l76f344();
    self->machine.~UnkStateMachine_l76f344();
    fn_710024f6d4(reinterpret_cast<UnkBaseComponent*>(self));
}
