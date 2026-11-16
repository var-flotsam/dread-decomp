// Unit 25: resolves a key's id and file-path pair through the resource registry (g_unk_7101d4bbc0): when the
// path is one of the entry's names it is used as it is; otherwise the entry's directory (or the key's path) is
// joined with each of the entry's names and the newest file the file registry reports wins. Names and layouts
// are placeholders.
#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/animation/CBlendSpace1D.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "base/animation/CAnimationResource.h"
#include "game/UnkTU27.h"
#include "game/UnkTU57.h"
#include "../../game_aca57c/UnkBufferString.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkFilePathStrIdBase;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkRegistry_7100d92158;
extern char g_unk_7101d4e578[] __attribute__((visibility("hidden")));
bool fn_710007fbc0(const CRntString* self, const CRntString* other);

// An id as it is read here: its pooled string.
struct UnkIdWord_l01a39c {
    UnkStringData* data;
};

// The string of an id, counting the read.
template <class Id>
inline const CRntString& IdString(const Id& id) {
    ++id.data->reads;
    return id.data->String();
}

struct UnkKey_l01a39c {
    UnkIdWord_l01a39c id;
    UnkFilePathStrIdBase path;  // 0x8
};

struct UnkNames_l01a39c {
    u8 pad_0x0[0x30];
    UnkIdWord_l01a39c* names;  // 0x30
    u32 name_count;            // 0x38
};

struct UnkSearchEntry_l01a39c {
    UnkNames_l01a39c* names;
    u8 pad_0x8[0x18];
    UnkIdWord_l01a39c directory;  // 0x20, a file-path id
};

// The string id of a pair: a copy takes a reference before the old string is released.
struct UnkPairId_l01a39c {
    UnkStringData* data;

    UnkPairId_l01a39c& operator=(const UnkPairId_l01a39c& other) {
        UnkStringData* old = data;
        if (old != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47108;
            if (d != &pool->empty)
                ++d->ref_count;
            if (old != &pool->empty) {
                if (--old->ref_count == 0)
                    fn_71000803a4(pool, old);
            }
            data = d;
        }
        return *this;
    }
};

// The file-path id of a pair, assigned as CFilePathStrId is.
struct UnkPairPath_l01a39c {
    UnkStringData* data;

    UnkPairPath_l01a39c& operator=(const UnkPairPath_l01a39c& other) {
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
};

// A file-path id built from a string (fn_710001a270).
struct UnkBuiltPath_l01a39c : UnkFilePathStrIdBase {
    explicit UnkBuiltPath_l01a39c(const CRntString& str) { fn_710001a270(this, str, true); }
    ~UnkBuiltPath_l01a39c() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            UnkStringData* d = data;
            if (d && d != &pool->empty) {
                if (--d->ref_count == 0) {
                    fn_71000803a4(pool, d);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkPair_l01a39c {
    UnkPairId_l01a39c id;
    UnkPairPath_l01a39c path;

    UnkPair_l01a39c() { fn_710008d2a0(reinterpret_cast<UnkPathPairTemp_b45a1*>(this)); }
    UnkPair_l01a39c(const void* tag, const UnkFilePathStrIdBase& path) {
        fn_710008d88c(reinterpret_cast<UnkStrIdPathPair*>(this), tag, &path);
    }
    UnkPair_l01a39c(const UnkPair_l01a39c& other) {
        fn_710008d830(reinterpret_cast<UnkStrIdPathPair*>(this), reinterpret_cast<const UnkStrIdPathPair*>(&other));
    }
    ~UnkPair_l01a39c() { fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(this)); }
};

// What the file registry reports about a path.
struct UnkFileInfo_l01a39c {
    bool found = false;
    u32 flags = 0;  // 0x4
    u64 field_0x8;
    u64 field_0x10;
    s64 time;  // 0x18
};

struct UnkFileQuery_l01a39c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(const UnkPair_l01a39c* pair, UnkFileInfo_l01a39c* info, const u32* mask);
};

// A zero whose constructor isn't constexpr, so the static is guarded.
struct UnkQueryZero_l01a39c {
    u32 value;
    UnkQueryZero_l01a39c() : value(0) {}
};

inline const UnkQueryZero_l01a39c& UnkQueryZero() {
    static UnkQueryZero_l01a39c s_value asm("g_unk_7101d4e560");
    return s_value;
}

// The complement of that zero's low five bits.
inline u32 UnkQueryDefaultMask() {
    static u32 s_mask asm("g_unk_7101d4e550") = ~UnkQueryZero().value & 0x1f;
    return s_mask;
}

UnkPair_l01a39c fn_710001a39c(const UnkKey_l01a39c* key) {
    UnkSearchEntry_l01a39c* entry = reinterpret_cast<UnkSearchEntry_l01a39c*>(
        fn_7100020320(g_unk_7101d4bbc0, reinterpret_cast<const UnkRegistryKey*>(key)));
    UnkNames_l01a39c* names = entry->names;
    for (UnkIdWord_l01a39c *p = names->names, *e = p + names->name_count; p != e; ++p) {
        if (fn_710007fbc0(&IdString(key->path), &IdString(*p)))
            return UnkPair_l01a39c(g_unk_7101d4e578, key->path);
    }

    UnkBufferString<0x100, g_unk_7101a194b8> dir;
    if (entry->directory.data != &g_unk_7101d47200->empty) {
        fn_710007eaa0(dir.Str(), &IdString(entry->directory));
        // The key's file name: what follows the last '/' of its path.
        const CRntString& path = IdString(key->path);
        u32 start = fn_710007fb10(&IdString(key->path), '/', -1) + 1;
        u32 length = path.length - start;
        const char* text = path.data + start;
        UnkBufferString<0x100, g_unk_7101a194b8> name;
        fn_710007e7f8(name.Str(), text, length);
        fn_710007f3b4(dir.Str(), name.Str());
    } else {
        fn_710007eaa0(dir.Str(), &IdString(key->path));
    }

    UnkPair_l01a39c result;
    s64 newest = 0;
    names = entry->names;
    for (UnkIdWord_l01a39c *p = names->names, *e = p + names->name_count; p != e; ++p) {
        UnkPair_l01a39c candidate(g_unk_7101d4e578, UnkBuiltPath_l01a39c(fn_710007eee0(dir.Str(), &IdString(*p))));
        UnkFileInfo_l01a39c info;
        UnkRegistry_0x70* reg = g_unk_7101d4b5a0;
        if (!reg) {
            reg = static_cast<UnkRegistry_0x70*>(fn_71000839a0(0x70, 0x3f));
            fn_7100087388(reg);
            g_unk_7101d4b5a0 = reg;
        }
        u32 mask = UnkQueryDefaultMask();
        reinterpret_cast<UnkFileQuery_l01a39c*>(reg)->vf_0x20(&candidate, &info, &mask);
        if (newest == 0 || (info.found && info.time > newest)) {
            result.id = candidate.id;
            result.path = candidate.path;
            newest = (info.flags & 4) ? info.time : 0;
        }
    }
    return result;
}
