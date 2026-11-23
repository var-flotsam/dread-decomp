// UnkTU811 (CEmmyAIComponent's unit): the base state change from `from` to `to`. Entering a state sets the
// state colour on the actor's two patrol materials and the cone search effect's colours; entering state 3
// from another state also replays the effect named by its hash at the stored position, then the state's
// timeline key is pushed. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/CFXComponent.h"
#include "game/UnkActor.h"
#include "game/UnkEmmyLabAI_A2_b70.h"
#include "game/UnkEmmyLabMark_A2_b70.h"
#include "game/UnkTU134.h"
#include "game/UnkTU811.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkTU2712.h"
#include "types.h"

using base::math::CVector4D;
using base::math::UnkVector4D;

// A shader constant bound to a material colour: where the value is uploaded.
struct UnkShaderConst_l54b93c {
    u8 pad_0x0[0x30];
    CVector4D* value;  // 0x30
};

struct UnkMaterial_l54b93c {
    u8 pad_0x0[0x14c];
    CVector4D color;  // 0x14c
    CVector4D color2;  // 0x15c
    u8 pad_0x16c[0x1f0 - 0x16c];
    UnkShaderConst_l54b93c* colorConst;  // 0x1f0
    UnkShaderConst_l54b93c* color2Const;  // 0x1f8

    // Sets the colour and uploads it to its shader constant, if bound.
    void SetColor(const CVector4D& c) {
        fn_710007b518(&color, &c);
        if (colorConst)
            *colorConst->value = c;
    }
    void SetColor2(const CVector4D& c) {
        fn_710007b518(&color2, &c);
        if (color2Const)
            *color2Const->value = c;
    }
};

struct UnkMaterialList_l54b93c {
    u8 pad_0x0[0x48];
    UnkMaterial_l54b93c** items;  // 0x48
};

struct UnkMaterialSet_l54b93c {
    u8 pad_0x0[0x78];
    UnkMaterialList_l54b93c* list;  // 0x78
};

struct UnkModel_l54b93c {
    u8 pad_0x0[0x138];
    UnkMaterialSet_l54b93c* materials;  // 0x138

    UnkMaterial_l54b93c* Material(u32 index) { return materials->list->items[index]; }
};

struct UnkShape_l54b93c {
    u8 pad_0x0[0x50];
    UnkModel_l54b93c* model;  // 0x50
};

struct UnkMode_l54b93c {
    u8 pad_0x0[0x70];
    s32 mode;  // 0x70
};

struct UnkFxPart_l54b93c {
    u8 pad_0x0[0xb];
    u8 index;  // 0xb
    u8 pad_0xc[0x160 - 0xc];
    UnkMaterialSet_l54b93c* materials;  // 0x160
};

struct UnkEmmyAI_l54b93c {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x14b84 - 0x38];
    UnkVec3f effectPos;  // 0x14b84
    u8 pad_0x14b90[0x2a010 - 0x14b90];
    u32 material;  // 0x2a010
    u8 pad_0x2a014[0xc];
    CVector4D color;  // 0x2a020
    u32 material2;  // 0x2a030
};
static_assert(__builtin_offsetof(UnkEmmyAI_l54b93c, color) == 0x2a020, "UnkEmmyAI_l54b93c layout");

const CVector4D* fn_7100572130(s32 state);
UnkVector4D fn_710054b4b0(UnkEmmyAI_l54b93c* self);
extern const u64* g_unk_7101a48c70[] __attribute__((visibility("hidden")));

void fn_710054b93c(UnkEmmyLabAI_A2_b70* ai, s32 from, s32 to) {
    UnkEmmyAI_l54b93c* self = reinterpret_cast<UnkEmmyAI_l54b93c*>(ai);
    if (to != 0) {
        UnkActor* owner = self->owner;
        UnkModel_l54b93c* model = static_cast<UnkShape_l54b93c*>(owner->field_0x228)->model;
        UnkMode_l54b93c* mode = static_cast<UnkMode_l54b93c*>(owner->field_0x1b8);
        s32 state;
        if (mode && mode->mode == 2)
            state = 0x7fffffff;
        else if (fn_710054ad2c(reinterpret_cast<UnkEmmyAIView*>(self)) ||
                 fn_710054aecc(reinterpret_cast<UnkEmmyAI2_b62a3*>(self)))
            state = 0;
        else if (fn_7100543b30(reinterpret_cast<UnkEmmyAIView*>(self)))
            state = 1;
        else
            state = fn_71005436e8(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(self)) ? 2 : 0x7fffffff;

        const CVector4D* stateColor = fn_7100572130(state);
        fn_710007b518(&self->color, stateColor);
        model->Material(self->material)->SetColor(self->color);
        if (self->material2 != 0xffffffff)
            model->Material(self->material2)->SetColor(self->color);

        UnkObject* fx = static_cast<UnkObject*>(self->owner->field_0x280);
        UnkFxPart_l54b93c* cone = reinterpret_cast<UnkFxPart_l54b93c*>(
            fn_7101152b90(reinterpret_cast<UnkWorld*>(fx), StringCrc64("conepatrolsearch", 16)));
        if (cone && fn_710114e680(fx, cone->index)) {
            f32 red;
            if (fn_7100543b30(reinterpret_cast<UnkEmmyAIView*>(self)))
                red = 1.0f;
            else
                red = fn_710054aecc(reinterpret_cast<UnkEmmyAI2_b62a3*>(self)) ? 1.0f : 0.0f;
            CVector4D tint = {red, 0.0f, 0.0f, 1.0f};
            cone->materials->list->items[0]->SetColor(fn_710054b4b0(self));
            cone->materials->list->items[0]->SetColor2(tint);
        }
    }

    if (from == 3) {
        UnkObject* fx = static_cast<UnkObject*>(self->owner->field_0x280);
        UnkPart* part = fn_7101152b90(reinterpret_cast<UnkWorld*>(fx), 0xb0e46785072be2e2ull);
        if (part && fn_710114e668(fx, reinterpret_cast<UnkFxPart_l54b93c*>(part)->index)) {
            fn_7100146d84(reinterpret_cast<UnkFxObject_ovb0041*>(part), &self->effectPos);
            fn_7101152c0c(static_cast<UnkObject*>(self->owner->field_0x280), part, 1, 0);
            fn_7101156bd0(static_cast<UnkObject*>(self->owner->field_0x280), part, 5.0f, 1.0f, 1.0f, true, true);
        }
    }

    // States 1 to 4 each have a timeline key.
    u32 key = static_cast<u32>(to) - 1;
    if (key < 4)
        fn_7100d7c828(self->owner, g_unk_7101a48c70[static_cast<s32>(key)], false);
}
