// CAccessPointComponent unit (tools/tu_map.py, unit 1581): the access point's destructor body. It releases
// the members fn_7100a6c9f4 builds, in reverse: the name, the registry map, the arrays, the listener, the
// deferred-set registration, the four states, the state machine, the chained map, three string ids and the
// callback set, then runs the usable component's destructor. Names and layouts are placeholders.
#include "base/UnkChainedMap.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityGhostAura.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CUsableComponent.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

extern void* g_unk_7101a8fe10[];
extern void* g_unk_7101a90320[];
extern void* g_unk_7101a902c8[];
void fn_7100065f0c(char* self);
struct UnkNamedIds;
void fn_7100a73ec8(UnkNamedIds* self);

struct UnkClone_fn_7100a73fd0;
using UnkApRegistrations_la6ce48 = base::UnkDeferredSet<UnkClone_fn_7100a73fd0>;
extern template void UnkApRegistrations_la6ce48::Remove(UnkClone_fn_7100a73fd0*);

// A callback slot: the operations it was bound with destroy it.
struct UnkApSlot_la6ce48;
struct UnkApSlotOps_la6ce48 {
    u8 pad_0x0[0x40];
    void (*destroy)(UnkApSlot_la6ce48* slot);
};
struct UnkApSlot_la6ce48 {
    u64 field_0x0;
    UnkApSlotOps_la6ce48* ops;

    ~UnkApSlot_la6ce48() {
        if (ops) {
            ops->destroy(this);
            ops = nullptr;
        }
    }
};
static_assert(sizeof(UnkApSlot_la6ce48) == 0x10, "layout");

// A 0x40-byte entry destroyed through its vtable.
struct UnkApEntry_la6ce48 {
    virtual ~UnkApEntry_la6ce48();
    u8 pad_0x8[0x38];
};
static_assert(sizeof(UnkApEntry_la6ce48) == 0x40, "layout");

struct UnkApCallbacks_la6ce48 {
    void** vtable;
    CRntVector<UnkApSlot_la6ce48> items;  // 0x8
    u32 field_0x30;
    UnkRefObject* field_0x38;
    CStrId id_0x40;
    CStrId id_0x48;
    u16 field_0x50;
    CStrId id_0x58;
};
static_assert(sizeof(UnkApCallbacks_la6ce48) == 0x60, "layout");

struct UnkApDelegate_la6ce48 {
    void* object;
    void (*function)();
};

struct UnkApMachine_la6ce48;

struct UnkApState_la6ce48 {
    void** vtable;
    UnkApMachine_la6ce48* machine;
    u32 id;
    base::reflection::UnkReflValue value;  // 0x18
    UnkApDelegate_la6ce48 field_0x30;
    UnkApDelegate_la6ce48 field_0x40;
    UnkApDelegate_la6ce48 field_0x50;
    u64 field_0x60;
    u64 field_0x68;
};
static_assert(sizeof(UnkApState_la6ce48) == 0x70, "layout");

struct UnkApMachine_la6ce48 {
    void** vtable;
    void* owner;
    UnkApState_la6ce48* current;
    UnkApState_la6ce48* previous;
    u32 count;
    u32 states[0x20];  // UnkApState_la6ce48* entries, 4-byte aligned
    u32 pending;
    bool busy;
    bool field_0xa9;
    u8 pad_0xaa[0xb8 - 0xaa];
};
static_assert(sizeof(UnkApMachine_la6ce48) == 0xb8, "layout");

// A node of the chained map: its ids, then the list link.
struct UnkApMapNode_la6ce48 {
    u8 ids[0x30];
    UnkApMapNode_la6ce48* next;
};

struct UnkApRegistry_la6ce48 {
    bool initialized;
    CStrId name;
    UnkOwnedStrIdTable table;  // 0x10
};
static_assert(sizeof(UnkApRegistry_la6ce48) == 0x38, "layout");

