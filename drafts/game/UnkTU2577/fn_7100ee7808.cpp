// Unit 2577 (CSubAreaManager::CTunableSubAreaManager): the files a change of sub-area loads and unloads. The
// previous sub-area's files the current one lacks go to `unload`, the current one's the previous lacked go to
// `load`; with `all`, the two lists are the whole of each. Names and layouts are placeholders.
#include <new>

#include "base/UnkResPathVec.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU2577.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;

struct UnkIdList;

// The part of a sub-area read here: its files.
struct UnkSubAreaFiles_lee7808 {
    u8 pad_0x0[0xd8];
    UnkResPathVec files;  // 0xd8
};

// The id's pooled string, counting the read.
inline const UnkStringData* CountedString(const CFilePathStrId& id) {
    ++id.data->reads;
    return id.data;
}

inline void Clear(UnkResPathVec* v) {
    for (u64 i = 0; i < v->count; ++i)
        v->data[i].~CFilePathStrId();
    v->count = 0;
}

inline void Release(UnkResPathVec* v) {
    if (v->data) {
        Clear(v);
        v->allocator->vf_0x48(v->data);
        v->capacity = 0;
    }
}

inline void CopyFiles(UnkResPathVec* out, const UnkSubArea* area) {
    Clear(out);
    fn_71000d0958(out, &reinterpret_cast<const UnkSubAreaFiles_lee7808*>(area)->files);
}

inline CFilePathStrId* Find(const UnkResPathVec& v, const CFilePathStrId& id) {
    for (u64 i = 0; i < v.count; ++i)
        if (v.data[i] == id)
            return &v.data[i];
    return &v.data[v.count];
}

// Appends a counted copy of `id` (PushBack's growth, including its hang).
inline void Append(UnkResPathVec* v, const CFilePathStrId& id) {
    u32 n = v->count;
    u32 c = v->capacity;
    if (c <= n && v->step != 0)
        c = (n / v->step + 1) * v->step;
    if ((s32)c <= (s32)n)
        for (;;) {
        }
    fn_7100018450(v, c);
    new (&v->data[v->count]) CFilePathStrId(id);
    ++v->count;
}

// Whether `id` is the table's own path (counting the reads, as the original does).
inline bool IsTablePath(const CFilePathStrId& id) {
    const UnkStringData* table = CountedString(GameManagerIdTable().field_0x5f8);
    CountedString(id);
    return CountedString(id)->Key() == table->Key();
}

void fn_7100ee7808(CSubAreaManager* self, UnkSubArea* prev, UnkSubArea* cur, UnkIdList* load, UnkIdList* unload,
                   bool all) {
    UnkResPathVec* loadPaths = reinterpret_cast<UnkResPathVec*>(load);
    UnkResPathVec* unloadPaths = reinterpret_cast<UnkResPathVec*>(unload);
    Clear(loadPaths);
    Clear(unloadPaths);

    UnkResPathVec curFiles(0xf);
    if (cur)
        CopyFiles(&curFiles, cur);
    UnkResPathVec prevFiles(0xf);
    if (prev)
        CopyFiles(&prevFiles, prev);

    if (all) {
        fn_71000d0958(loadPaths, &curFiles);
        fn_71000d0958(unloadPaths, &prevFiles);
    } else {
        for (CFilePathStrId* it = prevFiles.data; it != prevFiles.data + prevFiles.count; ++it) {
            if (IsTablePath(*it))
                continue;
            if (Find(curFiles, *it) == &curFiles.data[curFiles.count])
                Append(unloadPaths, *it);
        }
        for (CFilePathStrId* it = curFiles.data; it != curFiles.data + curFiles.count; ++it) {
            if (IsTablePath(*it))
                continue;
            if (Find(prevFiles, *it) == &prevFiles.data[prevFiles.count])
                Append(loadPaths, *it);
        }
    }

    Release(&prevFiles);
    Release(&curFiles);
}
