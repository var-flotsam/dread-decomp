// Unit 42 of tools/tu_map.py (the timeline events' unit): grows a timeline's timed events (the owned
// events and their times in two arrays sharing a count) to hold at least `capacity`. Names and
// layouts are placeholders.
#include <cstring>

#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

struct UnkTimedEvents;

// The timed events with the allocation flags and the allocator that follow them.
using UnkTimedEventsStorage_l06981c = base::global::CRntSmallDictionary<f32, UnkOwnedPtr>;

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710006981c(UnkTimedEvents* timed, u32 capacity) {
    auto* self = reinterpret_cast<UnkTimedEventsStorage_l06981c*>(timed);
    if (self->capacity >= capacity)
        return;
    auto* events = static_cast<UnkOwnedPtr*>(self->allocator->vf_0x40((u64)capacity * sizeof(UnkOwnedPtr), 8, self->flags));
    f32* times = static_cast<f32*>(self->allocator->vf_0x40((u64)capacity * sizeof(f32), 8, self->flags));
    if (UnkOwnedPtr* src = self->values) {
        for (u64 i = 0; i < self->count; ++i, ++src) {
            UnkOwned* moved = src->ptr;
            src->ptr = nullptr;
            events[i].ptr = moved;
            src->~UnkOwnedPtr();
        }
        self->allocator->vf_0x48(self->values);
    }
    if (self->keys) {
        memcpy(times, self->keys, (u64)self->count * sizeof(f32));
        self->allocator->vf_0x48(self->keys);
    }
    self->values = events;
    self->keys = times;
    self->capacity = capacity;
}