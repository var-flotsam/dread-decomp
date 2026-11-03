// CActionSwitcherOnPullGrapplePointComponent unit (tools/tu_map.py). fn_71008261e0: after the
// base test (fn_710082cb24) passes and the switcher isn't on (+0x98), a pull counts when it comes
// from the grapple point its definition (+0x90) names (+0x78) and is at least as strong (+0x20 of
// the message, by size) as the definition's threshold (+0xa0). Names and layouts are placeholders.
#include "types.h"

struct UnkPullSource_b50 {
    u8 pad_0x0[0x20];
    void* field_0x20;
};

struct UnkPullSender_b50 {
    u8 pad_0x0[0x10];
    UnkPullSource_b50* field_0x10;
};

struct UnkPullMessage_b50 {
    UnkPullSender_b50* sender;
    u8 pad_0x8[0x18];
    f32 field_0x20;
};

struct UnkPullDef_b50 {
    u8 pad_0x0[0x78];
    void* field_0x78;
    u8 pad_0x80[0x20];
    f32 field_0xa0;
};

struct UnkPullSwitcher_b50 {
    u8 pad_0x0[0x90];
    UnkPullDef_b50* def;
    bool field_0x98;
};

// The base test (both pull components share it).
bool fn_710082cb24(void* self, void* arg, void* msg);

bool fn_71008261e0(UnkPullSwitcher_b50* self, void* arg, UnkPullMessage_b50* msg) {
    if (!fn_710082cb24(self, arg, msg) || self->field_0x98)
        return false;
    UnkPullSource_b50* source = msg->sender->field_0x10;
    UnkPullDef_b50* def = self->def;
    if (source->field_0x20 != def->field_0x78)
        return false;
    return __builtin_fabsf(msg->field_0x20) >= def->field_0xa0;
}
