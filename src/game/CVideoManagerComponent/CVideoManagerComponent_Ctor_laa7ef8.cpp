// CVideoManagerComponent's unit: the component's constructor. On the component base (fn_71010c4148):
// the vtable, then the two videos' string ids (+0x40, +0x48) and file paths (+0x50, +0x58), each
// built empty and then set to the global empty id or path. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CComponent.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

extern char g_unk_7101a94b18[];  // vtable

struct UnkVideoManager_laa7ef8 {
    void* vtable;
    u8 pad_0x8[0x38];
    CStrId video_0x40;
    CStrId video_0x48;
    CFilePathStrId path_0x50;
    CFilePathStrId path_0x58;
};

// An empty string id: the default constructor, inlined.
static inline void InitEmptyStrId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100aa7ef8(UnkVideoManager_laa7ef8* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a94b18 + 0x10;
    InitEmptyStrId(&self->video_0x40);
    InitEmptyStrId(&self->video_0x48);
    new (&self->path_0x50) CFilePathStrId();
    new (&self->path_0x58) CFilePathStrId();
    self->video_0x40 = base::global::CStrId::Empty();
    self->video_0x48 = base::global::CStrId::Empty();
    self->path_0x50 = g_unk_7101d470e8;
    self->path_0x58 = g_unk_7101d470e8;
}
