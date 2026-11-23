// msapi::res::CRenderRes's unit (tools/tu_map.py unit 238): builds the file path of a render resource
// from its key: the directory its name picks (post-process or shader sources by platform, or surfaces),
// then the name, '.', the type, '.' and the extension, and returns the type and path pair.
// Names and layouts are placeholders.
#include "../../../../game_aca57c/UnkBufferString.h"
#include "base/UnkResKey.h"
#include "base/UnkStrIdPathPair.h"
#include "base/animation/CBlendSpace1D.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "game/UnkTU27.h"
#include "game/UnkTU57.h"
#include "game/msapi/res/CRenderRes.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

// The type the paths this builds are paired with.
__attribute__((visibility("hidden"))) extern const UnkResType g_unk_716e554390;

// Whether the string starts with `str`.
bool fn_710007fbf8(const base::global::CRntString* self, const char* str);

struct UnkRenderResKey_l1b1680 {
    CStrId id;
    CFilePathStrId path;  // 0x8
};

struct UnkRenderResInfo_l1b1680 {
    u8 pad_0x0[0x18];
    u32 name_start;  // 0x18: where the name starts in the key's path
};

struct UnkRenderResEntry_l1b1680 {
    u8 pad_0x0[0x20];
    UnkRenderResInfo_l1b1680* info;  // 0x20
};

// A string with inline storage, built empty, from another string or from a run of characters.
template <u32 N, void** Vtable>
struct UnkRenderResString_l1b1680 : UnkBufferString<N, Vtable> {
    UnkRenderResString_l1b1680() {}
    explicit UnkRenderResString_l1b1680(const CRntString& text) { CRntString_Assign(this->Str(), &text); }
    UnkRenderResString_l1b1680(const char* chars, u32 length) { CRntString_AssignPrefix(this->Str(), chars, length); }
};

typedef UnkRenderResString_l1b1680<0x40, g_unk_7101a1b678> UnkShortString_l1b1680;
typedef UnkRenderResString_l1b1680<0x100, g_unk_7101a194b8> UnkLongString_l1b1680;

// A file-path id built from a string out of line (fn_710001a270).
struct UnkRenderResPathId_l1b1680 {
    union {
        CFilePathStrId id;
    };
    explicit UnkRenderResPathId_l1b1680(const CRntString& str) { fn_710001a270(&id, str, true); }
    ~UnkRenderResPathId_l1b1680() { id.~CFilePathStrId(); }
};

// The type and path pair, returned in memory.
struct UnkRenderResPair_l1b1680 {
    void* id;
    void* path;
    UnkRenderResPair_l1b1680(const void* type, const CFilePathStrId& p) {
        fn_710008d88c(reinterpret_cast<UnkStrIdPathPair*>(this), type, &p);
    }
    UnkRenderResPair_l1b1680(const UnkRenderResPair_l1b1680& other);
};

// The id's text, counting the read.
template <class Id>
static inline const CRntString& Text(const Id& id) {
    ++id.data->reads;
    return id.data->String();
}

// Appends the platform directory the name ends with (after its last '_'), then '/'.
static inline void AppendPlatformDir(CRntString* dir, const CRntString* name) {
    u32 start = CRntString_FindLastChar(name, '_', -1) + 1;
    u32 length = name->length - start;
    {
        UnkShortString_l1b1680 platform(name->data + start, length);
        CRntString_Append(dir, platform.Str());
    }
    CRntString_AppendChar(dir, '/');
}

UnkRenderResPair_l1b1680 fn_71001b1680(const UnkRenderResKey_l1b1680* key) {
    UnkRenderResEntry_l1b1680* entry = reinterpret_cast<UnkRenderResEntry_l1b1680*>(
        fn_7100020320(g_unk_7101d4bbc0, reinterpret_cast<const UnkRegistryKey*>(key)));
    u32 start = entry->info->name_start;
    s32 end = CRntString_FindLastChar(&Text(key->path), '.', -1);
    const CRntString& path = Text(key->path);
    if (end == -1)
        end = path.length;
    u32 length = end - start;
    UnkShortString_l1b1680 name(path.data + start, length);
    UnkShortString_l1b1680 type(Text(g_unk_716e453ef8.name));
    UnkShortString_l1b1680 dir;
    if (fn_710007fbf8(name.Str(), "pp_")) {
        CRntString_AssignCString(dir.Str(), "system/engine/postprocess/win/");
        AppendPlatformDir(dir.Str(), name.Str());
    } else if (CRntString_Contains(name.Str(), "_windx")) {
        CRntString_AssignCString(dir.Str(), "system/engine/shaders/win/");
        AppendPlatformDir(dir.Str(), name.Str());
    } else {
        CRntString_AssignCString(dir.Str(), "system/engine/surfaces/");
        CRntString_Assign(type.Str(), &Text(g_unk_716e453f78.name));
    }

    UnkLongString_l1b1680 text(*dir.Str());
    CRntString_Append(text.Str(), name.Str());
    CRntString_AppendChar(text.Str(), '.');
    CRntString_Append(text.Str(), type.Str());
    CRntString_AppendChar(text.Str(), '.');
    CRntString_Append(text.Str(), &Text(g_unk_7101d4e590));
    UnkRenderResPathId_l1b1680 file(*text.Str());
    return UnkRenderResPair_l1b1680(&g_unk_716e554390, file.id);
}
