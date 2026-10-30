#pragma once

// The lookup of a map kept as two parallel arrays (values, then the ids that name them, then a
// count), inlined into a method of the object that embeds the map: the value `key` names, or null.
// The blackboard's sections are one such map (game/CBlackboard.h); the other users declare a
// layout with members `values`, `ids` and `count` at their offsets. String ids are compared as raw
// 64-bit words; a slot is returned and compared with the end, as base/UnkCloneCalls.h's keyed
// arrays do (written otherwise the compiler folds the comparison away).

#include "base/global/CStrId.h"
#include "types.h"

template <class Self>
__attribute__((always_inline)) inline auto UnkParallelFindSlot(Self* self, const base::global::CStrId& key)
    -> decltype(&self->values[0]) {
    for (u64 i = 0; i < self->count; ++i) {
        if (*reinterpret_cast<const u64*>(&self->ids[i]) == *reinterpret_cast<const u64*>(&key))
            return &self->values[i];
    }
    return &self->values[self->count];
}

// The same over ids that are plain words compared by value.
template <class Self, class K>
__attribute__((always_inline)) inline auto UnkParallelFindSlotByValue(Self* self, K key)
    -> decltype(&self->values[0]) {
    for (u64 i = 0; i < self->count; ++i) {
        if (self->ids[i] == key)
            return &self->values[i];
    }
    return &self->values[self->count];
}

template <class Self>
__attribute__((always_inline)) inline auto UnkParallelFind(Self* self, const base::global::CStrId& key)
    -> decltype(+self->values[0]) {
    auto* slot = UnkParallelFindSlot(self, key);
    return slot != &self->values[self->count] ? *slot : nullptr;
}

template <class Self, class K>
__attribute__((always_inline)) inline auto UnkParallelFindByValue(Self* self, K key) -> decltype(+self->values[0]) {
    auto* slot = UnkParallelFindSlotByValue(self, key);
    return slot != &self->values[self->count] ? *slot : nullptr;
}
