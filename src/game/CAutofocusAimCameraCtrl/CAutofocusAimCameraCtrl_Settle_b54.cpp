// CAutofocusAimCameraCtrl unit (tools/tu_map.py, the name from reflection): consumes the pending flag at
// +0x240. With it set, the elapsed value at +0x1d8 is scaled by the tunable's +0x24 (otherwise taken as 0);
// below the tunable's +0x2c threshold the value becomes +0x24 minus itself, else 0. The tunable is
// CTunableAutofocusAimCameraCtrl's global (src/reflection/CAutofocusAimCameraCtrl.cpp). Names and layouts
// are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

extern base::reflection::UnkTunable g_unk_717270f910;

struct UnkAutofocusTunable_b54 {
    u8 pad_0x0[0x24];
    f32 duration;  // 0x24
    u8 pad_0x28[4];
    f32 threshold;  // 0x2c
};

struct UnkAutofocusCtrl_b54 {
    u8 pad_0x0[0x1d8];
    f32 value;  // 0x1d8
    u8 pad_0x1dc[0x64];
    bool pending;  // 0x240
};

void fn_7100723408(UnkAutofocusCtrl_b54* self) {
    const UnkAutofocusTunable_b54* tunable = reinterpret_cast<const UnkAutofocusTunable_b54*>(&g_unk_717270f910);
    f32 ratio = self->pending ? self->value / tunable->duration : 0.0f;
    self->pending = false;
    if (ratio >= tunable->threshold)
        self->value = 0.0f;
    else
        self->value = tunable->duration - self->value;
}
