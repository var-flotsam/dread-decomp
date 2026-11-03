// base::file::CThreadSafeDevice's unit (tools/tu_map.py unit 68): two more builders of the string
// id and file-path id pair (base/UnkStrIdPathPair.h), as fn_710008d680
// (src/base/file/CThreadSafeDevice.cpp): both ids start empty, then fn_710008d99c fills them from
// a file-path id built from the text. fn_710008d2ec takes the text of a file-path id (counting a
// use in its pool entry, +0x44), fn_710008d4c0 a string whose text is its first word. Names and
// layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// As their callers declare them (src/game/CSubAreaManager.cpp, src/game/UnkObjectFileLoader.cpp).
struct UnkStrPair_0x10;
struct UnkEntry_0x30;

void fn_710008d99c(const CFilePathStrId* path, CStrId* id, CFilePathStrId* file);

namespace {

struct UnkIdPathPair_b49a1 {
    CStrId id;
    CFilePathStrId path;
};

// The use count may alias the id: the original reloads the id's entry after counting the use.
typedef u32 __attribute__((may_alias)) UnkAliasU32_b49a1;

// A pool entry as the file-path builder reads it: the text at +0x10 and a use count at +0x44.
struct UnkPathEntry_b49a1 {
    u8 unk_0x0[0x10];
    const char* text;
    u8 unk_0x18[0x2c];
    u32 uses;
};

inline void InitEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}
inline void InitEmpty(CFilePathStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    id->data = &g_unk_7101d47200->empty;
}

}  // namespace

void fn_710008d2ec(UnkStrPair_0x10* s, const CFilePathStrId* path) {
    UnkIdPathPair_b49a1* pair = reinterpret_cast<UnkIdPathPair_b49a1*>(s);
    InitEmpty(&pair->id);
    InitEmpty(&pair->path);
    ++*reinterpret_cast<UnkAliasU32_b49a1*>(&reinterpret_cast<UnkPathEntry_b49a1*>(path->data)->uses);
    const char* text = reinterpret_cast<UnkPathEntry_b49a1*>(path->data)->text;
    CFilePathStrId p(text ? text : g_unk_710162e2ed, true);
    fn_710008d99c(&p, &pair->id, &pair->path);
}

void fn_710008d4c0(UnkStrPair_0x10* s, const UnkEntry_0x30* e) {
    UnkIdPathPair_b49a1* pair = reinterpret_cast<UnkIdPathPair_b49a1*>(s);
    InitEmpty(&pair->id);
    InitEmpty(&pair->path);
    const char* text = *reinterpret_cast<const char* const*>(e);
    CFilePathStrId p(text ? text : g_unk_710162e2ed, true);
    fn_710008d99c(&p, &pair->id, &pair->path);
}
