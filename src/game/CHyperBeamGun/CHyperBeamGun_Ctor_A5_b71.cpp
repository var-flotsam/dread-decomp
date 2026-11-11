// CHyperBeamGun (the name from reflection): the in-place constructor its reflection descriptor points to
// (fn_0x28). After the wave beam gun base constructor (fn_7100867aa0) it sets the vtable, clears its fields,
// copies the empty sound handle into three slots, sets two empty weak references (-1 serials), assigns the
// base's colour at +0x1854 (0, 1, 0, 1) and the base's kind at +0x1874, then runs the class entries
// (fn_7100023854). Defined under the `void()` symbol src/reflection/CHyperBeamGun.cpp declares. Names and
// layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/core/CBaseObject.h"
#include "game/UnkTU101.h"
#include "types.h"

struct UnkNode;
struct UnkWaveBeamGun_A5_b71;
void fn_7100867aa0(UnkWaveBeamGun_A5_b71* self);
extern void* g_unk_7101a77de8[];

struct UnkSoundRef;
struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;
};

struct UnkHyperRef_A5_b71 {
    void* ref;
    s64 serial;
};

struct UnkHyperBeamGunCtor_A5_b71 {
    void* vtable;
    u8 pad_0x8[0x1854 - 0x8];
    base::color::CColor4F color;  // 0x1854
    u8 pad_0x1864[0x10];
    u32 kind;  // 0x1874
    u8 pad_0x1878[0x10];
    u64 field_0x1888;
    u64 field_0x1890;
    u64 field_0x1898;
    u32 field_0x18a0;
    u8 pad_0x18a4[0x4];
    UnkSoundHandle sound_a;  // 0x18a8
    UnkSoundHandle sound_b;  // 0x18b8
    UnkSoundHandle sound_c;  // 0x18c8
    UnkHyperRef_A5_b71 ref_a;  // 0x18d8
    UnkHyperRef_A5_b71 ref_b;  // 0x18e8
    u16 field_0x18f8;
    u8 field_0x18fa;
    u8 pad_0x18fb[0x1];
    u32 field_0x18fc;
    u16 field_0x1900;
    u8 pad_0x1902[0x2];
    u32 field_0x1904;
    u8 pad_0x1908[0x4];
    u32 field_0x190c;
    u8 pad_0x1910[0x4];
    u32 field_0x1914;
    s64 field_0x1918;
};
static_assert(__builtin_offsetof(UnkHyperBeamGunCtor_A5_b71, field_0x1918) == 0x1918, "layout");

void fn_7100848e00_A5_b71(UnkHyperBeamGunCtor_A5_b71* self) asm("_Z13fn_7100848e00v");
void fn_7100848e00_A5_b71(UnkHyperBeamGunCtor_A5_b71* self) {
    fn_7100867aa0(reinterpret_cast<UnkWaveBeamGun_A5_b71*>(self));
    self->vtable = reinterpret_cast<char*>(g_unk_7101a77de8) + 0x10;
    self->field_0x1888 = 0;
    self->field_0x1890 = 0;
    self->field_0x1898 = 0;
    self->field_0x18a0 = 0;
    self->sound_a = g_unk_7108b6c460;
    self->sound_b = g_unk_7108b6c460;
    self->sound_c = g_unk_7108b6c460;
    self->ref_a.ref = nullptr;
    self->ref_a.serial = -1;
    self->ref_b.ref = nullptr;
    self->ref_b.serial = -1;
    self->field_0x18f8 = 0;
    self->field_0x18fa = 0;
    self->field_0x18fc = 0;
    self->field_0x1900 = 0x101;
    self->field_0x1904 = 0;
    self->field_0x190c = 0;
    self->field_0x1914 = 0;
    self->field_0x1918 = -1;
    base::color::CColor4F c(0.0f, 1.0f, 0.0f, 1.0f);
    fn_7100030728(&self->color, &c);
    self->kind = 5;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
