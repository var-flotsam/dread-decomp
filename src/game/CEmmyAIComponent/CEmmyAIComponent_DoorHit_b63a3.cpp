// The CEmmyAIComponent unit: fn_710055b61c. For an event whose two indices (+0x2, +0x1e) are set
// and whose id (+0x38) is empty, it follows the event's +0xf8 object to a door lock (fn_71002af860)
// and hands that to the object at +0x2a268 (fn_7100705bfc). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkWeakTarget;
struct UnkDoorLock;
UnkDoorLock* fn_71002af860(UnkWeakTarget*);
void fn_7100705bfc(void* self, UnkDoorLock* lock);

struct UnkEmmyEventHolder_b63a3 {
    u8 pad_0x0[0x10];
    UnkWeakTarget* target;
};

struct UnkEmmyEvent_b63a3 {
    u8 pad_0x0[0x2];
    s16 field_0x2;
    u8 pad_0x4[0x1e - 0x4];
    s16 field_0x1e;
    u8 pad_0x20[0x38 - 0x20];
    base::global::UnkStringData* field_0x38;
    u8 pad_0x40[0xf8 - 0x40];
    UnkEmmyEventHolder_b63a3* field_0xf8;
};

struct UnkEmmyAI_DoorHit_b63a3 {
    u8 pad_0x0[0x2a268];
    u8 field_0x2a268[8];
};

bool fn_710055b61c(UnkEmmyAI_DoorHit_b63a3* self, UnkEmmyEvent_b63a3* event) {
    if (event->field_0x2 == -1 || event->field_0x1e == -1)
        return false;
    if (event->field_0x38 != &g_unk_7101d47108->empty)
        return false;
    UnkEmmyEventHolder_b63a3* holder = event->field_0xf8;
    if (!holder)
        return false;
    UnkWeakTarget* target = holder->target;
    if (!target)
        return false;
    UnkDoorLock* lock = fn_71002af860(target);
    if (!lock)
        return false;
    fn_7100705bfc(self->field_0x2a268, lock);
    return true;
}
