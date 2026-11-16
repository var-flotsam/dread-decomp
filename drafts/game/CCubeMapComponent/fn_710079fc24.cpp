// CCubeMapComponent's unit: sets the intensity, optionally scaled by the current one, either at once
// (written to both render objects) or as a fade over a duration. Names and layouts are placeholders.
#include "types.h"

struct UnkCubeMapRenderData_l79fc24 {
    u8 pad_0x0[0x7c];
    f32 intensity;  // 0x7c
};

struct UnkCubeMapRender_l79fc24 {
    u8 pad_0x0[0x128];
    UnkCubeMapRenderData_l79fc24* data;  // 0x128
};

struct UnkCubeMap_l79fc24 {
    u8 pad_0x0[0x50];
    UnkCubeMapRender_l79fc24* render_a;  // 0x50
    u8 pad_0x58[0x8];
    UnkCubeMapRender_l79fc24* render_b;  // 0x60
    u8 pad_0x68[0x8];
    f32 value;     // 0x70
    f32 from;      // 0x74
    f32 to;        // 0x78
    f32 duration;  // 0x7c
    f32 time;      // 0x80
};

void fn_710079fc24(UnkCubeMap_l79fc24* self, f32 intensity, bool relative, f32 duration) {
    f32 current = self->render_b->data->intensity;
    if (duration <= 0.001f) {
        f32 target = relative ? current * intensity : intensity;
        self->to = target;
        self->from = target;
        self->value = target;
        self->render_a->data->intensity = target;
        self->render_b->data->intensity = target;
    } else {
        self->from = self->value;
        self->to = relative ? current * intensity : intensity;
        self->duration = duration;
        self->time = 0.0f;
    }
}
