// Liquid pool (unit 1394, CCharClassLiquidPoolComponent): turns a height into a ratio of the pool's extent and
// passes it on to the simulation at +0xc88's +0x10. The three virtual getters (slots 0x180 and 0x178) return a
// vector through x8. Names and layouts are placeholders.
#include "game/CLiquidSimulationComponent.h"
#include "types.h"

struct UnkLiquidVec_A5_b70 {
    f32 x, y, z;
    UnkLiquidVec_A5_b70(const UnkLiquidVec_A5_b70& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkLiquidSimHolder_A5_b70 {
    u8 unk_0x0[0x10];
    void* sim;
};

struct UnkLiquidPool_A5_b70 {
    void** vtable;
    u8 unk_0x8[0xc80];
    UnkLiquidSimHolder_A5_b70* holder;  // +0xc88

    UnkLiquidVec_A5_b70 vf_0x180() {
        return reinterpret_cast<UnkLiquidVec_A5_b70 (*)(UnkLiquidPool_A5_b70*)>(vtable[0x180 / 8])(this);
    }
    UnkLiquidVec_A5_b70 vf_0x178() {
        return reinterpret_cast<UnkLiquidVec_A5_b70 (*)(UnkLiquidPool_A5_b70*)>(vtable[0x178 / 8])(this);
    }
};

void fn_7100901900(UnkLiquidPool_A5_b70* self, const f32* height, u32 a, bool b) {
    f32 h = *height;
    UnkLiquidVec_A5_b70 pos = self->vf_0x180();
    f32 base = pos.x;
    UnkLiquidVec_A5_b70 ext = self->vf_0x178();
    f32 extent = ext.x;
    UnkLiquidVec_A5_b70 size = self->vf_0x178();
    if (self->holder != nullptr && self->holder->sim != nullptr) {
        f32 width = size.x;
        f32 ratio = (h - (base + extent * -0.5f)) / width;
        fn_7100905d8c(self->holder->sim, ratio, a, b);
    }
}
