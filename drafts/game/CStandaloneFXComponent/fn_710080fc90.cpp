// CStandaloneFXComponent's unit: fn_710080fc90 reads an effect file's kind from its path's extension
// ("bcptl" is 0, "bcmdl" 3); false when the path has no extension or another one. Names and layouts are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "game/UnkTU57.h"
#include "../../game_aca57c/UnkBufferString.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// The character that starts an extension.
extern char g_unk_710164784c;

typedef UnkBufferString<0x100, g_unk_7101a194b8> UnkFxPathText_l80fc90;

// A copy of another string.
struct UnkFxPathCopy_l80fc90 : UnkFxPathText_l80fc90 {
    explicit UnkFxPathCopy_l80fc90(const CRntString& text) { fn_710007eaa0(Str(), &text); }
};

// A counted part of a string.
struct UnkFxPathPart_l80fc90 : UnkFxPathText_l80fc90 {
    UnkFxPathPart_l80fc90(const char* str, u32 length) { fn_710007e7f8(Str(), str, length); }
};

// A string id built from a string's text, out of line (fn_7100000850).
struct UnkFxTextId_l80fc90 {
    UnkStringData* data;

    explicit UnkFxTextId_l80fc90(const CRntString& text) {
        fn_7100000850(reinterpret_cast<base::global::UnkStrIdBase*>(this), text, true);
    }
    ~UnkFxTextId_l80fc90() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
    bool operator==(const CStrId& other) const { return data == other.data; }
};

bool fn_710080fc90(const CFilePathStrId* path, u32* out) {
    ++path->data->reads;
    UnkFxPathCopy_l80fc90 full(path->data->String());
    s32 dot = fn_710007fb10(full.Str(), g_unk_710164784c, -1);
    if (dot < 0)
        return false;
    u32 start = dot + 1;
    u32 length = full.Str()->length - start;
    const char* text = full.Str()->data + start;
    UnkFxTextId_l80fc90 extension(*UnkFxPathPart_l80fc90(text, length).Str());
    if (extension == CStrId("bcptl", true)) {
        *out = 0;
        return true;
    }
    if (extension == CStrId("bcmdl", true)) {
        *out = 3;
        return true;
    }
    return false;
}
