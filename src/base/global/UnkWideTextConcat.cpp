// From unit 41 of tools/tu_map.py (base::global::timeline::CEvent): the constructor of the localized
// text of src/base/global/UnkWideTextMethods.cpp (a vtable, the UTF-32 characters at +0x8, their count
// at +0x10, an allocator at +0x18) from two strings with their lengths: it allocates room for both
// (when any), copies the first, appends the second at the first one's length and counts the result.
// Also the substring from `start` to `end` (-1: the end; note it copies `end - start` bytes, not
// characters, then terminates at that many characters), and the append of `length` characters
// (through a 16 KiB stack copy for short texts, else through a concatenated temporary assigned with
// the vtable's slot 3). Names and layouts are placeholders.
#include <stddef.h>

#include "base/UnkVector.h"
#include "types.h"

extern "C" size_t wcslen(const wchar_t* s);
extern "C" void* memcpy(void* dst, const void* src, size_t n);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);

extern char g_unk_7101a1a0d0[];
// The shared empty text (L"").
extern const wchar_t g_unk_7101647684[] __attribute__((visibility("hidden")));

struct UnkWideTextCat {
    void** vtable;
    wchar_t* chars;
    u32 count;
    base::UnkAllocator* allocator;

    void Allocate(u32 length) {
        base::UnkAllocator* a = fn_7100083990();
        chars = static_cast<wchar_t*>(a->vf_0x40((u64)(length + 1) * 4, 8, 0x11));
        wcscpy(chars, g_unk_7101647684);
        count = wcslen(chars);
    }
    base::UnkAllocator* GetAllocator() const {
        base::UnkAllocator* a = allocator;
        if (!a)
            a = fn_7100083990();
        return a;
    }
    void Resize(const wchar_t* text, u32 length) {
        reinterpret_cast<void (*)(UnkWideTextCat*, const wchar_t*, u32)>(vtable[2])(this, text, length);
    }
    void Assign(const UnkWideTextCat* other) {
        reinterpret_cast<void (*)(UnkWideTextCat*, const UnkWideTextCat*)>(vtable[3])(this, other);
    }
};

static inline void Construct(UnkWideTextCat* self, const wchar_t* first, u32 first_length, const wchar_t* second,
                             u32 second_length) {
    self->chars = nullptr;
    self->count = 0;
    self->allocator = nullptr;
    self->vtable = reinterpret_cast<void**>(g_unk_7101a1a0d0 + 0x10);
    u32 length = first_length + second_length;
    if (length != 0)
        self->Allocate(length);
    if (first)
        wcscpy(self->chars, first);
    if (second)
        wcscpy(self->chars + first_length, second);
    self->count = wcslen(self->chars);
}

static inline void Destroy(UnkWideTextCat* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101a1a0d0 + 0x10);
    if (self->chars) {
        if (self->chars != g_unk_7101647684)
            self->GetAllocator()->vf_0x48(self->chars);
        self->chars = nullptr;
        self->count = 0;
    }
}

void fn_7100066d6c(UnkWideTextCat* self, const wchar_t* first, u32 first_length, const wchar_t* second,
                   u32 second_length) {
    Construct(self, first, first_length, second, second_length);
}

UnkWideTextCat fn_7100067484(const UnkWideTextCat* self, u32 start, u32 end) {
    if (end == (u32)-1)
        end = self->count;
    u32 length = end - start;
    UnkWideTextCat result;
    result.chars = nullptr;
    result.count = 0;
    result.allocator = nullptr;
    result.vtable = reinterpret_cast<void**>(g_unk_7101a1a0d0 + 0x10);
    if (length + 1 != 0)
        result.Allocate(length + 1);
    memcpy(result.chars, self->chars + start, length);
    result.chars[length] = 0;
    result.count = wcslen(result.chars);
    return result;
}

void fn_710006723c(UnkWideTextCat* self, const wchar_t* text, u32 length) {
    if (length == 0)
        return;
    s32 count = self->count;
    wchar_t buffer[0x1000];
    if (count < 0x4000) {
        wcscpy(buffer, self->chars ? self->chars : g_unk_7101647684);
        self->Resize(nullptr, count + length);
        wcscpy(self->chars, buffer);
        wcscpy(self->chars + count, text);
        self->count = wcslen(self->chars);
    } else {
        const wchar_t* chars = self->chars;
        UnkWideTextCat joined;
        joined.vtable = reinterpret_cast<void**>(g_unk_7101a1a0d0 + 0x10);
        joined.chars = nullptr;
        joined.count = 0;
        joined.allocator = nullptr;
        u32 total = (u32)count + length;
        if (total != 0)
            joined.Allocate(total);
        if (chars)
            wcscpy(joined.chars, chars);
        if (text)
            wcscpy(joined.chars + (u32)count, text);
        joined.count = wcslen(joined.chars);
        self->Assign(&joined);
        Destroy(&joined);
    }
}
