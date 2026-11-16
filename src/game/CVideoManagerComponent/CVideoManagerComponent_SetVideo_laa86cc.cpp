// CVideoManagerComponent's unit: sets the first video from a file path (fn_7100aa8a54 sets the
// second). An empty path clears the video's string id (+0x40) and path (+0x50); otherwise the path,
// prefixed with "rom:" unless it has it, becomes the id, and the id's text becomes the path. Names and
// layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

struct UnkVideoManager_laa86cc {
    u8 pad_0x0[0x40];
    CStrId video;  // 0x40
    u8 pad_0x48[0x8];
    CFilePathStrId path;  // 0x50
};

// A string id built from a string out of line (fn_7100000850), released like CStrId.
struct UnkBuiltStrId_laa86cc {
    union {
        CStrId id;
    };

    explicit UnkBuiltStrId_laa86cc(const CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkBuiltStrId_laa86cc() { id.~CStrId(); }
};

// A file-path id built from a string out of line (fn_710001a270), released like CFilePathStrId.
struct UnkBuiltPathId_laa86cc {
    union {
        CFilePathStrId id;
    };

    explicit UnkBuiltPathId_laa86cc(const CRntString& str) { fn_710001a270(&id, str, true); }
    ~UnkBuiltPathId_laa86cc() { id.~CFilePathStrId(); }
};

// An id's string, counting the read.
template <class Id>
static inline const CRntString& IdString(const Id& id) {
    ++id.data->reads;
    return id.data->String();
}

void fn_7100aa86cc(UnkVideoManager_laa86cc* self, const CFilePathStrId& path) {
    if (path.data == &g_unk_7101d47200->empty) {
        self->video = g_unk_7101d470e0;
        self->path = g_unk_7101d470e8;
        return;
    }
    ++path.data->reads;
    if (path.data != &g_unk_7101d47200->empty && fn_710007fb9c(&path.data->String(), "rom:")) {
        self->video = static_cast<CStrId&&>(UnkBuiltStrId_laa86cc(IdString(path)).id);
        self->path = static_cast<CFilePathStrId&&>(UnkBuiltPathId_laa86cc(IdString(self->video)).id);
    } else {
        CRntString name("rom:");
        const CRntString& text = IdString(path);
        fn_710007f3e4(&name, text.data ? text.data : g_unk_710162e2ed, text.length);
        self->video = static_cast<CStrId&&>(UnkBuiltStrId_laa86cc(name).id);
        self->path = static_cast<CFilePathStrId&&>(UnkBuiltPathId_laa86cc(IdString(self->video)).id);
    }
}
