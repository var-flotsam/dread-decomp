// Unit 2635: requests a state change on an object that keeps a current/previous state
// object and notifies a listener. Names and layouts are placeholders.
#include "types.h"

struct UnkStateObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 prevId);
    virtual void vf_0x18();
    u8 pad_0x8[8];
    s32 id;
};

struct UnkStateListener {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkStateObj* prev, UnkStateObj* next);
    virtual void vf_0x28(UnkStateObj* cur, UnkStateObj* next);
};

#pragma pack(push, 1)
struct UnkStateOwner {
    u8 pad_0x0[0x30];
    UnkStateListener listener;
    u8 pad_0x38[8];
    UnkStateObj* current;
    UnkStateObj* previous;
    u32 count;
    UnkStateObj* states[1];
    u8 pad_0x5c[0xd4 - 0x5c];
    s32 requested;
    u8 locked;
    u8 pad_0xd9[0xb68 - 0xd9];
    u32 stateParam;
    f32 timeParam;
    u32 zero;
};
#pragma pack(pop)

void fn_7100fd0d40(UnkStateOwner* self, u32 state, f32 time) {
    self->zero = 0;
    self->timeParam = time;
    self->stateParam = state;
    if (!(time <= 0.0f))
        goto out;
    self->requested = state;
    if (state >= 8 || self->locked)
        goto out;
    {
        UnkStateObj* cur = self->current;
        if (cur && cur->id == (s32)state)
            goto done;
        UnkStateObj* next = nullptr;
        bool found = false;
        UnkStateObj* const* it = self->states;
        for (u32 n = self->count; n != 0; n--, it++) {
            if ((*it)->id == (s32)state) {
                next = *it;
                found = next != nullptr;
                break;
            }
        }
        if (!found && state != 0)
            goto done;
        UnkStateObj* old = nullptr;
        if (cur) {
            cur->vf_0x18();
            self->listener.vf_0x28(self->current, next);
            old = self->current;
        }
        self->current = next;
        self->previous = old;
        if (found) {
            s32 prevId = old ? old->id : 0x7fffffff;
            next->vf_0x10(prevId);
            self->listener.vf_0x20(self->previous, next);
        }
    }
done:
    self->requested = 0x7fffffff;
out:;
}
