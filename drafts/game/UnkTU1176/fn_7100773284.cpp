// Unit 1176 (hint 'CCentralUnitComponent'): when the event's key matches the unit's (+0x538) and the unit's
// state machine (+0x30) is in state 3, requests state 2 ("none" 8). Names and layouts are placeholders.
#include "../../../src/game/UnkSmRequest_A5_b73.h"
#include "types.h"

struct UnkCentralUnit;

struct UnkCentralUnitBody_l773284 {
    u8 pad_0x0[0x30];
    UnkSmMachine_A5_b73 machine;
    u8 pad_0xe0[0x538 - 0xe0];
    u64 key;
};
static_assert(__builtin_offsetof(UnkCentralUnitBody_l773284, key) == 0x538, "layout");

// Moves the machine from state 3 to state 2.
static inline void Advance(UnkSmMachine_A5_b73* machine) {
    UnkSmState_A5_b73* current = machine->current;
    if (current == nullptr || current->id != 3)
        return;
    RequestState_A5_b73<15, 8>(machine, 2);
}

void fn_7100773284(UnkCentralUnit* unit, void* a, void* b, void* c, void* d, bool e) {
    auto* self = reinterpret_cast<UnkCentralUnitBody_l773284*>(unit);
    if (*static_cast<u64*>(a) != self->key)
        return;
    Advance(&self->machine);
}
