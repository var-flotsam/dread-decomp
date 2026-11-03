#include "types.h"

struct UnkCameraBoundaryData {
    u8 unk_0x0[0xc40];
    f32 field_0xc40;
};

struct UnkCameraBoundaryOwner {
    u8 unk_0x0[0x10];
    UnkCameraBoundaryData* data;
};

static inline UnkCameraBoundaryData* GetData(UnkCameraBoundaryOwner* owner) {
    return owner ? owner->data : nullptr;
}

struct CCameraBoundaryCtrl {
    u8 unk_0x0[0xb0];
    UnkCameraBoundaryOwner* owner;
    u8 unk_0xb8[0x8];
    f32 field_0xc0;
};

f32 fn_71010e63ec(UnkCameraBoundaryData* data, CCameraBoundaryCtrl* ctrl, f32 a, f32 b, f32 c);

f32 fn_710076438c(CCameraBoundaryCtrl* self, f32 dt) {
    UnkCameraBoundaryOwner* owner = self->owner;
    f32 value = owner->data->field_0xc40;
    if (!(value > 0.0f))
        return value;
    return fn_71010e63ec(GetData(owner), self, self->field_0xc0, 4.0f, dt);
}
