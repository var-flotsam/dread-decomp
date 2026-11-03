// CHecathonPlanktonFXComponent (tools/tu_map.py unit 1271; the name from reflection): clears the
// intensity at +0x1294 and, with a model (+0x12c0), writes (intensity, 0, 0, 1) to the parameter at
// +0x14c of the model's instance (fn_71001be428 of its +0x130; fn_710007b518) and to its material
// block (+0x1f0, its +0x30) when there is one. Names and layout are placeholders.
#include "base/math/CVector4D.h"
#include "types.h"

using base::math::CVector4D;

struct UnkPlanktonMaterial_b44a2 {
    u8 pad_0x0[0x30];
    CVector4D* params;
};

struct UnkPlanktonInstance_b44a2 {
    u8 pad_0x0[0x14c];
    CVector4D param;
    u8 pad_0x15c[0x94];
    UnkPlanktonMaterial_b44a2* material;  // 0x1f0
};

struct UnkPlanktonModel_b44a2 {
    u8 pad_0x0[0x130];
    void* field_0x130;
};

UnkPlanktonInstance_b44a2* fn_71001be428(void* model);

struct UnkPlanktonFX_b44a2 {
    u8 pad_0x0[0x1294];
    f32 intensity;
    u8 pad_0x1298[0x28];
    UnkPlanktonModel_b44a2* model;  // 0x12c0
};

void fn_710080c9bc(UnkPlanktonFX_b44a2* self) {
    self->intensity = 0.0f;
    if (!self->model)
        return;
    UnkPlanktonInstance_b44a2* instance = fn_71001be428(self->model->field_0x130);
    CVector4D value = {self->intensity, 0.0f, 0.0f, 1.0f};
    fn_710007b518(&instance->param, &value);
    if (instance->material)
        *instance->material->params = value;
}
