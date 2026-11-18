// CCharClassBaseTriggerComponent's unit (tools/tu_map.py unit 1562): the destructor. Every entry is
// erased (fn_7100a47170), the pending exits are told through the virtual 0x1b8, the collider goes back
// to the collision manager, then the node at +0xa10, the callback set at +0x9d0, the player's name and
// the entries are destroyed before the activatable base destructor (fn_71007b00cc). Names and layouts
// are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CDemolitionBlockComponent.h"
#include "game/UnkCallbackSet_7101a20950.h"
#include "game/UnkTU2506.h"
#include "game/UnkTU341.h"
#include "game/UnkTU365.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a8ebc8[];

struct UnkTriggerActor_la46b64;

struct UnkTriggerInfo_la46b64 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;

    UnkTriggerInfo_la46b64(const UnkTriggerInfo_la46b64& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8) {}
};

// An actor in the trigger: a counted reference to its handle and the entry info.
struct UnkTriggerEntry_la46b64 {
    UnkRefCounted* handle;
    UnkTriggerInfo_la46b64 info;

    ~UnkTriggerEntry_la46b64() {
        if (UnkRefCounted* p = handle) {
            if (--p->ref_count == 0)
                p->Delete();
        }
    }
};

// An exit still to be reported: the actor and the entry info.
struct UnkTriggerExit_la46b64 {
    UnkTriggerActor_la46b64* actor;
    UnkTriggerInfo_la46b64 info;
};

struct UnkBaseTrigger_la46b64;

struct UnkBaseTriggerVtbl_la46b64 {
    void* pad_0x0[0x1b8 / 8];
    void (*OnExit)(UnkBaseTrigger_la46b64* self, UnkTriggerActor_la46b64* actor,
                   UnkTriggerInfo_la46b64* info);  // 0x1b8
};

struct UnkBaseTrigger_la46b64 {
    UnkBaseTriggerVtbl_la46b64* vtable;
    u8 pad_0x8[0x80];
    UnkCollisionObject* collider;  // 0x88
    u8 pad_0x90[0x18];
    u32 count;                                          // 0xa8
    u8 storage[32 * sizeof(UnkTriggerEntry_la46b64)];  // 0xac
    u32 exit_count;                                     // 0x3ac
    u8 exit_storage[64 * sizeof(UnkTriggerExit_la46b64)];  // 0x3b0
    CStrId player_name;                                 // 0x9b0
    u32 field_0x9b8;
    u8 pad_0x9bc[0x14];
    UnkCallbackSet_7101a20950 set;  // 0x9d0
    u8 node[0x8];                   // 0xa10

    UnkTriggerEntry_la46b64* items() { return reinterpret_cast<UnkTriggerEntry_la46b64*>(storage); }
    UnkTriggerEntry_la46b64* LastEntry() { return items() + count - 1; }
    UnkTriggerExit_la46b64* exits() { return reinterpret_cast<UnkTriggerExit_la46b64*>(exit_storage); }
    UnkTriggerExit_la46b64* LastExit() { return exits() + exit_count - 1; }
};
static_assert(__builtin_offsetof(UnkBaseTrigger_la46b64, player_name) == 0x9b0, "layout");
static_assert(__builtin_offsetof(UnkBaseTrigger_la46b64, node) == 0xa10, "layout");

void fn_7100a47170(UnkBaseTrigger_la46b64* self, UnkTriggerEntry_la46b64* const* entry);

static inline UnkManager_7100863a8c* Manager() {
    UnkManager_7100863a8c* m = g_unk_71726a4788;
    if (!m) {
        m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return m;
}

// The derived triggers' destructors call this one with their own pointer (src/clones/clones_03.cpp).
void fn_7100a46b64(char* object) {
    UnkBaseTrigger_la46b64* self = reinterpret_cast<UnkBaseTrigger_la46b64*>(object);
    self->vtable = reinterpret_cast<UnkBaseTriggerVtbl_la46b64*>(g_unk_7101a8ebc8 + 2);
    while (self->count != 0) {
        UnkTriggerEntry_la46b64* last = self->LastEntry();
        fn_7100a47170(self, &last);
    }
    self->field_0x9b8 = 0;
    while (self->exit_count != 0) {
        UnkTriggerExit_la46b64 exit = *self->LastExit();
        --self->exit_count;
        self->vtable->OnExit(self, exit.actor, &exit.info);
    }
    if (self->collider) {
        fn_7100d46c60(Manager(), self->collider);
        self->collider = nullptr;
    }
    fn_710002f67c(self->node);
    self->set.~UnkCallbackSet_7101a20950();
    self->player_name.Reset();
    self->exit_count = 0;
    for (u32 i = 0; i < self->count; ++i)
        self->items()[i].~UnkTriggerEntry_la46b64();
    self->count = 0;
    fn_71007b00cc(reinterpret_cast<UnkActivatable_b44a2<0x48>*>(self));
}
