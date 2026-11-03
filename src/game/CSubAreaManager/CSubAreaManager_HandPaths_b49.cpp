// CSubAreaManager's unit (tools/tu_map.py unit 2577): hands every file-path id of the array at +0x3b8
// (count at +0x3c0) to the file device singleton (created on first use, as src/game/CSubAreaManager.cpp
// does) with fn_7100089850 and flag 1, then clears the flag at +0x3b0. Names and layouts are placeholders.
#include "base/file/UnkFileDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/UnkVector.h"
#include "types.h"

using base::global::CFilePathStrId;

// As src/game/CSubAreaManager.cpp declares it.
extern base::file::UnkFileDevice* g_unk_7101d4d828;
void fn_7100089850(base::file::UnkFileDevice* self, const CFilePathStrId* path, u32 flags);

struct UnkSubAreaPaths_b49a6 {
    u8 pad_0x0[0x3b0];
    bool field_0x3b0;
    u8 pad_0x3b1[7];
    CFilePathStrId* paths;  // 0x3b8
    u32 count;              // 0x3c0
};

void fn_7100ef1580(UnkSubAreaPaths_b49a6* self) {
    base::file::UnkFileDevice* device = g_unk_7101d4d828;
    if (!device) {
        device = static_cast<base::file::UnkFileDevice*>(fn_71000839a0(0x1348, 0x3f));
        fn_71000893a8(device);
        g_unk_7101d4d828 = device;
    }
    for (CFilePathStrId *it = self->paths, *end = self->paths + self->count; it != end; ++it)
        fn_7100089850(device, it, 1);
    self->field_0x3b0 = false;
}
