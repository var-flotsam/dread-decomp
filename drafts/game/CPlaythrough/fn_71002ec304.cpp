// Unit 424 (CPlaythrough's neighbours): builds the lower-case file path "rom:" + base + the owner's
// name + '/' + the entry's record name + '/' + its file name, for the entry stored under a key, and
// returns it as a file-path id (the empty one when there is none). Names and layouts are placeholders.
#include <string.h>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/UnkCloneCalls.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkTU57.h"
#include "types.h"
#include "../../../src/game_aca57c/UnkBufferString.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkFilePathStrIdBase;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::reflection::CClass;

__attribute__((visibility("hidden"))) extern CRntString g_unk_71726ad748;
extern const char g_unk_710164784b;

struct UnkDefObject_l2ec304 {
    virtual const CClass* vf_0x0();
};

// What fn_71002f2d30 finds for an entry's name: two named parts of the path.
struct UnkPathRecord_l2ec304 {
    u64 field_0x0;
    CStrId folder;  // 0x8
    CStrId file;    // 0x10
};

struct UnkEntry_l2ec304 {
    u64 field_0x0;
    CStrId name;  // 0x8
};

UnkPathRecord_l2ec304* fn_71002f2d30(UnkDefObject_l2ec304* def, const CStrId* name);

struct UnkEntriesTag_l2ec304;

struct UnkPathOwner_l2ec304 {
    u8 pad_0x0[0x58];
    CStrId name;  // 0x58
    u8 pad_0x60[0x10];
    UnkDefObject_l2ec304* object;  // 0x70
    base::UnkKeyedArray<UnkEntriesTag_l2ec304, u64, UnkEntry_l2ec304*> entries;  // 0x78
};
static_assert(__builtin_offsetof(UnkPathOwner_l2ec304, entries) == 0x78, "UnkPathOwner_l2ec304 layout");

// The id's string, counting the read.
inline const CRntString* CountedString(const CStrId& id) {
    ++id.data->reads;
    return &id.data->String();
}

// The string's text, or the empty text.
inline const char* CStr(const CRntString* str) {
    return str->data ? str->data : g_unk_710162e2ed;
}

// Copies `src` into `dst` with A-Z turned into a-z.
inline void CopyLower_l2ec304(char* dst, const char* src) {
    dst[0] = 0;
    u32 length = static_cast<u32>(strlen(src));
    if (length) {
        for (u32 i = 0; i < length; ++i) {
            char c = src[i];
            if (static_cast<u32>(c - 'A') < 26)
                c += 0x20;
            dst[i] = c;
        }
        dst[length] = 0;
    }
}

// A file-path id built from a string.
struct UnkPathId_l2ec304 {
    UnkFilePathStrIdBase id;
    explicit UnkPathId_l2ec304(const CRntString& str) { fn_710001a270(&id, str, true); }
};

// Releases `dst`'s id and takes `src`'s; `src` gets the empty id of the pool loaded first.
inline void TakePathId(CFilePathStrId& dst, UnkFilePathStrIdBase&& src) {
    UnkStringPool* pool = g_unk_7101d47200;
    if (pool) {
        UnkStringData* data = dst.data;
        if (data && data != &pool->empty && --data->ref_count == 0) {
            StringPool_Release(pool, data);
            pool = g_unk_7101d47200;
        }
    }
    dst.data = src.data;
    src.data = &pool->empty;
}

CFilePathStrId fn_71002ec304(UnkPathOwner_l2ec304* self, const u64* key) {
    CFilePathStrId result(g_unk_7101d470e8);
    UnkEntry_l2ec304** entry = self->entries.Find(*key);
    if (entry != self->entries.End()) {
        UnkDefObject_l2ec304* object = self->object;
        UnkDefObject_l2ec304* def = object && fn_710009d50c(object->vf_0x0(), fn_71002effc4()) ? object : nullptr;
        if (UnkPathRecord_l2ec304* record = fn_71002f2d30(def, &(*entry)->name)) {
            char lower[0x80];
            UnkBufferString<0x80, g_unk_7101a1b750> path("rom:");
            CRntString_Append(path.Str(), &g_unk_71726ad748);
            CRntString_Append(path.Str(), CountedString(self->name));
            char separator = g_unk_710164784b;
            CRntString_AppendChar(path.Str(), separator);
            CRntString_Append(path.Str(), CountedString(record->folder));
            CRntString_AppendChar(path.Str(), separator);
            CopyLower_l2ec304(lower, record->file.Text());
            {
                UnkBufferString<0x80, g_unk_7101a1b750> lowered(lower);
                CRntString_AppendCString(path.Str(), CStr(lowered.Str()));
            }
            TakePathId(result, UnkPathId_l2ec304(*path.Str()).id);
        }
    }
    return result;
}
