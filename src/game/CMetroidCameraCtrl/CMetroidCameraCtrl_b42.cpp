// CMetroidCameraCtrl unit (tools/tu_map.py, reflection hint). fn_7100935cb8: sets the camera's
// distance: the requested one is kept at +0x3c0; a non-positive request falls back to 2250 for the
// target at +0x258 (and +0x254 with the flag), then to the distance of the camera found through
// +0xb8 (+0x10, its +0x40) or 1665.8074; the result goes to +0x258 and +0x70 (and +0x40 with the
// flag). Names and layouts are placeholders.
#include "game/UnkCameraCtrl.h"
#include "types.h"

struct UnkCameraData_b42 {
    u8 pad_0x0[0x40];
    f32 distance;  // 0x40
};

struct UnkCameraRef_b42 {
    u8 pad_0x0[0x10];
    UnkCameraData_b42* camera;  // 0x10
};

struct UnkCameraCtrl_b42 {
    u8 pad_0x0[0x40];
    f32 field_0x40;
    u8 pad_0x44[0x2c];
    f32 field_0x70;
    u8 pad_0x74[0x44];
    UnkCameraRef_b42* ref;  // 0xb8
    u8 pad_0xc0[0x194];
    f32 field_0x254;
    f32 field_0x258;
    u8 pad_0x25c[0x164];
    f32 requested;  // 0x3c0
};

// Declared as src/game/CSubAreaManager.cpp does.
void fn_7100935cb8(f32 distance, UnkCameraCtrl* ctrl, bool flag) {
    UnkCameraCtrl_b42* self = reinterpret_cast<UnkCameraCtrl_b42*>(ctrl);
    f32 target = distance > 0.0f ? distance : 2250.0f;
    self->requested = distance;
    self->field_0x258 = target;
    if (flag)
        self->field_0x254 = target;
    if (!(distance > 0.0f)) {
        UnkCameraData_b42* camera;
        if (self->ref && (camera = self->ref->camera))
            distance = camera->distance;
        else
            distance = 1665.8074f;
    }
    self->field_0x258 = distance;
    self->field_0x70 = distance;
    if (flag)
        self->field_0x40 = distance;
}
