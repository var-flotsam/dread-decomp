// CVideoManagerComponent's unit (tools/tu_map.py): calls on the scene-state singleton (g_unk_7101d47320,
// created on first use as src/engine/HostArguments.cpp's SceneState). Stopping the two videos whose
// string ids (+0x40, +0x48) are set (channels 0x19 and 0x1a, fn_7100f00f4c), and playing a video on a
// channel: unless its id is empty, the id's text (a CRntString at +0x10 of its data, whose count at +0x44
// is raised and never lowered, as in the original) becomes a file path (fn_710001a270), and the channel is
// stopped (fn_7100f00f4c), given the path (fn_7100f00af4) and started (fn_7100f01838). Names and layouts
// are placeholders.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkStruct_0x128;
extern UnkStruct_0x128* g_unk_7101d47320;
void fn_7100ef3494(UnkStruct_0x128* self);
void fn_7100f00f4c(UnkStruct_0x128*, u32, bool);
void fn_7100f00af4(UnkStruct_0x128* self, u32 channel, const CFilePathStrId* path, bool flag);
void fn_7100f01838(UnkStruct_0x128* self, u32 channel);

static inline UnkStruct_0x128* SceneState() {
    UnkStruct_0x128* s = g_unk_7101d47320;
    if (!s) {
        s = static_cast<UnkStruct_0x128*>(fn_71000839a0(0x150, 0x3f));
        fn_7100ef3494(s);
        g_unk_7101d47320 = s;
    }
    return s;
}

struct UnkVideoManager_ovb0088 {
    u8 pad_0x0[0x40];
    CStrId video_0x40;
    CStrId video_0x48;
};

void fn_7100aa85d0(UnkVideoManager_ovb0088* self) {
    if (self->video_0x40.data != &g_unk_7101d47108->empty)
        fn_7100f00f4c(SceneState(), 0x19, false);
    if (self->video_0x48.data != &g_unk_7101d47108->empty)
        fn_7100f00f4c(SceneState(), 0x1a, false);
}

struct UnkVideoIdData_ovb0088 {
    u8 pad_0x0[0x10];
    u8 text[0x34];  // a CRntString starts here
    union {
        s32 count;  // 0x44
    };
};

struct UnkVideoId_ovb0088 {
    UnkVideoIdData_ovb0088* data;
};

// A file path filled by fn_710001a270, released as a CFilePathStrId.
struct UnkVideoPath_ovb0088 : base::global::UnkFilePathStrIdBase {
    ~UnkVideoPath_ovb0088() { reinterpret_cast<CFilePathStrId*>(this)->~CFilePathStrId(); }
};

void fn_7100aa8494(UnkVideoManager_ovb0088* self, u32 channel, const UnkVideoId_ovb0088* id) {
    if (id->data == reinterpret_cast<UnkVideoIdData_ovb0088*>(&g_unk_7101d47108->empty))
        return;
    ++id->data->count;
    UnkVideoPath_ovb0088 path;
    fn_710001a270(&path, *reinterpret_cast<const base::global::CRntString*>(id->data->text), true);
    fn_7100f00f4c(SceneState(), channel, true);
    fn_7100f00af4(SceneState(), channel, reinterpret_cast<const CFilePathStrId*>(&path), true);
    fn_7100f01838(SceneState(), channel);
}
