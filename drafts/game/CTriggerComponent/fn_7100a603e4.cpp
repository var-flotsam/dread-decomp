// CTriggerComponent unit (tools/tu_map.py, unit 1578): an actor entered the trigger. The player's name is
// kept; the actor's weak handle and the contact's three words are appended to the component's records;
// the component's owner joins the actor's list of triggers; for the player, the game manager is told
// whether every trigger it is in allows it (+0x238 clear or +0x23e & 0x12) and whether any is active.
// Then the component's vf_0x180 handles the contact. Names and layouts are placeholders.
#include <new>

#include "base/UnkLinkedList.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

void fn_7100f0e208(void* self, bool value);
void fn_7100f0e21c(void* self, bool value);

struct UnkTriggerActor_la603e4;

// The trigger component an actor has at +0x1e8, as read here.
struct UnkTriggerState_la603e4 {
    u8 pad_0x0[0x238];
    bool active;  // 0x238
    u8 pad_0x239[5];
    u8 flags;  // 0x23e
};

struct UnkTriggerNode_la603e4 {
    UnkTriggerActor_la603e4* actor;
    UnkTriggerNode_la603e4* next;
};

struct UnkTriggerActor_la603e4 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
    u8 pad_0x38[0x1b0];
    UnkTriggerState_la603e4* trigger;  // 0x1e8
    u8 pad_0x1f0[0x130];
    base::UnkLinkedList<UnkTriggerNode_la603e4> triggers;  // 0x320
};
static_assert(__builtin_offsetof(UnkTriggerActor_la603e4, triggers) == 0x320, "layout");

struct UnkTriggerInfo_la603e4 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;

    UnkTriggerInfo_la603e4() = default;
    UnkTriggerInfo_la603e4(const UnkTriggerInfo_la603e4& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8) {}
    UnkTriggerInfo_la603e4& operator=(const UnkTriggerInfo_la603e4& other) {
        field_0x0 = other.field_0x0;
        field_0x4 = other.field_0x4;
        field_0x8 = other.field_0x8;
        return *this;
    }
};

// What the trigger hands over about the actor.
struct UnkTriggerContact_la603e4 {
    UnkTriggerActor_la603e4* actor;
    UnkTriggerInfo_la603e4 info;  // 0x8
};

// A counted reference to a weak handle.
struct UnkHandleRef_la603e4 {
    UnkWeakHandle* handle;

    explicit UnkHandleRef_la603e4(UnkWeakHandle* h) : handle(h) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkHandleRef_la603e4() {
        if (handle && --handle->ref_count == 0)
            handle->vf_0x10();
    }
};

#pragma pack(push, 4)
struct UnkTriggerRecord_la603e4 {
    UnkWeakHandle* handle;
    UnkTriggerInfo_la603e4 info;  // 0x8
    u8 pad_0x14[4];

    UnkTriggerRecord_la603e4(UnkWeakHandle* h, UnkTriggerInfo_la603e4 i) : handle(nullptr) {
        AssignRef(&handle, h);
        info = i;
    }
};
#pragma pack(pop)
static_assert(sizeof(UnkTriggerRecord_la603e4) == 0x18, "record");

struct UnkTriggerComponent_la603e4 {
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
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180(UnkTriggerActor_la603e4* actor, const UnkTriggerInfo_la603e4* info);

    u8 pad_0x8[0x28];
    UnkTriggerActor_la603e4* owner;  // 0x30
    u8 pad_0x38[0x28];
    u32 count;  // 0x60
    UnkTriggerRecord_la603e4 records[16];  // 0x64
    u8 pad_0x1e4[4];
    CStrId player_name;  // 0x1e8
};
static_assert(__builtin_offsetof(UnkTriggerComponent_la603e4, player_name) == 0x1e8, "layout");

static inline UnkTriggerActor_la603e4* Player() {
    return reinterpret_cast<UnkTriggerActor_la603e4*>(fn_7100dec788(g_unk_7101d47310));
}

// The game manager's part at +0x38 that the player's trigger state goes to.
static inline void* TriggerState() {
    return reinterpret_cast<char*>(g_unk_7101d47310) + 0x38;
}

void fn_7100a603e4(UnkTriggerComponent_la603e4* self, UnkTriggerContact_la603e4* contact) {
    if (contact->actor == Player())
        self->player_name = contact->actor->name;

    UnkHandleRef_la603e4 handle(fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(contact->actor)));
    UnkTriggerRecord_la603e4* records = self->records;
    new (&records[self->count]) UnkTriggerRecord_la603e4(handle.handle, contact->info);
    ++self->count;

    UnkTriggerActor_la603e4* actor = contact->actor;
    UnkTriggerNode_la603e4* node = static_cast<UnkTriggerNode_la603e4*>(
        actor->triggers.allocator->vf_0x40(sizeof(UnkTriggerNode_la603e4), 8, actor->triggers.flags));
    if (node) {
        node->actor = self->owner;
        node->next = nullptr;
        actor->triggers.PushBack(node);
    }

    UnkTriggerActor_la603e4* entered = contact->actor;
    if (Player() == entered) {
        bool allowed = true;
        bool active = false;
        for (UnkTriggerNode_la603e4* n = entered->triggers.head; n; n = n->next) {
            UnkTriggerState_la603e4* state = n->actor->trigger;
            if (!state)
                continue;
            allowed = allowed && (!state->active || (state->flags & 0x12));
            active = active || state->active;
            if (!allowed && active)
                break;
        }
        fn_7100f0e208(TriggerState(), allowed);
        fn_7100f0e21c(TriggerState(), active);
    }

    self->vf_0x180(contact->actor, &contact->info);
}
