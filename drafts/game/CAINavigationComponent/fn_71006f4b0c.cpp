// CAINavigationComponent: forwards an event to the active navigation state and switches state.
// Names and layouts are placeholders.
#include "types.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"

struct UnkNavEvent {
    u8 pad_0x00[0x34];
    u32 kind;
};

struct UnkNavState {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Enter(u32 prevId);
    virtual void Exit(u32 nextId);
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void HandleEvent(UnkNavEvent* ev, bool flag);
    u8 pad_0x08[8];
    u32 id;
};

struct __attribute__((packed)) UnkNavStatePtr {
    UnkNavState* state;
};

struct UnkNavMachine {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void OnEnter(UnkNavState* prev, UnkNavState* next);
    virtual void OnExit(UnkNavState* cur, UnkNavState* next);
    u8 pad_0x08[8];
    UnkNavState* current;
    UnkNavState* previous;
    u32 count;
    UnkNavStatePtr states[9];
    u8 pad_0x6c[0xa4 - 0x6c];
    u32 pendingId;
    u8 locked;
};

struct UnkNavMachineHolder {
    UnkNavMachine* machine;
};

struct UnkNavNotify;
struct UnkNavNotifyVtbl {
    void* pad[0x428 / 8];
    void (*Notify)(UnkNavNotify* self, u32 a, u32 b);
};
struct UnkNavNotify {
    UnkNavNotifyVtbl* vtbl;
};

struct UnkNavOwner {
    u8 pad_0x00[0x220];
    UnkAnimationComponent* animation;
    u8 pad_0x228[0x240 - 0x228];
    UnkNavNotify* notify;
};

struct UnkNavigationComponent {
    u8 pad_0x00[0x30];
    UnkNavOwner* owner;
    u8 pad_0x38[0x4c - 0x38];
    u8 flag_0x4c;
    u8 pad_0x4d[0xb0 - 0x4d];
    UnkNavMachineHolder* holder;
};

void fn_71006f4b0c(UnkNavigationComponent* self, UnkNavEvent* ev, bool flag) {
    if (ev->kind == 6) {
        UnkAnimationComponent* anim = self->owner->animation;
        if (anim && fn_710117b348(anim, fn_7100c2e0c8()))
            return;
    }
    // keeps the event kind loaded again after the ignore check, as the original does
    asm volatile("" ::: "memory");
    self->flag_0x4c = 1;
    u32 kind = ev->kind;
    switch (kind) {
    case 9:
    case 0: {
        UnkNavMachineHolder* h = self->holder;
        if (h && h->machine) {
            UnkNavState* cur = h->machine->current;
            if (cur)
                cur->HandleEvent(ev, flag);
        }
        return;
    }
    default:
        break;
    }
    if (UnkNavNotify* n = self->owner->notify) {
        u32 curId = 9;
        if (self->holder && self->holder->machine)
            curId = self->holder->machine->current->id;
        n->vtbl->Notify(n, curId, kind);
    }
    UnkNavMachineHolder* h = self->holder;
    if (!h)
        return;
    if (kind < 9 && h->machine) {
        UnkNavMachine* m = h->machine;
        for (UnkNavStatePtr* p = m->states, *end = p + m->count; p != end; p++) {
            UnkNavState* s = p->state;
            if (s->id == kind) {
                if (s)
                    s->HandleEvent(ev, flag);
                h = self->holder;
                break;
            }
        }
        if (!h)
            return;
    }
    UnkNavMachine* m = h->machine;
    if (!m)
        return;
    m->pendingId = kind;
    if (kind >= 9 || m->locked)
        return;
    UnkNavState* cur = m->current;
    if (!cur || cur->id != kind) {
        UnkNavState* next = nullptr;
        UnkNavStatePtr* p = m->states;
        UnkNavStatePtr* end = p + m->count;
        for (; p != end; p++) {
            if (p->state->id == kind)
                break;
        }
        if (p != end)
            next = p->state;
        bool found;
        UnkNavState* target;
        if (next) {
            found = true;
            target = next;
        } else if (kind == 0) {
            found = false;
            target = nullptr;
        } else {
            goto done;
        }
        {
            UnkNavState* prev = nullptr;
            if (cur) {
                cur->Exit(kind);
                m->OnExit(m->current, target);
                prev = m->current;
            }
            m->current = next;
            m->previous = prev;
            if (found) {
                next->Enter(prev ? prev->id : 9);
                m->OnEnter(m->previous, target);
            }
        }
    }
done:
    m->pendingId = 9;
}
