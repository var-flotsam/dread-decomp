// CShockWavePoolComponent's unit (tools/tu_map.py): fn_71009f5730 constructs the member at +0x40: its vtable,
// the damage request at +0x8 (fn_71008c0c74), cleared values and defaults, empty string, file-path and
// runtime string ids (the last file-path id with its constructor inlined) and a string id built from "shockwavepool". Names and layouts are placeholders.
#include <new>

#include "base/UnkNamedRegistry.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU1370.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

void fn_71009f5730(char* self);

extern void* g_unk_7101a664a0[];

// Two pairs of floats, (1, 0) and (0, 1) by default.
struct UnkPair2_l9f5730 {
    f32 x0;
    f32 y0;
    f32 x1;
    f32 y1;
};

struct UnkShockWavePool_l9f5730 {
    void** vtable;
    u8 request[0x100];  // 0x8, built by fn_71008c0c74
    bool field_0x108;
    bool field_0x109;
    f32 field_0x10c;
    f32 field_0x110;
    f32 field_0x114;
    bool field_0x118;
    f32 field_0x11c;
    f32 field_0x120;
    f32 field_0x124;
    f32 field_0x128;
    f32 field_0x12c;
    f32 field_0x130;
    bool field_0x134;
    CStrId field_0x138;
    bool field_0x140;
    bool field_0x141;
    CFilePathStrId field_0x148;
    CFilePathStrId field_0x150;
    base::UnkNamedEntry field_0x158;
    u64 field_0x188;
    bool field_0x190;
    UnkPair2_l9f5730 field_0x194;
    bool field_0x1a4;
    u32 field_0x1a8;
    CStrId field_0x1b0;
    bool field_0x1b8;
    u8 pad_0x1b9[7];
    bool field_0x1c0;
    f32 field_0x1c4;
    f32 field_0x1c8;
    f32 field_0x1cc;
    f32 field_0x1d0;
    f32 field_0x1d4;
    CFilePathStrId field_0x1d8;
    u8 pad_0x1e0[4];
    f32 field_0x1e4;
    f32 field_0x1e8;
    f32 field_0x1ec;
    f32 field_0x1f0;
    f32 field_0x1f4;
    f32 field_0x1f8;
    bool field_0x1fc;
    f32 field_0x200;
    bool field_0x204;
    f32 field_0x208;
    bool field_0x20c;
    u64 field_0x210;
    u64 field_0x218;
    f32 field_0x220;
};
static_assert(__builtin_offsetof(UnkShockWavePool_l9f5730, field_0x1c0) == 0x1c0, "layout");
static_assert(__builtin_offsetof(UnkShockWavePool_l9f5730, field_0x1e4) == 0x1e4, "layout");
static_assert(__builtin_offsetof(UnkShockWavePool_l9f5730, field_0x220) == 0x220, "layout");

void fn_71009f5730(char* p) {
    UnkShockWavePool_l9f5730* self = reinterpret_cast<UnkShockWavePool_l9f5730*>(p);
    self->vtable = &g_unk_7101a664a0[2];
    fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(self->request));
    self->field_0x108 = false;
    self->field_0x109 = false;
    self->field_0x10c = 0.0f;
    self->field_0x110 = 0.0f;
    self->field_0x114 = 0.0f;
    self->field_0x118 = false;
    self->field_0x11c = 0.0f;
    self->field_0x120 = 20.0f;
    self->field_0x124 = 0.0f;
    self->field_0x128 = 0.0f;
    self->field_0x12c = 0.0f;
    self->field_0x130 = 0.0f;
    self->field_0x134 = false;
    new (&self->field_0x138) CStrId(g_unk_710162e2ed, true);
    self->field_0x140 = false;
    self->field_0x141 = false;
    new (&self->field_0x148) CFilePathStrId(g_unk_710162e2ed, true);
    new (&self->field_0x150) CFilePathStrId(g_unk_710162e2ed, true);
    CRntString_ConstructFromCString(&self->field_0x158, g_unk_710162e2ed);
    self->field_0x188 = 0;
    self->field_0x190 = false;
    self->field_0x194 = UnkPair2_l9f5730{1.0f, 0.0f, 0.0f, 1.0f};
    self->field_0x1a4 = false;
    self->field_0x1a8 = 0;
    new (&self->field_0x1b0) CStrId("shockwavepool", true);
    self->field_0x1b8 = false;
    self->field_0x1c0 = false;
    self->field_0x1c4 = 0.0f;
    self->field_0x1c8 = 0.0f;
    self->field_0x1cc = 0.0f;
    self->field_0x1d0 = 3.40282347e+38f;
    self->field_0x1d4 = -3.40282347e+38f;
    self->field_0x1d8.data = nullptr;
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    self->field_0x1d8.data = &g_unk_7101d47200->empty;
    self->field_0x1e4 = 0.0f;
    self->field_0x1e8 = 0.0f;
    self->field_0x1ec = 0.0f;
    self->field_0x1f0 = 0.0f;
    self->field_0x1f4 = 0.0f;
    self->field_0x1f8 = 0.0f;
    self->field_0x1fc = false;
    self->field_0x200 = 50.0f;
    self->field_0x204 = false;
    self->field_0x208 = 1.0f;
    self->field_0x20c = false;
    self->field_0x210 = 0;
    self->field_0x218 = 0;
    self->field_0x220 = -1.0f;
}
