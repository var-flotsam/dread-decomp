// CMagmaKraidPistonPlatformComponent's unit (tools/tu_map.py 1404): on message 1, the piston leaves state 4 for
// state 1 and wakes Kraid's AI (fn_71005fe634), or else goes to state 2; then it drops the weak handle at +0x2d0
// and unlinks its listener at +0x2b8. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkStateMachineFind.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::CClass;

struct UnkKraidAI_b57a4;
void fn_71005fe634(UnkKraidAI_b57a4* self);

// The string id that names Kraid's entity (reached through the GOT).
extern base::global::CStrId g_unk_7172736a18;

struct UnkPistonMachine_l919254;

struct UnkPistonState_l919254 {
    void** vtable;
    UnkPistonMachine_l919254* machine;
    u32 id;

    void Enter(u32 previous_id) {
        reinterpret_cast<void (*)(UnkPistonState_l919254*, u32)>(vtable[0x10 / 8])(this, previous_id);
    }
    void Leave(u32 next_id) {
        reinterpret_cast<void (*)(UnkPistonState_l919254*, u32)>(vtable[0x18 / 8])(this, next_id);
    }
};

static const u32 kNoState_l919254 = 0x7fffffff;

struct UnkPistonMachine_l919254 {
    void** vtable;
    void* owner;
    UnkPistonState_l919254* current;
    UnkPistonState_l919254* previous;
    u32 count;
    u32 states[0x20];  // UnkPistonState_l919254* entries, 4-byte aligned
    UnkPistonState_l919254** States() { return reinterpret_cast<UnkPistonState_l919254**>(states); }
    u32 requested;  // 0xa4
    bool locked;    // 0xa8
    u8 pad_0xa9[0xb0 - 0xa9];

    void OnEntered(UnkPistonState_l919254* from, UnkPistonState_l919254* to) {
        reinterpret_cast<void (*)(UnkPistonMachine_l919254*, UnkPistonState_l919254*, UnkPistonState_l919254*)>(
            vtable[0x20 / 8])(this, from, to);
    }
    void OnLeaving(UnkPistonState_l919254* from, UnkPistonState_l919254* to) {
        reinterpret_cast<void (*)(UnkPistonMachine_l919254*, UnkPistonState_l919254*, UnkPistonState_l919254*)>(
            vtable[0x28 / 8])(this, from, to);
    }

    UnkPistonState_l919254* Find(u32 id) {
        return FindStateById(States(), count, id);
    }

    // Leaves the current state for the one with `id`, unless it is current already.
    void ChangeTo(u32 id) {
        if (current && current->id == id)
            return;
        UnkPistonState_l919254* next = Find(id);
        if (!next)
            return;
        if (current) {
            current->Leave(id);
            OnLeaving(current, next);
        }
        previous = current;
        current = next;
        next->Enter(previous ? previous->id : kNoState_l919254);
        OnEntered(previous, next);
    }

    // Asks for the state with `id` and, unless the machine is locked, enters it now.
    void Request(u32 id) {
        u32& pending = requested;
        pending = id;
        if (locked)
            return;
        ChangeTo(id);
        requested = kNoState_l919254;
    }
};
static_assert(sizeof(UnkPistonMachine_l919254) == 0xb0, "layout");

struct UnkKraidComponent_l919254 {
    void** vtable;

    const CClass* Type() { return reinterpret_cast<const CClass* (*)(UnkKraidComponent_l919254*)>(vtable[0])(this); }
};

struct UnkKraidEntity_l919254 {
    u8 pad_0x0[0x240];
    UnkKraidComponent_l919254* component;  // 0x240
};

struct UnkPistonMessage_l919254 {
    u32 kind;
};

struct UnkPistonPlatform_l919254 {
    u8 pad_0x0[0x40];
    UnkPistonMachine_l919254 machine;  // 0x40
    u8 pad_0xf0[0x2b8 - 0xf0];
    u8 listener[0x18];        // 0x2b8
    UnkWeakHandle* handle;    // 0x2d0
};
static_assert(__builtin_offsetof(UnkPistonPlatform_l919254, handle) == 0x2d0, "layout");

bool fn_7100919254(UnkPistonPlatform_l919254* self, const UnkPistonMessage_l919254* message) {
    if (message->kind != 1)
        return true;
    UnkPistonState_l919254* current = self->machine.current;
    if (current && current->id == 4) {
        self->machine.Request(1);
        UnkGameManager* manager = g_unk_7101d47310;
        u64 key;
        alignas(8) u32 kind;
        u32 kind_value = g_unk_71727bebb8;
        key = g_unk_7172736a18.data->Key();
        kind = kind_value;
        UnkKraidEntity_l919254* kraid =
            reinterpret_cast<UnkKraidEntity_l919254*>(fn_7100debbc8(manager, &key, &kind));
        if (kraid) {
            UnkKraidComponent_l919254* ai = kraid->component;
            if (ai && fn_710009d50c(ai->Type(), fn_71005fac74()))
                fn_71005fe634(reinterpret_cast<UnkKraidAI_b57a4*>(ai));
        }
    } else {
        self->machine.Request(2);
    }
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->listener);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    return true;
}
