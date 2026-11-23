// CGooplotAIComponent (tools/tu_map.py unit 851): fades the owner's model, setting the first colour
// component of each of its meshes from the remaining count at +0x147f8 against the tunable's
// (+0x2c), then reapplies the colour of the "gooplottail" part's first mesh. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/CCharClassGooplotAIComponent.h"
#include "game/CFXComponent.h"
#include "types.h"

struct UnkGooplotTunable_l599ec4 {
    u8 pad_0x0[0x2c];
    s32 field_0x2c;
};

struct UnkGooplotShader_l599ec4 {
    u8 pad_0x0[0x30];
    base::math::CVector4D* color;  // 0x30
};

struct UnkGooplotMesh_l599ec4 {
    u8 pad_0x0[0x15c];
    base::math::CVector4D color;  // 0x15c
    u8 pad_0x16c[0x1f8 - 0x16c];
    UnkGooplotShader_l599ec4* shader;  // 0x1f8

    void SetColor(const base::math::CVector4D& value) {
        fn_710007b518(&color, &value);
        if (shader)
            *shader->color = value;
    }
};

struct UnkGooplotMeshList_l599ec4 {
    u8 pad_0x0[0x2c];
    u32 count;  // 0x2c
    u8 pad_0x30[0x18];
    UnkGooplotMesh_l599ec4** meshes;  // 0x48
};

struct UnkGooplotModelData_l599ec4 {
    u8 pad_0x0[0x78];
    UnkGooplotMeshList_l599ec4* meshes;  // 0x78
};

struct UnkGooplotModel_l599ec4 {
    u8 pad_0x0[0x138];
    UnkGooplotModelData_l599ec4* data;  // 0x138
};

struct UnkGooplotPart_l599ec4 {
    u8 pad_0x0[0xb];
    u8 kind;  // 0xb
    u8 pad_0xc[0x154];
    UnkGooplotModelData_l599ec4* data;  // 0x160
};

struct UnkGooplotRender_l599ec4 {
    u8 pad_0x0[0x50];
    UnkGooplotModel_l599ec4* model;  // 0x50
};

struct UnkGooplotOwner_l599ec4 {
    u8 pad_0x0[0x228];
    UnkGooplotRender_l599ec4* render;  // 0x228
    u8 pad_0x230[0x50];
    UnkObject* parts;  // 0x280
};

struct UnkGooplotAI_l599ec4 {
    u8 pad_0x0[0x30];
    UnkGooplotOwner_l599ec4* owner;  // 0x30
    u8 pad_0x38[0x147f8 - 0x38];
    s32 field_0x147f8;
};
static_assert(__builtin_offsetof(UnkGooplotAI_l599ec4, field_0x147f8) == 0x147f8, "field_0x147f8");

void fn_7100599ec4(UnkGooplotAI_l599ec4* self) {
    UnkGooplotModel_l599ec4* model = self->owner->render->model;
    u32 count = model->data->meshes->count;
    const UnkGooplotTunable_l599ec4& tunable = reinterpret_cast<UnkGooplotTunable_l599ec4&>(g_unk_71726e72b8);
    f32 alpha = 1.0f - (f32)(self->field_0x147f8 - 1) / (f32)tunable.field_0x2c;
    for (u32 i = 0; i < count; ++i) {
        UnkGooplotMesh_l599ec4* mesh = model->data->meshes->meshes[i];
        base::math::UnkVector4D color(mesh->color);
        color.x = alpha;
        mesh->SetColor(color);
    }

    UnkObject* parts = self->owner->parts;
    if (!parts)
        return;
    UnkGooplotPart_l599ec4* part = reinterpret_cast<UnkGooplotPart_l599ec4*>(
        fn_7101152b90(reinterpret_cast<UnkWorld*>(parts), StringCrc64("gooplottail", 11)));
    if (part)
        part = fn_710114e680(parts, part->kind) ? part : nullptr;
    UnkGooplotMesh_l599ec4* mesh = part->data->meshes->meshes[0];
    base::math::UnkVector4D color(mesh->color);
    mesh->SetColor(color);
}
