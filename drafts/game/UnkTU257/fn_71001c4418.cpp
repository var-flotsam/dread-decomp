// UnkTU257 (tools/tu_map.py unit 257): sets a label node's text (the layout text at +0xb0) and counts
// its visible characters: a control character 1 to 9 (not the one right after another) and colour
// tags "{c<digit>}" are not counted.
// Names and layouts are placeholders.
#include "base/global/timeline/CEvent.h"
#include "game/UnkTU257.h"
#include "types.h"

// The layout text (an UnkLayoutText) as this function reads it: the characters and their count.
struct UnkTextBuffer_l1c4418 {
    void* vtable;
    const char32_t* data;  // 0x8
    u32 length;            // 0x10
};

struct UnkTextNode_l1c4418 {
    u8 pad_0x0[0xb0];
    u8 text_0xb0[0x18];  // an UnkLayoutText
    u8 pad_0xc8[0x14];
    u32 visible_count;  // 0xdc
    u8 pad_0xe0[0x2];
    bool dirty;  // 0xe2
};
static_assert(__builtin_offsetof(UnkTextNode_l1c4418, visible_count) == 0xdc, "layout");
static_assert(__builtin_offsetof(UnkTextNode_l1c4418, dirty) == 0xe2, "layout");

// Whether a colour tag "{c<digit>}" (or "{C<digit>}") starts at `i`.
static inline bool IsColorTag(const UnkTextBuffer_l1c4418& text, u32 i) {
    if (i + 4 > text.length)
        return false;
    const char32_t* s = text.data;
    if (s[i] != U'{' || s[i + 3] != U'}')
        return false;
    if (s[i + 2] < U'0' || s[i + 2] > U'9')
        return false;
    return s[i + 1] == U'c' || s[i + 1] == U'C';
}

void fn_71001c4418(void* p, const s32* string) {
    UnkTextNode_l1c4418* self = static_cast<UnkTextNode_l1c4418*>(p);
    fn_7100066af0(reinterpret_cast<UnkLayoutText*>(self->text_0xb0), reinterpret_cast<const char32_t*>(string));
    self->dirty = true;
    self->visible_count = 0;
    const UnkTextBuffer_l1c4418* text = reinterpret_cast<const UnkTextBuffer_l1c4418*>(self->text_0xb0);
    if (!text->data)
        return;
    u32 length = text->length;
    s32 control = 0;  // the control character skipped last, until a character is counted
    for (u32 i = 0; i < length; ++i) {
        if (control == 0 && string[i] >= 1 && string[i] <= 9) {
            control = string[i];
            continue;
        }
        if (IsColorTag(*text, i)) {
            i += 3;
            continue;
        }
        control = 0;
        ++self->visible_count;
    }
}
