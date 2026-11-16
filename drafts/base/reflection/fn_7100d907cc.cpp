// Unit 2525 (base::reflection): the copy-assignment of the environment state (0x450 bytes, the
// environment manager's +0x28), member by member. Names and layouts are placeholders.
#include "base/UnkResKey.h"
#include "base/global/CFilePathStrId.h"
#include "base/math/CVector4D.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::math::CVector4D;

struct UnkEnvPair_d907cc {
    f32 v[2];
};
struct UnkEnvTriple_d907cc {
    f32 v[3];
};
struct UnkEnvBlock20_d907cc {
    u32 v[8];
};
struct UnkEnvBlock58_d907cc {
    u64 v[11];
};
struct UnkEnvBlock24_d907cc {
    u32 v[9];
};
struct UnkEnvBytes_d907cc {
    u8 bytes[0x13d];
};

struct UnkEnvState_d907cc {
    CVector4D field_0x0;
    CVector4D field_0x10;
    CVector4D field_0x20;
    UnkEnvTriple_d907cc field_0x30;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    CVector4D field_0x54;
    CVector4D field_0x64;
    CVector4D field_0x74;
    CVector4D field_0x84;
    CVector4D field_0x94;
    UnkEnvBlock20_d907cc field_0xa4;
    UnkEnvBlock20_d907cc field_0xc4;
    CVector4D field_0xe4;
    CVector4D field_0xf4;
    UnkEnvPair_d907cc field_0x104;
    CVector4D field_0x10c;
    CVector4D field_0x11c;
    CVector4D field_0x12c;
    CVector4D field_0x13c;
    CVector4D field_0x14c;
    CVector4D field_0x15c;
    u32 field_0x16c;
    u32 field_0x170;
    u32 field_0x174;
    u32 field_0x178;
    u32 field_0x17c;
    u32 field_0x180;
    u32 field_0x184;
    u32 field_0x188;
    u32 field_0x18c;
    u32 field_0x190;
    u32 field_0x194;
    u32 field_0x198;
    u32 field_0x19c;
    u32 field_0x1a0;
    u32 field_0x1a4;
    u32 field_0x1a8;
    u32 field_0x1ac;
    u32 field_0x1b0;
    u32 field_0x1b4;
    u32 field_0x1b8;
    u32 field_0x1bc;
    u32 field_0x1c0;
    u32 field_0x1c4;
    u32 field_0x1c8;
    u32 field_0x1cc;
    bool field_0x1d0;
    CVector4D field_0x1d4;
    CVector4D field_0x1e4;
    CVector4D field_0x1f4;
    UnkEnvBlock24_d907cc field_0x204;
    u32 field_0x228;
    u32 field_0x22c;
    u32 field_0x230;
    u32 field_0x234;
    u32 field_0x238;
    u32 field_0x23c;
    CVector4D field_0x240;
    CVector4D field_0x250;
    CVector4D field_0x260;
    u16 field_0x270;
    CFilePathStrId path;  // 0x278
    UnkResRef resource;   // 0x280
    UnkEnvBlock58_d907cc field_0x288;
    CVector4D field_0x2e0;
    CVector4D field_0x2f0;
    CVector4D field_0x300;
    UnkEnvBytes_d907cc field_0x310;
};
static_assert(__builtin_offsetof(UnkEnvState_d907cc, field_0x1d4) == 0x1d4, "layout");
static_assert(__builtin_offsetof(UnkEnvState_d907cc, path) == 0x278, "layout");
static_assert(__builtin_offsetof(UnkEnvState_d907cc, field_0x310) == 0x310, "layout");
static_assert(sizeof(UnkEnvState_d907cc) == 0x450, "layout");

