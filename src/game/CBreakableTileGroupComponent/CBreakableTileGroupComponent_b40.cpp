// CBreakableTileGroupComponent unit (tools/tu_map.py unit 1167): fn_7100758388 is an assignment of a
// record: three words, a string id (copied), the flag at +0x40, then the array of 0x40-byte entries at
// +0x18 taken over from the other record (base::UnkVector's move assignment; the entries are destroyed
// through their vtables first). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkTileEntry_bt {
    virtual ~UnkTileEntry_bt();
    u8 pad_0x8[0x38];
};

struct UnkTileRecord_bt {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    base::global::CStrId field_0x10;
    base::UnkVector<UnkTileEntry_bt> field_0x18;
    bool field_0x40;
};

UnkTileRecord_bt* fn_7100758388(UnkTileRecord_bt* self, UnkTileRecord_bt* other) {
    self->field_0x0 = other->field_0x0;
    self->field_0x4 = other->field_0x4;
    self->field_0x8 = other->field_0x8;
    self->field_0x10 = other->field_0x10;
    self->field_0x40 = other->field_0x40;
    self->field_0x18 = static_cast<base::UnkVector<UnkTileEntry_bt>&&>(other->field_0x18);
    return self;
}
