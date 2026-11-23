// CActorDef's unit (tools/tu_map.py unit 345): the file path of an actor definition. The name, lower
// cased, plus the "actordef" category's extension is looked for under "actors/<folder>/<name>/charclasses/"
// in each actor folder in turn; the first one the registry knows is the path. Names and layouts are placeholders.
// Needs CFilePathStrId(const CRntString&, bool) asm("fn_710001a270") in base/global/CFilePathStrId.h.
#include "../../../src/game_aca57c/UnkBufferString.h"
#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkStrPair.h"
#include "base/animation/CAnimationResource.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkTU2527.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

struct UnkKey_71011938bc;

using UnkActorName_l25539c = UnkBufferString<0x40, g_unk_7101a1b678>;
using UnkActorFileName_l25539c = UnkBufferString<0x80, g_unk_7101a1b750>;
using UnkActorPath_l25539c = UnkBufferString<0x100, g_unk_7101a194b8>;

// A resource category as registered (game_s7/fn_7100d96670.cpp): its types, each starting with the
// type's file extension id.
struct UnkResCategory_l25539c {
    u8 pad_0x0[0x30];
    const CStrId* types;  // 0x30
};

// The address of a temporary, which lives until the end of the full expression it is made in.
static inline const CRntString* Addr(const CRntString& tmp) { return &tmp; }

// The id's string, counting the read.
static inline const CRntString* UseString(const CStrId& id) {
    ++id.data->reads;
    return &id.data->String();
}

// `text` in lower case (ASCII letters only).
static inline UnkActorName_l25539c LowerCase(const char* text) {
    char lower[0x100];
    lower[0] = 0;
    u32 length = __builtin_strlen(text);
    if (length) {
        for (u32 i = 0; i < length; ++i) {
            char c = text[i];
            lower[i] = (u8)(c - 'A') < 26 ? c + 0x20 : c;
        }
        lower[length] = 0;
    }
    return UnkActorName_l25539c(lower);
}

// Whether the registry (created on first use) knows the file at `path`.
inline bool IsRegistered(const CRntString& path) {
    UnkRegistry_0x70* registry = g_unk_7101d4b5a0;
    if (!registry) {
        registry = static_cast<UnkRegistry_0x70*>(fn_71000839a0(0x70, 0x3f));
        fn_7100087388(registry);
        g_unk_7101d4b5a0 = registry;
    }
    UnkStrPair_0x10 key;
    fn_710008d680(&key, path.data ? path.data : g_unk_710162e2ed);
    bool found = fn_7100087850(registry, &key);
    fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&key));
    return found;
}

// Concatenation (the string operators, out of line).
static inline CRntString operator+(const char* a, const CRntString& b) { return CRntString_ConcatCStringFirst(a, &b); }
static inline CRntString operator+(const CRntString& a, const char* b) { return CRntString_ConcatCString(&a, b); }
static inline CRntString operator+(const CRntString& a, const CRntString& b) { return CRntString_Concat(&a, &b); }

CFilePathStrId fn_710025539c(UnkKey_71011938bc* const* key) {
    const CStrId& id = *reinterpret_cast<const CStrId*>(key);
    UnkActorName_l25539c name = LowerCase(id.Text());
    UnkActorFileName_l25539c file;
    CRntString_Assign(file.Str(), name.Str());
    CRntString_Append(file.Str(), UseString(reinterpret_cast<UnkResCategory_l25539c*>(g_unk_71727b9530)->types[0]));
    UnkActorPath_l25539c path;
    bool found;
    CRntString_Assign(path.Str(), Addr("actors/weapons/" + *name.Str() + "/charclasses/" + *file.Str()));
    found = IsRegistered(*path.Str());
    if (!found) {
        CRntString_Assign(path.Str(), Addr("actors/characters/" + *name.Str() + "/charclasses/" + *file.Str()));
        found = IsRegistered(*path.Str());
    }
    if (!found) {
        CRntString_Assign(path.Str(), Addr("actors/props/" + *name.Str() + "/charclasses/" + *file.Str()));
        found = IsRegistered(*path.Str());
    }
    if (!found) {
        CRntString_Assign(path.Str(), Addr("actors/logic/" + *name.Str() + "/charclasses/" + *file.Str()));
        found = IsRegistered(*path.Str());
    }
    if (!found) {
        CRntString_Assign(path.Str(), Addr("actors/sounds/" + *name.Str() + "/charclasses/" + *file.Str()));
        found = IsRegistered(*path.Str());
    }
    if (!found) {
        CRntString_Assign(path.Str(), Addr("actors/items/" + *name.Str() + "/charclasses/" + *file.Str()));
        found = IsRegistered(*path.Str());
    }
    if (!found) {
        CRntString_Assign(path.Str(), Addr("actors/spawnpoints/" + *name.Str() + "/charclasses/" + *file.Str()));
        found = IsRegistered(*path.Str());
    }
    if (!found) {
        CRntString_Assign(path.Str(), Addr("actors/events/" + *name.Str() + "/charclasses/" + *file.Str()));
        found = IsRegistered(*path.Str());
    }
    if (!found) {
        CRntString_Assign(path.Str(), Addr("actors/swarms/" + *name.Str() + "/charclasses/" + *file.Str()));
        found = IsRegistered(*path.Str());
    }
    if (!found) {
        CRntString_Assign(path.Str(), Addr("actors/fx/" + *name.Str() + "/charclasses/" + *file.Str()));
        found = IsRegistered(*path.Str());
    }
    if (found)
        return CFilePathStrId(*path.Str(), true);
    return CFilePathStrId();
}
