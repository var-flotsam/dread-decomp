// behaviortree::CGroundShockerFollowPathAction's unit (tools/tu_map.py unit 857): through the owner
// (+0x18), its +0x298 object and that one's +0xb0 slot, the state word (+0x10) of the current
// entry's record; 2 is passed through, anything else (or a missing link) gives 6. Names are
// placeholders.
#include "types.h"

struct UnkRecord857 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkEntry857 {
    u8 pad_0x0[0x10];
    UnkRecord857* record;  // 0x10
};

struct UnkSlot857 {
    UnkEntry857* entry;  // 0x0
};

struct UnkInner857 {
    u8 pad_0x0[0xb0];
    UnkSlot857* slot;  // 0xb0
};

struct UnkOwner857 {
    u8 pad_0x0[0x298];
    UnkInner857* inner;  // 0x298
};

struct UnkAction857 {
    u8 pad_0x0[0x18];
    UnkOwner857* owner;  // 0x18
};

s32 fn_71005a206c(const UnkAction857* self) {
    UnkSlot857* slot = self->owner->inner->slot;
    if (slot != nullptr && slot->entry != nullptr) {
        s32 state = slot->entry->record->state;
        return state == 2 ? state : 6;
    }
    s32 none = 6;
    asm("" : "+r"(none));
    return none;
}
