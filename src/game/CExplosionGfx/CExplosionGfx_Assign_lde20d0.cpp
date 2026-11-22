// CExplosionGfx's unit: the explosion's copy assignment, as the implicit one does it: runs of plain members whole,
// colours and 4D vectors through their out-of-line assignments, the paths by reference, the rest member by member.
// Names and layouts are placeholders.
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
