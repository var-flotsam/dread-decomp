// CThreadSafeDevice (unit 68): splits a file-path id "device:path" into the device's string id and
// the path after the colon; a path without a colon has no device, and the empty path gives two
// empty ids. Names and layouts are placeholders.
#include <string.h>

#include "../../../game_aca57c/UnkBufferString.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/file/IDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

namespace {

// A pooled file path: its text (a CRntString at +0x10) and how often the text was read (+0x44).
struct UnkPooledPath_l08d99c {
    u8 pad_0x0[0x10];
    CRntString text;
    u32 field_0x40;
    s32 reads;
};
static_assert(__builtin_offsetof(UnkPooledPath_l08d99c, reads) == 0x44, "UnkPooledPath layout");

// The path's text, counting the read.
inline const char* Chars(const CFilePathStrId& id) {
    auto* data = reinterpret_cast<UnkPooledPath_l08d99c*>(id.data);
    ++data->reads;
    const char* s = data->text.data;
    return s ? s : g_unk_710162e2ed;
}

using UnkString200_l08d99c = UnkBufferString<0x200, g_unk_7101a1a5a8>;

// A counted part of a string, on 0x200 bytes of inline storage.
struct UnkPart200_l08d99c : UnkString200_l08d99c {
    __attribute__((always_inline)) UnkPart200_l08d99c(const char* str, u32 length) {
        fn_710007e7f8(Str(), str, length);
    }
};

// A string id built from a string's text, out of line (fn_7100000850), moved out of.
struct UnkTextStrId_l08d99c {
    UnkStringData* data;

    explicit UnkTextStrId_l08d99c(const CRntString& text) {
        fn_7100000850(reinterpret_cast<base::global::UnkStrIdBase*>(this), text, true);
    }
    ~UnkTextStrId_l08d99c() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = &pool->empty;
        }
    }
};

// Releases `id` (leaving the pool's empty string), then takes `other`'s string, leaving `other` empty.
template <UnkStringPool*& Pool, class Id, class Other>
inline void ReleaseAndTake(Id* id, Other&& other) {
    if (UnkStringPool* pool = Pool) {
        if (id->data && id->data != &pool->empty) {
            --id->data->ref_count;
            if (id->data->ref_count == 0) {
                fn_71000803a4(pool, id->data);
                pool = Pool;
            }
        }
        id->data = &pool->empty;
    }
    id->data = other.data;
    other.data = &Pool->empty;
}

}  // namespace

void fn_710008d99c(const CFilePathStrId* path, CStrId* id, CFilePathStrId* file) {
    if (path->data == &g_unk_7101d47200->empty) {
        *id = g_unk_7101d592c8;
        *file = g_unk_7101d470e8;
        return;
    }
    const char* colon = strchr(Chars(*path), ':');
    if (!colon) {
        *id = g_unk_7101d592c8;
        *file = *path;
        return;
    }
    UnkString200_l08d99c full(Chars(*path));
    u32 length = colon - Chars(*path);
    if (length == u32(-1))  // no length: the whole string
        length = full.Str()->length;
    const char* text = full.Str()->data;
    UnkPart200_l08d99c device(text, length);
    ReleaseAndTake<g_unk_7101d47108>(id, UnkTextStrId_l08d99c(*device.Str()));
    ReleaseAndTake<g_unk_7101d47200>(file, CFilePathStrId(colon + 1, true));
}
