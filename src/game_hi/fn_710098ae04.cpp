// Handles a movement-state params message: forwards it to the matching state, records the requested
// state id and, unless state changes are locked, switches the active state to it.
#include "types.h"

void* fn_71009b0304();
bool fn_710009d50c(void* cls, void* base);

struct UnkEvent {
    virtual void* vf0();
    virtual void vf8();
    virtual void vf10();
    virtual u32 vf18();
};

struct UnkState {
    virtual void vf0();
    virtual void vf8();
    virtual void vf10(u32 prevId);
    virtual void vf18(u32 nextId);
    virtual void vf20();
    virtual void vf28();
    virtual void vf30(UnkEvent* ev);
    void* field_0x8;
    u32 field_0x10;
};

struct UnkStateList {
    u32 count;
    u8 storage[16 * sizeof(void*)];
    UnkState** begin() { return reinterpret_cast<UnkState**>(storage); }
    UnkState** end() { return begin() + count; }
};

struct UnkStateMachine {
    virtual void vf0();
    virtual void vf8();
    virtual void vf10();
    virtual void vf18();
    virtual void vf20(UnkState* prev, UnkState* next);
    virtual void vf28(UnkState* prev, UnkState* next);
    void* field_0x8;
    UnkState* field_0x10;
    UnkState* field_0x18;
    UnkStateList field_0x20;
    u32 field_0xa4;
    bool field_0xa8;

    UnkState* Find(u32 id) {
        UnkState** it = field_0x20.begin();
        for (; it != field_0x20.end(); ++it) {
            if ((*it)->field_0x10 == id)
                break;
        }
        return it != field_0x20.end() ? *it : nullptr;
    }

    void ApplyPending() {
        if (field_0xa4 < 15) {
            ChangeState(field_0xa4);
            field_0xa4 = 15;
        }
    }

    void ChangeState(u32 id) {
        if (field_0x10 && field_0x10->field_0x10 == id)
            return;
        UnkState* next = Find(id);
        if (!next && id != 0)
            return;
        if (field_0x10)
            field_0x10->vf18(id);
        vf28(field_0x10, next);
        UnkState* prev = field_0x10;
        field_0x10 = next;
        field_0x18 = prev;
        if (!next)
            return;
        next->vf10(prev ? prev->field_0x10 : 15);
        vf20(field_0x18, next);
    }
};

// NON_MATCHING(minor): the original keeps the looked-up next state, a copy of it and a "has next" flag in
// separate registers through the state switch; same calls and stores in the same order.
void fn_710098ae04(UnkStateMachine* self, UnkEvent* ev) {
    if (!fn_710009d50c(ev->vf0(), fn_71009b0304()))
        return;
    u32 id = ev->vf18();
    if (id <= 14) {
        if (UnkState* s = self->Find(id))
            s->vf30(ev);
    }
    self->field_0xa4 = id;
    if (!self->field_0xa8)
        self->ApplyPending();
}
