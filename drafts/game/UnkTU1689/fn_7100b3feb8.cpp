// GUI::CMapMenuController's unit (1689): enters a mode. With bit 0 of the flags set the mode is stored
// (+0x6f0) and the controller moves to entry 4; otherwise it moves to the mode itself. Either way the state
// machine at +0x918 is asked for that state. Names and layouts are placeholders.
#include "../../../src/game/UnkSmRequest_A5_b73.h"
#include "game/UnkTU1689.h"
#include "types.h"

// As src/game/GUI/CMapMenuController_Timers_b43.cpp declares it.
struct UnkMapTimersController_b43a3;

struct UnkMapModeController_lb3feb8 {
    u8 pad_0x0[0x6f0];
    s32 mode;  // 0x6f0
    u8 pad_0x6f4[0x918 - 0x6f4];
    UnkSmMachine_A5_b73 machine;  // 0x918
};
static_assert(__builtin_offsetof(UnkMapModeController_lb3feb8, machine) == 0x918, "layout");

static const u32 kNoState = 0x7fffffff;
static const u32 kLastState = 4;

static inline UnkSmState_A5_b73* FindState_lb3feb8(UnkSmMachine_A5_b73* machine, u32 id) {
    for (UnkSmStatePtr_A5_b73 *p = machine->states, *end = p + machine->count; p != end; ++p) {
        if ((*p)->id == id)
            return *p;
    }
    return nullptr;
}

static inline void RequestState_lb3feb8(UnkSmMachine_A5_b73* machine, u32 id) {
    machine->pending = id;
    if (id > kLastState || machine->busy)
        return;
    UnkSmState_A5_b73* current = machine->current;
    if (!current || current->id != id) {
        UnkSmState_A5_b73* next = FindState_lb3feb8(machine, id);
        if (next || id == 0) {
            if (current) {
                current->vf_0x18(id);
                machine->vf_0x28(machine->current, next);
            }
            machine->previous = machine->current;
            machine->current = next;
            if (next) {
                next->vf_0x10(machine->previous ? machine->previous->id : kNoState);
                machine->vf_0x20(machine->previous, next);
            }
        }
    }
    machine->pending = kNoState;
}

// Moves the controller to an entry and its state.
static inline void ShowEntry_lb3feb8(UnkMapModeController_lb3feb8* self, s32 entry) {
    fn_7100b3e5ac(reinterpret_cast<UnkMapMenuControllerClose*>(self), entry, 0);
    RequestState_lb3feb8(&self->machine, entry);
}

// Moves the controller to the map (entry 4).
static inline void ShowMap_lb3feb8(UnkMapModeController_lb3feb8* self) {
    ShowEntry_lb3feb8(self, 4);
}

void fn_7100b3feb8(UnkMapTimersController_b43a3* controller, s32 mode, u32 flags) {
    UnkMapModeController_lb3feb8* self = reinterpret_cast<UnkMapModeController_lb3feb8*>(controller);
    if (flags & 1) {
        self->mode = mode;
        ShowMap_lb3feb8(self);
        return;
    }
    ShowEntry_lb3feb8(self, mode);
}
