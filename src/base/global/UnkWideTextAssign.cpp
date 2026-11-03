// From unit 41 of tools/tu_map.py (base::global::timeline::CEvent): assigning a UTF-32 string of
// `count` characters to the localized text of src/base/global/UnkLanguageTextCopy.cpp (0x20 bytes:
// a vtable, the characters at +0x8, their count at +0x10, an allocator at +0x18). The old buffer is
// freed unless it is the shared empty string; the new one comes from the text's allocator (the
// default one when null, flags 0x11). Names and layouts are placeholders.
#include <stddef.h>

#include "base/UnkVector.h"
#include "types.h"

extern "C" size_t wcslen(const wchar_t* s);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);

// The shared empty text (L"").
extern const wchar_t g_unk_7101647684[] __attribute__((visibility("hidden")));

struct UnkWideTextBuf {
    void* vtable;
    wchar_t* chars;
    u32 count;
    base::UnkAllocator* allocator;

    base::UnkAllocator* GetAllocator() const {
        base::UnkAllocator* a = allocator;
        if (!a)
            a = fn_7100083990();
        return a;
    }
};
static_assert(sizeof(UnkWideTextBuf) == 0x20, "UnkWideTextBuf layout");

void fn_7100066f68(UnkWideTextBuf* self, const wchar_t* str, u32 count) {
    if (self->chars) {
        if (self->chars == str)
            return;
        if (self->chars != g_unk_7101647684)
            self->GetAllocator()->vf_0x48(self->chars);
        self->chars = nullptr;
        self->count = 0;
    }
    if (!str && !count)
        return;
    self->chars = static_cast<wchar_t*>(self->GetAllocator()->vf_0x40((u64)(count + 1) * 4, 8, 0x11));
    wcscpy(self->chars, (count != 0 && str != nullptr) ? str : g_unk_7101647684);
    self->count = wcslen(self->chars);
}
