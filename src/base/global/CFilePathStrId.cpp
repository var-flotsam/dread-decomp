// The file-path string id's constructor from a CRntString (fn_710001a270), out of line: the
// counterpart of fn_7100000850 (src/base/global/fn_71000003d4.cpp) on the file-path pool.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"

void fn_710001a270(base::global::UnkFilePathStrIdBase* self, const base::global::CRntString& str, bool flag) {
    self->data = nullptr;
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    const char* text = str.data ? str.data : g_unk_710162e2ed;
    u32 length = str.length;
    if (length) {
        u64 hash = str.key;
        self->data = fn_71000804f8(g_unk_7101d47200, hash);
        if (self->data)
            ++self->data->ref_count;
        else
            self->data = fn_71000801e4(g_unk_7101d47200, hash, text, length, flag);
    }
    if (!self->data)
        self->data = &g_unk_7101d47200->empty;
}

// The constructor from a string (fn_71000166c8): the inline definition's out-of-line copy.
base::global::CFilePathStrId::CFilePathStrId(const char* str, bool flag)
    : CFilePathStrId(base::global::UnkStrIdInline(), str, flag) {}

// Clang 9 ignores an asm label on a constructor, so the address's symbol aliases the complete one.
#if __clang_major__ <= 9
extern "C" void fn_71000166c8() __attribute__((alias("_ZN4base6global14CFilePathStrIdC2EPKcb")));
#endif
