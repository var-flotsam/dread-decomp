// CTriggerComponent unit (tools/tu_map.py): the unit's 0x60-byte record (constructor fn_7100a6317c in
// CTriggerComponent_b47.cpp, move constructor fn_7100a63224): its destructor fn_7100a632e8 frees the
// array of owned objects at +0x30 and releases the two string ids; fn_7100a6340c is its memberwise
// move-assignment (the ids released and taken over, the plain fields copied, the array taken over
// after its elements are destroyed). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using base::global::CStrId;

// A string id whose move-assignment releases the old id like a destructor, then takes the other's.
struct UnkTriggerId_ovb0067 {
    CStrId id;

    UnkTriggerId_ovb0067& operator=(UnkTriggerId_ovb0067&& other) {
        id.ReleaseAndTake(other.id);
        return *this;
    }
};

// The record (0x60 bytes; constructor fn_7100a6317c, move constructor fn_7100a63224).
struct UnkTriggerOwned_b56 {
    UnkTriggerId_ovb0067 field_0x0;
    s32 field_0x8;
    UnkTriggerId_ovb0067 field_0x10;
    bool field_0x18;
    bool field_0x19;
    f32 field_0x1c;
    f32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    base::UnkVector<UnkOwnedPtr> entries;  // 0x30
    bool field_0x58;

    // The memberwise move-assignment, out of line (fn_7100a6340c).
    UnkTriggerOwned_b56& operator=(UnkTriggerOwned_b56&& other) asm("fn_7100a6340c");
};
static_assert(__builtin_offsetof(UnkTriggerOwned_b56, entries) == 0x30, "UnkTriggerOwned_b56 layout");

void fn_7100a632e8(UnkTriggerOwned_b56* self) {
    self->entries.Free();
    self->field_0x10.id.Reset();
    self->field_0x0.id.Reset();
}

UnkTriggerOwned_b56& UnkTriggerOwned_b56::operator=(UnkTriggerOwned_b56&& other) = default;
