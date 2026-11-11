// CMovableGrapplePointComponent's unit (1292): after the pull components' base test (fn_710082cb24), a pull counts
// when it is strong enough (|+0x20| at least 0.96) and comes from the source fn_710082a500 picks for the side the
// puller is on (the sign of its +0x44 x against the point's position, virtual slot 0x188). Shape of fn_710082e1cc
// (CPullOffGrapplePointComponent_Accepts_A5_b65.cpp). Names and layouts are placeholders.
#include "game/CPullableGrapplePointComponent.h"
#include "types.h"

struct UnkMovablePullSource_A5_b70 {
    u8 pad_0x0[0x20];
    void* field_0x20;
};

struct UnkMovablePullSender_A5_b70 {
    u8 pad_0x0[0x10];
    UnkMovablePullSource_A5_b70* field_0x10;
};

struct UnkMovablePullMessage_A5_b70 {
    UnkMovablePullSender_A5_b70* sender;
    u8 pad_0x8[0x18];
    f32 strength;  // 0x20
};

struct UnkMovablePuller_A5_b70 {
    u8 pad_0x0[0x44];
    f32 x;
};

struct UnkMovableVec_A5_b70 {
    f32 x, y, z;
    UnkMovableVec_A5_b70(const UnkMovableVec_A5_b70& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkMovableGrapplePoint_A5_b70 {
    void** vtable;

    UnkMovableVec_A5_b70 vf_0x188() {
        return reinterpret_cast<UnkMovableVec_A5_b70 (*)(UnkMovableGrapplePoint_A5_b70*)>(vtable[0x188 / 8])(this);
    }
};

void** fn_710082a500(UnkMovableGrapplePoint_A5_b70* self, f32 side);

bool fn_710082a444(UnkMovableGrapplePoint_A5_b70* self, UnkMovablePuller_A5_b70* puller,
                   UnkMovablePullMessage_A5_b70* msg) {
    if (!fn_710082cb24(self, puller, msg))
        return false;
    if (!(__builtin_fabsf(msg->strength) >= 0.96f))
        return false;
    f32 d = puller->x - self->vf_0x188().x;
    void** source = fn_710082a500(self, (f32)((d > 0.0f) - (d < 0.0f)));
    return msg->sender->field_0x10->field_0x20 == *source;
}
