// CTriggerComponent unit (tools/tu_map.py): the copy-assignment of a record holding a weak reference
// (game_s2/UnkWeakHandle.h) and three words. Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// A weak reference and three words after it.
struct UnkTriggerWeakRecord_ovb0067 {
    UnkWeakHandle* handle;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;

    // The copy-assignment, out of line (fn_7100a64a58): the handle of the other's target is taken
    // (created on first use) unless both are the same record.
    UnkTriggerWeakRecord_ovb0067& operator=(const UnkTriggerWeakRecord_ovb0067& other) asm("fn_7100a64a58");
};

UnkTriggerWeakRecord_ovb0067& UnkTriggerWeakRecord_ovb0067::operator=(const UnkTriggerWeakRecord_ovb0067& other) {
    if (this != &other)
        CopyWeakRef(&handle, other.handle);
    field_0x8 = other.field_0x8;
    field_0xc = other.field_0xc;
    field_0x10 = other.field_0x10;
    return *this;
}
