// Constructor in the CCharClassGoliathXAIComponent unit (vtable 0x7101a4b340; the destructors are in
// CCharClassGoliathXAIComponent.cpp): the base constructor fn_71003e8db0, the base's vtable
// (0x7101a4c200) and its member at +0x530 (fn_710058d960), then this class's vtable and its members at
// +0x628 and +0x720 (fn_7100583588, fn_710058397c), a few flags and a 1.2f factor, and finally the
// virtual at slot 3. Names and layouts are placeholders.
#include "game/CCharClassBaseBigFistAIComponent.h"
#include "types.h"

struct UnkGoliathXAI_b55;
void fn_710058d960(void* self);
void fn_7100583588(void* self);
void fn_710058397c(void* self);
extern void* g_unk_7101a4b340[];
extern void* g_unk_7101a4c200[];

struct UnkGoliathXAI_b55 {
    void** vtable;
    u8 pad_0x8[0x401];
    bool field_0x409;
    u8 pad_0x40a;
    bool field_0x40b;
    u8 pad_0x40c[0xd4];
    f32 field_0x4e0;
    u8 pad_0x4e4[0x18];
    u16 field_0x4fc;
    u8 pad_0x4fe[0xe];
    bool field_0x50c;
    u8 pad_0x50d[0x23];
    u8 field_0x530[0xf8];
    u8 field_0x628[0xf8];
    u8 field_0x720[0x8];

    void vf_0x18() { reinterpret_cast<void (*)(UnkGoliathXAI_b55*)>(vtable[3])(this); }
};

void fn_7100587da4(UnkGoliathXAI_b55* self) {
    fn_71003e8db0(self);
    self->vtable = g_unk_7101a4c200 + 2;
    fn_710058d960(self->field_0x530);
    self->vtable = g_unk_7101a4b340 + 2;
    fn_7100583588(self->field_0x628);
    fn_710058397c(self->field_0x720);
    self->field_0x40b = true;
    self->field_0x4fc = 0x101;
    self->field_0x4e0 = 1.2f;
    self->field_0x409 = false;
    self->field_0x50c = false;
    self->vf_0x18();
}
