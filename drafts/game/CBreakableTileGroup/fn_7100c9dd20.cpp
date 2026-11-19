// CBreakableTileGroup's unit: the destructor of one 0xa8-byte breakable tile: removes its render
// object, then releases its three id/path slots (last first) and its name id.
// Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU315.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

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

void fn_7100c9dd20(UnkBreakableTile* self) {
    if (self->field_0x48 != 0xffffffff) {
        fn_7100219e10(g_unk_7101d47300, self->field_0x48);
        self->field_0x48 = 0xffffffff;
    }
    for (int i = 3; i-- > 0;) {
        UnkTileIdSlot& slot = self->slots[i];
        fn_7100027044(reinterpret_cast<UnkStrIdPathResource*>(&slot));
        if (UnkCountedResource* resource = slot.resource)
            if (--resource->ref_count == 0)
                fn_710001737c(resource);
        slot.path.~CFilePathStrId();
        slot.id.~CStrId();
    }
    self->field_0x20.~CStrId();
}
