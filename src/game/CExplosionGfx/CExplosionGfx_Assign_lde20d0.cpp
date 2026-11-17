// CExplosionGfx's unit: the explosion's copy assignment, as the implicit one does it: runs of plain members whole,
// colours and 4D vectors through their out-of-line assignments, the paths by reference, the rest member by member.
// Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/global/CFilePathStrId.h"
#include "base/math/CVector4D.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CFilePathStrId;
using base::math::UnkVector4D;

// Plain words copied whole.
struct UnkWords2_de1c40 {
    u64 words[2];
};
struct UnkWords4_de1c40 {
    u64 words[4];
};

// The two-component vectors are copied member by member.
struct UnkPair_de1c40 {
    f32 x;
    f32 y;
};

struct UnkExplosionGfx_de1c40 {
    void* vtable;  // 0x0
    u64 field_0x8;  // 0x8
    u64 field_0x10;  // 0x10
    CColor4F color_0x18;  // 0x18
    CColor4F color_0x28;  // 0x28
    CColor4F color_0x38;  // 0x38
    f32 field_0x48;  // 0x48
    UnkVector4D oConstantColor0TransitionInit;  // 0x4c
    UnkVector4D oConstantColor0TransitionEnd;  // 0x5c
    UnkVector4D oConstantColor1TransitionInit;  // 0x6c
    UnkVector4D oConstantColor1TransitionEnd;  // 0x7c
    f32 fConstantColor0TransitionTime;  // 0x8c
    f32 fConstantColor1TransitionTime;  // 0x90
    s32 eConstant0TransitionMode;  // 0x94
    s32 eConstant1TransitionMode;  // 0x98
    CFilePathStrId sParticlesPath;  // 0xa0
    CFilePathStrId sAdditionalParticlesPath;  // 0xa8
    CFilePathStrId sMeshPath;  // 0xb0
    f32 fMainExplosionScale;  // 0xb8
    UnkPair_de1c40 vRandomScale;  // 0xbc
    f32 fRadius;  // 0xc4
    f32 fRadiusAmplitude;  // 0xc8
    f32 fRandomArcAmplitude;  // 0xcc
    u32 uAmount;  // 0xd0
    f32 fLength;  // 0xd4
    UnkPair_de1c40 vRandomDelay;  // 0xd8
    f32 values_0xe0[81];  // 0xe0
    UnkWords2_de1c40 words_0x228;  // 0x228
    UnkWords4_de1c40 words_0x238;  // 0x238
    UnkWords4_de1c40 words_0x258;  // 0x258
};
static_assert(__builtin_offsetof(UnkExplosionGfx_de1c40, sParticlesPath) == 0xa0, "layout");
static_assert(__builtin_offsetof(UnkExplosionGfx_de1c40, words_0x228) == 0x228, "layout");
static_assert(sizeof(UnkExplosionGfx_de1c40) == 0x278, "layout");

UnkExplosionGfx_de1c40* fn_7100de20d0(UnkExplosionGfx_de1c40* self, const UnkExplosionGfx_de1c40* other) {
    // field_0x8 and field_0x10.
    __builtin_memcpy(&self->field_0x8, &other->field_0x8, 0x10);
    // The colours by member call: the implicit assignment this is takes the destination's address
    // before the source's (`a = b` would take the source's first).
    self->color_0x18.operator=(other->color_0x18);
    self->color_0x28.operator=(other->color_0x28);
    self->color_0x38.operator=(other->color_0x38);
    self->field_0x48 = other->field_0x48;
    fn_710007b518(&self->oConstantColor0TransitionInit, &other->oConstantColor0TransitionInit);
    fn_710007b518(&self->oConstantColor0TransitionEnd, &other->oConstantColor0TransitionEnd);
    fn_710007b518(&self->oConstantColor1TransitionInit, &other->oConstantColor1TransitionInit);
    fn_710007b518(&self->oConstantColor1TransitionEnd, &other->oConstantColor1TransitionEnd);
    // The two times and the two transition modes.
    __builtin_memcpy(&self->fConstantColor0TransitionTime, &other->fConstantColor0TransitionTime, 0x10);
    self->sParticlesPath = other->sParticlesPath;
    self->sAdditionalParticlesPath = other->sAdditionalParticlesPath;
    self->sMeshPath = other->sMeshPath;
    self->fMainExplosionScale = other->fMainExplosionScale;
    self->vRandomScale.x = other->vRandomScale.x;
    self->vRandomScale.y = other->vRandomScale.y;
    // fRadius to fLength.
    __builtin_memcpy(&self->fRadius, &other->fRadius, 0x14);
    self->vRandomDelay.x = other->vRandomDelay.x;
    self->vRandomDelay.y = other->vRandomDelay.y;
    // The implicit assignment takes these one by one.
#pragma clang loop unroll(full)
    for (u32 i = 0; i < 81; i++)
        self->values_0xe0[i] = other->values_0xe0[i];
    self->words_0x228 = other->words_0x228;
    self->words_0x238 = other->words_0x238;
    self->words_0x258 = other->words_0x258;
    return self;
}
