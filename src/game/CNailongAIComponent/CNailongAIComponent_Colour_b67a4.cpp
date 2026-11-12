// CNailongAIComponent's unit (tools/tu_map.py, the name from reflection): sets the slot colour through
// the owner's +0x228 chain from a value, as CNailongAIComponent_b46.cpp's handler does: in state 2
// (+0x148c8) white after the second call on its +0x138, with alpha from fn_71012399d0 (the value and
// 4); else a grey of the squared value (not below 0), opaque. Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "game/UnkTU127.h"
#include "game/UnkTU253.h"
#include "types.h"

using base::math::CVector4D;

// As src/game/CNailongAIComponent/CNailongAIComponent_b46.cpp declares them.
struct UnkStruct_71001436f4;

struct UnkNailongSlots_b67a4 {
    u8 pad_0x0[0x138];
    void* field_0x138;
};

struct UnkNailongModel_b67a4 {
    u8 pad_0x0[0x50];
    UnkNailongSlots_b67a4* slots;
};

struct UnkNailongActor_b67a4 {
    u8 pad_0x0[0x228];
    UnkNailongModel_b67a4* model;
};

struct UnkNailongColourAI_b67a4 {
    u8 pad_0x0[0x30];
    UnkNailongActor_b67a4* actor;
    u8 pad_0x38[0x148c8 - 0x38];
    u32 state;
};

f32 fn_71012399d0(f32 value, f32 scale);

void fn_7100620ca4(UnkNailongColourAI_b67a4* self, f32 value) {
    if (self->state == 2) {
        f32 alpha = fn_71012399d0(value, 4.0f);
        CVector4D colour;
        colour.x = 1.0f;
        colour.y = 1.0f;
        colour.z = 1.0f;
        colour.w = 1.0f;
        fn_71001c287c(self->actor->model->slots->field_0x138, 0, 1, &colour, 1);
        colour.w = alpha;
        fn_71001436f4(reinterpret_cast<UnkStruct_71001436f4*>(self->actor->model->slots), 1, &colour, 1);
    } else {
        f32 v = value * value;
        v = v <= 0.0f ? 0.0f : v;
        UnkNailongSlots_b67a4* slots = self->actor->model->slots;
        CVector4D colour;
        colour.x = v;
        colour.y = v;
        colour.z = v;
        colour.w = 1.0f;
        fn_71001436f4(reinterpret_cast<UnkStruct_71001436f4*>(slots), 0, &colour, 1);
    }
}
