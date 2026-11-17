// Unit 2594: starts the eight-state machine of a context. Stamps the start time, hands every state its
// inputs, registers the states with the machine (on its first start), lets each state initialise, then
// requests state 5. Names and layouts are placeholders.
#include "game/UnkTU0.h"
#include "types.h"

struct UnkGfxFrameSource {
    u8 unk_0x0[0x2c];
    f32 field_0x2c;
};

struct UnkTU2594Machine;

struct UnkTU2594State {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // updated
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
    virtual void vf_0x78(void* input);
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();  // initialise

    UnkTU2594Machine* owner;
    u32 id;
    u8 pad_0x14[0x68 - 0x14];
    void* field_0x68;
};
static_assert(__builtin_offsetof(UnkTU2594State, field_0x68) == 0x68, "layout");

typedef UnkTU2594State* __attribute__((aligned(4))) UnkTU2594StatePtr;

struct UnkTU2594Machine {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkTU2594State* previous, UnkTU2594State* next);
    virtual void vf_0x28(UnkTU2594State* current, UnkTU2594State* next);

    void* field_0x8;
    UnkTU2594State* current;
    UnkTU2594State* previous;
    u32 count;
    UnkTU2594StatePtr states[16];
    u32 pending;
    bool busy;

    UnkTU2594State* Find(u32 id) {
        UnkTU2594StatePtr* end = states + count;
        for (UnkTU2594StatePtr* p = states; p != end; ++p) {
            if ((*p)->id == id)
                return *p;
        }
        return nullptr;
    }

    void Add(UnkTU2594State* state) {
        UnkTU2594StatePtr* end = states + count;
        for (UnkTU2594StatePtr* p = states; p != end; ++p) {
            if ((*p)->id == state->id)
                return;
        }
        state->owner = this;
        states[count++] = state;
    }

    void Switch(u32 id) {
        UnkTU2594State* from = current;
        if (!from || from->id != id) {
            UnkTU2594State* next = Find(id);
            if (next) {
                if (from) {
                    from->vf_0x18(id);
                    vf_0x28(current, next);
                }
                previous = current;
                current = next;
                next->vf_0x10(previous ? previous->id : 9);
                vf_0x20(previous, next);
            }
        }
        pending = 9;
    }

    void Request(u32 id) {
        pending = id;
        if (!busy)
            Switch(id);
    }
};
static_assert(__builtin_offsetof(UnkTU2594Machine, pending) == 0xa4, "layout");

struct UnkTU2594Context {
    UnkTU2594Machine* machine;
    UnkTU2594State* states[8];
    f32 start_time;
    bool field_0x4c;
};

struct UnkTU2594Owner;

void fn_7100f12504(UnkTU2594Owner* self, UnkTU2594Context* context, void* user, void* input, void* data) {
    context->start_time = g_unk_7101d47328->field_0x2c;
    context->field_0x4c = false;
    for (u32 i = 0; i < 8; ++i) {
        context->states[i]->vf_0x78(input);
        context->states[i]->field_0x68 = data;
    }
    if (context->machine->count == 0) {
        for (u32 i = 0; i < 8; ++i)
            context->machine->Add(context->states[i]);
    }
    for (u32 i = 0; i < 8; ++i)
        context->states[i]->vf_0x90();
    context->machine->field_0x8 = user;
    context->machine->Request(5);
}
