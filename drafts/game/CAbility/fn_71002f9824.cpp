// CAbility's unit (tools/tu_map.py unit 427): moves the ability's state machine between states 3, 4 and 6
// by whether virtual slot 0x190 holds, then calls slot 0xa0 with true. Names and layouts are placeholders.
#include "types.h"

struct UnkAbilityState_l2f9824 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Enter(s32 previous);  // 0x10
    virtual void Leave(s32 next);      // 0x18
    u64 field_0x8;
    u32 id;
};

struct UnkAbilityMachine_l2f9824 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void Entered(UnkAbilityState_l2f9824* previous, UnkAbilityState_l2f9824* next);  // 0x20
    virtual void Left(UnkAbilityState_l2f9824* current, UnkAbilityState_l2f9824* next);      // 0x28

    u64 field_0x8;
    UnkAbilityState_l2f9824* current;
    UnkAbilityState_l2f9824* previous;
    u32 count;
    u32 states[0x20];  // UnkAbilityState_l2f9824* entries, 4-byte aligned
    s32 pending;       // 7: none
    bool busy;

    UnkAbilityState_l2f9824** States() { return reinterpret_cast<UnkAbilityState_l2f9824**>(states); }

    // The pending state, or the current one's when none is pending.
    s32 State() const {
        s32 s = pending;
        if (s == 7 && current)
            s = current->id;
        return s;
    }

    UnkAbilityState_l2f9824* Find(u32 id) {
        UnkAbilityState_l2f9824** begin = States();
        UnkAbilityState_l2f9824** end = begin + count;
        UnkAbilityState_l2f9824** p = begin;
        while (p != end && (*p)->id != id)
            ++p;
        return p != end ? *p : nullptr;
    }

    // Asks for state `id` and, unless busy, switches to it now.
    void Request(s32 id) {
        pending = id;
        if (busy)
            return;
        if (!current || current->id != (u32)id) {
            if (UnkAbilityState_l2f9824* next = Find(id)) {
                if (current) {
                    current->Leave(id);
                    Left(current, next);
                }
                previous = current;
                current = next;
                next->Enter(previous ? previous->id : 7);
                Entered(previous, next);
            }
        }
        pending = 7;
    }
};
static_assert(sizeof(UnkAbilityMachine_l2f9824) == 0xb0, "layout");

struct UnkAbility_l2f9824 {
    void** vtable;
    u64 field_0x8;
    UnkAbilityMachine_l2f9824 machine;  // 0x10

    template <class R = void, class... A>
    R Virtual(u32 offset, A... args) {
        return reinterpret_cast<R (*)(UnkAbility_l2f9824*, A...)>(vtable[offset / 8])(this, args...);
    }
};

void fn_71002f9824(UnkAbility_l2f9824* self) {
    bool held = self->Virtual<bool>(0x190);
    if (held && self->machine.State() == 3) {
        self->machine.Request(4);
        return;
    }
    if (held)
        return;
    switch (self->machine.State()) {
    case 0:
    case 1:
    case 2:
    case 3:
        return;
    case 6:
        if (!self->Virtual<bool>(0xb0))
            return;
        break;
    default:
        self->machine.Request(3);
        break;
    }
    self->Virtual(0xa0, true);
}
