// CDredhedAIComponent unit (tools/tu_map.py): one of two tuning values, picked by whether the character class's word at +0x600 is zero. Names and layouts are placeholders.
#include "game/CCharClassBlockableWarningRadius.h"
#include "game/UnkAITunablesValue_ovb0001.h"
#include "game/UnkAITunables_ovb0001.h"
#include "types.h"

struct UnkDredhedCharClass_ovb0001 {
    u8 pad_0x0[0x600];
    s32 field_0x600;
};

struct UnkDredhedAI_ovb0001 {
    u8 pad_0x0[0x14730];
    UnkDredhedCharClass_ovb0001* charClass;  // 0x14730
};

f32 fn_71004e7a4c(UnkDredhedAI_ovb0001* self) {
    return self->charClass->field_0x600 == 0 ? g_unk_71726c4890.field_0x40.value : g_unk_71726c4890.field_0x58;
}
