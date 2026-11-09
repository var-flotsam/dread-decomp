// CFlockingSwarmControllerComponent unit (tools/tu_map.py unit 1158): fn_7100742440 updates the hue byte
// (+0x3c) of swarm member `index` (0x148-byte members of the class data, as in
// CFlockingSwarmControllerComponent_Reset_A3_b72.cpp) while its velocity (+0x70) is not about zero. Mode
// 1 of the tuning (+0x328, its +0x434) steps the hue towards one taken from the x speed (by 1, or by 5
// when more than half the wheel away); mode 0 sets it from the heading's angle. The hue wraps at 255.
// The class data is read without a check, as in the game. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkSwarmMember_hue_ovb0063 {
    u8 pad_0x0[0x3c];
    u8 hue;  // 0x3c
    u8 pad_0x3d[0x33];
    CVector3D velocity;  // 0x70
    u8 pad_0x7c[0xcc];
};
static_assert(sizeof(UnkSwarmMember_hue_ovb0063) == 0x148, "UnkSwarmMember_hue_ovb0063 layout");

struct UnkSwarmClassData_hue_ovb0063 {
    u8 pad_0x0[0x50];
    UnkSwarmMember_hue_ovb0063* members;  // 0x50
    u8 pad_0x58[0xb8];
    s32 member_count;  // 0x110
};

struct UnkSwarmClassHolder_hue_ovb0063 {
    u8 pad_0x0[0x10];
    UnkSwarmClassData_hue_ovb0063* data;  // 0x10
};

struct UnkSwarmTuning_hue_ovb0063 {
    u8 pad_0x0[0x434];
    s32 hue_mode;  // 0x434
};

struct UnkSwarmCtrl_hue_ovb0063 {
    u8 pad_0x0[0xe8];
    UnkSwarmClassHolder_hue_ovb0063* klass;  // 0xe8
    u8 pad_0xf0[0x238];
    UnkSwarmTuning_hue_ovb0063* tuning;  // 0x328
};

// Whether v is outside [-0.1, 0.1] (or not a number).
static inline bool NotNearZero(f32 v) {
    return !(v + 0.1f >= 0.0f) || !(v - 0.1f <= 0.0f);
}

void fn_7100742440(UnkSwarmCtrl_hue_ovb0063* self, s32 index) {
    UnkSwarmClassData_hue_ovb0063* data = reinterpret_cast<UnkSwarmClassData_hue_ovb0063*>(self->klass);
    if (data)
        data = self->klass->data;
    UnkSwarmMember_hue_ovb0063* member =
        index < data->member_count && data->members ? &data->members[index] : nullptr;
    const CVector3D& v = member->velocity;
    s32 hue = member->hue;
    if (NotNearZero(v.x) || NotNearZero(v.y) || NotNearZero(v.z)) {
        switch (self->tuning->hue_mode) {
        case 0: {
            CVector2D d;
            fn_710007b430(&d, &v);
            f32 length = sqrtf(d.x * d.x + d.y * d.y);
            if (length != 0.0f) {
                d.x = d.x / length;
                d.y = d.y / length;
            }
            f32 angle = acosf(-d.x) / 6.2831855f;
            if (d.y > 0.0f)
                angle = 1.0f - angle;
            hue = (s32)(angle * 255.0f);
            break;
        }
        case 1: {
            s32 target = (s32)(fabsf(v.x) * 15.0f);
            if (v.x > 0.0f)
                target = 255 - target;
            if (hue > target) {
                if (hue - target >= 0x81)
                    hue += 5;
                else
                    hue -= 5;
            } else {
                if (target - hue >= 0x81)
                    hue -= 1;
                else
                    hue += 1;
            }
            break;
        }
        }
    }
    member->hue = hue % 255;
}
