#pragma once

// base::global::CRntString: a string value type (0x30 bytes). The name comes from reflection
// (fn_7100007f84 describes it). Reflected default values and descriptions build one from a
// literal (fn_710007e704) or copy a global one (fn_710007ea60), and destroy it with
// fn_710007ecd4. The same three functions build the keys of the named registry
// (UnkNamedEntry in base/UnkNamedRegistry.h, the same layout). Member names are placeholders
// inferred from the code.
//
// The string is a heap copy from `allocator` (the default allocator when null), unless
// `borrowed` is set: then it points at memory the string doesn't own (fn_710007ecb4) and
// assignments leave it alone. An empty string is null or points at g_unk_7101647847.

#include "base/global/CRntVector.h"
#include "types.h"

// The empty string the copies use; never freed.
extern const char g_unk_7101647847[] __attribute__((visibility("hidden")));
// The vtable of the allocator object that follows a string with inline storage: callers store
// `g_unk_7101a194b8 + 2` (the address point) after fn_710007e6e4. Reached through the GOT slot at
// 0x7101cbb250, which is not itself the vtable.
extern void* g_unk_7101a194b8[];
// The same for a string with 0x40 bytes of inline storage.
extern void* g_unk_7101a1b678[];

namespace base::global {

class CRntString {
public:
    // The global empty string (g_unk_7101d58d10, below): default strings are copied from it.
    static CRntString& Empty();

    // Leaves owns_allocator unset (inlined in fn_710007eee0).
    CRntString() {
        borrowed = false;
        data = nullptr;
        length = 0;
        key = -1;
        allocator = nullptr;
    }
    CRntString(const char* str);           // fn_710007e704
    CRntString(const CRntString& other);   // fn_710007ea60
    CRntString(CRntString&& other);        // fn_710007eab4
    ~CRntString();                         // fn_710007ecd4

    UnkAllocator* GetAllocator() const {
        UnkAllocator* a = allocator;
        if (!a)
            a = fn_7100083990();
        return a;
    }

    char* data;
    u32 length;
    UnkAllocator* allocator;  // +0x10
    bool owns_allocator;      // +0x18: copies take the allocator only when set
    s64 key;                  // +0x20: the string's CRC-64, -1 when empty
    bool borrowed;            // +0x28
};
static_assert(sizeof(CRntString) == 0x30, "CRntString layout");

}  // namespace base::global

// The destructor, as a function (fn_710007ecd4): frees the heap copy. It is also what
// __cxa_atexit registers for global strings, and the second step of destroying a string with
// inline storage (after fn_710007ed40).
void CRntString_Destroy(base::global::CRntString* self);
// Copies [str, str + length) in (fn_710007ebc8).
void CRntString_SetText(base::global::CRntString* self, const char* str, u32 length, s64 key);
void CRntString_AppendChars(base::global::CRntString* self, const char* str, u32 length);  // appends
// Appends another string's text (fn_710007f3b4, through fn_710007f3e4 with its data and length);
// returns the string.
base::global::CRntString* CRntString_Append(base::global::CRntString* self, const base::global::CRntString* other);
// Whether two strings differ, by their keys (fn_710007f220).
bool CRntString_KeysDiffer(const base::global::CRntString* a, const base::global::CRntString* b);
// Whether the string's text equals a C string (fn_710007f140, src/base/global/CRntString.cpp).
bool CRntString_EqualsCString(const base::global::CRntString* self, const char* str);
// Appends one character (through fn_710007f3e4); returns the string.
base::global::CRntString* CRntString_AppendChar(base::global::CRntString* self, char c);
// Whether the string contains `str` (fn_710007fb9c).
bool CRntString_Contains(const base::global::CRntString* self, const char* str);
// Concatenation: a new string (returned in memory) with a's text followed by b's
// (fn_710007eee0, src/base/global/CRntString.cpp).
base::global::CRntString CRntString_Concat(const base::global::CRntString* a, const base::global::CRntString* b);

// Builds an empty string that allocates from `allocator` (fn_710007e6e4): the first step of
// constructing a string with inline storage, whose allocator object follows the string (the
// callers set that object's vtable afterwards). Leaves owns_allocator and borrowed clear.
void CRntString_InitWithAllocator(base::global::CRntString* self, base::UnkAllocator* allocator);
// Assigns a C string (fn_710007e77c) or another string's text and key (fn_710007eaa0), both
// through fn_710007ebc8.
void CRntString_AssignCString(base::global::CRntString* self, const char* str);
void CRntString_Assign(base::global::CRntString* self, const base::global::CRntString* other);
// Move-constructs from `other`, which is left empty: the heap copy and allocator are taken over,
// unless `other` allocates from an allocator it doesn't own (inline storage), when the text is
// copied instead.
void CRntString_MoveConstruct(base::global::CRntString* self, base::global::CRntString* other);
// Clears the string: frees the heap copy (unless borrowed) and resets data, length and key; the
// allocator is kept. The first step of destroying a string with inline storage, before
// fn_710007ecd4.
void CRntString_Clear(base::global::CRntString* self);

// A global empty string: what a string id with no data reads as (its text, +0x10 of the id's
// data), and the default modifiers passed with a path.
extern base::global::CRntString g_unk_7101d58d10;
inline base::global::CRntString& base::global::CRntString::Empty() { return g_unk_7101d58d10; }
