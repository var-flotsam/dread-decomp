#pragma once

// The aim component (unit 437 of tools/tu_map.py; CAimComponent by its reflection hint). For now its
// target record, the one layout every source of the unit uses. Names and layouts are placeholders.
#include "game/CAbilityShinespark.h"
#include "game/UnkAssignedSlot.h"
#include "types.h"

// A candidate target: two counted slots and 13 words (its position at words 1-3). 0x44 bytes are copied
// both ways, by the copy constructor (fn_7100331c7c) and by the assignment; the last word is never
// written. Packed to 4 because the target list keeps them from +0xc (fn_7100354d04, a stride of 0x48).
#pragma pack(push, 4)
struct UnkAimTarget {
    UnkAssignedSlot slot;
    UnkAssignedSlot slot_0x8;
    u32 words[13];  // 0x10
    u32 unk_0x44;

    UnkAimTarget() {}
    UnkAimTarget(const UnkAimTarget& other) { fn_7100331c7c(this, &other); }

    f32 X() const { return *reinterpret_cast<const f32*>(&words[1]); }
    f32 Y() const { return *reinterpret_cast<const f32*>(&words[2]); }
    f32 Z() const { return *reinterpret_cast<const f32*>(&words[3]); }

    UnkAimTarget& operator=(const UnkAimTarget& other) {
        if (this != &other) {
            slot = other.slot;
            slot_0x8 = other.slot_0x8;
            for (u32 i = 0; i < 13; ++i)
                words[i] = other.words[i];
        }
        return *this;
    }
};
#pragma pack(pop)
static_assert(sizeof(UnkAimTarget) == 0x48, "UnkAimTarget layout");
