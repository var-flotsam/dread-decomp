// Unit 1452 (tools/tu_map.py): tells the grapple point (fn_710097f1e0, when it is a CPullableGrapplePointComponent)
// the owner (fn_710097f1b8) and the flag at +0xe0, then requests state 1 of the state machine at +0x30 and applies
// it (leave the current state, enter the new one, "none" is 4). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU1450.h"
#include "types.h"

using base::reflection::CType;

struct UnkUnit1452_b56;
struct UnkUnit1452Owner_b56;

struct UnkGrapplePoint_l9806bc {
    virtual const CType* vf_0x0();
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
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0();
    virtual void vf_0x1c8(UnkUnit1452Owner_b56* owner, bool flag);
};

struct UnkSmState_l9806bc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

// The state pointers sit at a 4-byte aligned offset.
typedef UnkSmState_l9806bc* __attribute__((aligned(4))) UnkSmStatePtr_l9806bc;

struct UnkSmMachine_l9806bc {
    static const u32 kLast = 3;
    static const u32 kNone = 4;

    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_l9806bc* previous, UnkSmState_l9806bc* next);
    virtual void vf_0x28(UnkSmState_l9806bc* current, UnkSmState_l9806bc* next);

    u64 field_0x8;
    UnkSmState_l9806bc* current;
    UnkSmState_l9806bc* previous;
    u32 count;
    UnkSmStatePtr_l9806bc states[16];
    u32 pending;
    bool busy;

    UnkSmState_l9806bc* Find(u32 id) {
        UnkSmStatePtr_l9806bc* p = states;
        for (u64 n = count * sizeof(UnkSmStatePtr_l9806bc); n != 0; n -= sizeof(UnkSmStatePtr_l9806bc), ++p) {
            if ((*p)->id == id)
                return *p;
        }
        return nullptr;
    }

    void Switch(u32 id) {
        if (!current || current->id != id) {
            UnkSmState_l9806bc* next = Find(id);
            if (next || id == 0) {
                if (current) {
                    current->vf_0x18(id);
                    vf_0x28(current, next);
                }
                previous = current;
                current = next;
                if (next) {
                    next->vf_0x10(previous ? previous->id : kNone);
                    vf_0x20(previous, next);
                }
            }
        }
        pending = kNone;
    }

    void Request(u32 id) {
        pending = id;
        if (id > kLast || busy)
            return;
        Switch(id);
    }

    void ApplyPending() {
        u32 id = pending;
        if (id > kLast)
            return;
        Switch(id);
    }
};

struct UnkGrappleStep_l9806bc {
    u8 pad_0x0[0x30];
    UnkSmMachine_l9806bc machine;
    bool field_0xe0;
};

void fn_71009806bc(UnkGrappleStep_l9806bc* self) {
    UnkUnit1452_b56* base = reinterpret_cast<UnkUnit1452_b56*>(self);
    if (UnkGrapplePoint_l9806bc* obj = reinterpret_cast<UnkGrapplePoint_l9806bc*>(fn_710097f1e0(base))) {
        UnkGrapplePoint_l9806bc* grapple = fn_710009d50c(obj->vf_0x0(), fn_710082c6ac()) ? obj : nullptr;
        grapple->vf_0x1c8(fn_710097f1b8(base), self->field_0xe0);
    }
    self->machine.Request(1);
    self->machine.ApplyPending();
}
