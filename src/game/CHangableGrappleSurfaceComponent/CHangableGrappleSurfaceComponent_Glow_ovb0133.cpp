// CHangableGrappleSurfaceComponent's unit (tools/tu_map.py): fades the surface's glow material (key
// 0x117cd46281e7f67a) to its own colour with alpha `factor`, the same code as the magnet sliding
// block's fn_71009210d4 (src/game/CMagnetSlidingBlockComponent/CMagnetSlidingBlockComponent_Slide_ovb0086.cpp).
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/color/CColor4F.h"
#include "game/CMaterialFXComponent.h"
#include "game/UnkTU218.h"
#include "types.h"

using base::color::CColor4F;
using base::math::UnkVector4D;

struct UnkGlowMaterial_ovb0133 {
    u8 pad_0x0[0x14c];
    f32 r, g, b, a;  // 0x14c
};

struct UnkGlowMaterials_ovb0133 {
    u8 pad_0x0[0x48];
    UnkGlowMaterial_ovb0133** entries;  // 0x48
};

struct UnkGlowModelRes_ovb0133 {
    u8 pad_0x0[0x78];
    UnkGlowMaterials_ovb0133* materials;  // 0x78
};

struct UnkGlowModel_ovb0133 {
    u8 pad_0x0[0x138];
    UnkGlowModelRes_ovb0133* res;  // 0x138
};

struct UnkGlowModelComponent_ovb0133 {
    u8 pad_0x0[0x50];
    UnkGlowModel_ovb0133* model;  // 0x50
};

// A material colour change as fn_71009272c4 builds it (kind at +0, a string id at +0x38).
struct UnkGlowColorChange_ovb0133 {
    u32 kind;
    u8 pad_0x4[0x34];
    union {
        base::UnkStrIdMember id;  // 0x38
    };
    u8 pad_0x40[0x18];

    UnkGlowColorChange_ovb0133() {}  // built by fn_71009272c4
    ~UnkGlowColorChange_ovb0133() { id.~UnkStrIdMemberOf(); }
};

struct UnkMagnetColorChange_b0086;
struct UnkMagnetFx_b0086;
void fn_71009272c4(UnkMagnetColorChange_b0086* self, u32 index, bool flag, const UnkVector4D* from,
                   const UnkVector4D* to, f32 time);

struct UnkGlowOwner_ovb0133 {
    u8 pad_0x0[0x228];
    UnkGlowModelComponent_ovb0133* model;  // 0x228
    u8 pad_0x230[0x58];
    UnkMagnetFx_b0086* fx;  // 0x288
};

struct UnkGlowSurface_ovb0133 {
    u8 pad_0x0[0x30];
    UnkGlowOwner_ovb0133* owner;
};

struct UnkGrappleSurface_A5_b65;

static inline f32 NonNegative(f32 v) { return v <= 0.0f ? 0.0f : v; }

void fn_710082566c(UnkGrappleSurface_A5_b65* surface, f32 factor) {
    UnkGlowSurface_ovb0133* self = reinterpret_cast<UnkGlowSurface_ovb0133*>(surface);
    u64 key = 0x117cd46281e7f67a;
    UnkGlowModelComponent_ovb0133* model = self->owner->model;
    UnkMagnetFx_b0086* fx = self->owner->fx;
    if (!model || !fx)
        return;
    UnkGlowModel_ovb0133* m = model->model;
    if (!m)
        return;
    u32 index;
    if (!fn_7100199d88(m->res->materials, &key, &index))
        return;
    u32 i = index;
    UnkGlowMaterial_ovb0133* mat = m->res->materials->entries[i];
    CColor4F to(NonNegative(mat->r), NonNegative(mat->g), NonNegative(mat->b), NonNegative(mat->a));
    CColor4F from(NonNegative(mat->r), NonNegative(mat->g), NonNegative(mat->b), factor);
    UnkGlowColorChange_ovb0133 change;
    UnkVector4D vto = fn_7100030878(&to);
    UnkVector4D vfrom = fn_7100030878(&from);
    fn_71009272c4(reinterpret_cast<UnkMagnetColorChange_b0086*>(&change), i, false, &vto, &vfrom, 0.1f);
    change.kind = 2;
    fn_7100926aa0(fx, reinterpret_cast<UnkMagnetColorChange_b0086*>(&change));
}
