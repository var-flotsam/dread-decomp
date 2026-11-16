// Unit 2599 (hint 'CEnvironmentData::SFogTransition'): a constructor of an environment data block of
// 0x3a8 bytes: twelve sections of settings, each followed by its transitions array (flags 0x53), and an
// empty string id. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::UnkStringData;

struct UnkTransition_f42880;

struct UnkEnvSection_0_f42880 {
    u32 field_0x0;
    f32 field_0x4 = 0.0f;
    f32 field_0x8 = 0.0f;
    f32 field_0xc = 0.0f;
    f32 field_0x10 = 0.0f;
    u8 pad_0x14[0xc];
    f32 field_0x20 = 0.0f;
    f32 field_0x24 = 0.0f;
    f32 field_0x28 = 0.0f;
    f32 field_0x2c = 0.0f;
    f32 field_0x30 = 0.0f;
    f32 field_0x34 = 0.0f;
    u8 pad_0x38[0x8];
    CRntVector<UnkTransition_f42880> transitions{0x53};
};
static_assert(sizeof(UnkEnvSection_0_f42880) == 0x68, "UnkEnvSection_0_f42880 layout");

struct UnkEnvSection_68_f42880 {
    u32 field_0x0;
    f32 field_0x4 = 0.0f;
    f32 field_0x8 = 0.0f;
    f32 field_0xc = 0.0f;
    f32 field_0x10 = 0.0f;
    u8 pad_0x14[0x4];
    CRntVector<UnkTransition_f42880> transitions{0x53};
};
static_assert(sizeof(UnkEnvSection_68_f42880) == 0x40, "UnkEnvSection_68_f42880 layout");

struct UnkEnvSection_a8_f42880 {
    u8 pad_0x0[0x14];
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    u8 pad_0x24[0x4];
    CRntVector<UnkTransition_f42880> transitions{0x53};
};
static_assert(sizeof(UnkEnvSection_a8_f42880) == 0x50, "UnkEnvSection_a8_f42880 layout");

struct UnkEnvSection_f8_f42880 {
    u32 field_0x0;
    f32 field_0x4 = 0.0f;
    f32 field_0x8 = 0.0f;
    f32 field_0xc = 0.0f;
    f32 field_0x10 = 0.0f;
    u8 pad_0x14[0xc];
    CRntVector<UnkTransition_f42880> transitions{0x53};
};
static_assert(sizeof(UnkEnvSection_f8_f42880) == 0x48, "UnkEnvSection_f8_f42880 layout");

struct UnkEnvSection_140_f42880 {
    u32 field_0x0;
    f32 field_0x4 = 0.0f;
    f32 field_0x8 = 0.0f;
    f32 field_0xc = 0.0f;
    f32 field_0x10 = 0.0f;
    f32 field_0x14 = 0.0f;
    f32 field_0x18 = 0.0f;
    f32 field_0x1c = 0.0f;
    f32 field_0x20 = 0.0f;
    f32 field_0x24 = 0.0f;
    f32 field_0x28 = 0.0f;
    f32 field_0x2c = 0.0f;
    f32 field_0x30 = 0.0f;
    u8 pad_0x34[0x4];
    f32 field_0x38 = 0.0f;
    f32 field_0x3c = 0.0f;
    CRntVector<UnkTransition_f42880> transitions{0x53};
};
static_assert(sizeof(UnkEnvSection_140_f42880) == 0x68, "UnkEnvSection_140_f42880 layout");

struct UnkEnvSection_1a8_f42880 {
    u32 field_0x0;
    f32 field_0x4 = 0.0f;
    f32 field_0x8 = 0.0f;
    f32 field_0xc = 0.0f;
    u32 field_0x10;
    f32 field_0x14 = 0.0f;
    f32 field_0x18 = 0.0f;
    f32 field_0x1c = 0.0f;
    CRntVector<UnkTransition_f42880> transitions{0x53};
};
static_assert(sizeof(UnkEnvSection_1a8_f42880) == 0x48, "UnkEnvSection_1a8_f42880 layout");

struct UnkEnvSection_1f0_f42880 {
    u32 field_0x0;
    f32 field_0x4 = 0.0f;
    f32 field_0x8 = 0.0f;
    f32 field_0xc = 0.0f;
    CRntVector<UnkTransition_f42880> transitions{0x53};
};
static_assert(sizeof(UnkEnvSection_1f0_f42880) == 0x38, "UnkEnvSection_1f0_f42880 layout");

