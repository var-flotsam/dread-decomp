// CGunComponent's unit (tools/tu_map.py unit 1304): passes one of two hashed ids (by bit 0 of the
// flag) to the owner's (+0x30) fn_7100d7c828 with false. Names are placeholders.
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkGunComponent_b45a2 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

void fn_710083f1f8(UnkGunComponent_b45a2* self, bool on) {
    u64 key = on ? 0x7627e8df694eaaf2 : 0x5a54d77ae2197a78;
    fn_7100d7c828(self->owner, &key, false);
}
