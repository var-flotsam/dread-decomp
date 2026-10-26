// Constructor of an object with eight 0x150-byte entries: runs the base constructor
// (fn_7100fe1028), sets both vtables, constructs each entry with its kind (fn_7100fd10cc) and
// gives all but the first a derived vtable.
#include "types.h"
#include "game/sound/CMusicManager.h"

struct UnkEntry_0x150 {
    void** vtable;
    u8 unk_0x8[0x148];
};

struct UnkObject_7100fcfcdc {
    void** vtable;
    u8 unk_0x8[0x28];
    void** vtable_0x30;
    void* field_0x38;
    void* field_0x40;
    void* field_0x48;
    u32 field_0x50;
    u8 unk_0x54[0x80];
    s32 field_0xd4;
    u16 field_0xd8;
    UnkEntry_0x150 entries[8];
    u32 field_0xb60;
    bool field_0xb64;
    u32 field_0xb68;
    f32 field_0xb6c;
    f32 field_0xb70;
};

extern void* g_unk_7101cabfa8[];
extern void* g_unk_7101cabe10[];
extern void* g_unk_7101cabd88[];
extern void* g_unk_7101cabd00[];
extern void* g_unk_7101cabbe8[];
extern void* g_unk_7101cabb60[];
extern void* g_unk_7101cabf20[];
extern void* g_unk_7101cabe98[];
void fn_7100fd10cc(UnkEntry_0x150* self, s32 kind);

void fn_7100fcfcdc(UnkMusicBank_0xb78* bank, u32 kind) {
    auto* self = reinterpret_cast<UnkObject_7100fcfcdc*>(bank);  // this file's view of the bank
    fn_7100fe1028(reinterpret_cast<UnkMusicChannel_0x30*>(self), kind);  // its base, the channel
    self->field_0x38 = nullptr;
    self->field_0x40 = nullptr;
    self->field_0x48 = nullptr;
    self->field_0x50 = 0;
    self->field_0xd4 = 0x7fffffff;
    self->field_0xd8 = 0;
    self->vtable = &g_unk_7101cabfa8[2];
    self->vtable_0x30 = &g_unk_7101cabfa8[21];
    fn_7100fd10cc(&self->entries[0], 0);
    fn_7100fd10cc(&self->entries[1], 1);
    self->entries[1].vtable = &g_unk_7101cabe10[2];
    fn_7100fd10cc(&self->entries[2], 2);
    self->entries[2].vtable = &g_unk_7101cabd88[2];
    fn_7100fd10cc(&self->entries[3], 4);
    self->entries[3].vtable = &g_unk_7101cabd00[2];
    fn_7100fd10cc(&self->entries[4], 6);
    self->entries[4].vtable = &g_unk_7101cabbe8[2];
    fn_7100fd10cc(&self->entries[5], 7);
    self->entries[5].vtable = &g_unk_7101cabb60[2];
    fn_7100fd10cc(&self->entries[6], 3);
    self->entries[6].vtable = &g_unk_7101cabf20[2];
    fn_7100fd10cc(&self->entries[7], 5);
    self->entries[7].vtable = &g_unk_7101cabe98[2];
    self->field_0xb60 = 0;
    self->field_0xb64 = false;
    self->field_0xb68 = 0;
    self->field_0xb6c = -1.0f;
    self->field_0xb70 = -1.0f;
}
