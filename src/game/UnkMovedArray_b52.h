#pragma once

// The move helper (CType+0x38) of the iterator value over a reflected array of arrays, as
// base::refl_IterValueMove: the (index, array) pair a value owns or refers to is moved in, the
// source array left empty. Shared by the units of CCooldownXBossLavaCarpetDef,
// CCooldownXBossFireBallDef, CAcidBlobsLaunchPattern, CBouncingCreaturesLaunchPattern and
// CKraidSpinningNailsDef. Names are placeholders.
#include "base/UnkCollectionIter.h"
#include "types.h"

// An array with base::UnkVector's layout whose move takes the buffer and empties the source.
struct UnkMovedArray_b52 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    void* allocator;

    UnkMovedArray_b52(UnkMovedArray_b52&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
};
