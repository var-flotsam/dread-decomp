// base::global::CRntString: assignment, construction from a literal or a string id,
// concatenation, appending, comparisons and searches.
#include <ctype.h>
#include <string.h>

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"

using base::global::CRntString;

// Replaces the string with a copy of `str` (or an empty block of `length` + 1 bytes when str is
// null). The original calls it out of line from the functions below; noinline keeps it so here.
__attribute__((noinline)) void fn_710007ebc8(CRntString* self, const char* str, u32 length, s64 key) {
    if (self->borrowed)
        return;
    if (self->data) {
        if (self->data == str)
            return;
        if (self->data != g_unk_7101647847)
            self->GetAllocator()->vf_0x48(self->data);
        self->data = nullptr;
        self->length = 0;
        self->key = -1;
    }
    if (!str && !length)
        return;
    self->data = static_cast<char*>(self->GetAllocator()->vf_0x40(length + 1, 8, 0x11));
    strcpy(self->data, length && str ? str : g_unk_7101647847);
    self->length = strlen(self->data);
    self->key = key;
}

// NON_MATCHING(minor): block layout; Clang duplicates the CRC call for a null string, where the
// original merges both lengths into one call.
void fn_710007e704(CRntString* self, const char* str) {
    self->owns_allocator = false;
    self->borrowed = false;
    self->data = nullptr;
    self->length = 0;
    self->key = -1;
    self->allocator = nullptr;
    u32 length = str ? strlen(str) : 0;
    fn_710007ebc8(self, str, length, fn_7100001570(str, length));
}

// Assigns a literal.
// NON_MATCHING(minor): as fn_710007e704.
void fn_710007e77c(CRntString* self, const char* str) {
    u32 length = str ? strlen(str) : 0;
    fn_710007ebc8(self, str, length, fn_7100001570(str, length));
}

// The pooled string data behind a string id, as copying it into a CRntString sees it.
struct UnkPooledString {
    u8 unk_0x0[0x10];
    const char* str;
    u32 length;
    u8 unk_0x1c[0x14];
    s64 key;  // +0x30
    u8 unk_0x38[0xc];
    // A union member: Clang gives union accesses no type-based alias info, so the copies below
    // reload the id after the increment, as the original does.
    union {
        u32 copies;  // +0x44
    };
};

struct UnkPooledStrId {
    UnkPooledString* data;
};

// Assigns a string id's text (and counts the copy in the pool).
void fn_710007eb6c(CRntString* self, const UnkPooledStrId* id) {
    ++id->data->copies;
    UnkPooledString* d = id->data;
    fn_710007ebc8(self, d->str ? d->str : "", d->length, d->key);
}

void fn_710007eb20(CRntString* self, const UnkPooledStrId* id) {
    self->borrowed = false;
    self->data = nullptr;
    self->length = 0;
    self->key = -1;
    self->allocator = nullptr;
    fn_710007eb6c(self, id);
}

// Finds `c` from `start` on; -1 when absent.
s32 fn_710007facc(const CRntString* self, char c, u32 start) {
    for (u32 i = start; i < self->length; ++i)
        if (self->data[i] == c)
            return i;
    return -1;
}

// Finds `c` from `start` backwards (from the end when start is negative); -1 when absent.
s32 fn_710007fb10(const CRntString* self, char c, s32 start) {
    if (start < 0) {
        start = self->length - 1;
        if (start < 0)
            return -1;
    }
    for (s32 i = start; i >= 0; --i)
        if (self->data[i] == c)
            return i;
    return -1;
}

bool fn_710007fb9c(const CRntString* self, const char* str) {
    return self->data && strstr(self->data, str);
}

bool fn_710007fbc0(const CRntString* self, const CRntString* other) {
    return self->data && strstr(self->data, other->data ? other->data : "");
}

bool fn_710007fc80(const CRntString* self, const CRntString* prefix) {
    if (!self->data)
        return false;
    u32 n = prefix->length;
    if (self->length < n)
        return false;
    for (u32 i = 0; i < n; ++i)
        if ((prefix->data ? prefix->data : "")[i] != self->data[i])
            return false;
    return true;
}

bool fn_710007fce0(const CRntString* self, const char* suffix) {
    if (!self->data)
        return false;
    size_t n = strlen(suffix);
    if (n > self->length)
        return false;
    return strcmp(suffix, self->data + (self->length - n)) == 0;
}

