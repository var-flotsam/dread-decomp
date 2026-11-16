// Unit 99 of tools/tu_map.py (base::snd::CSoundSystemATK's range): fn_7100100804 takes a use of the sound
// file `path` names (made absolute with a leading '/', then sanitised by fn_71000f63b0): when the file map
// knows it, the sound thread is told (command 0x1d with its key), the file's first user adds its key to the
// list, and the use is counted. Whether the map knew it; true while the global flag g_unk_7108b6ab60 is
// set. Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/snd/UnkSoundSystemATK.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU57.h"
#include "game/UnkTU99.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::UnkFilePathStrIdBase;
using base::global::UnkStringPool;
using base::snd::UnkSoundSystemATK;

// A file-path id's pooled data as this reads it: its text (whose CRC-64 is the file's key) and a count of
// the reads of that text.
struct UnkPathData_l100804 {
    u8 pad_0x0[0x10];
    CRntString text;  // 0x10
    u32 field_0x40;
    s32 reads;  // 0x44
};

static inline UnkPathData_l100804* PathData(const UnkFilePathStrIdBase& id) {
    return reinterpret_cast<UnkPathData_l100804*>(id.data);
}

// The text of a file-path id, counting the read.
static inline const CRntString& PathString(const CFilePathStrId& id) {
    ++PathData(id)->reads;
    return PathData(id)->text;
}
static inline const char* PathChars(const CFilePathStrId& id) {
    const char* s = PathString(id).data;
    return s ? s : g_unk_710162e2ed;
}

// A file-path id built from a string out of line (fn_710001a270) and released like one.
struct UnkBuiltPathId_l100804 : UnkFilePathStrIdBase {
    explicit UnkBuiltPathId_l100804(const CRntString& str) { fn_710001a270(this, str, true); }
    ~UnkBuiltPathId_l100804() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
    const CFilePathStrId& Id() const { return static_cast<const CFilePathStrId&>(static_cast<const UnkFilePathStrIdBase&>(*this)); }
};

// A loaded file of the sound system's map: its key, how many users it has, and the next node of its
// bucket's chain.
struct UnkSoundFileNode_l100804 {
    u64 key;    // 0x0
    s32 users;  // 0x8
    u8 pad_0xc[0x14];
    UnkSoundFileNode_l100804* next;  // 0x20
};

// The members of the sound system the file functions touch: the keys of the files in use and the map of
// the loaded files by key.
struct UnkSoundSystemFiles_l100804 {
    u8 pad_0x0[0x490];
    base::global::CRntVector<u64> keys;  // 0x490
    u8 pad_0x4b8[0x10];
    UnkSoundFileNode_l100804** buckets;  // 0x4c8
    u32 used;                            // 0x4d0
    u32 bucket_count;                    // 0x4d4
    u8 pad_0x4d8[0x400568 - 0x4d8];
    nn::os::MutexType mutex;  // 0x400568
};
static_assert(__builtin_offsetof(UnkSoundSystemFiles_l100804, buckets) == 0x4c8, "layout");

bool fn_7100100804(UnkSoundSystemATK* self, const CFilePathStrId& path) {
    bool found = true;
    if (!(g_unk_7108b6ab60 & 1)) {
        fn_7100132cd0();
        UnkSoundSystemFiles_l100804* files = reinterpret_cast<UnkSoundSystemFiles_l100804*>(self);
        nn::os::MutexType* mutex = &files->mutex;
        fn_710006691c(mutex);
        found = false;
        {
            CRntString name(PathString(path));
            if (path.data != &g_unk_7101d47200->empty && PathChars(path)[0] != '/') {
                fn_710007e77c(&name, "/");
                fn_710007f564(&name, PathChars(path));
            }
            CFilePathStrId id(fn_71000f63b0(UnkBuiltPathId_l100804(name).Id()));
            if (files->used) {
                u64 key = PathData(id)->text.key;
                u32 key32 = key;
                UnkSoundFileNode_l100804** bucket = key32 % files->bucket_count + files->buckets;
                if (bucket) {
                    for (UnkSoundFileNode_l100804* node = *bucket; node; node = node->next) {
                        if (node->key == key) {
                            u64 command = key;
                            fn_71000faac4(self, 0x1d, &command);
                            UnkSoundFileNode_l100804* n = *(key32 % files->bucket_count + files->buckets);
                            while (n->key != key)
                                n = n->next;
                            if (n->users == 0)
                                files->keys.PushBack(key);
                            ++n->users;
                            found = true;
                            break;
                        }
                    }
                }
            }
        }
        fn_7100066920(mutex);
    }
    return found;
}