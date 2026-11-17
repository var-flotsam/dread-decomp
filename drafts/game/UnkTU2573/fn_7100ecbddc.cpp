// Unit 2573: a shot's constructor (the objects CTunableShotManager's resize fn_7100ede9e4 builds): default
// values, two empty vectors (the first reserved for five entries), the identity transform, five parts and
// the liquid-splash sound preset as its path. Names and layouts are placeholders.
#include <new>

#include "base/UnkMatrix44.h"
#include "base/UnkNamedRegistry.h"
#include "base/UnkPathIdsFromText.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU25.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

extern void* g_unk_7101ca9fe0[];

struct UnkObj290_b43a3;
struct UnkGunShot_ovb0070;
void fn_710085c338(base::global::CRntVector<UnkGunShot_ovb0070>* shots, u32 n);

struct UnkShotRecord18_lecbddc {
    UnkRefObject* ref;
    u8 pad_0x8[0x10];
};

struct UnkShotRecord10_lecbddc {
    UnkRefObject* ref;
    u8 pad_0x8[0x8];
};

// One of the shot's five parts: a position, four factors starting at one, and a flag.
struct UnkShotPart_lecbddc {
    f32 x;
    f32 y;
    f32 z;
    f32 factors[4];  // 0xc
    u32 field_0x1c;  // not set by the constructor
    u32 field_0x20;
    bool field_0x24;

    UnkShotPart_lecbddc()
        : x(0.0f), y(0.0f), z(0.0f), factors{1.0f, 1.0f, 1.0f, 1.0f}, field_0x20(0), field_0x24(false) {}
};
static_assert(sizeof(UnkShotPart_lecbddc) == 0x28, "placeholder layout");

struct UnkShot_lecbddc {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    f32 field_0x10;
    u8 pad_0x14[0x14];
    UnkRefObject* field_0x28;
    UnkRefObject* field_0x30;
    s32 field_0x38;
    u32 field_0x3c[11];
    UnkRefObject* field_0x68;
    UnkRefObject* field_0x70;
    UnkRefObject* field_0x78;
    u8 pad_0x80[0xc];
    u32 field_0x8c;
    u8 pad_0x90[0x4];
    f32 field_0x94[24];
    u8 pad_0xf4[0x4];
    CRntVector<UnkShotRecord18_lecbddc> field_0xf8;
    CRntVector<UnkShotRecord10_lecbddc> field_0x120;
    u32 field_0x148[3];
    u8 pad_0x154[0x4];
    UnkStrIdPathResource path;  // 0x158
    u32 field_0x170;
    u32 field_0x174;
    u8 pad_0x178[0x4];
    UnkMatrix44 transform;  // 0x17c
    bool field_0x1bc;
    u32 field_0x1c0;
    UnkShotPart_lecbddc parts[5];  // 0x1c4
    bool field_0x28c;
};
static_assert(__builtin_offsetof(UnkShot_lecbddc, field_0xf8) == 0xf8, "placeholder layout");
static_assert(__builtin_offsetof(UnkShot_lecbddc, path) == 0x158, "placeholder layout");
static_assert(__builtin_offsetof(UnkShot_lecbddc, transform) == 0x17c, "placeholder layout");
static_assert(__builtin_offsetof(UnkShot_lecbddc, parts) == 0x1c4, "placeholder layout");
static_assert(sizeof(UnkShot_lecbddc) == 0x290, "placeholder layout");

void fn_7100ecbddc(UnkObj290_b43a3* shot) {
    UnkShot_lecbddc* self = reinterpret_cast<UnkShot_lecbddc*>(shot);
    self->vtable = &g_unk_7101ca9fe0[2];
    self->field_0x8 = 6;
    self->field_0xc = 0;
    self->field_0x10 = 3.0f;
    self->field_0x28 = nullptr;
    self->field_0x30 = nullptr;
    self->field_0x38 = -1;
    self->field_0x3c[0] = 0;
    self->field_0x3c[1] = 0;
    self->field_0x3c[2] = 0;
    self->field_0x3c[3] = 0;
    self->field_0x3c[4] = 0;
    self->field_0x3c[5] = 0;
    self->field_0x3c[6] = 0;
    self->field_0x3c[7] = 0;
    self->field_0x3c[8] = 0;
    self->field_0x3c[9] = 0;
    self->field_0x3c[10] = 0;
    self->field_0x68 = nullptr;
    self->field_0x70 = nullptr;
    self->field_0x78 = nullptr;
    self->field_0x8c = 0;
    for (u32 i = 0; i < 24; ++i)
        self->field_0x94[i] = 0.0f;
    new (&self->field_0xf8) CRntVector<UnkShotRecord18_lecbddc>(0xf);
    fn_710085c338(reinterpret_cast<CRntVector<UnkGunShot_ovb0070>*>(&self->field_0xf8), 5);
    new (&self->field_0x120) CRntVector<UnkShotRecord10_lecbddc>(0xf);
    self->field_0x148[0] = 0;
    self->field_0x148[1] = 0;
    self->field_0x148[2] = 0;
    new (&self->path.ids.id) CStrId(g_unk_7101d470e0);
    new (&self->path.ids.path) base::global::CFilePathStrId(g_unk_7101d470e8);
    self->path.resource = nullptr;
    self->field_0x170 = 0;
    self->field_0x174 = 0;
    UnkCopyMatrix(&self->transform, g_unk_7101a1a680);
    self->field_0x1bc = false;
    self->field_0x1c0 = 0;
    for (u32 i = 0; i < 5; ++i)
        new (&self->parts[i]) UnkShotPart_lecbddc();
    self->field_0x28c = false;
    // The two axes inside the block at 0x94.
    self->field_0x94[9] = 1.0f;
    self->field_0x94[10] = 0.0f;
    self->field_0x94[11] = 0.0f;
    self->field_0x94[12] = 0.0f;
    self->field_0x94[13] = 1.0f;
    self->field_0x94[14] = 0.0f;

    base::UnkNamedEntry text;
    fn_710007e704(&text, "audiopreset:system/snd/presets/weapons/common/liquidsplash");
    fn_71000267f8(&self->path,
                  UnkPathIdsFromText(*reinterpret_cast<const base::global::CRntString*>(&text)).ids);
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&text));
}
