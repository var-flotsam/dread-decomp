// Unit 2666 (lua_bind): plays the audio preset named by the string argument, resolved under
// "audiopreset:system/snd/presets/". Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkPathIdsFromText.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU0.h"
#include "game/UnkTU25.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU57.h"
#include "game/sound/CSoundManager.h"
#include "types.h"
#include "game/UnkBufferStringPool.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

struct lua_State;

// The vtable of the allocator that follows a string with 0x80 bytes of inline storage.

// A string with 0x80 bytes of inline storage (0xc8 bytes).
struct UnkInlineString_l08e6e0 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x80];
    u64 field_0xb8;
    u32 field_0xc0;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    explicit UnkInlineString_l08e6e0(const char* text) {
        CRntString_InitWithAllocator(Str(), reinterpret_cast<base::IAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a1b750 + 2);
        field_0xb8 = 0;
        field_0xc0 = 0;
        CRntString_AssignCString(Str(), text);
    }
    ~UnkInlineString_l08e6e0() {
        CRntString_Clear(Str());
        CRntString_Destroy(Str());
    }
};

// The preset: its id and path with the resource they resolve to.
struct UnkPreset_l08e6e0 {
    CStrId id;
    CFilePathStrId path;
    UnkCountedResource* resource;

    UnkPreset_l08e6e0() : id(g_unk_7101d470e0), path(g_unk_7101d470e8), resource(nullptr) {}
    // Empties the ids and drops the resource (the ids' destructors run after it).
    void Release() {
        fn_7100027044(Resource());
        if (UnkCountedResource* r = resource) {
            if (--r->ref_count == 0)
                fn_710001737c(r);
        }
    }
    UnkStrIdPathResource* Resource() { return reinterpret_cast<UnkStrIdPathResource*>(this); }
    bool IsSet() const {
        return id.data != &g_unk_7101d47108->empty && path.data != &g_unk_7101d47200->empty;
    }
};

// The string with every '#' written as '/'.
inline UnkInlineString_l08e6e0 ReplaceHashes(const CRntString& str) {
    char text[0x200];
    u32 length = str.length;
    text[length] = '\0';
    for (u32 i = 0; i < length; ++i) {
        char c = str.data[i];
        if (c == '#')
            text[i] = '/';
        else
            text[i] = c;
    }
    return UnkInlineString_l08e6e0(text);
}

int fn_710108e6e0(lua_State* L) {
    const char* name;
    UnkPreset_l08e6e0 preset;
    if (fn_71010c0270(L, "s", &name)) {
        UnkInlineString_l08e6e0 text(name);
        if (!preset.IsSet() && text.Str()->data && text.Str()->data[0]) {
            ReplaceHashes(*text.Str());  // the result is unused in the original

            CRntString path = CRntString_ConcatCStringFirst("audiopreset:system/snd/presets/", text.Str());
            UnkPathIdsFromText ids(path);
            fn_71000267f8(preset.Resource(), ids.ids);
        }
        if (preset.IsSet())
            fn_7101024e28(g_unk_7101d47318, reinterpret_cast<const UnkStrIdPathPair*>(&preset), nullptr, 0);
    }
    preset.Release();
    return 0;
}