// Case-insensitive equality with a C string.
// NON_MATCHING(minor): the original tests the character before the difference (ccmp order and
// the loop's exit branches).
bool fn_710007edc4(const CRntString* self, const char* str) {
    const u8* a = reinterpret_cast<const u8*>(self->data);
    const u8* b = reinterpret_cast<const u8*>(str);
    s32 d = 0;
    if (a != b) {
        u8 c = *a;
        d = tolower(c) - tolower(*b);
        while (c && d == 0) {
            ++a;
            ++b;
            c = *a;
            d = tolower(c) - tolower(*b);
        }
    }
    return d == 0;
}

// The same over at most `n` characters.
// NON_MATCHING(minor): the two pointer increments are swapped.
bool fn_710007ee58(const CRntString* self, const char* str, u32 n) {
    const u8* a = reinterpret_cast<const u8*>(self->data);
    const u8* b = reinterpret_cast<const u8*>(str);
    s32 d = 0;
    if (n && a != b) {
        do {
            u8 c = *a;
            d = tolower(c) - tolower(*b);
            if (!c || d)
                break;
            ++a;
            ++b;
        } while (--n);
    }
    return d == 0;
}

// Appends one character.
CRntString* fn_710007f5ac(CRntString* self, char c) {
    char buffer[2] = {c, 0};
    fn_710007f3e4(self, buffer, 1);
    return self;
}

// Concatenation (the result is returned in memory).

// Concatenation.
// NON_MATCHING(minor): the original stores `borrowed` first when it initialises the result.
CRntString fn_710007eee0(const CRntString* a, const CRntString* b) {
    u32 a_length = a->length;
    const char* a_data = a->data;
    u32 total = b->length + a_length;
    const char* b_data = b->data;
    CRntString result;
    fn_710007ebc8(&result, nullptr, total, 0);
    if (total) {
        if (a_data)
            strcpy(result.data, a_data);
        if (b_data)
            strcpy(result.data + a_length, b_data);
        result.length = strlen(result.data);
        result.key = fn_7100001570(result.data, result.length);
    }
    return result;
}

// NON_MATCHING(minor): as fn_710007eee0 (the order of the result's initial stores).
CRntString fn_710007f050(const char* a, const CRntString* b) {
    u32 a_length = a ? strlen(a) : 0;
    u32 total = b->length + a_length;
    const char* b_data = b->data;
    CRntString result;
    fn_710007ebc8(&result, nullptr, total, 0);
    if (total) {
        if (a)
            strcpy(result.data, a);
        if (b_data)
            strcpy(result.data + a_length, b_data);
        result.length = strlen(result.data);
        result.key = fn_7100001570(result.data, result.length);
    }
    return result;
}

// Orders null and empty strings after all others.
static inline s32 Compare(const char* a, const char* b) {
    if (a == b)
        return 0;
    bool a_empty = !a || !*a;
    bool b_empty = !b || !*b;
    if (a_empty && b_empty)
        return 0;
    if (a_empty)
        return 1;
    if (b_empty)
        return -1;
    return strcmp(a, b);
}

bool fn_710007f140(const CRntString* self, const char* str) {
    if (self->length != (str ? (u32)strlen(str) : 0))
        return false;
    return Compare(self->data, str) == 0;
}

bool fn_710007f320(const CRntString* a, const CRntString* b) {
    return Compare(a->data, b->data) < 0;
}

// Appends `length` characters of `str`. Short strings are rebuilt through a stack copy; long ones
// through a new block.
// NON_MATCHING(minor): Clang turns the last calls of the long-string path into tail calls; the
// original returns through the shared epilogue.
void fn_710007f3e4(CRntString* self, const char* str, u32 length) {
    if (!length || self->borrowed)
        return;
    s32 old = self->length;
    char* data = self->data;
    if (old < 256) {
        char buffer[256];
        strcpy(buffer, data ? data : "");
        fn_710007ebc8(self, nullptr, old + length, 0);
        strcpy(self->data, buffer);
        strcpy(self->data + old, str);
        char* d = self->data;
        self->length = strlen(d);
        self->key = fn_7100001570(d, self->length);
    } else {
        u32 total = (u32)old + length;
        if (total) {
            char* p = static_cast<char*>(fn_7100083990()->vf_0x40(total + 1, 8, 0x11));
            *p = 0;
            if (data)
                strcpy(p, data);
            if (str)
                strcpy(p + (u32)old, str);
            u32 n = strlen(p);
            fn_710007ebc8(self, p, n, fn_7100001570(p, n));
            if (p && p != g_unk_7101647847)
                fn_7100083990()->vf_0x48(p);
        } else {
            fn_710007ebc8(self, nullptr, 0, -1);
        }
    }
}

// Trimming: the kept part is copied into a temporary string (with 0x100 bytes of inline storage
// when the string is short enough) and assigned back.
#include "base/global/CRntStringAssign.h"

