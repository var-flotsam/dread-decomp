// CDoorGrapplePointComponent unit (tools/tu_map.py, the name from reflection): fn_71008295e0 accepts a
// pull when the base test (fn_710082cb24) passes, the owner's (+0x30) door (fn_71002af860) answers
// fn_71008d5e28, the pull comes from one of the grapple points the definition (+0x90) lists (+0x78,
// count +0x80; searched as std::find over UnkIter) and the pull's strength (+0x20) is at least 0.94 in
// magnitude. Names and layouts are placeholders.
#include <algorithm>
#include <math.h>

#include "../../game_s7/UnkIter.h"
#include "game/CPullableGrapplePointComponent.h"
#include "game/UnkTU367.h"
#include "types.h"

struct UnkWeakTarget;
struct UnkDoorLock;

bool fn_71008d5e28(UnkDoorLock* door);

struct UnkDoorPullSource_ovb0093 {
    u8 pad_0x0[0x20];
    void* field_0x20;
};

struct UnkDoorPullSender_ovb0093 {
    u8 pad_0x0[0x10];
    UnkDoorPullSource_ovb0093* field_0x10;
};

struct UnkDoorPullMessage_ovb0093 {
    UnkDoorPullSender_ovb0093* sender;
    u8 pad_0x8[0x18];
    f32 field_0x20;
};

struct UnkDoorGrappleDef_ovb0093 {
    u8 pad_0x0[0x78];
    void** points;  // 0x78
    u32 count;      // 0x80
};

struct UnkDoorGrapple_ovb0093 {
    u8 pad_0x0[0x30];
    UnkWeakTarget* owner;  // 0x30
    u8 pad_0x38[0x58];
    UnkDoorGrappleDef_ovb0093* def;  // 0x90
};

bool fn_71008295e0(UnkDoorGrapple_ovb0093* self, void* arg, UnkDoorPullMessage_ovb0093* msg) {
    if (!fn_710082cb24(self, arg, msg))
        return false;
    if (!fn_71008d5e28(fn_71002af860(self->owner)))
        return false;
    UnkDoorGrappleDef_ovb0093* def = self->def;
    void* key = msg->sender->field_0x10->field_0x20;
    UnkIter<void*> first = {def->points};
    UnkIter<void*> last = {def->points + def->count};
    if (std::find(first, last, key) == last)
        return false;
    return fabsf(msg->field_0x20) >= 0.94f;
}
