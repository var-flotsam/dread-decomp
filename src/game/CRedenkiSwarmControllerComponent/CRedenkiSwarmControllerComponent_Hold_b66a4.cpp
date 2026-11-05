// CRedenkiSwarmControllerComponent's unit (tools/tu_map.py): an update step. Takes the position
// fn_710064fef8 answers and the area the component's vf_0x1b8 (mode +0x390 set) or vf_0x1b0 gives;
// while the area contains the position in 2D (its vf_0x50), the time at +0x56c grows by `dt`, else
// it is cleared; then the base step fn_7100741aa8. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_b38.cpp defines it.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkSwarmArea_b66a4 {
    void** vtable;
    bool Contains(const base::math::CVector2D* p) {
        return reinterpret_cast<bool (*)(UnkSwarmArea_b66a4*, const base::math::CVector2D*)>(vtable[0x50 / 8])(this, p);
    }
};

struct UnkSwarmHold_b66a4 {
    void** vtable;
    u8 pad_0x8[0x390 - 0x8];
    u32 mode;  // 0x390
    u8 pad_0x394[0x56c - 0x394];
    f32 time;  // 0x56c
    UnkSwarmArea_b66a4* Area(u32 slot) {
        return reinterpret_cast<UnkSwarmArea_b66a4* (*)(UnkSwarmHold_b66a4*)>(vtable[slot / 8])(this);
    }
};

UnkVec3Result fn_710064fef8(UnkSwarmHold_b66a4* self);
void fn_7100741aa8(UnkSwarmHold_b66a4* self, f32 dt);

void fn_710064fe44(UnkSwarmHold_b66a4* self, f32 dt) {
    base::math::CVector2D pos2;
    UnkVec3Result pos = fn_710064fef8(self);
    UnkSwarmArea_b66a4* area = self->mode ? self->Area(0x1b8) : self->Area(0x1b0);
    if (area) {
        fn_710007b430(&pos2, reinterpret_cast<const base::math::CVector3D*>(&pos));
        if (area->Contains(&pos2)) {
            self->time += dt;
            fn_7100741aa8(self, dt);
            return;
        }
    }
    self->time = 0.0f;
    fn_7100741aa8(self, dt);
}
