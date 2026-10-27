// base::file::CThreadSafeDevice's unit (tools/tu_map.py unit 68: 0x710008c7d4-0x710008ef54). Holds
// fn_710008d680, which builds a string id and file-path id pair (base/UnkStrIdPathPair.h) from a
// path. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// The id and path pair (base/UnkStrIdPathPair.h) as its callers declare it (src/game/CScenario.cpp).
struct UnkStrPair_0x10 {
    void* a;
    void* b;
};

struct UnkIdPathPair {
    CStrId id;
    CFilePathStrId path;
};

// Sets `id` and `file` from the file-path id `path` (same unit, not decompiled yet).
void fn_710008d99c(const CFilePathStrId* path, CStrId* id, CFilePathStrId* file);

// The empty id of each pool (the pool created first if there is none yet).
static inline void InitEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}
static inline void InitEmpty(CFilePathStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    id->data = &g_unk_7101d47200->empty;
}

// Builds the pair for `path`: both ids start empty and fn_710008d99c fills them from the path's id.
void fn_710008d680(UnkStrPair_0x10* s, const char* path) {
    UnkIdPathPair* pair = reinterpret_cast<UnkIdPathPair*>(s);
    InitEmpty(&pair->id);
    InitEmpty(&pair->path);
    CFilePathStrId p(path, true);
    fn_710008d99c(&p, &pair->id, &pair->path);
}
