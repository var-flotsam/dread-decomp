// Component updates that run the base update (fn_71010c424c) and then their small state machine (the
// update of src/game/UnkStateMachineSet_b61.cpp, fn_7100a0a584, whose template is copied here):
// applies the pending state, then updates the current state and applies what it asked for, at most ten
// times, while marked busy. Each one's own unit. Names are placeholders.
#include "types.h"

struct UnkSmState_A5_b62 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // updated
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_A5_b62* __attribute__((aligned(4))) UnkSmStatePtr_A5_b62;

struct UnkSmMachine_A5_b62 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_A5_b62* previous, UnkSmState_A5_b62* next);
    virtual void vf_0x28(UnkSmState_A5_b62* current, UnkSmState_A5_b62* next);

    u64 field_0x8;
    UnkSmState_A5_b62* current;
    UnkSmState_A5_b62* previous;
    u32 count;
    UnkSmStatePtr_A5_b62 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_A5_b62 {
    u8 pad_0x0[Offset];
    UnkSmMachine_A5_b62 machine;
};

__attribute__((always_inline)) static inline bool FindState_A5_b62(UnkSmMachine_A5_b62* self, u32 id,
                                                               UnkSmState_A5_b62*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_A5_b62);
    asm("" : "+r"(n));
    for (UnkSmStatePtr_A5_b62* p = self->states; n != 0; ++p, n -= sizeof(UnkSmStatePtr_A5_b62)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

// Switches to state `id` (the b59 "apply pending" body) and clears the pending id.
template <u32 None>
__attribute__((always_inline)) static inline void SwitchState_A5_b62(UnkSmMachine_A5_b62* self, u32 id,
                                                                 UnkSmState_A5_b62* current) {
    if (!current || current->id != id) {
        UnkSmState_A5_b62* found;
        UnkSmState_A5_b62* next;
        u32 enter;
        if (FindState_A5_b62(self, id, found)) {
            next = found;
            enter = true;
        } else {
            if (id != 0)
                goto done;
            enter = false;
            asm("" : "+r"(enter));
            next = nullptr;
            asm("" : "+r"(next));
        }
        if (current) {
            current->vf_0x18(id);
            self->vf_0x28(self->current, next);
        }
        self->previous = self->current;
        self->current = found;
        if (enter) {
            typedef void (*EnteredFn)(UnkSmState_A5_b62*, s32);
            s32 previous_id = self->previous ? self->previous->id : None;
            EnteredFn entered = (*reinterpret_cast<EnteredFn**>(found))[2];
            entered(next, previous_id);
            self->vf_0x20(self->previous, next);
        }
    }
done:
    self->pending = None;
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void Apply_A5_b62(UnkSmMachine_A5_b62* self) {
    u32 id = self->pending;
    if (id > Last)
        return;
    SwitchState_A5_b62<None>(self, id, self->current);
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void Update_A5_b62(UnkSmMachine_A5_b62* self, f32 dt) {
    self->busy = true;
    if (self->pending != None)
        Apply_A5_b62<Last, None>(self);
    u32 i = 0;
    while (UnkSmState_A5_b62* current = self->current) {
        current->vf_0x20(dt);
        if (self->pending == None)
            break;
        Apply_A5_b62<Last, None>(self);
        if (i++ > 8)
            break;
    }
    self->busy = false;
}

struct UnkParent;
void fn_71010c424c(UnkParent*);

#define UNK_SM_UPDATE_AFTER_A5_B62(addr, offset, last, none)     \
    void fn_##addr(UnkSmOwner_A5_b62<offset>* self, f32 dt) {    \
        fn_71010c424c(reinterpret_cast<UnkParent*>(self));       \
        Update_A5_b62<last, none>(&self->machine, dt);           \
    }

UNK_SM_UPDATE_AFTER_A5_B62(71007b2994, 0x40, 3, 4)
UNK_SM_UPDATE_AFTER_A5_B62(71007b82f4, 0x58, 2, 0x7fffffff)
UNK_SM_UPDATE_AFTER_A5_B62(71007dea54, 0xa0, 3, 0x7fffffff)
UNK_SM_UPDATE_AFTER_A5_B62(71008a16e4, 0x40, 4, 5)
UNK_SM_UPDATE_AFTER_A5_B62(71009185f0, 0x40, 4, 0x7fffffff)
UNK_SM_UPDATE_AFTER_A5_B62(7100886c10, 0x208, 4, 5)

// The same update with a pointer argument for the states (two more units).
template <u32 Last, u32 None, class Arg>
__attribute__((always_inline)) static inline void UpdateWith_A5_b62(UnkSmMachine_A5_b62* self, Arg arg) {
    typedef void (*UpdatedFn)(UnkSmState_A5_b62*, Arg);
    self->busy = true;
    if (self->pending != None)
        Apply_A5_b62<Last, None>(self);
    u32 i = 0;
    while (UnkSmState_A5_b62* current = self->current) {
        (*reinterpret_cast<UpdatedFn**>(current))[4](current, arg);
        if (self->pending == None)
            break;
        Apply_A5_b62<Last, None>(self);
        if (i++ > 8)
            break;
    }
    self->busy = false;
}

#define UNK_SM_UPDATE_WITH_A5_B62(addr, offset, last, none)             \
    void fn_##addr(UnkSmOwner_A5_b62<offset>* self, void* arg) {        \
        UpdateWith_A5_b62<last, none, void*>(&self->machine, arg);      \
    }

UNK_SM_UPDATE_WITH_A5_B62(7100980880, 0x30, 3, 4)
UNK_SM_UPDATE_WITH_A5_B62(710097cd18, 0x30, 4, 5)

// The same after another base update.
char* fn_71010c4234(char*);

#define UNK_SM_UPDATE_AFTER2_A5_B62(addr, offset, last, none)    \
    void fn_##addr(UnkSmOwner_A5_b62<offset>* self, f32 dt) {    \
        fn_71010c4234(reinterpret_cast<char*>(self));            \
        Update_A5_b62<last, none>(&self->machine, dt);           \
    }

UNK_SM_UPDATE_AFTER2_A5_B62(710091f798, 0x280, 5, 6)

// CFrozenComponent's unit: then the unit's own update.
void fn_71007f9d7c(UnkSmOwner_A5_b62<0x228>*, f32);

void fn_71007f9910(UnkSmOwner_A5_b62<0x228>* self, f32 dt) {
    fn_71010c4234(reinterpret_cast<char*>(self));
    Update_A5_b62<4, 5>(&self->machine, dt);
    fn_71007f9d7c(self, dt);
}


// CCentralUnitComponent's unit: base update fn_710024fb24, then two more updates of the unit's own.
struct UnkCentralUnit_b46a2;
void fn_710024fb24(void*);
void fn_7100771ea4(UnkSmOwner_A5_b62<0x30>*, f32);
void fn_71007722f4(UnkCentralUnit_b46a2*, f32);

void fn_7100771a34(UnkSmOwner_A5_b62<0x30>* self, f32 dt) {
    fn_710024fb24(self);
    Update_A5_b62<7, 8>(&self->machine, dt);
    fn_7100771ea4(self, dt);
    fn_71007722f4(reinterpret_cast<UnkCentralUnit_b46a2*>(self), dt);
}