UnkEnvState_d907cc* fn_7100d907cc(UnkEnvState_d907cc* self, const UnkEnvState_d907cc* other) {
    fn_710007b518(&self->field_0x0, &other->field_0x0);
    fn_710007b518(&self->field_0x10, &other->field_0x10);
    fn_710007b518(&self->field_0x20, &other->field_0x20);
    self->field_0x30 = other->field_0x30;
    self->field_0x3c = other->field_0x3c;
    self->field_0x40 = other->field_0x40;
    self->field_0x44 = other->field_0x44;
    self->field_0x48 = other->field_0x48;
    self->field_0x4c = other->field_0x4c;
    self->field_0x50 = other->field_0x50;
    fn_710007b518(&self->field_0x54, &other->field_0x54);
    fn_710007b518(&self->field_0x64, &other->field_0x64);
    fn_710007b518(&self->field_0x74, &other->field_0x74);
    fn_710007b518(&self->field_0x84, &other->field_0x84);
    fn_710007b518(&self->field_0x94, &other->field_0x94);
    self->field_0xa4 = other->field_0xa4;
    self->field_0xc4 = other->field_0xc4;
    fn_710007b518(&self->field_0xe4, &other->field_0xe4);
    fn_710007b518(&self->field_0xf4, &other->field_0xf4);
    self->field_0x104 = other->field_0x104;
    fn_710007b518(&self->field_0x10c, &other->field_0x10c);
    fn_710007b518(&self->field_0x11c, &other->field_0x11c);
    fn_710007b518(&self->field_0x12c, &other->field_0x12c);
    fn_710007b518(&self->field_0x13c, &other->field_0x13c);
    fn_710007b518(&self->field_0x14c, &other->field_0x14c);
    fn_710007b518(&self->field_0x15c, &other->field_0x15c);
    self->field_0x16c = other->field_0x16c;
    self->field_0x170 = other->field_0x170;
    self->field_0x174 = other->field_0x174;
    self->field_0x178 = other->field_0x178;
    self->field_0x17c = other->field_0x17c;
    self->field_0x180 = other->field_0x180;
    self->field_0x184 = other->field_0x184;
    self->field_0x188 = other->field_0x188;
    self->field_0x18c = other->field_0x18c;
    self->field_0x190 = other->field_0x190;
    self->field_0x194 = other->field_0x194;
    self->field_0x198 = other->field_0x198;
    self->field_0x19c = other->field_0x19c;
    self->field_0x1a0 = other->field_0x1a0;
    self->field_0x1a4 = other->field_0x1a4;
    self->field_0x1a8 = other->field_0x1a8;
    self->field_0x1ac = other->field_0x1ac;
    self->field_0x1b0 = other->field_0x1b0;
    self->field_0x1b4 = other->field_0x1b4;
    self->field_0x1b8 = other->field_0x1b8;
    self->field_0x1bc = other->field_0x1bc;
    self->field_0x1c0 = other->field_0x1c0;
    self->field_0x1c4 = other->field_0x1c4;
    self->field_0x1c8 = other->field_0x1c8;
    self->field_0x1cc = other->field_0x1cc;
    self->field_0x1d0 = other->field_0x1d0;
    fn_710007b518(&self->field_0x1d4, &other->field_0x1d4);
    fn_710007b518(&self->field_0x1e4, &other->field_0x1e4);
    fn_710007b518(&self->field_0x1f4, &other->field_0x1f4);
    self->field_0x204 = other->field_0x204;
    self->field_0x228 = other->field_0x228;
    self->field_0x22c = other->field_0x22c;
    self->field_0x230 = other->field_0x230;
    self->field_0x234 = other->field_0x234;
    self->field_0x238 = other->field_0x238;
    self->field_0x23c = other->field_0x23c;
    fn_710007b518(&self->field_0x240, &other->field_0x240);
    fn_710007b518(&self->field_0x250, &other->field_0x250);
    fn_710007b518(&self->field_0x260, &other->field_0x260);
    self->field_0x270 = other->field_0x270;
    self->path = other->path;
    self->resource = other->resource;
    self->field_0x288 = other->field_0x288;
    fn_710007b518(&self->field_0x2e0, &other->field_0x2e0);
    fn_710007b518(&self->field_0x2f0, &other->field_0x2f0);
    fn_710007b518(&self->field_0x300, &other->field_0x300);
    self->field_0x310 = other->field_0x310;
    return self;
}
