// The string-id constructors, out of line: from a C string (UnkStrIdBase(const char*, bool),
// fn_71000003d4), which game code calls for every CStrId and UnkStrId it builds from a literal,
// and from a CRntString (fn_7100000850), which reuses the string's cached CRC-64 as the pool hash.
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"

using base::global::CRntString;
using base::global::UnkStrIdBase;
using base::global::UnkStrIdInline;

void fn_71000003d4(UnkStrIdBase* self, const char* str, bool flag) {
    new (self) UnkStrIdBase(UnkStrIdInline(), str, flag);
}

// The empty string the constructor passes to the pool when the CRntString has no text.
extern const char g_unk_710162e2ed[] __attribute__((visibility("hidden")));

void fn_7100000850(UnkStrIdBase* self, const CRntString& str, bool flag) {
    self->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    const char* text = str.data ? str.data : g_unk_710162e2ed;
    u32 length = str.length;
    if (length) {
        u64 hash = str.key;
        self->data = fn_71000804f8(g_unk_7101d47108, hash);
        if (self->data)
            ++self->data->ref_count;
        else
            self->data = fn_71000801e4(g_unk_7101d47108, hash, text, length, flag);
    }
    if (!self->data)
        self->data = &g_unk_7101d47108->empty;
}
