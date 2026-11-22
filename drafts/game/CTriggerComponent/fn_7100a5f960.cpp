// CTriggerComponent unit (tools/tu_map.py, unit 1578): drops an actor from the trigger. Its record is
// found and removed (the last record moved into its place), the name is cleared when the game manager
// says so (fn_7100df5c38), the component's vf_0x188 handles the exit, the owner leaves the actor's list
// of triggers; for the player the game manager is told again whether every trigger it is in allows it
// and whether any is active. With no record left, vf_0x1a0 runs. Names and layouts are placeholders.
#include <algorithm>
#include <new>
#include <string.h>

#include "base/UnkLinkedList.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"
#include "../../../src/game_s7/UnkIter.h"

using base::global::CStrId;

void fn_7100f0e208(void* self, bool value);
void fn_7100f0e21c(void* self, bool value);

struct UnkTriggerActor_la5f960;

// The trigger component an actor has at +0x1e8, as read here.
struct UnkTriggerState_la5f960 {
    u8 pad_0x0[0x238];
    bool active;  // 0x238
    u8 pad_0x239[5];
    u8 flags;  // 0x23e
};

struct UnkTriggerNode_la5f960 {
    UnkTriggerActor_la5f960* actor;
    UnkTriggerNode_la5f960* next;
};

struct UnkTriggerActor_la5f960 {
    u8 pad_0x0[0x1e8];
    UnkTriggerState_la5f960* trigger;  // 0x1e8
    u8 pad_0x1f0[0x130];
    base::UnkLinkedList<UnkTriggerNode_la5f960> triggers;  // 0x320
};
static_assert(__builtin_offsetof(UnkTriggerActor_la5f960, triggers) == 0x320, "layout");

struct UnkTriggerInfo_la5f960 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};

#pragma pack(push, 4)
// A weak reference to an actor in the trigger and three words.
struct UnkTriggerRecord_la5f960 {
    UnkWeakHandle* handle;
    UnkTriggerInfo_la5f960 info;  // 0x8
    u8 pad_0x14[4];

    ~UnkTriggerRecord_la5f960() {
        if (UnkWeakHandle* h = handle) {
            if (--h->ref_count == 0)
                h->vf_0x10();
        }
    }
    // The copy-assignment, out of line.
    UnkTriggerRecord_la5f960& operator=(const UnkTriggerRecord_la5f960& other) asm("fn_7100a64a58");

    UnkTriggerActor_la5f960* Actor() const {
        return reinterpret_cast<UnkTriggerActor_la5f960*>(handle ? handle->target : nullptr);
    }
};
#pragma pack(pop)
static_assert(sizeof(UnkTriggerRecord_la5f960) == 0x18, "record");

struct UnkTriggerComponent_la5f960 {
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
    virtual void vf_0x188(UnkTriggerActor_la5f960* actor, const UnkTriggerInfo_la5f960* info);
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0(UnkTriggerActor_la5f960* actor, const UnkTriggerInfo_la5f960* info);

    u8 pad_0x8[0x28];
    UnkTriggerActor_la5f960* owner;  // 0x30
    u8 pad_0x38[0x28];
    u32 count;  // 0x60
    UnkTriggerRecord_la5f960 records[16];  // 0x64
    u8 pad_0x1e4[4];
    CStrId player_name;  // 0x1e8

    void Resize(u32 n) {
        if (n > count) {
            for (u64 i = count; i < n; ++i)
                new (&records[i]) UnkTriggerRecord_la5f960();  // zeroed
        } else {
            for (u64 i = n; i < count; ++i)
                records[i].~UnkTriggerRecord_la5f960();
        }
        count = n;
    }

    // Removes `pos`, moving the last record into its place; returns where the next one is.
    UnkTriggerRecord_la5f960* Erase(UnkTriggerRecord_la5f960* pos) {
        UnkTriggerRecord_la5f960* last = records + count - 1;
        if (last != pos)
            *pos = *last;
        Resize(count - 1);
        return pos;
    }
};
static_assert(__builtin_offsetof(UnkTriggerComponent_la5f960, player_name) == 0x1e8, "layout");

static inline UnkTriggerActor_la5f960* Player() {
    return reinterpret_cast<UnkTriggerActor_la5f960*>(fn_7100dec788(g_unk_7101d47310));
}

// The game manager's part at +0x38 that the player's trigger state goes to.
static inline void* TriggerState() {
    return reinterpret_cast<char*>(g_unk_7101d47310) + 0x38;
}

void fn_7100a5f960(UnkTriggerComponent_la5f960* self, UnkTriggerActor_la5f960* actor) {
    using Iter = UnkIter<UnkTriggerRecord_la5f960>;
    Iter end{self->records + self->count};
    Iter found = std::find_if(Iter{self->records}, end, [actor](const UnkTriggerRecord_la5f960& r) {
        return r.handle && r.handle->target == reinterpret_cast<UnkWeakTarget*>(actor);
    });
    if (found == end)
        return;
    UnkTriggerRecord_la5f960* it = found.p;

    if (fn_7100df5c38(g_unk_7101d47310, actor))
        self->player_name = g_unk_7101d470e0;

    self->Erase(it);
    self->vf_0x188(actor, nullptr);

    UnkTriggerNode_la5f960* node = actor->triggers.head;
    while (node && node->actor != self->owner)
        node = node->next;
    if (node)
        actor->triggers.Remove(node);

    if (Player() == actor) {
        bool allowed = true;
        bool active = false;
        for (UnkTriggerNode_la5f960* n = actor->triggers.head; n; n = n->next) {
            UnkTriggerState_la5f960* state = n->actor->trigger;
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
