// Unit 25 of tools/tu_map.py: the registry lookup of src/base/UnkUnit25/UnkRegistryMake_b48.cpp,
// but when the key is not registered the pair is built from a path made of the key's path up to
// its first '.', then '.', the key's id, '.' and a global id (fn_710008d88c). Names and layouts
// are placeholders.
#include "../../game_aca57c/UnkBufferString.h"
#include "base/UnkStrIdPathPair.h"
#include "base/animation/CBlendSpace1D.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "game/UnkTU25.h"
#include "game/UnkTU27.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

struct UnkRegistry_7100d92158;

struct UnkExtKey_ovb0274 {
    CStrId id;
    CFilePathStrId path;
};

struct UnkPathPairTemp_b45a1;

struct UnkExtPair_ovb0274 {
    void* id;
    void* path;
    UnkExtPair_ovb0274() { fn_710008d2a0(reinterpret_cast<UnkPathPairTemp_b45a1*>(this)); }
    UnkExtPair_ovb0274(const void* tag, const CFilePathStrId& p) {
        fn_710008d88c(reinterpret_cast<UnkStrIdPathPair*>(this), tag, &p);
    }
    UnkExtPair_ovb0274(const UnkExtPair_ovb0274& o);
};

struct UnkExtFactoryEntry_ovb0274 {
    u8 pad_0x0[0x38];
    void* context;  // 0x38
    UnkExtPair_ovb0274 (*make)(void* context, const UnkExtKey_ovb0274* key);  // 0x40
};

struct UnkFactoryEntry;
struct UnkRegistryKey;

// A file-path id built from a string out of line (fn_710001a270).
struct UnkExtPathId_ovb0274 {
    union {
        CFilePathStrId id;
    };
    explicit UnkExtPathId_ovb0274(const CRntString& str) { fn_710001a270(&id, str, true); }
    ~UnkExtPathId_ovb0274() { id.~CFilePathStrId(); }
};

// The text of an id's pooled data (a CRntString at +0x10) with its use count (+0x44) raised; the
// count is written untyped as the original reads the id again afterwards.
template <class Id>
static inline const CRntString* IdText(const Id& id) {
    u8* data = reinterpret_cast<u8*>(id.data);
    u32 uses;
    __builtin_memcpy(&uses, data + 0x44, sizeof(uses));
    ++uses;
    __builtin_memcpy(data + 0x44, &uses, sizeof(uses));
    return &id.data->String();
}

UnkExtPair_ovb0274 fn_710001a0ac(const UnkExtKey_ovb0274* key) {
    UnkExtFactoryEntry_ovb0274* e = reinterpret_cast<UnkExtFactoryEntry_ovb0274*>(
        fn_7100020320(g_unk_7101d4bbc0, reinterpret_cast<const UnkRegistryKey*>(key)));
    if (!e)
        return UnkExtPair_ovb0274();
    IdText(key->path);
    u8* stemData = reinterpret_cast<u8*>(key->path.data);
    IdText(key->path);
    s32 dot = CRntString_FindChar(&key->path.data->String(), '.', 0);
    if (dot == -1)
        dot = reinterpret_cast<const CRntString*>(stemData + 0x10)->length;
    const char* stemChars = reinterpret_cast<const CRntString*>(stemData + 0x10)->data;
    UnkBufferString<0x100, g_unk_7101a194b8> text;
    CRntString_AssignPrefix(text.Str(), stemChars, dot);
    CRntString_AppendChar(text.Str(), '.');
    CRntString_Append(text.Str(), IdText(key->id));
    CRntString_AppendChar(text.Str(), '.');
    CRntString_Append(text.Str(), IdText(g_unk_7101d4e590));
    UnkExtPathId_ovb0274 path(*text.Str());
    return UnkExtPair_ovb0274(g_unk_7101d4e570, path.id);
}
