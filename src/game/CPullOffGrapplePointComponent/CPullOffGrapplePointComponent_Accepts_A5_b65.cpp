// CPullOffGrapplePointComponent's unit: after the pull components' base test (fn_710082cb24), a pull
// counts when it comes from the unit's grapple source (g_unk_7172725078) and is strong enough (|+0x20|
// at least 0.94). Shape of fn_7100827978 (CAIGrapplePointComponent_b50.cpp). Names are placeholders.
#include "types.h"

struct UnkPullOffSource_A5_b65 {
    u8 pad_0x0[0x20];
    void* field_0x20;
};

struct UnkPullOffSender_A5_b65 {
    u8 pad_0x0[0x10];
    UnkPullOffSource_A5_b65* field_0x10;
};

struct UnkPullOffMessage_A5_b65 {
    UnkPullOffSender_A5_b65* sender;
    u8 pad_0x8[0x18];
    f32 strength;  // 0x20
};

extern void* g_unk_7172725078 __attribute__((visibility("hidden")));

bool fn_710082cb24(void* self, void* arg, void* msg);

bool fn_710082e1cc(void* self, void* arg, UnkPullOffMessage_A5_b65* msg) {
    if (!fn_710082cb24(self, arg, msg))
        return false;
    if (msg->sender->field_0x10->field_0x20 != g_unk_7172725078)
        return false;
    return __builtin_fabsf(msg->strength) >= 0.94f;
}
