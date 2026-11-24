// CSwarmControllerComponent's unit: erases a slot from the swarm's slot map (game/UnkSwarmSlot.h) by
// moving the last one into its place, with its key; returns the slot.
// Names and layouts are placeholders.
#include "game/UnkSwarmSlot.h"
#include "types.h"

UnkSwarmSlot* CSwarmControllerComponent_EraseSlotBySwappingLast(UnkSwarmSlots* self, UnkSwarmSlot* slot) {
    UnkSwarmSlot* first = self->values;
    u32 index = (u32)(((char*)slot - (char*)first) >> 6);
    first[index].Destroy();
    u32 last = self->count - 1;
    if (last != index) {
        self->values[index].MoveFrom(self->values[last]);
        self->values[self->count - 1].Destroy();
        self->keys[index] = self->keys[self->count - 1];
    }
    self->count = self->count - 1;
    return &self->values[index];
}
