// build-flags: no-strict-aliasing
// CCharClassFXComponent's unit: the read. After the base, replaces the effect paths with the ones in the
// file, each with two words, and preloads each through the model manager; an entry marked as a list adds
// and preloads every path of that list instead. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkModelManager.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntFile;

// An effect path and a word that starts at zero.
struct UnkFxEntry_l78f700 {
    CFilePathStrId path;
    u64 field_0x8;
};

// Grows the paths' array to hold `capacity` entries.
void fn_710078fe5c(base::global::CRntVector<UnkFxEntry_l78f700>* paths, u32 capacity);

struct UnkCollisionClassDef;
bool fn_71011aced4(UnkCollisionClassDef* self, CRntFile* file);  // the base read

void fn_71011aa0d4(void* unused, u32 kind, const CFilePathStrId* path, u32 count, bool flag);

// A list of effect paths, each at +8 of its 16-byte entry.
struct UnkFxListEntry_l78f9a8 {
    u64 field_0x0;
    CFilePathStrId path;
};

struct UnkFxList_l78f9a8 {
    UnkFxListEntry_l78f9a8* data;
    u32 count;
};

UnkFxList_l78f9a8* fn_71011a7fcc(const CFilePathStrId* path);

struct UnkFXComponent_l78f9a8 {
    void** vtable;
    u8 pad_0x8[0x50];
    base::global::CRntVector<UnkFxEntry_l78f700> paths;  // 0x58
};

static inline UnkModelManager* GetManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

// The entry being added: it takes over the reference of the path it is built from, and hands it on to
// the array.
struct UnkFxNewEntry_l78f9a8 {
    union {
        CFilePathStrId path;
    };
    u64 words;

    UnkFxNewEntry_l78f9a8(CFilePathStrId& from, u64 words) : words(words) {
        path.data = from.data;
        from.data = &g_unk_7101d47200->empty;
    }
    ~UnkFxNewEntry_l78f9a8() { path.~CFilePathStrId(); }
};

// Appends the path with its two words, growing as the array's PushBack does (hanging when it cannot).
static inline void AddPath(base::global::CRntVector<UnkFxEntry_l78f700>& v, const CFilePathStrId& path, u32 a,
                           u32 b) {
    CFilePathStrId copy = path;
    UnkFxNewEntry_l78f9a8 entry(copy, (u64)b << 32 | a);
    u32 n = v.count;
    u32 cap = v.capacity;
    if (cap <= n && v.step != 0)
        cap = v.GrownCapacity(n);
    if ((s32)n >= (s32)cap)
        for (;;) {
        }
    fn_710078fe5c(&v, cap);
    UnkFxEntry_l78f700* slot = &v.data[v.count];
    slot->path.data = entry.path.data;
    entry.path.data = &g_unk_7101d47200->empty;
    slot->field_0x8 = entry.words;
    ++v.count;
}

bool fn_710078f9a8(UnkFXComponent_l78f9a8* self, CRntFile* file) {
    bool result = fn_71011aced4(reinterpret_cast<UnkCollisionClassDef*>(self), file);
    if (!result)
        return result;
    s32 count = 0;
    fn_710007c714(file, &count, 4, 1);
    self->paths.Clear();
    fn_710078fe5c(&self->paths, count);
    for (s32 i = 0; i < count; ++i) {
        const char* name;
        fn_710007cee4(file, &name);
        u32 a;
        fn_710007c714(file, &a, 4, 1);
        u32 b;
        fn_710007c714(file, &b, 4, 1);
        bool is_list;
        fn_710007c714(file, &is_list, 1, 1);
        CFilePathStrId path(name, true);
        if (is_list) {
            GetManager();  // created before the list is looked up
            UnkFxList_l78f9a8* list = fn_71011a7fcc(&path);
            for (UnkFxListEntry_l78f9a8 *it = list->data, *end = it + list->count; it != end; ++it) {
                AddPath(self->paths, it->path, a, b);
                fn_71011aa0d4(GetManager(), b, &it->path, a, false);
            }
        } else {
            AddPath(self->paths, path, a, b);
            fn_71011aa0d4(GetManager(), b, &path, a, false);
        }
    }
    return result;
}
