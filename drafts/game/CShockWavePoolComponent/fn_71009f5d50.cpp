// CShockWavePoolComponent's unit (tools/tu_map.py, unit 1506): the copy-assignment of a shock wave's
// settings: the damage block, words, string ids, file paths, a string, a handle, vectors and a segment.
// Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/CCentralUnitCannonBeamMovementComponent.h"
#include "game/malloc.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

// Not a plain struct to the ABI (it has a constructor), so a copy leaves its tail padding alone.
struct UnkShockWaveBlock_l9f5d50 {
    UnkShockWaveBlock_l9f5d50();
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    bool field_0x1c;
};

struct UnkShockWavePair_l9f5d50 {
    u32 field_0x0;
    u32 field_0x4;
};

struct UnkShockWaveRange_l9f5d50 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

// The same: a copy leaves its tail padding alone.
struct UnkShockWaveFlags_l9f5d50 {
    UnkShockWaveFlags_l9f5d50();
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    bool field_0x10;
};

struct UnkShockWaveDef_l9f5d50 {
    void** vtable;
    u8 field_0x8[0x100];  // the damage block (fn_7100400364 copies it)
    u16 field_0x108;
    u32 field_0x10c;
    u32 field_0x110;
    u32 field_0x114;
    UnkShockWaveBlock_l9f5d50 field_0x118;
    CStrId field_0x138;
    u16 field_0x140;
    CFilePathStrId field_0x148;
    CFilePathStrId field_0x150;
    CRntString field_0x158;
    base::UnkRefHandle field_0x188;
    bool field_0x190;
    base::math::CVector4D field_0x194;
    UnkShockWavePair_l9f5d50 field_0x1a4;
    CStrId field_0x1b0;
    UnkShockWaveRange_l9f5d50 field_0x1b8;
    CFilePathStrId field_0x1d8;
    u32 field_0x1e0;
    u32 field_0x1e4;
    u32 field_0x1e8;
    u32 field_0x1ec;
    u32 field_0x1f0;
    u32 field_0x1f4;
    u32 field_0x1f8;
    UnkShockWaveFlags_l9f5d50 field_0x1fc;
    u8 field_0x210[0x14];  // a 2D segment (fn_7100125b44 copies it)
    u32 field_0x224;
};
static_assert(__builtin_offsetof(UnkShockWaveDef_l9f5d50, field_0x138) == 0x138, "layout");
static_assert(__builtin_offsetof(UnkShockWaveDef_l9f5d50, field_0x158) == 0x158, "layout");
static_assert(__builtin_offsetof(UnkShockWaveDef_l9f5d50, field_0x194) == 0x194, "layout");
static_assert(__builtin_offsetof(UnkShockWaveDef_l9f5d50, field_0x1b8) == 0x1b8, "layout");
static_assert(__builtin_offsetof(UnkShockWaveDef_l9f5d50, field_0x1fc) == 0x1fc, "layout");
static_assert(__builtin_offsetof(UnkShockWaveDef_l9f5d50, field_0x224) == 0x224, "layout");

UnkShockWaveDef_l9f5d50* fn_71009f5d50(UnkShockWaveDef_l9f5d50* self, const UnkShockWaveDef_l9f5d50* other) {
    fn_7100400364(reinterpret_cast<UnkWeaponDamage_ovb0054*>(self->field_0x8),
                  reinterpret_cast<const UnkWeaponDamage_ovb0054*>(other->field_0x8));
    self->field_0x108 = other->field_0x108;
    self->field_0x10c = other->field_0x10c;
    self->field_0x110 = other->field_0x110;
    self->field_0x114 = other->field_0x114;
    self->field_0x118 = other->field_0x118;
    self->field_0x138 = other->field_0x138;
    self->field_0x140 = other->field_0x140;
    self->field_0x148 = other->field_0x148;
    self->field_0x150 = other->field_0x150;
    CRntString_Assign(&self->field_0x158, &other->field_0x158);
    fn_710031d208(&self->field_0x188, &other->field_0x188);
    self->field_0x190 = other->field_0x190;
    fn_710007b518(&self->field_0x194, &other->field_0x194);
    self->field_0x1a4 = other->field_0x1a4;
    self->field_0x1b0 = other->field_0x1b0;
    self->field_0x1b8 = other->field_0x1b8;
    self->field_0x1d8 = other->field_0x1d8;
    self->field_0x1e0 = other->field_0x1e0;
    self->field_0x1e4 = other->field_0x1e4;
    self->field_0x1e8 = other->field_0x1e8;
    self->field_0x1ec = other->field_0x1ec;
    self->field_0x1f0 = other->field_0x1f0;
    self->field_0x1f4 = other->field_0x1f4;
    self->field_0x1f8 = other->field_0x1f8;
    self->field_0x1fc = other->field_0x1fc;
    fn_7100125b44(reinterpret_cast<UnkSegment2D*>(self->field_0x210),
                  reinterpret_cast<const UnkSegment2D*>(other->field_0x210));
    self->field_0x224 = other->field_0x224;
    return self;
}