struct UnkEnvSection_228_f42880 {
    u32 field_0x0;
    f32 field_0x4 = 0.0f;
    f32 field_0x8 = 0.0f;
    f32 field_0xc = 0.0f;
    f32 field_0x10 = 0.0f;
    u8 pad_0x14[0xc];
    f32 field_0x20 = 0.0f;
    f32 field_0x24 = 0.0f;
    u32 field_0x28;
    f32 field_0x2c = 0.0f;
    f32 field_0x30 = 0.0f;
    f32 field_0x34 = 0.0f;
    f32 field_0x38 = 0.0f;
    u8 pad_0x3c[0x4];
    f32 field_0x40 = 0.0f;
    f32 field_0x44 = 1.0f;
    f32 field_0x48 = 0.0f;
    f32 field_0x4c = 1.0f;
    CRntVector<UnkTransition_f42880> transitions{0x53};
};
static_assert(sizeof(UnkEnvSection_228_f42880) == 0x78, "UnkEnvSection_228_f42880 layout");

struct UnkEnvSection_2a0_f42880 {
    u8 pad_0x0[0x8];
    CFilePathStrId field_0x8;
    CRntVector<UnkTransition_f42880> transitions{0x53};
};
static_assert(sizeof(UnkEnvSection_2a0_f42880) == 0x38, "UnkEnvSection_2a0_f42880 layout");

struct UnkEnvSection_2d8_f42880 {
    f32 field_0x0 = 1.0f;
    f32 field_0x4 = 0.5f;
    f32 field_0x8 = 0.5f;
    u8 pad_0xc[0x4];
    CRntVector<UnkTransition_f42880> transitions{0x53};
};
static_assert(sizeof(UnkEnvSection_2d8_f42880) == 0x38, "UnkEnvSection_2d8_f42880 layout");

struct UnkEnvSection_310_f42880 {
    f32 field_0x0 = 1.0f;
    f32 field_0x4 = 0.0f;
    f32 field_0x8 = 0.0f;
    f32 field_0xc = 0.0f;
    f32 field_0x10 = 0.0f;
    f32 field_0x14 = 0.0f;
    f32 field_0x18 = 1.0f;
    f32 field_0x1c = 1.0f;
    f32 field_0x20 = 1.0f;
    f32 field_0x24 = 1.0f;
    u32 field_0x28 = 0;
    bool field_0x2c = false;
    CRntVector<UnkTransition_f42880> transitions{0x53};
};
static_assert(sizeof(UnkEnvSection_310_f42880) == 0x58, "UnkEnvSection_310_f42880 layout");

struct UnkEnvSection_368_f42880 {
    f32 field_0x0 = 1.0f;
    f32 field_0x4 = 1.0f;
    f32 field_0x8 = 0.0f;
    f32 field_0xc = 0.0f;
    CRntVector<UnkTransition_f42880> transitions{0x53};
};
static_assert(sizeof(UnkEnvSection_368_f42880) == 0x38, "UnkEnvSection_368_f42880 layout");

struct UnkEnvData_f42880 {
    UnkEnvSection_0_f42880 field_0x0;
    UnkEnvSection_68_f42880 field_0x68;
    UnkEnvSection_a8_f42880 field_0xa8;
    UnkEnvSection_f8_f42880 field_0xf8;
    UnkEnvSection_140_f42880 field_0x140;
    UnkEnvSection_1a8_f42880 field_0x1a8;
    UnkEnvSection_1f0_f42880 field_0x1f0;
    UnkEnvSection_228_f42880 field_0x228;
    UnkEnvSection_2a0_f42880 field_0x2a0;
    UnkEnvSection_2d8_f42880 field_0x2d8;
    UnkEnvSection_310_f42880 field_0x310;
    UnkEnvSection_368_f42880 field_0x368;
    UnkStringData* field_0x3a0;

    UnkEnvData_f42880() asm("fn_7100f42880");
};
static_assert(sizeof(UnkEnvData_f42880) == 0x3a8, "UnkEnvData_f42880 layout");
static_assert(__builtin_offsetof(UnkEnvData_f42880, field_0x3a0) == 0x3a0, "UnkEnvData_f42880 layout");

UnkEnvData_f42880::UnkEnvData_f42880() {
    field_0x3a0 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    field_0x3a0 = &g_unk_7101d47108->empty;
    field_0xa8.field_0x14 = 1.0f;
    field_0xa8.field_0x18 = 1.0f;
    field_0xa8.field_0x1c = 1000.0f;
    field_0xa8.field_0x20 = 1000.0f;
}