struct UnkAp_la6ce48 {
    void** vtable;
    u8 pad_0x8[0x540 - 0x8];
    UnkApCallbacks_la6ce48 callbacks;  // 0x540
    UnkChainedMap map;  // 0x5a0
    UnkApMachine_la6ce48 machine;  // 0x5e8
    UnkApState_la6ce48 states[4];  // 0x6a0
    UnkApRegistrations_la6ce48* registrations;  // 0x860
    UnkApDelegate_la6ce48 field_0x868;
    void** listener_vtable;  // 0x878
    u64 field_0x880;
    u64 field_0x888;
    UnkWeakHandle* listener_link;  // 0x890
    CRntVector<UnkApEntry_la6ce48> listener_items;  // 0x898
    CRntVector<CStrId> list;  // 0x8c0
    u8 pad_0x8e8[0x928 - 0x8e8];
    UnkApRegistry_la6ce48 registry;  // 0x928
    u8 pad_0x960[0x978 - 0x960];
    char field_0x978[0x18];
    CStrId name;  // 0x990
    bool field_0x998;
};
static_assert(__builtin_offsetof(UnkAp_la6ce48, listener_link) == 0x890, "layout");
static_assert(__builtin_offsetof(UnkAp_la6ce48, registry) == 0x928, "layout");
static_assert(__builtin_offsetof(UnkAp_la6ce48, name) == 0x990, "layout");

static inline void DestroyState(UnkApState_la6ce48* state) {
    state->field_0x30 = {nullptr, nullptr};
    state->field_0x40 = {nullptr, nullptr};
    state->field_0x50 = {nullptr, nullptr};
    state->field_0x60 = 0;
    state->field_0x68 = 0;
    state->vtable = g_unk_7101a902c8 + 2;
    fn_71000a1c60(&state->value);
}

static inline void Release(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

void fn_7100a6ce48(UnkAp_la6ce48* self) {
    self->vtable = g_unk_7101a8fe10 + 2;
    self->name.~CStrId();
    fn_7100065f0c(self->field_0x978);

    UnkApRegistry_la6ce48& registry = self->registry;
    registry.table.~UnkOwnedStrIdTable();
    registry.name.~CStrId();

    self->list.Free();
    self->listener_items.Free();

    self->listener_vtable = g_unk_7101a22d48 + 2;
    if (UnkWeakHandle* link = self->listener_link) {
        if (UnkWeakTarget* target = link->target) {
            fn_710117b3f0(target, &self->listener_vtable);
            link = self->listener_link;
        }
        if (link)
            Release(link);
    }

    if (self->registrations) {
        self->registrations->Remove(reinterpret_cast<UnkClone_fn_7100a73fd0*>(&self->registrations));
        self->registrations = nullptr;
    }
    self->field_0x868.function = nullptr;
    self->field_0x868.object = nullptr;

    DestroyState(&self->states[3]);
    DestroyState(&self->states[2]);
    DestroyState(&self->states[1]);
    DestroyState(&self->states[0]);

    UnkApMachine_la6ce48& machine = self->machine;
    machine.vtable = g_unk_7101a90320 + 2;
    machine.current = nullptr;
    machine.previous = nullptr;
    machine.count = 0;

    UnkChainedMap& map = self->map;
    if (map.count) {
        UnkApMapNode_la6ce48* node = reinterpret_cast<UnkApMapNode_la6ce48*>(map.tail);
        while (node) {
            UnkApMapNode_la6ce48* next = node->next;
            fn_7100a73ec8(reinterpret_cast<UnkNamedIds*>(node));
            map.allocator->vf_0x48(node);
            node = next;
        }
        __builtin_memset(map.buckets.data, 0, (u64)map.buckets.capacity * sizeof(UnkMapNode*));
        map.count = 0;
        map.head = nullptr;
        map.tail = nullptr;
    }
    map.buckets.Free();

    self->callbacks.id_0x58.~CStrId();
    self->callbacks.id_0x48.~CStrId();
    self->callbacks.id_0x40.~CStrId();
    self->callbacks.vtable = g_unk_7101a33fa0 + 2;
    Release(self->callbacks.field_0x38);
    self->callbacks.vtable = g_unk_7101a20998 + 2;
    self->callbacks.items.Free();

    fn_7100a9e6bc(reinterpret_cast<char*>(self));
}
