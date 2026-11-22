// CThreadSafeDevice (unit 68): splits a file-path id "device:dir/name.ext" into the directory (a
// file-path id, with its slash), the name and the extension (string ids); each output may be null.
// Names and layouts are placeholders.
#include "../../../game_aca57c/UnkBufferString.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

namespace {

// The string of an id, counting the read.
template <class Id>
inline const CRntString& IdString(const Id& id) {
    ++id.data->reads;
    return id.data->String();
}

// A counted part of a string, on 0x200 bytes of inline storage.
struct UnkPart200_l08de50 : UnkBufferString<0x200, g_unk_7101a1a5a8> {
    __attribute__((always_inline)) UnkPart200_l08de50(const char* str, u32 length) {
        CRntString_AssignPrefix(Str(), str, length);
    }
};

// An id of the pool `Pool` built from a string's text, out of line (`Build`).
template <UnkStringPool*& Pool, class Base, void (*Build)(Base*, const CRntString&, bool)>
struct UnkTextId_l08de50 {
    UnkStringData* data;

    explicit UnkTextId_l08de50(const CRntString& text) { Build(reinterpret_cast<Base*>(this), text, true); }
    ~UnkTextId_l08de50() {
        if (UnkStringPool* pool = Pool) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = Pool;
                }
            }
            data = &pool->empty;
        }
    }
};
using UnkTextPath_l08de50 =
    UnkTextId_l08de50<g_unk_7101d47200, base::global::UnkFilePathStrIdBase, fn_710001a270>;
using UnkTextStrId_l08de50 = UnkTextId_l08de50<g_unk_7101d47108, base::global::UnkStrIdBase, fn_7100000850>;

// Makes `id` share `other`'s string: the new string gains a reference before the old one is released.
template <UnkStringPool*& Pool, class Id, class Other>
inline void Share(Id* id, const Other& other) {
    UnkStringData* old = id->data;
    if (old != other.data) {
        UnkStringData* d = other.data;
        UnkStringPool* pool = Pool;
        if (d != &pool->empty)
            ++d->ref_count;
        if (id->data != &pool->empty) {
            --id->data->ref_count;
            if (id->data->ref_count == 0)
                fn_71000803a4(pool, id->data);
        }
        id->data = d;
    }
}

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

bool fn_710008de50(const CFilePathStrId* path, CFilePathStrId* dir, CStrId* name, CStrId* ext) {
    const CRntString& text = IdString(*path);
    s32 colon = CRntString_FindChar(&text, ':', 0);
    s32 slash = CRntString_FindLastChar(&text, '/', -1);
    s32 dot = CRntString_FindLastChar(&text, '.', -1);
    if (dir) {
        if (slash >= 0) {
            u32 start = colon + 1;
            Share<g_unk_7101d47200>(dir, UnkTextPath_l08de50(*UnkPart200_l08de50(text.data + start, slash - colon).Str()));
        } else {
            Share<g_unk_7101d47200>(dir, CFilePathStrId(g_unk_7101d470e8));
        }
    }
    if (ext) {
        if (dot >= 0) {
            u32 start = dot + 1;
            u32 length = text.length - start;
            Share<g_unk_7101d47108>(ext, UnkTextStrId_l08de50(*UnkPart200_l08de50(text.data + start, length).Str()));
        } else {
            Share<g_unk_7101d47108>(ext, CStrId(g_unk_7101d470e0));
        }
    }
    if (name) {
        u32 start = slash + 1;
        s32 end = dot == -1 ? (s32)text.length : dot;
        u32 length = end - start;
        ReleaseAndTake<g_unk_7101d47108>(name, UnkTextStrId_l08de50(*UnkPart200_l08de50(text.data + start, length).Str()));
    }
    return true;
}
