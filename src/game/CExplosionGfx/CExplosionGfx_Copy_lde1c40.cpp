// CExplosionGfx's unit: the explosion's copy constructor: its vtable, then every member copied as the implicit
// copy does it (runs of plain members whole, vectors and colours member by member, the paths with a reference
// each). Names and layouts are placeholders.
#include <new>

#include "base/color/CColor4F.h"
#include "base/global/CFilePathStrId.h"
#include "base/math/CVector4D.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CFilePathStrId;
using base::math::UnkVector4D;

extern char g_unk_7101aa9988[];

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

void fn_7100de1c40(UnkExplosionGfx_de1c40* self, const UnkExplosionGfx_de1c40* other) {
    self->vtable = g_unk_7101aa9988 + 0x10;
    // field_0x8 and field_0x10.
    __builtin_memcpy(&self->field_0x8, &other->field_0x8, 0x10);
    new (&self->color_0x18) CColor4F(other->color_0x18);
    new (&self->color_0x28) CColor4F(other->color_0x28);
    new (&self->color_0x38) CColor4F(other->color_0x38);
    self->field_0x48 = other->field_0x48;
    new (&self->oConstantColor0TransitionInit) UnkVector4D(other->oConstantColor0TransitionInit);
    new (&self->oConstantColor0TransitionEnd) UnkVector4D(other->oConstantColor0TransitionEnd);
    new (&self->oConstantColor1TransitionInit) UnkVector4D(other->oConstantColor1TransitionInit);
    new (&self->oConstantColor1TransitionEnd) UnkVector4D(other->oConstantColor1TransitionEnd);
    // The two times and the two transition modes.
    __builtin_memcpy(&self->fConstantColor0TransitionTime, &other->fConstantColor0TransitionTime, 0x10);
    new (&self->sParticlesPath) CFilePathStrId(other->sParticlesPath);
    new (&self->sAdditionalParticlesPath) CFilePathStrId(other->sAdditionalParticlesPath);
    new (&self->sMeshPath) CFilePathStrId(other->sMeshPath);
    self->fMainExplosionScale = other->fMainExplosionScale;
    self->vRandomScale.x = other->vRandomScale.x;
    self->vRandomScale.y = other->vRandomScale.y;
    // fRadius to fLength.
    __builtin_memcpy(&self->fRadius, &other->fRadius, 0x14);
    self->vRandomDelay.x = other->vRandomDelay.x;
    self->vRandomDelay.y = other->vRandomDelay.y;
    // The implicit copy takes these one by one.
#pragma clang loop unroll(full)
    for (u32 i = 0; i < 81; i++)
        self->values_0xe0[i] = other->values_0xe0[i];
    self->words_0x228 = other->words_0x228;
    self->words_0x238 = other->words_0x238;
    self->words_0x258 = other->words_0x258;
}
