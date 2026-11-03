// CCutsceneComponent's unit (tools/tu_map.py): asks the cutscene manager (g_unk_7101d47330,
// fn_71011986e0) about the component's path id (+0x150), passed as a copy. Names are placeholders.
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

// As src/engine/HostArguments.cpp declares it.
extern void* g_unk_7101d47330;
u32 fn_71011986e0(void* manager, const CFilePathStrId* path);

struct UnkCutsceneComponent_b53 {
    u8 pad_0x0[0x150];
    CFilePathStrId path;  // 0x150
};

bool fn_71007a6d6c(UnkCutsceneComponent_b53* self) {
    void* manager = g_unk_7101d47330;
    CFilePathStrId path(self->path);
    return fn_71011986e0(manager, &path) != 0;
}
