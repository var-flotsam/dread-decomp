// Unit 246 of tools/tu_map.py (0x71001bb7cc-0x71001bbb3c, no reflected class): loads a material by path
// from the resource manager g_unk_716e352450 (as src/base/spatial/UnkBillboard.cpp does), puts it in the
// part at 0x78, notes whether its kind is other than 1 and releases the material it replaced. Returns
// whether the load succeeded. The slot is written through a pointer to it (the original reloads the
// part afterwards). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkDecalResource;
struct UnkDecalResourceMgr;
extern UnkDecalResourceMgr* g_unk_716e352450;
UnkDecalResource* fn_710019ffe8(UnkDecalResourceMgr* mgr, const CFilePathStrId* path, bool flag);
void fn_71001a080c(UnkDecalResourceMgr* mgr, UnkDecalResource* resource);

struct UnkMaterial246 {
    u8 pad_0x0[0x18];
    u32 kind;
};

struct UnkPart246 {
    u8 pad_0x0[0x20];
    UnkMaterial246* material;
    u8 pad_0x28[0x44];
    u32 field_0x6c;
};

struct UnkObject246 {
    u8 pad_0x0[0x78];
    UnkPart246* part;
};

bool fn_71001bb9c8(UnkObject246* self, const CFilePathStrId* path) {
    UnkMaterial246* material = (UnkMaterial246*)fn_710019ffe8(g_unk_716e352450, path, true);
    if (!material)
        return false;
    UnkMaterial246** slot = &self->part->material;
    UnkMaterial246* old = *slot;
    *slot = material;
    self->part->field_0x6c = material->kind != 1;
    if (old)
        fn_71001a080c(g_unk_716e352450, (UnkDecalResource*)old);
    return true;
}
