// CBreakableTileGroup's unit: the constructor of one 0xa8-byte breakable tile: no index, an
// empty name id, three id/path slots set from the unit's three static pairs.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/UnkStaticStrId.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkStaticId;
using base::UnkStaticPathId;
using base::global::CFilePathStrId;
using base::global::CStrId;

// A string id and a file path.
struct UnkStaticIdPath {
    UnkStaticId id;
    UnkStaticPathId path;
};
extern UnkStaticIdPath g_unk_7172795f50[3] __attribute__((visibility("hidden")));

// A string id and a file path (both the defaults) and the resource they resolve to.
struct UnkTileIdSlot {
    CStrId id;
    CFilePathStrId path;
    UnkCountedResource* resource;

    UnkTileIdSlot() : id(g_unk_7101d470e0), path(g_unk_7101d470e8), resource(nullptr) {}
};

struct UnkTwoWords {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkBreakableTile {
    s32 field_0x0;  // index
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    CStrId field_0x20;
    f32 field_0x28;
    f32 field_0x2c;
    void* field_0x30;
    s32 field_0x38;
    bool field_0x3c;
    f32 field_0x40;
    u32 field_0x44;
    u64 field_0x48;  // render object, or 0xffffffff
    UnkTwoWords field_0x50;
    UnkTileIdSlot slots[3];
};
static_assert(sizeof(UnkBreakableTile) == 0xa8, "UnkBreakableTile placeholder layout");

void fn_7100c9db18(UnkBreakableTile* self) {
    self->field_0x0 = -1;
    self->field_0x4 = 0;
    self->field_0x8 = 0;
    self->field_0xc = 0;
    self->field_0x10 = 0;
    self->field_0x14 = 0;
    self->field_0x18 = 0.5f;
    self->field_0x1c = 0.0f;
    self->field_0x20.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x20.data = &g_unk_7101d47108->empty;
    self->field_0x28 = 0.0f;
    self->field_0x2c = 0.0f;
    self->field_0x30 = nullptr;
    self->field_0x38 = 1;
    self->field_0x3c = false;
    self->field_0x40 = -1.0f;
    self->field_0x44 = 0;
    self->field_0x48 = 0xffffffff;
    self->field_0x50 = UnkTwoWords();
    for (UnkTileIdSlot& slot : self->slots)
        new (&slot) UnkTileIdSlot;
    for (int i = 0; i < 3; i++)
        fn_71000267f8(reinterpret_cast<UnkStrIdPathResource*>(&self->slots[i]),
                      reinterpret_cast<const UnkStrIdPathPair&>(g_unk_7172795f50[i]));
}
