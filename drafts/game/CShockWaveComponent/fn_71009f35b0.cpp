// CShockWaveComponent's unit (tools/tu_map.py unit 1505): the component's constructor: its base part
// (fn_71008c0c74), then its flags, ranges, string ids ("shockwave" among them) and limits.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkNamedRegistry.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CShockWaveComponent.h"
#include "game/UnkTU1370.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

extern void* g_unk_7101a2e1e8[];  // vtable

// Four floats set together.
struct UnkFloat4_l9f35b0 {
    f32 x, y, z, w;
};

struct UnkShockWaveCtor_l9f35b0 {
    void** vtable;
    u8 base[0x100];  // 0x8, built by fn_71008c0c74
    bool field_0x108;
    u8 pad_0x109[0x3];
    u32 field_0x10c;
    bool field_0x110;
    u8 pad_0x111[0x3];
    f32 field_0x114[3];
    bool field_0x120;
    u8 pad_0x121[0x3];
    f32 field_0x124;
    f32 field_0x128;
    f32 field_0x12c[3];
    bool field_0x138;
    u8 pad_0x139[0x7];
    CStrId field_0x140;
    bool field_0x148;
    bool field_0x149;
    u8 pad_0x14a[0x6];
    CFilePathStrId field_0x150;
    base::UnkNamedEntry field_0x158;
    void* field_0x188;
    bool field_0x190;
    u8 pad_0x191[0x3];
    UnkFloat4_l9f35b0 field_0x194;
    bool field_0x1a4;
    u8 pad_0x1a5[0x3];
    u32 field_0x1a8;
    u8 pad_0x1ac[0x4];
    CStrId name;  // 0x1b0
    bool field_0x1b8;
    u8 pad_0x1b9[0x7];
    bool field_0x1c0;
    u8 pad_0x1c1[0x3];
    u32 field_0x1c4;
    u32 field_0x1c8;
    f32 min;  // 0x1cc
    f32 max;  // 0x1d0
    f32 lowest;  // 0x1d4
    CFilePathStrId field_0x1d8;
    u8 pad_0x1e0[0x4];
    u32 field_0x1e4[6];
    bool field_0x1fc;
    u8 pad_0x1fd[0x3];
    f32 field_0x200;
    bool field_0x204;
    u8 pad_0x205[0x3];
    f32 field_0x208;
};
static_assert(__builtin_offsetof(UnkShockWaveCtor_l9f35b0, field_0x158) == 0x158, "layout");
static_assert(__builtin_offsetof(UnkShockWaveCtor_l9f35b0, field_0x1d8) == 0x1d8, "layout");
static_assert(__builtin_offsetof(UnkShockWaveCtor_l9f35b0, field_0x208) == 0x208, "layout");

void fn_71009f35b0(char* raw) {
    UnkShockWaveCtor_l9f35b0* self = reinterpret_cast<UnkShockWaveCtor_l9f35b0*>(raw);
    self->vtable = &g_unk_7101a2e1e8[2];
    fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(self->base));
    self->field_0x108 = false;
    self->field_0x10c = 0;
    self->field_0x110 = false;
    self->field_0x114[0] = 0.0f;
    self->field_0x114[1] = 0.0f;
    self->field_0x114[2] = 0.0f;
    self->field_0x120 = false;
    self->field_0x124 = 0.0f;
    self->field_0x128 = 6.2831855f;
    self->field_0x12c[0] = 0.0f;
    self->field_0x12c[1] = 0.0f;
    self->field_0x12c[2] = 0.0f;
    self->field_0x138 = false;
    new (&self->field_0x140) CStrId(g_unk_710162e2ed, true);
    self->field_0x148 = false;
    self->field_0x149 = false;
    new (&self->field_0x150) CFilePathStrId(g_unk_710162e2ed, true);
    fn_710007e704(&self->field_0x158, g_unk_710162e2ed);
    self->field_0x188 = nullptr;
    self->field_0x190 = false;
    static const UnkFloat4_l9f35b0 kInitial = {1.0f, 0.0f, 0.0f, 1.0f};
    self->field_0x194 = kInitial;
    self->field_0x1a4 = false;
    self->field_0x1a8 = 0;
    new (&self->name) CStrId("shockwave", true);
    self->field_0x1b8 = false;
    self->field_0x1c0 = false;
    self->field_0x1c4 = 0;
    self->field_0x1c8 = 0;
    self->min = 0.0f;
    self->max = 3.4028235e38f;
    self->lowest = -3.4028235e38f;
    new (&self->field_0x1d8) CFilePathStrId();
    for (u32& v : self->field_0x1e4)
        v = 0;
    self->field_0x1fc = false;
    self->field_0x200 = 50.0f;
    self->field_0x204 = false;
    self->field_0x208 = 1.0f;
}
