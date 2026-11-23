// CExplosionGfx's unit: the explosion's copy constructor: its vtable, then every member copied as the implicit
// copy does it (runs of plain members whole, vectors and colours member by member, the paths with a reference
// each). Names and layouts are placeholders.
#include <new>

#include "base/color/CColor4F.h"
#include "base/global/CFilePathStrId.h"
#include "base/math/CVector4D.h"
#include "game/UnkExplosionGfx_de1c40.h"
#include "game/UnkPair_de1c40.h"
#include "game/UnkWords2_de1c40.h"
#include "game/UnkWords4_de1c40.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CFilePathStrId;
using base::math::UnkVector4D;

extern char g_unk_7101aa9988[];

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
