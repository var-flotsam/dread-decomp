// CEmmyWaveMovementComponent's unit (tools/tu_map.py unit 796): fn_71005026d0 sets the wave size (+0x5dc)
// when it changes (or when bit 0 of `mode` forces it): the collision object named g_unk_71726ddc40 gets its
// shape resized to the size less 40 (at least 20) when the shape is of kind 2, and the effects "emywave",
// "wave" and "wavewall" of the owner's FX world (+0x280) get the size over the base size (+0x5e0) as a
// uniform scale. The parameter types are those of src/game/CEmmyWaveMovementComponent/fn_7100502628.cpp.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkChargeComponent.h"
#include "types.h"

struct UnkWorld;
struct UnkPart;
UnkPart* fn_7101152b90(UnkWorld* world, u64 hash);

struct UnkSphereShape_ovb0135;
void fn_7100d618f8(UnkSphereShape_ovb0135* shape, f32 size);

// The collision object's name.
extern base::global::CStrId g_unk_71726ddc40 __attribute__((visibility("hidden")));

struct UnkWaveShape_ovb0136 {
    void** vtable;
    s32 vf_0x18() { return reinterpret_cast<s32 (*)(UnkWaveShape_ovb0136*)>(vtable[0x18 / 8])(this); }
    void vf_0x78() { reinterpret_cast<void (*)(UnkWaveShape_ovb0136*)>(vtable[0x78 / 8])(this); }
};

struct UnkWaveCollider_ovb0136 {
    void** vtable;
    u8 pad_0x8[0x150];
    UnkWaveShape_ovb0136* shape;  // 0x158
    void vf_0x48() { reinterpret_cast<void (*)(UnkWaveCollider_ovb0136*)>(vtable[0x48 / 8])(this); }
};

struct UnkWaveFx_ovb0136 {
    void** vtable;
    void vf_0x20(const UnkVec3f* scale) {
        reinterpret_cast<void (*)(UnkWaveFx_ovb0136*, const UnkVec3f*)>(vtable[0x20 / 8])(this, scale);
    }
};

static inline UnkWorld* FxWorld(UnkChargeComponent* self) {
    return *reinterpret_cast<UnkWorld**>(reinterpret_cast<u8*>(self->field_0x30) + 0x280);
}

static inline void ScaleFx(UnkWorld* world, const char* name, u64 length, f32 scale) {
    if (auto* fx = reinterpret_cast<UnkWaveFx_ovb0136*>(fn_7101152b90(world, fn_7100001570(name, length)))) {
        UnkVec3f v(scale, scale, scale);
        fx->vf_0x20(&v);
    }
}

void fn_71005026d0(UnkChargeComponent* self, u32 mode, f32 value) {
    if (!(mode & 1) && self->field_0x5dc == value)
        return;
    self->field_0x5dc = value;
    if (auto* collision = static_cast<UnkCollisionContainer*>(self->field_0x30->field_0x2a0)) {
        auto* collider = reinterpret_cast<UnkWaveCollider_ovb0136*>(fn_71008640a4(collision, &g_unk_71726ddc40));
        if (collider) {
            UnkWaveShape_ovb0136* shape = collider->shape;
            if (shape && shape->vf_0x18() == 2) {
                f32 size = self->field_0x5dc + -40.0f;
                fn_7100d618f8(reinterpret_cast<UnkSphereShape_ovb0135*>(shape), !(size < 20.0f) ? size : 20.0f);
                shape->vf_0x78();
                collider->vf_0x48();
            }
        }
    }
    UnkWorld* world = FxWorld(self);
    if (!world)
        return;
    f32 scale = self->field_0x5dc / *reinterpret_cast<f32*>(reinterpret_cast<u8*>(self) + 0x5e0);
    ScaleFx(world, "emywave", 7, scale);
    ScaleFx(FxWorld(self), "wave", 4, scale);
    ScaleFx(FxWorld(self), "wavewall", 8, scale);
}
