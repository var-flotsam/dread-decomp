// base::global::CRntString: assignment of a counted prefix of a C string (fn_710007e7f8).
#include <string.h>

#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"

using base::UnkAllocator;
using base::global::CRntString;

// A string with 0x100 bytes of inline storage: its allocator (the object at +0x30, a block of
// the same sort as the one in src/game/GUI/CIngameMenu/fn_7100b2de48.cpp) hands out that storage.
struct UnkInlineString256 {
    CRntString str;
    char* allocator_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;
};

// The string's clear (fn_710007ed40) and destructor (fn_710007ecd4) are one body: both are inlined
// into the temporaries' destruction here, one after the other.
static inline void ClearInline(CRntString& s) {
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

static inline void ReleaseTemporary(CRntString& s) {
    ClearInline(s);
    ClearInline(s);
}

// NON_MATCHING(minor): the two temporaries' destruction (the string's clear and destructor, both
// inlined) is emitted in a different shape: the original keeps the old data pointer in a register
// and tests it again after the clear. The behaviour is the same.
void fn_710007e7f8(CRntString* self, const char* str, u32 length) {
    if (self->borrowed)
        return;
    if (length == 0) {
        if (self->data) {
            if (self->data != g_unk_7101647847)
                self->GetAllocator()->vf_0x48(self->data);
            self->data = nullptr;
            self->length = 0;
            self->key = -1;
        }
        return;
    }
    u32 n = str ? strlen(str) : 0;
    if (length <= n)
        n = length;
    if (self->data == str) {
        if (n < 0x100) {
            UnkInlineString256 tmp;
            tmp.str.owns_allocator = false;
            tmp.str.borrowed = false;
            tmp.str.data = nullptr;
            tmp.str.length = 0;
            tmp.str.key = -1;
            tmp.str.allocator = reinterpret_cast<UnkAllocator*>(&tmp.allocator_vtable);
            tmp.field_0x138 = 0;
            tmp.field_0x140 = 0;
            tmp.allocator_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
            fn_710007e7f8(&tmp.str, str, n);
            fn_710007ebc8(self, tmp.str.data, tmp.str.length, tmp.str.key);
            ReleaseTemporary(tmp.str);
        } else {
            if (n == 0xffffffff)
                n = self->length;
            CRntString tmp;
            tmp.owns_allocator = false;
            tmp.borrowed = false;
            tmp.data = nullptr;
            tmp.length = 0;
            tmp.key = -1;
            tmp.allocator = nullptr;
            fn_710007e7f8(&tmp, str, n);
            fn_710007ebc8(self, tmp.data, tmp.length, tmp.key);
            ReleaseTemporary(tmp);
        }
    } else {
        fn_710007ebc8(self, nullptr, n, 0);
        strncpy(self->data, str, n);
        self->data[n] = 0;
        self->length = n;
        self->key = fn_7100001570(self->data, n);
    }
}
