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

#include "base/UnkVector.h"
#include "types.h"

// The empty string the copies use; never freed.
extern const char g_unk_7101647847[] __attribute__((visibility("hidden")));

namespace base::global {

class CRntString {
public:
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
void fn_710007ecd4(base::global::CRntString* self);
// Copies [str, str + length) in (fn_710007ebc8).
void fn_710007ebc8(base::global::CRntString* self, const char* str, u32 length, s64 key);
void fn_710007f3e4(base::global::CRntString* self, const char* str, u32 length);  // appends
// Appends one character (through fn_710007f3e4); returns the string.
base::global::CRntString* fn_710007f5ac(base::global::CRntString* self, char c);

// Builds an empty string that allocates from `allocator` (fn_710007e6e4): the first step of
// constructing a string with inline storage, whose allocator object follows the string (the
// callers set that object's vtable afterwards). Leaves owns_allocator and borrowed clear.
void fn_710007e6e4(base::global::CRntString* self, base::UnkAllocator* allocator);
// Assigns a C string (fn_710007e77c) or another string's text and key (fn_710007eaa0), both
// through fn_710007ebc8.
void fn_710007e77c(base::global::CRntString* self, const char* str);
void fn_710007eaa0(base::global::CRntString* self, const base::global::CRntString* other);
// Move-constructs from `other`, which is left empty: the heap copy and allocator are taken over,
// unless `other` allocates from an allocator it doesn't own (inline storage), when the text is
// copied instead.
void fn_710007eab4(base::global::CRntString* self, base::global::CRntString* other);
// Clears the string: frees the heap copy (unless borrowed) and resets data, length and key; the
// allocator is kept. The first step of destroying a string with inline storage, before
// fn_710007ecd4.
void fn_710007ed40(base::global::CRntString* self);
