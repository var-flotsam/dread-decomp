// GUI::CScene3D's unit (tools/tu_map.py): loads the scene's model. With a frame to show it in, the
// scene takes the frame and the model's name, loads <root>maps/levels/c50_gui/<name>/<name>.bmssd and
// hands the model to the render manager's "c50_gui" layer. Names and layouts are placeholders.
#include "base/file/CPackManagerDevice.h"
#include "base/file/UnkFileDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkScene3D_lb72b30 {
    u8 pad_0x0[0x288];
    CRntString name;  // 0x288: the scene's model
    s32 frame;        // 0x2b8
};

const CRntString* fn_7100def670(UnkGameManager* self);  // the data root
bool fn_7100089cf8(base::file::UnkFileDevice* self, const base::global::CFilePathStrId& path);
void fn_71002198cc(UnkRenderManager* self, const char* type, const char* layer, const char* name, s32 frame,
                   bool flag);

static inline CRntString Concat_lb72b30(const CRntString& a, const CRntString& b) { return CRntString_Concat(&a, &b); }
static inline CRntString Concat_lb72b30(const CRntString& a, const char* b) { return CRntString_ConcatCString(&a, b); }

static inline void Assign_lb72b30(CRntString* self, const CRntString& other) { CRntString_Assign(self, &other); }

extern base::file::UnkFileDevice* g_unk_7101d4d828;

static inline base::file::UnkFileDevice* FileDevice_lb72b30() {
    if (!g_unk_7101d4d828) {
        auto* device = static_cast<base::file::UnkFileDevice*>(fn_71000839a0(0x1348, 0x3f));
        fn_71000893a8(device);
        g_unk_7101d4d828 = device;
    }
    return g_unk_7101d4d828;
}

// A file-path id made from a string (fn_710001a270), released as CFilePathStrId is.
struct UnkScenePathId_lb72b30 : base::global::UnkFilePathStrIdBase {
    explicit UnkScenePathId_lb72b30(const CRntString& str) { fn_710001a270(this, str, true); }
    ~UnkScenePathId_lb72b30() { reinterpret_cast<base::global::CFilePathStrId*>(this)->~CFilePathStrId(); }
};

static inline void Load_lb72b30(base::file::UnkFileDevice* device, const UnkScenePathId_lb72b30& path) {
    fn_7100089cf8(device, reinterpret_cast<const base::global::CFilePathStrId&>(path));
}

void fn_7100b72b30(UnkScene3D_lb72b30* self, const CStrId& model) {
    s32 frame = fn_71011cde14(reinterpret_cast<UnkGuiDisplayObject*>(self))->frame;
    if (frame == 0x7fffffff)
        return;
    self->frame = frame;
    ++model.data->reads;
    Assign_lb72b30(&self->name, model.data->String());
    // The model's file: <root>maps/levels/c50_gui/<name>/<name>.bmssd.
    CRntString path("maps/levels/");
    Assign_lb72b30(&path, Concat_lb72b30(Concat_lb72b30(Concat_lb72b30(Concat_lb72b30(Concat_lb72b30(Concat_lb72b30(
                                                                                    *fn_7100def670(g_unk_7101d47310), path),
                                                                                "c50_gui/"),
                                                                 self->name),
                                                  "/"),
                                   self->name),
                    ".bmssd"));
    Load_lb72b30(FileDevice_lb72b30(), UnkScenePathId_lb72b30(path));
    fn_71000898c4(FileDevice_lb72b30());
    fn_71002198cc(g_unk_7101d47300, g_unk_710162e2ed, "c50_gui", self->name.data, self->frame, true);
}
