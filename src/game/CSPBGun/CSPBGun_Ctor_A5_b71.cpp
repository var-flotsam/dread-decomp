// CSPBGun (unit 1317, the name from reflection): the in-place constructor its reflection descriptor points to
// (fn_0x28). After the beam gun base constructor (fn_710082fa78) it sets the vtable, clears its own fields, copies
// the empty sound handle, sets its tunables (ranges, counts, an empty name) and the base's kind at +0x1874, then
// runs the class entries (fn_7100023854). Defined under the `void()` symbol src/reflection/CSPBGun.cpp declares.
// Names and layouts are placeholders.
#include "base/core/CBaseObject.h"
#include "types.h"

struct UnkNode;
struct UnkBeamGun_A5_b71;
void fn_710082fa78(UnkBeamGun_A5_b71* self);
extern void* g_unk_7101a79800[];

struct UnkSoundRef;
struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;
};
extern const UnkSoundHandle g_unk_7108b6c460;  // the empty handle

struct UnkSPBGunCtor_A5_b71 {
    void* vtable;
    u8 pad_0x8[0x1874 - 0x8];
    u32 kind;  // 0x1874
    u8 pad_0x1878[0x8];
    bool field_0x1880;
    u8 pad_0x1881[0x3];
    f32 field_0x1884;
    f32 field_0x1888;
    u8 pad_0x188c[0x4];
    UnkSoundHandle sound;  // 0x1890
    u32 field_0x18a0;
    u16 field_0x18a4;
    u8 pad_0x18a6[0x2];
    f32 field_0x18a8;
    f32 field_0x18ac;
    f32 field_0x18b0;
    f32 field_0x18b4;
    f32 field_0x18b8;
    f32 field_0x18bc;
    f32 field_0x18c0;
    f32 field_0x18c4;
    u32 field_0x18c8;
    u32 field_0x18cc;
    const char* name;  // 0x18d0
    u64 field_0x18d8;
    u64 field_0x18e0;
    bool field_0x18e8;
};
static_assert(__builtin_offsetof(UnkSPBGunCtor_A5_b71, field_0x18e8) == 0x18e8, "layout");

void fn_710085ece0_A5_b71(UnkSPBGunCtor_A5_b71* self) asm("_Z13fn_710085ece0v");
void fn_710085ece0_A5_b71(UnkSPBGunCtor_A5_b71* self) {
    char* block = reinterpret_cast<char*>(self) + 0x1880;
#if defined(__aarch64__)
    asm("" : "+r"(block));
#endif
    fn_710082fa78(reinterpret_cast<UnkBeamGun_A5_b71*>(self));
    self->vtable = reinterpret_cast<char*>(g_unk_7101a79800) + 0x10;
    *reinterpret_cast<bool*>(block) = false;
    self->field_0x1884 = 0.0f;
    self->field_0x1888 = 0.0f;
    self->sound = g_unk_7108b6c460;
    self->field_0x18a0 = 2;
    *reinterpret_cast<u16*>(block + 0x24) = 0;
    self->field_0x18a8 = 0.0f;
    self->field_0x18ac = 1.0f;
    self->field_0x18b0 = -20.0f;
    self->field_0x18b4 = 20.0f;
    self->field_0x18b8 = -50.0f;
    self->field_0x18bc = 50.0f;
    self->field_0x18c0 = 1.0f;
    self->field_0x18c4 = 0.0f;
    self->field_0x18c8 = 3;
    self->field_0x18cc = 6;
    self->name = "";
    self->field_0x18d8 = 0;
    self->field_0x18e0 = 0;
    self->field_0x18e8 = false;
    self->kind = 4;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
