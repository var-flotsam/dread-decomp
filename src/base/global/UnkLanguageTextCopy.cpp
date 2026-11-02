// From unit 41 of tools/tu_map.py (0x7100066630-0x71000684e4, base::global::timeline::CEvent): the
// copy constructor of the localized text src/game/UnkLanguage.cpp keeps (0x20 bytes: a vtable, the
// UTF-32 characters at +0x8, null when empty, their count at +0x10). The copy gets its own buffer
// from the default allocator (flags 0x11). Names and layouts are placeholders.
#include <stddef.h>

#include "base/UnkVector.h"
#include "types.h"

extern "C" size_t wcslen(const wchar_t* s);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);

// The text's vtable (reached through the GOT; the object points 0x10 past it).
extern char g_unk_7101a1a0d0[];

struct UnkWideText {
    void* vtable;
    wchar_t* chars;
    u32 count;
    u64 field_0x18;

};

// A view of a text's characters, taken before the copy allocates.
struct UnkWideView {
    const wchar_t* chars;
    u32 count;

    explicit UnkWideView(const UnkWideText& text) : chars(text.chars), count(text.chars ? text.count : 0) {}
    const wchar_t* CStr() const { return ((count != 0) & (chars != nullptr)) ? chars : L""; }
};
static_assert(sizeof(UnkWideText) == 0x20, "UnkWideText layout");

void fn_7100066cac(void* self, const void* other) {
    UnkWideText* text = static_cast<UnkWideText*>(self);
    const UnkWideText* src = static_cast<const UnkWideText*>(other);
    text->chars = nullptr;
    text->count = 0;
    text->vtable = g_unk_7101a1a0d0 + 0x10;
    text->field_0x18 = 0;
    UnkWideView view(*src);
    if (view.chars != nullptr || view.count != 0) {
        base::UnkAllocator* allocator = fn_7100083990();
        text->chars = static_cast<wchar_t*>(allocator->vf_0x40((u64)(view.count + 1) * 4, 8, 0x11));
        wcscpy(text->chars, view.CStr());
        text->count = wcslen(text->chars);
    }
}
