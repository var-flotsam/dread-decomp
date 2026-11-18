// CVideoManagerComponent's unit: sets a video from a file path. An empty path clears the video's
// string id (+0x48) and path (+0x58); otherwise the path, prefixed with "rom:" unless it has it,
// becomes the id, and the id's text becomes the path. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

struct UnkVideoManager_laa8a54 {
    u8 pad_0x0[0x48];
    CStrId video;  // 0x48
    u8 pad_0x50[0x8];
    CFilePathStrId path;  // 0x58
};

// A string id built from a string out of line (fn_7100000850), released like CStrId.
struct UnkBuiltStrId_laa8a54 {
    union {
        CStrId id;
    };

    explicit UnkBuiltStrId_laa8a54(const CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkBuiltStrId_laa8a54() { id.~CStrId(); }
};

// A file-path id built from a string out of line (fn_710001a270), released like CFilePathStrId.
struct UnkBuiltPathId_laa8a54 {
    union {
        CFilePathStrId id;
    };

    explicit UnkBuiltPathId_laa8a54(const CRntString& str) { fn_710001a270(&id, str, true); }
    ~UnkBuiltPathId_laa8a54() { id.~CFilePathStrId(); }
};

// An id's string, counting the read.
template <class Id>
static inline const CRntString& IdString(const Id& id) {
    ++id.data->reads;
    return id.data->String();
}

void fn_7100aa8a54(UnkVideoManager_laa8a54* self, const CFilePathStrId& path) {
    if (path.data == &g_unk_7101d47200->empty) {
        self->video = base::global::CStrId::Empty();
        self->path = g_unk_7101d470e8;
        return;
    }
    ++path.data->reads;
    if (path.data != &g_unk_7101d47200->empty && fn_710007fb9c(&path.data->String(), "rom:")) {
        self->video = static_cast<CStrId&&>(UnkBuiltStrId_laa8a54(IdString(path)).id);
        self->path = static_cast<CFilePathStrId&&>(UnkBuiltPathId_laa8a54(IdString(self->video)).id);
    } else {
        CRntString name("rom:");
        const CRntString& text = IdString(path);
        fn_710007f3e4(&name, text.data ? text.data : g_unk_710162e2ed, text.length);
        self->video = static_cast<CStrId&&>(UnkBuiltStrId_laa8a54(name).id);
        self->path = static_cast<CFilePathStrId&&>(UnkBuiltPathId_laa8a54(IdString(self->video)).id);
    }
}
