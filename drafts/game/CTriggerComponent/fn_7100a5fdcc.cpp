// CTriggerComponent unit (tools/tu_map.py, unit 1578): an actor left the trigger. The record `*it` is
// removed (the last record moved into its place), the player's name is cleared, the component's
// vf_0x188 handles the exit, the owner leaves the actor's list of triggers; for the player the game
// manager is told again whether every trigger it is in allows it and whether any is active. With no
// record left, vf_0x1a0 runs. Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "base/UnkLinkedList.h"
#include "base/global/CStrId.h"
#include "game/CTriggerComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

void fn_7100f0e208(void* self, bool value);
void fn_7100f0e21c(void* self, bool value);

struct UnkTriggerActor_la5fdcc;

// The trigger component an actor has at +0x1e8, as read here.
struct UnkTriggerState_la5fdcc {
    u8 pad_0x0[0x238];
    bool active;  // 0x238
    u8 pad_0x239[5];
    u8 flags;  // 0x23e
};

struct UnkTriggerNode_la5fdcc {
    UnkTriggerActor_la5fdcc* actor;
    UnkTriggerNode_la5fdcc* next;
};

struct UnkTriggerActor_la5fdcc {
    u8 pad_0x0[0x1e8];
    UnkTriggerState_la5fdcc* trigger;  // 0x1e8
    u8 pad_0x1f0[0x130];
    base::UnkLinkedList<UnkTriggerNode_la5fdcc> triggers;  // 0x320
};
static_assert(__builtin_offsetof(UnkTriggerActor_la5fdcc, triggers) == 0x320, "layout");

struct UnkTriggerInfo_la5fdcc {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};

#pragma pack(push, 4)
// A weak reference to an actor in the trigger and three words.
struct UnkTriggerRecord_la5fdcc {
    UnkWeakHandle* handle;
    UnkTriggerInfo_la5fdcc info;  // 0x8
    u8 pad_0x14[4];

    ~UnkTriggerRecord_la5fdcc() {
        if (UnkWeakHandle* h = handle) {
            if (--h->ref_count == 0)
                h->vf_0x10();
        }
    }
    // The copy-assignment, out of line.
    UnkTriggerRecord_la5fdcc& operator=(const UnkTriggerRecord_la5fdcc& other) asm("fn_7100a64a58");

    UnkTriggerActor_la5fdcc* Actor() const {
        return reinterpret_cast<UnkTriggerActor_la5fdcc*>(handle ? handle->target : nullptr);
    }
};
#pragma pack(pop)
static_assert(sizeof(UnkTriggerRecord_la5fdcc) == 0x18, "record");

struct UnkTriggerComponent_la5fdcc {
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
    virtual void vf_0x180();
    virtual void vf_0x188(UnkTriggerActor_la5fdcc* actor, const UnkTriggerInfo_la5fdcc* info);
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0(UnkTriggerActor_la5fdcc* actor, const UnkTriggerInfo_la5fdcc* info);

    u8 pad_0x8[0x28];
    UnkTriggerActor_la5fdcc* owner;  // 0x30
    u8 pad_0x38[0x28];
    u32 count;  // 0x60
    UnkTriggerRecord_la5fdcc records[16];  // 0x64
    u8 pad_0x1e4[4];
    CStrId player_name;  // 0x1e8

    void Resize(u32 n) {
        if (n > count) {
            for (u32 i = count; i < n; ++i)
                new (&records[i]) UnkTriggerRecord_la5fdcc();  // zeroed
        } else {
            for (u32 i = n; i < count; ++i)
                records[i].~UnkTriggerRecord_la5fdcc();
        }
        count = n;
    }

    // Removes `pos`, moving the last record into its place; returns where the next one is.
    UnkTriggerRecord_la5fdcc* Erase(UnkTriggerRecord_la5fdcc* pos) {
        UnkTriggerRecord_la5fdcc* last = records + count - 1;
        if (last != pos)
            *pos = *last;
        Resize(count - 1);
        return pos;
    }
};
static_assert(__builtin_offsetof(UnkTriggerComponent_la5fdcc, player_name) == 0x1e8, "layout");

static inline UnkTriggerActor_la5fdcc* Player() {
    return reinterpret_cast<UnkTriggerActor_la5fdcc*>(fn_7100dec788(g_unk_7101d47310));
}

// The game manager's part at +0x38 that the player's trigger state goes to.
static inline void* TriggerState() {
    return reinterpret_cast<char*>(g_unk_7101d47310) + 0x38;
}

void fn_7100a5fdcc(UnkTrigger_b50a5* trigger, UnkTriggerEntry_b50a5** __attribute__((noescape)) entry) {
    UnkTriggerComponent_la5fdcc* self = reinterpret_cast<UnkTriggerComponent_la5fdcc*>(trigger);
    UnkTriggerRecord_la5fdcc** it = reinterpret_cast<UnkTriggerRecord_la5fdcc**>(entry);

    UnkTriggerActor_la5fdcc* actor = (*it)->Actor();
    if (actor == Player())
        self->player_name = g_unk_7101d470e0;

    *it = self->Erase(*it);
    self->vf_0x188(actor, &(*it)->info);

    UnkTriggerNode_la5fdcc* node = actor->triggers.head;
    while (node && node->actor != self->owner)
        node = node->next;
    if (node)
        actor->triggers.Remove(node);

    if (actor == Player()) {
        bool allowed = true;
        bool active = false;
        for (UnkTriggerNode_la5fdcc* n = actor->triggers.head; n; n = n->next) {
            UnkTriggerState_la5fdcc* state = n->actor->trigger;
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

    if (self->count == 0)
        self->vf_0x1a0(actor, nullptr);
}
