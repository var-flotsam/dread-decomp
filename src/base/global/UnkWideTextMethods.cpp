// From unit 41 of tools/tu_map.py (base::global::timeline::CEvent): more members of the localized
// text of src/base/global/UnkLanguageTextCopy.cpp (a vtable, the UTF-32 characters at +0x8, their
// count at +0x10, an allocator at +0x18): the constructor from a CRntString (it allocates
// length + 1 characters from the default allocator with flags 0x11, starts them empty, converts
// the narrow text into them with fn_710006c000 and terminates it), and the deleting destructor. Names and layouts are placeholders.
#include <stddef.h>

#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

extern "C" size_t wcslen(const wchar_t* s);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);

extern char g_unk_7101a1a0d0[];
// The shared empty text (L"").
extern const wchar_t g_unk_7101647684[] __attribute__((visibility("hidden")));

// Converts a narrow string into at most `capacity` UTF-32 characters.
void fn_710006c000(const base::global::CRntString* src, wchar_t* dst, u32 capacity);

struct UnkWideTextObj {
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
    // Allocates `length` + 1 characters, empty.
    void Allocate(u32 length) {
        base::UnkAllocator* a = fn_7100083990();
        chars = static_cast<wchar_t*>(a->vf_0x40((u64)(length + 1) * 4, 8, 0x11));
        wcscpy(chars, g_unk_7101647684);
        count = wcslen(chars);
    }
};
static_assert(sizeof(UnkWideTextObj) == 0x20, "UnkWideTextObj layout");

void fn_7100066c00(UnkWideTextObj* self, const base::global::CRntString* src) {
    self->allocator = nullptr;
    self->chars = nullptr;
    self->count = 0;
    self->vtable = g_unk_7101a1a0d0 + 0x10;
    u32 length = src->length;
    if (length != 0) {
        self->Allocate(length);
        fn_710006c000(src, self->chars, length + 1);
        self->chars[length] = 0;
        self->count = length;
    }
}

void fn_7100066f04(UnkWideTextObj* self) {
    self->vtable = g_unk_7101a1a0d0 + 0x10;
    if (self->chars && self->chars != g_unk_7101647684)
        self->GetAllocator()->vf_0x48(self->chars);
    fn_7100083c00(self);
}
