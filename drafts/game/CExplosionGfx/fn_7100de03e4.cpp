// CExplosionGfx's unit: the explosion's constructor: its vtable, white colours, cleared transitions, three
// empty paths, the scale and amount defaults, ten cleared entries; then the colours set to the unit vector
// (clamped) and a pointer to each entry kept. Names and layouts are placeholders.
#include <new>

#include "base/color/CColor4F.h"
#include "base/global/CFilePathStrId.h"
#include "base/math/CVector4D.h"
#include "base/math/UnkOneVector3.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CFilePathStrId;
using base::math::CVector3D;
using base::math::UnkVector4D;

extern char g_unk_7101aa9988[];

struct UnkExplosionEntry_de03e4 {
    u32 field_0x0;
    CVector3D field_0x4;
    CVector3D field_0x10;
    u32 field_0x1c;
};

struct UnkExplosionGfx_de03e4 {
    void* vtable;  // 0x0
    CVector3D field_0x8;  // 0x8
    u32 pad_0x14;
    CColor4F colors[3];  // 0x18
    f32 field_0x48;  // 0x48
    UnkVector4D transitions[4];  // 0x4c
    f32 field_0x8c;
    f32 field_0x90;
    s32 field_0x94;
    s32 field_0x98;
    CFilePathStrId sParticlesPath;  // 0xa0
    CFilePathStrId sAdditionalParticlesPath;  // 0xa8
    CFilePathStrId sMeshPath;  // 0xb0
    f32 fMainExplosionScale;  // 0xb8
    f32 vRandomScale[2];  // 0xbc
    f32 fRadius;  // 0xc4
    f32 fRadiusAmplitude;  // 0xc8
    f32 fRandomArcAmplitude;  // 0xcc
    u32 uAmount;  // 0xd0
    f32 fLength;  // 0xd4
    f32 vRandomDelay[2];  // 0xd8
    u32 field_0xe0;  // 0xe0
    UnkExplosionEntry_de03e4 entries[10];  // 0xe4
    u32 pad_0x224;
    UnkExplosionEntry_de03e4* entry_ptrs[10];  // 0x228
};
static_assert(__builtin_offsetof(UnkExplosionGfx_de03e4, sParticlesPath) == 0xa0, "layout");
static_assert(__builtin_offsetof(UnkExplosionGfx_de03e4, entries) == 0xe4, "layout");
static_assert(sizeof(UnkExplosionGfx_de03e4) == 0x278, "layout");

static inline f32 ClampPositive(f32 x) { return x <= 0.0f ? 0.0f : x; }

// Sets a colour to a vector, opaque, each component clamped to be non-negative.
static inline void SetColor(CColor4F& color, const CVector3D& v) {
    CColor4F clamped(ClampPositive(v.x), ClampPositive(v.y), ClampPositive(v.z), 1.0f);
    color = clamped;
}

void fn_7100de03e4(UnkExplosionGfx_de03e4* self) {
    self->vtable = g_unk_7101aa9988 + 0x10;
    for (int i = 0; i < 3; ++i)
        self->colors[i].Set(1.0f, 1.0f, 1.0f, 1.0f);
    for (int i = 0; i < 4; ++i)
        new (&self->transitions[i]) UnkVector4D();
    new (&self->sParticlesPath) CFilePathStrId();
    new (&self->sAdditionalParticlesPath) CFilePathStrId();
    new (&self->sMeshPath) CFilePathStrId();
    self->fMainExplosionScale = 1.0f;
    self->vRandomScale[0] = 0.0f;
    self->vRandomScale[1] = 0.0f;
    self->fRadius = 0.0f;
    self->fRadiusAmplitude = 0.0f;
    self->uAmount = 1;
    self->fLength = -1.0f;
    self->vRandomDelay[0] = 0.0f;
    self->vRandomDelay[1] = 0.0f;
    self->field_0xe0 = 0;
    for (int i = 0; i < 10; ++i) {
        UnkExplosionEntry_de03e4& entry = self->entries[i];
        entry.field_0x4.x = 0.0f;
        entry.field_0x4.y = 0.0f;
        entry.field_0x4.z = 0.0f;
        entry.field_0x10.x = 0.0f;
        entry.field_0x10.y = 0.0f;
        entry.field_0x10.z = 0.0f;
    }
    // The scale.
    self->field_0x8.x = 1.0f;
    self->field_0x8.y = 1.0f;
    self->field_0x8.z = 1.0f;
    for (int i = 0; i < 3; ++i)
        SetColor(self->colors[i], g_unk_71016477a8);
    for (int i = 0; i < 10; ++i)
        self->entry_ptrs[i] = &self->entries[i];
}
