// CQuetzoaAIComponent unit (tools/tu_map.py): one of two tuning values, picked by whether the character class's word at +0x708 is zero. Names and layouts are placeholders.
#include "types.h"

// The AI tuning constants (g_unk_71726c4890), as far as these units read them. Each pair of values is
// read as two differently nested fields, so the choice between them stays a choice of address.
struct UnkAITunablesValue_ovb0001 {
    f32 value;
};

struct UnkAITunables_ovb0001 {
    u8 pad_0x0[0x30];
    UnkAITunablesValue_ovb0001 field_0x30;
    f32 field_0x34;
    u8 pad_0x38[0x8];
    UnkAITunablesValue_ovb0001 field_0x40;
    f32 field_0x44;
    u8 pad_0x48[0x10];
    f32 field_0x58;
    u8 pad_0x5c[0x4];
    UnkAITunablesValue_ovb0001 field_0x60;
};
extern UnkAITunables_ovb0001 g_unk_71726c4890;

struct UnkQuetzoaCharClass_ovb0001 {
    u8 pad_0x0[0x708];
    s32 field_0x708;
};

struct UnkQuetzoaAI_ovb0001 {
    u8 pad_0x0[0x14a28];
    UnkQuetzoaCharClass_ovb0001* charClass;  // 0x14a28
};

f32 fn_710064bbe4(UnkQuetzoaAI_ovb0001* self) {
    return self->charClass->field_0x708 == 0 ? g_unk_71726c4890.field_0x34 : g_unk_71726c4890.field_0x60.value;
}
