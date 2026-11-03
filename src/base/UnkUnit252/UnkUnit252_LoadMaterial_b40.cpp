// Unit 252 of tools/tu_map.py (0x71001bdeac-0x71001c071c, no reflected class): loads a material by
// path from the resource manager g_unk_716e352450 (as src/base/spatial/UnkBillboard.cpp does) into 0xe8
// and the part's copy at 0x118, releases the one it replaced and returns the new one. Names and
// layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkDecalResource;
struct UnkDecalResourceMgr;
extern UnkDecalResourceMgr* g_unk_716e352450;
UnkDecalResource* fn_710019ffe8(UnkDecalResourceMgr* mgr, const CFilePathStrId* path, bool flag);
void fn_71001a080c(UnkDecalResourceMgr* mgr, UnkDecalResource* resource);

struct UnkObject252 {
    u8 pad_0x0[0xe8];
    UnkDecalResource* material;
    u8 pad_0xf0[0x28];
    UnkDecalResource* part_material;  // 0x118
};

UnkDecalResource* fn_71001be020(UnkObject252* self, const CFilePathStrId* path) {
    UnkDecalResource* old = self->material;
    UnkDecalResource* material = fn_710019ffe8(g_unk_716e352450, path, true);
    self->material = material;
    self->part_material = material;
    if (old)
        fn_71001a080c(g_unk_716e352450, old);
    return self->material;
}
