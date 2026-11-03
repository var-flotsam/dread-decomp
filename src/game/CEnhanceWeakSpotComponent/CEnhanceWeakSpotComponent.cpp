// In the CEnhanceWeakSpotComponent unit: when enabled (+0x3d) and the state at +0x3e changes, stores it
// and hands the actor (+0x30) the definition's (+0x40) timeline id for the new state (+0x58 on,
// +0x60 off) (fn_7100d7c828). Names and layouts are placeholders.
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkWeakSpotDef {
    u8 pad_0x0[0x58];
    u64 on_id;
    u64 off_id;
};

struct UnkEnhanceWeakSpot {
    u8 pad_0x0[0x30];
    UnkActor* actor;
    u8 pad_0x38[5];
    bool field_0x3d;
    bool field_0x3e;
    u8 pad_0x3f;
    UnkWeakSpotDef* def;
};

void fn_71007d40a0(UnkEnhanceWeakSpot* self, bool on) {
    if (self->field_0x3d && self->field_0x3e != on) {
        self->field_0x3e = on;
        fn_7100d7c828(self->actor, on ? &self->def->on_id : &self->def->off_id, false);
    }
}
