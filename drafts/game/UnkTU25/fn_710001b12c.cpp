// Unit 25: resolves the file path of a key's id in a table of entries: the result starts as the key's
// id with the empty path; the entry found for the id joins each of two directories (the table's and the
// caller's) with its own directory and the key's file name, then each of its names with that file name, and
// the first path the file registry has becomes the result's. Names and layouts are placeholders.
#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/animation/CAnimationResource.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "game/UnkTU57.h"
#include "../../../src/game_aca57c/UnkBufferString.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkFilePathStrIdBase;
using base::global::UnkStrIdBase;
using base::global::UnkStringData;
using base::global::UnkStringPool;

using UnkPathString_l01b12c = UnkBufferString<0x100, g_unk_7101a194b8>;

// The string of an id, counting the read (the id is read again after the count is written).
template <class Id>
inline const CRntString& IdString(const Id& id) {
    ++id.data->reads;
    return id.data->String();
}

// The string and the text of a pooled string, counting the read.
inline const CRntString& StringOf(UnkStringData* data) {
    ++data->reads;
    return data->String();
}
inline const char* TextOf(UnkStringData* data) {
    ++data->reads;
    return data->text ? data->text : g_unk_710162e2ed;
}

inline bool IsEmpty(const CRntString& str) { return !str.data || !str.data[0]; }

// A file-path id built from a string (fn_710001a270).
struct UnkPathTemp_l01b12c : UnkFilePathStrIdBase {
    explicit UnkPathTemp_l01b12c(const CRntString& str) { fn_710001a270(this, str, true); }
    // Another reference to a path.
    UnkPathTemp_l01b12c(const UnkFilePathStrIdBase& other) : UnkFilePathStrIdBase(other) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    ~UnkPathTemp_l01b12c() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            UnkStringData* d = data;
            if (d && d != &pool->empty) {
                if (--d->ref_count == 0) {
                    StringPool_Release(pool, d);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkPairId_l01b12c {
    UnkStringData* data;

    UnkPairId_l01b12c() = default;
    UnkPairId_l01b12c(const UnkStrIdBase& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
};

struct UnkPairPath_l01b12c {
    UnkStringData* data;

    UnkPairPath_l01b12c() = default;
    UnkPairPath_l01b12c(const UnkFilePathStrIdBase& other) : data(other.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    // Releases the path, then takes the other's, which keeps the empty one.
    UnkPairPath_l01b12c& operator=(UnkPathTemp_l01b12c&& other) {
        UnkStringPool* pool = g_unk_7101d47200;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
        }
        data = other.data;
        other.data = &g_unk_7101d47200->empty;
        return *this;
    }
};

// An id and file-path pair (destroyed by fn_710008d8e8).
struct UnkPair_l01b12c {
    UnkPairId_l01b12c id;
    UnkPairPath_l01b12c path;

    UnkPair_l01b12c(const CStrId& id, const CFilePathStrId& path) : id(id), path(path) {}
    // The pair named by a path (fn_710008d2ec).
    explicit UnkPair_l01b12c(const CFilePathStrId& path) {
        fn_710008d2ec(reinterpret_cast<UnkStrPair_0x10*>(this), &path);
    }
    UnkPair_l01b12c(const UnkPairId_l01b12c* id, const UnkFilePathStrIdBase& path) {
        fn_710008d88c(reinterpret_cast<UnkStrIdPathPair*>(this), id, &path);
    }
    ~UnkPair_l01b12c() { fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(this)); }
};

struct UnkKey_l01b12c {
    CStrId id;
    CFilePathStrId path;  // 0x8
};

struct UnkEntry_l01b12c {
    u8 pad_0x0[0x50];
    CStrId* names;    // 0x50
    u32 name_count;   // 0x58
    u8 pad_0x5c[0x1c];
    UnkFilePathStrIdBase dir;  // 0x78
};
static_assert(sizeof(UnkEntry_l01b12c) == 0x80, "UnkEntry_l01b12c layout");

struct UnkTable_l01b12c {
    UnkEntry_l01b12c* values;
    CStrId* keys;  // 0x8
    u32 count;     // 0x10
    u8 pad_0x14[0x14];
    CFilePathStrId dir;  // 0x28

    // The entry of an id, or the end.
    UnkEntry_l01b12c* Find(const CStrId& id) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == id)
                return &values[i];
        return &values[count];
    }
};

// The registry that reports whether a file exists, created on first use.
inline UnkRegistry_0x70* FileRegistry() {
    UnkRegistry_0x70* reg = g_unk_7101d4b5a0;
    if (!reg) {
        reg = static_cast<UnkRegistry_0x70*>(fn_71000839a0(0x70, 0x3f));
        fn_7100087388(reg);
        g_unk_7101d4b5a0 = reg;
    }
    return reg;
}

inline bool FileExists(const UnkPair_l01b12c& pair) {
    return fn_7100087850(FileRegistry(), reinterpret_cast<const UnkStrPair_0x10*>(&pair));
}

UnkPair_l01b12c fn_710001b12c(UnkTable_l01b12c* self, const UnkKey_l01b12c* key, const CFilePathStrId* dir) {
    UnkPair_l01b12c result(key->id, g_unk_7101d470e8);
    UnkEntry_l01b12c* e = self->Find(key->id);
    if (e == &self->values[self->count])
        return result;

    UnkPair_l01b12c name(key->path);
    UnkPathString_l01b12c found;
    if (e->dir.data != &g_unk_7101d47200->empty) {
        UnkPathTemp_l01b12c dirs[2] = {self->dir, *dir};
        for (const UnkPathTemp_l01b12c& d : dirs) {
            if (result.id.data != &g_unk_7101d47108->empty && result.path.data != &g_unk_7101d47200->empty)
                break;
            if (d.data == &g_unk_7101d47200->empty)
                continue;
            // The directory's text up to and including its last '/'.
            s32 slash = CRntString_FindLastChar(&StringOf(d.data), '/', -1);
            s32 length = slash > 0 ? slash + 1 : -1;
            const CRntString& str = StringOf(d.data);
            if (length == -1)
                length = str.length;
            const char* text = d.data->text;
            UnkPathString_l01b12c path;
            CRntString_AssignPrefix(path.Str(), text, length);
            CRntString_Append(path.Str(), &IdString(e->dir));
            CRntString_Append(path.Str(), &StringOf(name.path.data));
            UnkPair_l01b12c candidate(&name.id, UnkPathTemp_l01b12c(*path.Str()));
            if (FileExists(candidate))
                CRntString_Assign(found.Str(), path.Str());
        }
    }

    if (IsEmpty(*found.Str())) {
        for (CStrId *p = e->names, *end = p + e->name_count; p != end; ++p) {
            UnkPathString_l01b12c path(p->Text());
            CRntString_AppendCString(path.Str(), TextOf(name.path.data));
            UnkPair_l01b12c candidate(&name.id, UnkPathTemp_l01b12c(*path.Str()));
            if (FileExists(candidate)) {
                CRntString_Assign(found.Str(), path.Str());
                break;
            }
        }
    }
    if (!IsEmpty(*found.Str()))
        result.path = UnkPathTemp_l01b12c(*found.Str());
    return result;
}
