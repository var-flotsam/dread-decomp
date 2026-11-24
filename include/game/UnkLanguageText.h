#pragma once

// One object that 2 files viewed in part (fn_710119ce68 is declared with it in both; both views are 0x20 bytes). Names and layouts are placeholders.
#include "types.h"

struct UnkLanguageText {
    u8 pad_0x0[0x8];
    const char32_t* chars;
    s32 count;
    u8 pad_0x14[0xc];

    explicit UnkLanguageText(const char32_t* text) { fn_7100066a50(this, text); }
    UnkLanguageText(const UnkLanguageText& other) { fn_7100066cac(this, &other); }
    ~UnkLanguageText() { fn_7100066e44(this); }
};
