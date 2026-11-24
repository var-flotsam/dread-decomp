#pragma once

// A slot of CSwarmControllerComponent's map at +0x278 (0x40 bytes, polymorphic): a buffer of its own
// and the words around it. The map keeps the slots and their keys in two parallel arrays
// (CRntSmallDictionary); fn_710074a568 erases one. Names and the layout are placeholders.

#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "game/CSwarmControllerComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkSwarmSlot {
    char* vtable;  // 0x0
    u32 field_0x8;
    u32 pad_0xc;
    void* field_0x10;
    void* buffer;  // 0x18
    u32 size;      // 0x20
    u32 capacity;  // 0x24
    u32 field_0x28;
    u8 field_0x2c;
    u8 pad_0x2d[3];
    u32 field_0x30;
    u8 pad_0x34[4];
    base::IAllocator* allocator;  // 0x38

    // The destructor: releases the buffer.
    void Destroy() {
        if (buffer) {
            size = 0;
            allocator->vf_0x48(buffer);
            capacity = 0;
        }
        vtable = g_unk_7101a20a08 + 0x10;
    }

    // The move constructor, into the slot at this address.
    void MoveFrom(UnkSwarmSlot& other) {
        field_0x8 = 0;
        vtable = g_unk_7101a6dee0 + 0x10;
        field_0x10 = other.field_0x10;
        buffer = other.buffer;
        size = other.size;
        capacity = other.capacity;
        field_0x28 = other.field_0x28;
        field_0x2c = other.field_0x2c;
        field_0x30 = other.field_0x30;
        allocator = other.allocator;
        other.buffer = nullptr;
        other.size = 0;
        other.capacity = 0;
    }
};
static_assert(sizeof(UnkSwarmSlot) == 0x40, "UnkSwarmSlot layout");

using UnkSwarmSlots = base::global::CRntSmallDictionary<void*, UnkSwarmSlot>;

// Erases `slot` by moving the last slot (and its key) into its place; returns the same position.
UnkSwarmSlot* fn_710074a568(UnkSwarmSlots* self, UnkSwarmSlot* slot);
