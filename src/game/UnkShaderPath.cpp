// A unit of the shader loading code (tools/tu_map.py: unit 204): the path of a shader binary,
// "system/shd/<name>_nx.bshdat", as a file-path id.
#include <cstring>
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

namespace {

// A string id's data as the name read sees it: the text at +0x10 and a use count at +0x44 that
// every read bumps (a union member: the original reloads the id after the increment).
struct UnkStrIdText {
    u8 pad_0x0[0x10];
    const char* str;
    u8 pad_0x18[0x2c];
    union {
        s32 field_0x44;
    };
};

const char* CStr(const CStrId& id) {
    ++reinterpret_cast<UnkStrIdText*>(id.data)->field_0x44;
    const char* str = reinterpret_cast<UnkStrIdText*>(id.data)->str;
    return str ? str : "";
}

}  // namespace

// The file-path id src/game_s7/fn_71001b1dd0.cpp receives (its own type there: a CFilePathStrId
// held raw); built here by CFilePathStrId's out-of-line constructor (fn_71000166c8).
struct UnkPathId {
    base::global::UnkStringData* data;

    __attribute__((always_inline)) UnkPathId(const char* path) { new (this) CFilePathStrId(path, true); }
    ~UnkPathId();
};

UnkPathId fn_7100169c74(const CStrId& name) {
    char path[0x400];
    strcpy(path, "system/shd/");
    strcat(path, CStr(name));
    strcat(path, "_nx.bshdat");
    return UnkPathId(path);
}