// A temporary string: CRntString's fields without its out-of-line destructor (the original has
// the destructor inlined; it is defined in this unit there). Optionally followed by 0x100 bytes of
// inline storage (as UnkInlineString256 in CRntStringAssign.cpp).
struct UnkTrimString {
    char* data;
    u32 length;
    base::UnkAllocator* allocator;
    bool owns_allocator;
    s64 key;
    bool borrowed;

    CRntString* Str() { return reinterpret_cast<CRntString*>(this); }
    base::UnkAllocator* GetAllocator() const { return allocator ? allocator : fn_7100083990(); }
};
static_assert(sizeof(UnkTrimString) == sizeof(CRntString), "UnkTrimString layout");

struct UnkTrimStringInline {
    UnkTrimString str;
    void** allocator_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;
};

static inline bool IsTrimSpace(u8 c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

// The string's clear (fn_710007ed40) and destructor (fn_710007ecd4) share one body; a temporary
// is destroyed with both, inlined one after the other.
__attribute__((always_inline)) static inline void ClearTemporary(UnkTrimString& s) {
    if (!s.borrowed) {
        if (!s.data)
            return;
        if (s.data != g_unk_7101647847)
            s.GetAllocator()->vf_0x48(s.data);
    }
    s.data = nullptr;
    s.length = 0;
    s.key = -1;
}

// Assigns `count` characters of `str` to `self` through a temporary on inline storage.
__attribute__((always_inline)) static inline void AssignShort(CRntString* self, const char* str, u32 count) {
    UnkTrimStringInline tmp;
    tmp.str.allocator = reinterpret_cast<base::UnkAllocator*>(&tmp.allocator_vtable);
    tmp.str.owns_allocator = false;
    tmp.str.borrowed = false;
    tmp.str.data = nullptr;
    tmp.str.length = 0;
    tmp.str.key = -1;
    tmp.allocator_vtable = g_unk_7101a194b8 + 2;
    tmp.field_0x138 = 0;
    tmp.field_0x140 = 0;
    fn_710007e7f8(tmp.str.Str(), str, count);
    fn_710007ebc8(self, tmp.str.data, tmp.str.length, tmp.str.key);
    ClearTemporary(tmp.str);
    ClearTemporary(tmp.str);
}

// The same through a temporary on the default allocator. The original knows that only
// fn_710007e7f8 writes this temporary (defined in the same unit there, it doesn't keep the
// pointer), so the text is read once and the second clear folds away.
__attribute__((always_inline)) static inline void AssignLong(CRntString* self, const char* str, u32 count) {
    UnkTrimString tmp;
    tmp.owns_allocator = false;
    tmp.borrowed = false;
    tmp.data = nullptr;
    tmp.length = 0;
    tmp.key = -1;
    tmp.allocator = nullptr;
    fn_710007e7f8(tmp.Str(), str, count);
    char* data = tmp.data;
    fn_710007ebc8(self, data, tmp.length, tmp.key);
    if (!tmp.borrowed) {
        if (!data)
            return;
        if (data != g_unk_7101647847)
            tmp.GetAllocator()->vf_0x48(data);
    }
    tmp.data = nullptr;
    tmp.key = -1;
    tmp.length = 0;
}

// Removes leading spaces, tabs and line breaks (a borrowed string is left alone).
// NON_MATCHING(minor): block layout and register allocation: the borrowed check returns through
// a copy of the epilogue (the original branches to the shared one), the scan compares the 64-bit
// counter for inequality (the original: the low 32 bits, unsigned) and the original reloads the
// text pointer for the copy; the same loads, stores and calls.
void fn_710007f700(CRntString* self) {
    if (self->borrowed)
        return;
    u32 length = self->length;
    if (!length)
        return;
    u32 i = 0;
    do {
        if (!IsTrimSpace(self->data[i])) {
            if (i == 0)
                return;
            break;
        }
        ++i;
    } while (i < length);
    if (length < 0x100)
        AssignShort(self, self->data + i, length - i);
    else
        AssignLong(self, self->data + i, length - i);
}

// Removes trailing spaces, tabs and line breaks (a borrowed string is left alone).
// NON_MATCHING(minor): scheduling and register allocation; the original addresses the scan from
// data - 1 and reloads the text pointer for the copy (nothing writes it in between).
void fn_710007f8e4(CRntString* self) {
    if (self->borrowed)
        return;
    s32 length = self->length;
    if (length <= 0)
        return;
    s64 n = length;
    do {
        if (!IsTrimSpace(self->data[n - 1]))
            break;
        --n;
    } while (n > 0);
    s32 count = (s32)n;
    if (count == length)
        return;
    if (count < 0x100)
        AssignShort(self, self->data, count == -1 ? length : count);
    else
        AssignLong(self, self->data, count);
}
