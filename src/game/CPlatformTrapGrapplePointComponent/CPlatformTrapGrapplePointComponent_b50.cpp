// CPlatformTrapGrapplePointComponent unit (tools/tu_map.py). fn_710082bcf4: the shape of
// fn_71008261e0 (CActionSwitcherOnPullGrapplePointComponent_b50.cpp): after the base test
// (fn_710082cb24) passes and the owner's (+0x30) state (fn_710082bd74) isn't set (fn_7100aca464),
// a pull counts when it comes from the grapple point the definition (+0x90) names (+0x78) and is
// at least 0.9 strong (+0x20 of the message, by size). Names and layouts are placeholders.
#include "types.h"

struct UnkTrapPullSource_b50 {
    u8 pad_0x0[0x20];
    void* field_0x20;
};

struct UnkTrapPullSender_b50 {
    u8 pad_0x0[0x10];
    UnkTrapPullSource_b50* field_0x10;
};

struct UnkTrapPullMessage_b50 {
    UnkTrapPullSender_b50* sender;
    u8 pad_0x8[0x18];
    f32 field_0x20;
};

struct UnkTrapPullDef_b50 {
    u8 pad_0x0[0x78];
    void* field_0x78;
};

struct UnkTrapPull_b50 {
    u8 pad_0x0[0x30];
    void* owner;
    u8 pad_0x38[0x58];
    UnkTrapPullDef_b50* def;  // 0x90
};

// The base test (both pull components share it).
bool fn_710082cb24(void* self, void* arg, void* msg);
void* fn_710082bd74(void* owner);
bool fn_7100aca464(void* state);

bool fn_710082bcf4(UnkTrapPull_b50* self, void* arg, UnkTrapPullMessage_b50* msg) {
    if (!fn_710082cb24(self, arg, msg) || fn_7100aca464(fn_710082bd74(self->owner)))
        return false;
    UnkTrapPullSource_b50* source = msg->sender->field_0x10;
    UnkTrapPullDef_b50* def = self->def;
    if (source->field_0x20 != def->field_0x78)
        return false;
    return __builtin_fabsf(msg->field_0x20) >= 0.9f;
}
