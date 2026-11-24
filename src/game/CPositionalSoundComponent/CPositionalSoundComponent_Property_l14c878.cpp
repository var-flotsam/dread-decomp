// CPositionalSoundComponent unit (tools/tu_map.py, the name from reflection): the property handler.
// Reads four floats, a flag, a float or the sound paths (one string each, while the stream has more
// and paths remain) from the property's stream; other keys go to the base handler (fn_71010c4250).
// Names and layouts are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "game/UnkComponent.h"
#include "types.h"

using namespace unk_prop;
using base::global::CFilePathStrId;
using base::global::CRntFile;

struct UnkPositionalSound_l14c878 {
    u8 unk_0x0[0x48];
    s32 path_count;  // 0x48
    u8 unk_0x4c[4];
    CFilePathStrId paths[4];  // 0x50
    f32 fMinAtt;
    f32 fMaxAtt;
    f32 fLaunchEvery;
    f32 fVol;
    f32 fPitch;
    u8 unk_0x84[0x24];
    bool bLoop;
};
static_assert(__builtin_offsetof(UnkPositionalSound_l14c878, bLoop) == 0xa8, "layout");

u32 fn_710114c878(UnkPositionalSound_l14c878* self, UnkProp* p) {
    if (Is(p, Key(0x1498)) && p->set) {
        CRntFile_ReadFormat(p->stream, "ffff", &self->fVol, &self->fPitch, &self->fMinAtt,
                      &self->fMaxAtt);
        return true;
    }
    if (Is(p, Key(0x1ff0)) && p->set) {
        CRntFile_ReadFormat(p->stream, "b", &self->bLoop);
        return true;
    }
    if (Is(p, Key(0x1fe8)) && p->set) {
        CRntFile_ReadFormat(p->stream, "f", &self->fLaunchEvery);
        return true;
    }
    if (Is(p, Key(0x1fa8)) && p->set) {
        CRntFile* file = reinterpret_cast<CRntFile*>(p->stream);
        for (int i = 0; file->position < file->size && i < self->path_count; ++i) {
            const char* path;
            CRntFile_ReadCString(file, &path);
            self->paths[i] = CFilePathStrId(path, true);
        }
        return true;
    }
    return fn_71010c4250(reinterpret_cast<UnkBaseComponent*>(self), reinterpret_cast<UnkComponentProp*>(p));
}
