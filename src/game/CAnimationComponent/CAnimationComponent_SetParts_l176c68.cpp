// CAnimationComponent unit: when the owner's flag at +0x1b1 is set, hands every track of the array at
// +0x40 (data at +0xc8, count at +0xd8, both re-read every step) the motion-part flags built from the
// low two bits of `*mode` (fn_7100714eb4, on the track's +0x38). Names and layouts are placeholders.
#include "game/UnkMotionRule.h"
#include "game/UnkTU1141.h"
#include "types.h"

struct UnkAnimTrack_l176c68 {
    u8 pad_0x0[0x38];
    u8 part[0x8];  // 0x38
};

struct UnkAnimTrackHolder_l176c68 {
    u8 pad_0x0[0x70];
    UnkAnimTrack_l176c68* track;  // 0x70
};

struct UnkAnimArray_l176c68 {
    u8 pad_0x0[0xc8];
    UnkAnimTrackHolder_l176c68** data;  // 0xc8
    u8 pad_0xd0[8];
    u32 count;  // 0xd8
};

struct UnkAnimOwner_l176c68 {
    u8 pad_0x0[0x1b1];
    bool field_0x1b1;
};

struct UnkAnimationComponent_l176c68 {
    u8 pad_0x0[0x30];
    UnkAnimOwner_l176c68* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkAnimArray_l176c68* array;  // 0x40
};

void fn_7101176c68(UnkAnimationComponent_l176c68* self, void* unused, const u32* mode) {
    if (!self->owner->field_0x1b1)
        return;
    UnkFlags_71007142e8 flags;
    // Which parts: the low two bits of the mode.
    switch (*mode & 3) {
    case 0:
        flags.bits = 0;
        break;
    case 1:
        flags.bits = 2;
        break;
    case 2:
        flags.bits = 4;
        break;
    case 3:
        flags.bits = 6;
        break;
    }
    UnkAnimArray_l176c68* array = self->array;
    if (array && array->count != 0) {
        UnkAnimTrackHolder_l176c68** it = array->data;
        do {
            fn_7100714eb4((*it)->track->part, &flags.bits);
            ++it;
            array = self->array;
        } while (it != array->data + array->count);
    }
}
